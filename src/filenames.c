/*
 * filenames.c: compose and dissect man page file names
 *
 * Copyright (C) 1994, 1995 Graeme W. Wilford. (Wilf.)
 * Copyright (C) 2001, 2002 Colin Watson.
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
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>

#if defined(STDC_HEADERS)
#  include <string.h>
#  include <stdlib.h>
#elif defined(HAVE_STRING_H)
#  include <string.h>
#elif defined(HAVE_STRINGS_H)
#  include <strings.h>
#else /* no string(s) header */
extern char *strrchr();
#endif /* no string(s) header */

#include <libintl.h>
#define _(String) gettext (String)

#include "manconfig.h"
#include "libdb/db_storage.h"
#include "lib/error.h"
#include "filenames.h"

static void gripe_bogus_manpage (const char *manpage)
{
	if (quiet < 2)
		error (0, 0, _("warning: %s: ignoring bogus filename"),
		       manpage);
}	  	  

char *make_filename (const char *path, const char *name,
		     struct mandata *in, char *type)
{
	static char *file;

	if (!name)
		name = in->name;    /* comes from dblookup(), so non-NULL */

	file = (char *) xrealloc (file, sizeof "//." + strlen (path) + 
				  strlen (type) + strlen (in->sec) +
				  strlen (name) + strlen (in->ext));
				   
	(void) sprintf (file, "%s/%s%s/%s.%s",
			path, type, in->sec, name, in->ext);

	if (in->comp && *in->comp != '-')	/* Is there an extension? */
		file = strappend (file, ".", in->comp, NULL);

	return file;
}

/* Fill in a mandata structure with information about a file name.
 * file is the name to examine. info points to the structure to be filled
 * in. req_name is the page name that was requested.
 * 
 * Returns either a pointer to the buffer which the fields in info point
 * into, to be freed by the caller, or NULL on error. The buffer will
 * contain either three or four null-terminated strings: the directory name,
 * the base of the file name in that directory, the section extension, and
 * optionally the compression extension (if COMP_SRC is defined).
 * 
 * Only the fields name, ext, sec, and comp are filled in by this function.
 * name is only set if it differs from req_name; otherwise it remains at
 * NULL.
 */
char *filename_info (const char *file, struct mandata *info,
		     const char *req_name)
{
	char *manpage = xstrdup (file);
	char *base_name = basename (manpage);
#ifdef COMP_SRC
	struct compression *comp;
#endif

	/* Bogus files either have (i) no period, ie no extension, (ii)
	   a compression extension, but no sectional extension, (iii)
	   a missmatch between the section they are under and the
	   sectional part of their extension. */

#ifdef COMP_SRC
	comp = comp_info (base_name);
	if (comp) {
		info->comp = comp->ext;
		*(comp->file) = '\0';		/* to strip the comp ext */
	} else
		info->comp = NULL;
#else /* !COMP_SRC */	
	info->comp = NULL;
#endif /* COMP_SRC */

	{
		char *ext = strrchr (base_name, '.');
		if (!ext) {
			/* no section extension */
			gripe_bogus_manpage (file);
			free (manpage);
			return NULL;
		}
		*ext++ = '\0';			/* set section ext */
		info->ext = ext;
	}

	*(base_name - 1) = '\0';		/* strip '/base_name' */ 
	info->sec = strrchr (manpage, '/') + 4;	/* set section name */

	if (strncmp (info->sec, info->ext, strlen (info->sec)) != 0) {
		/* missmatch in extension */
		gripe_bogus_manpage (file);
		free (manpage);
		return NULL;
	}

	if (req_name && !STREQ (base_name, req_name))
		info->name = xstrdup (base_name);
	else
		info->name = NULL;

	return manpage;
}