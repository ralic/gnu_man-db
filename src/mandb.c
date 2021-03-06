/*
 * mandb.c: used to create and initialise global man database.
 *  
 * Copyright (C) 1994, 1995 Graeme W. Wilford. (Wilf.)
 * Copyright (C) 2001, 2002, 2003, 2004, 2006, 2007, 2008, 2009, 2010, 2011,
 *               2012 Colin Watson.
 *
 * This file is part of man-db.
 *
 * man-db is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * man-db is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with man-db; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Tue Apr 26 12:56:44 BST 1994  Wilf. (G.Wilford@ee.surrey.ac.uk) 
 *
 * CJW: Security fixes. Make --test work. Purge old database entries.
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>	/* for chmod() */
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#ifdef MAN_OWNER
#  include <pwd.h>
#endif /* MAN_OWNER */

#include "argp.h"
#include "dirname.h"
#include "stat-time.h"
#include "timespec.h"
#include "utimens.h"
#include "xgetcwd.h"
#include "xvasprintf.h"

#include "gettext.h"
#define _(String) gettext (String)
#define N_(String) gettext_noop (String)

#include "manconfig.h"

#include "error.h"
#include "cleanup.h"
#include "hashtable.h"
#include "pipeline.h"
#include "security.h"

#include "mydbm.h"

#include "check_mandirs.h"
#include "filenames.h"
#include "manp.h"

char *program_name;
int quiet = 1;
extern int opt_test;		/* don't update db */
char *manp;
char *database = NULL;
extern char *extension;		/* for globbing.c */
extern int force_rescan;	/* for check_mandirs.c */
static char *single_filename = NULL;
extern char *user_config_file;	/* for manp.c */
#ifdef MAN_OWNER
struct passwd *man_owner;
#endif
static int purged = 0;
static int strays = 0;

static int check_for_strays = 1;
static int purge = 1;
static int user;
static int create;
static const char *arg_manp;

struct tried_catdirs_entry {
	char *manpath;
	int seen;
};

const char *argp_program_version = "mandb " PACKAGE_VERSION;
const char *argp_program_bug_address = PACKAGE_BUGREPORT;
error_t argp_err_exit_status = FAIL;

static const char args_doc[] = N_("[MANPATH]");

static struct argp_option options[] = {
	{ "debug",		'd',	0,		0,	N_("emit debugging messages") },
	{ "quiet",		'q',	0,		0,	N_("work quietly, except for 'bogus' warning") },
	{ "no-straycats",	's',	0,		0,	N_("don't look for or add stray cats to the dbs") },
	{ "no-purge",		'p',	0,		0,	N_("don't purge obsolete entries from the dbs") },
	{ "user-db",		'u',	0,		0,	N_("produce user databases only") },
	{ "create",		'c',	0,		0,	N_("create dbs from scratch, rather than updating") },
	{ "test",		't',	0,		0,	N_("check manual pages for correctness") },
	{ "filename",		'f',	N_("FILENAME"),	0,	N_("update just the entry for this filename") },
	{ "config-file",	'C',	N_("FILE"),	0,	N_("use this user configuration file") },
	{ 0, 'h', 0, OPTION_HIDDEN, 0 }, /* compatibility for --help */
	{ 0 }
};

static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
	static int quiet_temp = 0;

	switch (key) {
		case 'd':
			debug_level = 1;
			return 0;
		case 'q':
			++quiet_temp;
			return 0;
		case 's':
			check_for_strays = 0;
			return 0;
		case 'p':
			purge = 0;
			return 0;
		case 'u':
			user = 1;
			return 0;
		case 'c':
			create = 1;
			purge = 0;
			return 0;
		case 't':
			opt_test = 1;
			return 0;
		case 'f':
			single_filename = arg;
			create = 0;
			purge = 0;
			check_for_strays = 0;
			return 0;
		case 'C':
			user_config_file = arg;
			return 0;
		case 'h':
			argp_state_help (state, state->out_stream,
					 ARGP_HELP_STD_HELP);
			break;
		case ARGP_KEY_ARG:
			if (arg_manp)
				argp_usage (state);
			arg_manp = arg;
			return 0;
		case ARGP_KEY_SUCCESS:
			if (opt_test && !debug_level)
				quiet = 1;
			else if (quiet_temp == 1)
				quiet = 2;
			else
				quiet = quiet_temp;
			return 0;
	}
	return ARGP_ERR_UNKNOWN;
}

