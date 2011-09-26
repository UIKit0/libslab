/*
 * This file is part of libslab.
 *
 * Copyright (c) 2006 Novell, Inc.
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

#include "search-entry.h"
#include "search-entry-watermark.h"
#include <string.h>

typedef struct
{
	int width, height;
} NldSearchEntryPrivate;

#define NLD_SEARCH_ENTRY_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), NLD_TYPE_SEARCH_ENTRY, NldSearchEntryPrivate))

static void nld_search_entry_class_init (NldSearchEntryClass *);
static void nld_search_entry_init (NldSearchEntry *);
static void nld_search_entry_finalize (GObject *);

G_DEFINE_TYPE (NldSearchEntry, nld_search_entry, GTK_TYPE_ENTRY)

static void nld_search_entry_class_init (NldSearchEntryClass * nld_search_entry_class)
{
	GObjectClass *g_obj_class = G_OBJECT_CLASS (nld_search_entry_class);

	g_type_class_add_private (nld_search_entry_class, sizeof (NldSearchEntryPrivate));

	g_obj_class->finalize = nld_search_entry_finalize;
}

static void
nld_search_entry_init (NldSearchEntry * entry)
{
}

static void
nld_search_entry_finalize (GObject * object)
{
	G_OBJECT_CLASS (nld_search_entry_parent_class)->finalize (object);
}

GtkWidget *
nld_search_entry_new (void)
{
	return g_object_new (NLD_TYPE_SEARCH_ENTRY, NULL);
}
