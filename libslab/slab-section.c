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

#include "slab-section.h"

G_DEFINE_TYPE (SlabSection, slab_section, GTK_TYPE_BOX)

void
slab_section_set_selected (SlabSection * section, gboolean selected)
{
	if (selected == section->selected)
		return;
	section->selected = selected;
	gtk_widget_set_sensitive (section->title, selected);
}

GtkWidget *
slab_section_new_with_markup (const gchar * title_markup, SlabStyle style)
{
	SlabSection *section;
	GtkWidget *align;
	gchar * widget_theming_name;

	section = g_object_new (SLAB_SECTION_TYPE, NULL);
	gtk_box_set_homogeneous (GTK_BOX (section), FALSE);
	gtk_box_set_spacing (GTK_BOX (section), 0);
	section->style = style;
	section->selected = FALSE;

	align = gtk_alignment_new (0.5, 0.5, 1.0, 1.0);
	switch (style)
	{
	case Style1:
		gtk_alignment_set_padding (GTK_ALIGNMENT (align), 0, 0, 0, 0);
		widget_theming_name = "slab_section_style1";
		break;
	case Style2:
		gtk_alignment_set_padding (GTK_ALIGNMENT (align), SLAB_TOP_PADDING,
			SLAB_BOTTOM_PADDING, SLAB_LEFT_PADDING, 0);
		widget_theming_name = "slab_section_style2";
		break;
	default:
		g_assert_not_reached ();
	}
	gtk_box_pack_start (GTK_BOX (section), align, TRUE, TRUE, 0);

	section->childbox = GTK_BOX (gtk_box_new (FALSE, 10));
	gtk_orientable_set_orientation (GTK_ORIENTABLE (section->childbox),
					GTK_ORIENTATION_VERTICAL);
	gtk_container_add (GTK_CONTAINER (align), GTK_WIDGET (section->childbox));

	section->title = gtk_label_new (title_markup);
	gtk_widget_set_sensitive (section->title, FALSE);
	gtk_label_set_use_markup (GTK_LABEL (section->title), TRUE);
	gtk_misc_set_alignment (GTK_MISC (section->title), 0.0, 0.5);

	gtk_widget_set_name (GTK_WIDGET (section), widget_theming_name);

	gtk_box_pack_start (section->childbox, section->title, FALSE, FALSE, 0);

	return GTK_WIDGET (section);
}

GtkWidget *
slab_section_new (const gchar * title, SlabStyle style)
{
	GtkWidget *section;
	gchar *markup;

	markup = g_strdup_printf ("<span size=\"large\" weight=\"bold\">%s</span>", title);
	section = slab_section_new_with_markup (markup, style);

	g_free (markup);

	return section;
}

void
slab_section_set_title (SlabSection * section, const gchar * title)
{
	gchar *markup = g_strdup_printf ("<span size=\"large\">%s</span>", title);

	gtk_label_set_markup (GTK_LABEL (section->title), markup);

	g_free (markup);
}

void
slab_section_set_contents (SlabSection * section, GtkWidget * contents)
{
	section->contents = contents;

	gtk_box_pack_start (section->childbox, contents, FALSE, FALSE, 0);
}

static gboolean
slab_section_draw (GtkWidget *widget, cairo_t *cr)
{
	SlabSection *section = SLAB_SECTION (widget);

#if 1
	if (section->selected)
	{
		GdkRGBA rgba;
		GtkAllocation allocation;
		GtkStyleContext *context;

		gtk_widget_get_allocation (widget, &allocation);

		/* set the correct source color */
		context = gtk_widget_get_style_context (widget);
		gtk_style_context_get_color (context, GTK_STATE_SELECTED, &rgba);
		rgba.alpha = 0.2;
		gdk_cairo_set_source_rgba (cr, &rgba);

		g_warning ("TESTME: render slab section selected %d %d %d %d",
			   allocation.x, allocation.y,
			   allocation.width * 2, allocation.height);

		cairo_rectangle (cr, allocation.x, allocation.y,
				 allocation.width * 2, allocation.height);
		cairo_fill (cr);
	}
#endif

	GTK_WIDGET_CLASS (slab_section_parent_class)->draw (widget, cr);

	return FALSE;
}

static void
slab_section_finalize (GObject * obj)
{
	g_assert (IS_SLAB_SECTION (obj));
	(*G_OBJECT_CLASS (slab_section_parent_class)->finalize) (obj);
}

static void slab_section_class_init (SlabSectionClass * slab_section_class)
{
	GObjectClass *g_obj_class = G_OBJECT_CLASS (slab_section_class);

	g_obj_class->finalize = slab_section_finalize;

	GTK_WIDGET_CLASS (slab_section_class)->draw = slab_section_draw;
}

static void
slab_section_init (SlabSection * section)
{
	section->title = NULL;
	section->contents = NULL;
	gtk_orientable_set_orientation (GTK_ORIENTABLE (section),
					GTK_ORIENTATION_VERTICAL);
}