static struct argp argp = { options, parse_opt, args_doc };

struct dbpaths {
#ifdef NDBM
#  ifdef BERKELEY_DB
	char *dbfile;
	char *tmpdbfile;
#  else /* !BERKELEY_DB NDBM */
	char *dirfile;
	char *pagfile;
	char *tmpdirfile;
	char *tmppagfile;
#  endif /* BERKELEY_DB */
#else /* !NDBM */
	char *xfile;
	char *xtmpfile;
#endif /* NDBM */
};

#ifdef MAN_OWNER
extern uid_t ruid;
extern uid_t euid;
#endif /* MAN_OWNER */

static char *manpathlist[MAXDIRS];

extern int pages;

/* remove() with error checking */
static void check_remove (const char *path)
{
	if (remove (path) == -1 && errno != ENOENT)
		error (0, errno, _("can't remove %s"), path);
}

/* rename() with error checking */
static void check_rename (const char *from, const char *to)
{
	if (rename (from, to) == -1 && errno != ENOENT) {
		error (0, errno, _("can't rename %s to %s"), from, to);
		check_remove (from);
	}
}

/* chmod() with error checking */
static void check_chmod (const char *path, mode_t mode)
{
	if (chmod (path, mode) == -1) {
		error (0, errno, _("can't chmod %s"), path);
		check_remove (path);
	}
}

/* CPhipps 2000/02/24 - Copy a file. */
static int xcopy (const char *from, const char *to)
{
	FILE *ifp, *ofp;
	struct stat st;
	struct timespec times[2];
	static const size_t buf_size = 32 * 1024;
	char *buf;
	int ret = 0;

	ifp = fopen (from, "r");
	if (!ifp) {
		ret = -errno;
		if (errno == ENOENT)
			return 0;
		error (0, errno, "fopen %s", from);
		return ret;
	}

	if (fstat (fileno (ifp), &st) >= 0) {
		times[0] = get_stat_atime (&st);
		times[1] = get_stat_mtime (&st);
	} else {
		times[0].tv_sec = 0;
		times[0].tv_nsec = UTIME_OMIT;
		times[1].tv_sec = 0;
		times[1].tv_nsec = UTIME_OMIT;
	}

	ofp = fopen (to, "w");
	if (!ofp) {
		ret = -errno;
		error (0, errno, "fopen %s", to);
		fclose (ifp);
		return ret;
	}

	buf = xmalloc (buf_size);
	while (!feof (ifp) && !ferror (ifp)) {
		size_t in = fread (buf, 1, buf_size, ifp);
		if (in > 0) {
			if (fwrite (buf, 1, in, ofp) == 0 && ferror (ofp)) {
				ret = -errno;
				error (0, errno, _("can't write to %s"), to);
				break;
			}
		} else if (ferror (ifp)) {
			ret = -errno;
			error (0, errno, _("can't read from %s"), from);
			break;
		}
	}
	free (buf);

	fclose (ifp);
	fclose (ofp);

	if (ret < 0)
		check_remove (to);
	else {
		check_chmod (to, DBMODE);
		utimens (to, times);
	}

	return ret;
}

