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

#include "shell-window.h"

#include <gtk/gtk.h>

#include "app-resizer.h"

static void shell_window_class_init (ShellWindowClass *);
static void shell_window_init (ShellWindow *);

G_DEFINE_TYPE (ShellWindow, shell_window, GTK_TYPE_FRAME);

static GtkSizeRequestMode
shell_window_get_request_mode (GtkWidget *widget)
{
  return GTK_SIZE_REQUEST_CONSTANT_SIZE;
}

static void
shell_window_get_preferred_height (GtkWidget       *widget,
				   gint            *minimum_height,
				   gint            *natural_height)
{
	AppShellData * app_data = SHELL_WINDOW (widget)->data;

	GTK_WIDGET_CLASS (shell_window_parent_class)->get_preferred_height
					(widget, minimum_height, natural_height);
	*natural_height = (double)gdk_screen_height () * SIZING_HEIGHT_PERCENT;
//	fprintf (stderr, "heights: %d %d\n", *minimum_height, *natural_height);
}

static void
shell_window_get_preferred_width (GtkWidget       *widget,
				  gint            *minimum_width,
				  gint            *natural_width)
{
	AppShellData * app_data = SHELL_WINDOW (widget)->data;

	GTK_WIDGET_CLASS (shell_window_parent_class)->get_preferred_width
					(widget, minimum_width, natural_width);

	*natural_width = MAX (((gfloat) gdk_screen_width () * SIZING_HEIGHT_PERCENT), *natural_width);
//	fprintf (stderr, "widths: %d %d\n", *minimum_width, *natural_width);
}

static void
shell_window_class_init (ShellWindowClass * klass)
{
	GtkWidgetClass *wklass = GTK_WIDGET_CLASS (klass);

	wklass->get_request_mode = shell_window_get_request_mode;
	wklass->get_preferred_width = shell_window_get_preferred_width;
	wklass->get_preferred_height = shell_window_get_preferred_height;
}

static void
shell_window_init (ShellWindow * window)
{
	window->_hbox = NULL;
	window->_left_pane = NULL;
	window->_right_pane = NULL;
}

void
shell_window_clear_resize_handler (ShellWindow * win)
{
}

void
shell_window_set_contents (ShellWindow * shell, GtkWidget * left_pane, GtkWidget * right_pane)
{
	shell->_left_pane = gtk_alignment_new (0.5, 0.5, 1.0, 1.0);
	shell->_right_pane = gtk_alignment_new (0.5, 0.5, 1.0, 1.0);

	gtk_alignment_set_padding (GTK_ALIGNMENT (shell->_left_pane), 15, 15, 15, 15);
	gtk_alignment_set_padding (GTK_ALIGNMENT (shell->_right_pane), 0, 0, 0, 0);	/* space for vertical line */

	gtk_box_pack_start (shell->_hbox, shell->_left_pane, FALSE, FALSE, 0);
	gtk_box_pack_start (shell->_hbox, shell->_right_pane, TRUE, TRUE, 0);	/* this one takes any extra space */

	gtk_container_add (GTK_CONTAINER (shell->_left_pane), left_pane);
	gtk_container_add (GTK_CONTAINER (shell->_right_pane), right_pane);
}

GtkWidget *
shell_window_new (AppShellData * app_data)
{
	ShellWindow *window = g_object_new (SHELL_WINDOW_TYPE, NULL);

	window->data = app_data;

	gtk_widget_set_app_paintable (GTK_WIDGET (window), TRUE);
	gtk_frame_set_shadow_type(GTK_FRAME(window), GTK_SHADOW_NONE);

	window->_hbox = GTK_BOX (gtk_box_new (FALSE, 0));
	gtk_orientable_set_orientation (GTK_ORIENTABLE (window->_hbox),
					GTK_ORIENTATION_HORIZONTAL);
	gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET (window->_hbox));

	return GTK_WIDGET (window);
}
