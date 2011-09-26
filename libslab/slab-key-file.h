/*
 * This file is part of libslab.
 *
 * Copyright (c) 2011 Novell, Inc.
 *
 * Libslab is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * Libslab is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libslab; if not, write to the Free Software Foundation, Inc., 51
 * Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __SLAB_KEY_FILE_H__
#define __SLAB_KEY_FILE_H__

#include <glib.h>

G_BEGIN_DECLS

typedef struct _SlabKeyFile SlabKeyFile;

SlabKeyFile      *slab_key_file_new                 (void);
SlabKeyFile      *slab_key_file_new_from_path       (const gchar *file_path);
SlabKeyFile      *slab_key_file_new_from_unknown_id (const gchar *id);

void              slab_key_file_save                (SlabKeyFile *item,
						     const gchar *path);

SlabKeyFile      *slab_key_file_ref   (SlabKeyFile *item);
void              slab_key_file_unref (SlabKeyFile *item);

char             *slab_key_file_get (SlabKeyFile *keyfile, const char *key);
char             *slab_key_file_get_location (SlabKeyFile *keyfile);
char             *slab_key_file_get_locale (SlabKeyFile *keyfile, const char *key);

void              slab_key_file_set_string (SlabKeyFile *keyfile, const char *group,
					    const char *key, const char *value);

gboolean          slab_key_file_launch_default (SlabKeyFile *item);
gchar            *slab_key_file_get_docpath (SlabKeyFile *item);
gboolean          slab_key_file_open_help (SlabKeyFile *item);

gboolean          slab_key_file_is_a_terminal (const gchar *uri);
gboolean          slab_key_file_is_logout (const gchar *uri);
gboolean          slab_key_file_is_lockscreen (const gchar *uri);



G_END_DECLS

#endif /* __SLAB_KEY_FILE_H__ */