/* rename and chmod the database */
static void finish_up (struct dbpaths *dbpaths)
{
#ifdef NDBM
#  ifdef BERKELEY_DB
	check_rename (dbpaths->tmpdbfile, dbpaths->dbfile);
	check_chmod (dbpaths->dbfile, DBMODE);
	free (dbpaths->tmpdbfile);
	dbpaths->tmpdbfile = NULL;
#  else /* not BERKELEY_DB */
	check_rename (dbpaths->tmpdirfile, dbpaths->dirfile);
	check_chmod (dbpaths->dirfile, DBMODE);
	check_rename (dbpaths->tmppagfile, dbpaths->pagfile);
	check_chmod (dbpaths->pagfile, DBMODE);
	free (dbpaths->tmpdirfile);
	free (dbpaths->tmppagfile);
	dbpaths->tmpdirfile = dbpaths->tmppagfile = NULL;
#  endif /* BERKELEY_DB */
#else /* not NDBM */
	check_rename (dbpaths->xtmpfile, dbpaths->xfile);
	check_chmod (dbpaths->xfile, DBMODE);
	free (dbpaths->xtmpfile);
	dbpaths->xtmpfile = NULL;
#endif /* NDBM */
}

#ifdef MAN_OWNER
/* change the owner of global man databases */
static void do_chown (struct dbpaths *dbpaths)
{
#  ifdef NDBM
#    ifdef BERKELEY_DB
	chown_if_possible (dbpaths->dbfile);
#    else /* not BERKELEY_DB */
	chown_if_possible (dbpaths->dirfile);
	chown_if_possible (dbpaths->pagfile);
#    endif /* BERKELEY_DB */
#  else /* not NDBM */
	chown_if_possible (dbpaths->xfile);
#  endif /* NDBM */
}
#endif /* MAN_OWNER */

/* Update a single file in an existing database. */
static int update_one_file (const char *manpath, const char *filename)
{
	MYDBM_FILE dbf;

	dbf = MYDBM_RWOPEN (database);
	if (dbf) {
		struct mandata info;
		char *manpage;

		memset (&info, 0, sizeof (struct mandata));
		manpage = filename_info (filename, &info, "");
		if (info.name) {
			dbdelete (dbf, info.name, &info);
			purge_pointers (dbf, info.name);
			free (info.name);
		}
		free (manpage);

		test_manfile (dbf, filename, manpath);
	}
	MYDBM_CLOSE (dbf);

	return 1;
}

/* dont actually create any dbs, just do an update */
static int update_db_wrapper (const char *manpath, const char *catpath)
{
	int amount;

	if (single_filename)
		return update_one_file (manpath, single_filename);

	amount = update_db (manpath, catpath);
	if (amount != EOF)
		return amount;

	return create_db (manpath, catpath);
}

/* remove incomplete databases */
static void cleanup_sigsafe (void *arg)
{
	struct dbpaths *dbpaths = arg;

#ifdef NDBM
#  ifdef BERKELEY_DB
	if (dbpaths->tmpdbfile)
		unlink (dbpaths->tmpdbfile);
#  else /* !BERKELEY_DB NDBM */
	if (dbpaths->tmpdirfile)
		unlink (dbpaths->tmpdirfile);
	if (dbpaths->tmppagfile)
		unlink (dbpaths->tmppagfile);
#  endif /* BERKELEY_DB NDBM */
#else /* !NDBM */
	if (dbpaths->xtmpfile)
		unlink (dbpaths->xtmpfile);
#endif /* NDBM */
}

/* free database names */
static void cleanup (void *arg)
{
	struct dbpaths *dbpaths = arg;

#ifdef NDBM
#  ifdef BERKELEY_DB
	free (dbpaths->dbfile);
	free (dbpaths->tmpdbfile);
	dbpaths->dbfile = dbpaths->tmpdbfile = NULL;
#  else /* !BERKELEY_DB NDBM */
	free (dbpaths->dirfile);
	free (dbpaths->pagfile);
	free (dbpaths->tmpdirfile);
	free (dbpaths->tmppagfile);
	dbpaths->dirfile = dbpaths->pagfile = NULL;
	dbpaths->tmpdirfile = dbpaths->tmppagfile = NULL;
#  endif /* BERKELEY_DB NDBM */
#else /* !NDBM */
	free (dbpaths->xfile);
	free (dbpaths->xtmpfile);
	dbpaths->xfile = dbpaths->xtmpfile = NULL;
#endif /* NDBM */
	free (dbpaths);
}

#define CACHEDIR_TAG \
	"Signature: 8a477f597d28d172789f06886806bc55\n" \
	"# This file is a cache directory tag created by man-db.\n" \
	"# For information about cache directory tags, see:\n" \
	"#\thttp://www.brynosaurus.com/cachedir/\n"

/* sort out the database names */
static int mandb (struct dbpaths *dbpaths,
		  const char *catpath, const char *manpath, int global_manpath)
{
	int ret, amount;
	char *dbname;
	int should_create;

	dbname = mkdbname (catpath);
	database = xasprintf ("%s/%d", catpath, getpid ());

	if (!quiet) 
		printf (_("Processing manual pages under %s...\n"), manpath);

	if (!STREQ (catpath, manpath)) {
		char *cachedir_tag;
		int fd;
		int cachedir_tag_exists = 0;

		cachedir_tag = xasprintf ("%s/CACHEDIR.TAG", catpath);
		fd = open (cachedir_tag, O_RDONLY);
		if (fd < 0) {
			FILE *cachedir_tag_file;

			if (errno != ENOENT)
				check_remove (cachedir_tag);
			cachedir_tag_file = fopen (cachedir_tag, "w");
			if (cachedir_tag_file) {
				cachedir_tag_exists = 1;
				fputs (CACHEDIR_TAG, cachedir_tag_file);
				fclose (cachedir_tag_file);
			}
		} else {
			cachedir_tag_exists = 1;
			close (fd);
		}
		if (cachedir_tag_exists) {
			if (global_manpath)
				chown_if_possible (cachedir_tag);
			check_chmod (cachedir_tag, DBMODE);
		}
		free (cachedir_tag);
	}

	should_create = (create || force_rescan || opt_test);

#ifdef NDBM
#  ifdef BERKELEY_DB
	dbpaths->dbfile = xasprintf ("%s.db", dbname);
	free (dbname);
	dbpaths->tmpdbfile = xasprintf ("%s.db", database);
	if (!should_create) {
		if (xcopy (dbpaths->dbfile, dbpaths->tmpdbfile) < 0)
			should_create = 1;
	}
	if (should_create) {
		check_remove (dbpaths->tmpdbfile);
		ret = create_db (manpath, catpath);
		if (ret < 0)
			return ret;
		amount = ret;
	} else {
		ret = update_db_wrapper (manpath, catpath);
		if (ret < 0)
			return ret;
		amount = ret;
	}
#  else /* !BERKELEY_DB NDBM */
	dbpaths->dirfile = xasprintf ("%s.dir", dbname);
	dbpaths->pagfile = xasprintf ("%s.pag", dbname);
	free (dbname);
	dbpaths->tmpdirfile = xasprintf ("%s.dir", database);
	dbpaths->tmppagfile = xasprintf ("%s.pag", database);
	if (!should_create) {
		if (xcopy (dbpaths->dirfile, dbpaths->tmpdirfile) < 0 ||
		    xcopy (dbpaths->pagfile, dbpaths->tmppagfile) < 0)
			should_create = 1;
	}
	if (should_create) {
		check_remove (dbpaths->tmpdirfile);
		check_remove (dbpaths->tmppagfile);
		ret = create_db (manpath, catpath);
		if (ret < 0)
			return ret;
		amount = ret;
	} else {
		ret = update_db_wrapper (manpath, catpath);
		if (ret < 0)
			return ret;
		amount = ret;
	}
#  endif /* BERKELEY_DB NDBM */
#else /* !NDBM */
	dbpaths->xfile = dbname; /* steal memory */
	dbpaths->xtmpfile = xstrdup (database);
	if (!should_create) {
		if (xcopy (dbpaths->xfile, dbpaths->xtmpfile) < 0)
			should_create = 1;
	}
	if (should_create) {
		check_remove (dbpaths->xtmpfile);
		ret = create_db (manpath, catpath);
		if (ret < 0)
			return ret;
		amount = ret;
	} else {
		ret = update_db_wrapper (manpath, catpath);
		if (ret < 0)
			return ret;
		amount = ret;
	}
#endif /* NDBM */

	return amount;
}

static int process_manpath (const char *manpath, int global_manpath,
			    struct hashtable *tried_catdirs)
{
	char *catpath;
	struct tried_catdirs_entry *tried;
	struct stat st;
	int run_mandb = 0;
	struct dbpaths *dbpaths;
	int amount = 0;

	if (global_manpath) { 	/* system db */
		catpath = get_catpath (manpath, SYSTEM_CAT);
		assert (catpath);
	} else {		/* user db */
		catpath = get_catpath (manpath, USER_CAT);
		if (!catpath)
			catpath = xstrdup (manpath);
	}
	tried = XMALLOC (struct tried_catdirs_entry);
	tried->manpath = xstrdup (manpath);
	tried->seen = 0;
	hashtable_install (tried_catdirs, catpath, strlen (catpath), tried);

	if (stat (manpath, &st) < 0 || !S_ISDIR (st.st_mode))
		return 0;
	tried->seen = 1;

	if (single_filename) {
		/* The file might be in a per-locale subdirectory that we
		 * aren't processing right now.
		 */
		char *manpath_prefix = xasprintf ("%s/man", manpath);
		if (STRNEQ (manpath_prefix, single_filename,
		    strlen (manpath_prefix)))
			run_mandb = 1;
		free (manpath_prefix);
	} else
		run_mandb = 1;

	force_rescan = 0;
	if (purge) {
		database = mkdbname (catpath);
		purged += purge_missing (manpath, catpath, run_mandb);
		free (database);
		database = NULL;
	}

	dbpaths = XZALLOC (struct dbpaths);
	push_cleanup (cleanup, dbpaths, 0);
	push_cleanup (cleanup_sigsafe, dbpaths, 1);
	if (run_mandb) {
		int ret = mandb (dbpaths, catpath, manpath, global_manpath);
		if (ret < 0) {
			amount = ret;
			goto out;
		}
		amount += ret;
	}

	if (!opt_test && amount)
		finish_up (dbpaths);
#ifdef MAN_OWNER
	if (global_manpath)
		do_chown (dbpaths);
#endif /* MAN_OWNER */

out:
	cleanup_sigsafe (dbpaths);
	pop_cleanup (cleanup_sigsafe, dbpaths);
	cleanup (dbpaths);
	pop_cleanup (cleanup, dbpaths);
	free (database);
	database = NULL;

	if (check_for_strays && amount > 0) {
		database = mkdbname (catpath);
		strays += straycats (manpath);
		free (database);
		database = NULL;
	}

	free (catpath);

	return amount;
}

static int is_lang_dir (const char *base)
{
	return strlen (base) >= 2 &&
	       base[0] >= 'a' && base[0] <= 'z' &&
	       base[1] >= 'a' && base[1] <= 'z' &&
	       (!base[2] || base[2] < 'a' || base[2] > 'z');
}

static void tried_catdirs_free (void *defn)
{
	struct tried_catdirs_entry *tried = defn;

	free (tried->manpath);
	free (tried);
}

static void purge_catdir (const struct hashtable *tried_catdirs,
			  const char *path)
{
	struct stat st;

	if (stat (path, &st) == 0 && S_ISDIR (st.st_mode) &&
	    !hashtable_lookup (tried_catdirs, path, strlen (path))) {
		if (!quiet)
			printf (_("Removing obsolete cat directory %s...\n"),
				path);
		remove_directory (path, 1);
	}
}

static void purge_catsubdirs (const char *manpath, const char *catpath)
{
	DIR *dir;
	struct dirent *ent;
	struct stat st;

	dir = opendir (catpath);
	if (!dir)
		return;
	while ((ent = readdir (dir)) != NULL) {
		char *mandir, *catdir;

		if (!STRNEQ (ent->d_name, "cat", 3))
			continue;

		mandir = xasprintf ("%s/man%s", manpath, ent->d_name + 3);
		catdir = xasprintf ("%s/%s", catpath, ent->d_name);

		if (stat (mandir, &st) != 0 && errno == ENOENT) {
			if (!quiet)
				printf (_("Removing obsolete cat directory "
					  "%s...\n"), catdir);
			remove_directory (catdir, 1);
		}

		free (catdir);
		free (mandir);
	}
	closedir (dir);
}

/* Remove catdirs whose corresponding mandirs no longer exist.  For safety,
 * in case people set catdirs to silly locations, we only do this for the
 * cat* and NLS subdirectories of catdirs, but not for the top-level catdir
 * itself (which might contain other data, or which might be difficult for
 * mandb to recreate with the proper permissions).
 *
 * We need to be careful here to avoid removing catdirs just because we
 * happened not to inspect the corresponding mandir this time round.  If a
 * mandir was inspected and turned out not to exist, then its catdir is
 * clearly fair game for removal of NLS subdirectories.  These must match
 * the usual NLS pattern (two lower-case letters followed by nothing or a
 * non-letter).
 */
static void purge_catdirs (const struct hashtable *tried_catdirs)
{
	struct hashtable_iter *iter = NULL;
	const struct nlist *elt;

	while ((elt = hashtable_iterate (tried_catdirs, &iter)) != NULL) {
		const char *path = elt->name;
		struct tried_catdirs_entry *tried = elt->defn;
		char *base;
		DIR *dir;
		struct dirent *subdirent;

		base = base_name (path);
		if (is_lang_dir (base)) {
			/* expect to check this as a subdirectory later */
			free (base);
			continue;
		}
		free (base);

		purge_catsubdirs (tried->manpath, path);

		dir = opendir (path);
		if (!dir)
			continue;
		while ((subdirent = readdir (dir)) != NULL) {
			char *subdirpath;

			if (STREQ (subdirent->d_name, ".") ||
			    STREQ (subdirent->d_name, ".."))
				continue;
			if (STRNEQ (subdirent->d_name, "cat", 3))
				continue;
			if (!is_lang_dir (subdirent->d_name))
				continue;

			subdirpath = xasprintf ("%s/%s", path,
					        subdirent->d_name);

			tried = hashtable_lookup (tried_catdirs, subdirpath,
						  strlen (subdirpath));
			if (tried && tried->seen) {
				debug ("Seen mandir for %s; not deleting\n",
				       subdirpath);
				/* However, we may still need to purge cat*
				 * subdirectories.
				 */
				purge_catsubdirs (tried->manpath, subdirpath);
			} else
				purge_catdir (tried_catdirs, subdirpath);

			free (subdirpath);
		}
		closedir (dir);
	}
}

int main (int argc, char *argv[])
{
	char *sys_manp;
	int amount = 0;
	char **mp;
	struct hashtable *tried_catdirs;
#ifdef SIGPIPE
	struct sigaction sa;
#endif /* SIGPIPE */

#ifdef __profile__
	char *cwd;
#endif /* __profile__ */

	program_name = base_name (argv[0]);

	init_debug ();
	pipeline_install_post_fork (pop_all_cleanups);
	init_locale ();

#ifdef SIGPIPE
	/* Reset SIGPIPE to its default disposition.  Too many broken pieces
	 * of software (Python << 3.2, gnome-session, etc.) spawn child
	 * processes with SIGPIPE ignored, and this produces noise in cron
	 * mail.
	 */
	memset (&sa, 0, sizeof sa);
	sa.sa_handler = SIG_DFL;
	sigemptyset (&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction (SIGPIPE, &sa, NULL);
#endif /* SIGPIPE */

	if (argp_parse (&argp, argc, argv, 0, 0, 0))
		exit (FAIL);

#ifdef __profile__
	cwd = xgetcwd ();
	if (!cwd) {
		cwd = xmalloc (1);
		cwd[0] = '\0';
	}
#endif /* __profile__ */

#ifdef MAN_OWNER
	/* record who we are and drop effective privs for later use */
	init_security ();
#endif /* MAN_OWNER */

#ifdef MAN_OWNER
	man_owner = getpwnam (MAN_OWNER);
	if (man_owner == NULL)
		error (FAIL, 0,
		       _("the setuid man user \"%s\" does not exist"),
		       MAN_OWNER);
	if (!user && euid != 0 && euid != man_owner->pw_uid)
		user = 1;
#endif /* MAN_OWNER */

	read_config_file (user);

	/* This is required for get_catpath(), regardless */
	manp = get_manpath (NULL);	/* also calls read_config_file() */

	/* pick up the system manpath or use the supplied one */
	if (arg_manp) {
		free (manp);
		manp = xstrdup (arg_manp);
	} else if (!user) {
		sys_manp = get_mandb_manpath ();
		if (sys_manp) {
			free (manp);
			manp = sys_manp;
		} else
			error (0, 0,
			       _("warning: no MANDB_MAP directives in %s, "
				 "using your manpath"),
			       CONFIG_FILE);
	}

	debug ("manpath=%s\n", manp);

	/* get the manpath as an array of pointers */
	create_pathlist (manp, manpathlist); 

	/* finished manpath processing, regain privs */
	regain_effective_privs ();

	tried_catdirs = hashtable_create (tried_catdirs_free);

	for (mp = manpathlist; *mp; mp++) {
		int global_manpath = is_global_mandir (*mp);
		int ret;
		DIR *dir;
		struct dirent *subdirent;

		if (global_manpath) {	/* system db */
			if (user)
				continue;
		} else {		/* user db */
			drop_effective_privs ();
		}

		ret = process_manpath (*mp, global_manpath, tried_catdirs);
		if (ret < 0)
			exit (FATAL);
		amount += ret;

		dir = opendir (*mp);
		if (!dir) {
			error (0, errno, _("can't search directory %s"), *mp);
			goto next_manpath;
		}

		while ((subdirent = readdir (dir)) != NULL) {
			char *subdirpath;

			/* Look for per-locale subdirectories. */
			if (STREQ (subdirent->d_name, ".") ||
			    STREQ (subdirent->d_name, ".."))
				continue;
			if (STRNEQ (subdirent->d_name, "man", 3))
				continue;

			subdirpath = xasprintf ("%s/%s", *mp,
						subdirent->d_name);
			ret = process_manpath (subdirpath, global_manpath,
					       tried_catdirs);
			if (ret < 0)
				exit (FATAL);
			amount += ret;
			free (subdirpath);
		}

		closedir (dir);

next_manpath:
		if (!global_manpath)
			regain_effective_privs ();

		chkr_garbage_detector ();
	}

	purge_catdirs (tried_catdirs);
	hashtable_free (tried_catdirs);

	if (!quiet) {
		printf (ngettext ("%d man subdirectory contained newer "
				  "manual pages.\n",
				  "%d man subdirectories contained newer "
				  "manual pages.\n", amount),
			amount);
		printf (ngettext ("%d manual page was added.\n",
				  "%d manual pages were added.\n", pages),
			pages);
		if (check_for_strays)
			printf (ngettext ("%d stray cat was added.\n",
					  "%d stray cats were added.\n",
					  strays),
			        strays);
		if (purge)
			printf (ngettext ("%d old database entry was "
					  "purged.\n",
					  "%d old database entries were "
					  "purged.\n", purged),
				purged);
	}

#ifdef __profile__
	/* For profiling */
	if (cwd[0])
		chdir (cwd);
#endif /* __profile__ */

	free_pathlist (manpathlist);
	free (manp);
	if (create && !amount) {
		const char *must_create;
		if (!quiet)
			fprintf (stderr, _("No databases created."));
		must_create = getenv ("MAN_MUST_CREATE");
		if (must_create && STREQ (must_create, "1"))
			exit (FAIL);
	}
	free (program_name);
	exit (OK);
}
