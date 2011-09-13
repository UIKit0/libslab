#ifndef __LIBSLAB_UTILS_H__
#define __LIBSLAB_UTILS_H__

#include <glib.h>
#include <gtk/gtk.h>
#include <gconf/gconf-client.h>
#define GNOME_DESKTOP_USE_UNSTABLE_API 1
#include <libgnome-desktop/gnome-desktop-thumbnail.h>

G_BEGIN_DECLS

char             *libslab_keyfile_get (GKeyFile *keyfile, const char *key);
char             *libslab_keyfile_get_location (GKeyFile *keyfile);
char             *libslab_keyfile_get_locale (GKeyFile *keyfile, const char *key);

gboolean          libslab_gtk_image_set_by_id (GtkImage *image, const gchar *id);
GKeyFile *libslab_gnome_desktop_item_new_from_unknown_id (const gchar *id);
gboolean          libslab_gnome_desktop_item_launch_default (GKeyFile *item);
gchar            *libslab_gnome_desktop_item_get_docpath (GKeyFile *item);
gboolean          libslab_gnome_desktop_item_open_help (GKeyFile *item);
guint32           libslab_get_current_time_millis (void);
gint              libslab_strcmp (const gchar *a, const gchar *b);
gint              libslab_strlen (const gchar *a);
gpointer          libslab_get_gconf_value (const gchar *key);
void              libslab_set_gconf_value (const gchar *key, gconstpointer data);
guint             libslab_gconf_notify_add (const gchar *key, GConfClientNotifyFunc callback, gpointer user_data);
void              libslab_gconf_notify_remove (guint conn_id);
void              libslab_handle_g_error (GError **error, const gchar *msg_format, ...);
gboolean          libslab_desktop_item_is_a_terminal (const gchar *uri);
gboolean          libslab_desktop_item_is_logout (const gchar *uri);
gboolean          libslab_desktop_item_is_lockscreen (const gchar *uri);
gchar            *libslab_string_replace_once (const gchar *string, const gchar *key, const gchar *value);
void              libslab_spawn_command (const gchar *cmd);

GdkScreen *libslab_get_current_screen (void);

void libslab_thumbnail_factory_preinit (void);
GnomeDesktopThumbnailFactory *libslab_thumbnail_factory_get (void);

void libslab_checkpoint_init (const char *checkpoint_config_file_basename, const char *checkpoint_file_basename);
void libslab_checkpoint (const char *format, ...);

G_END_DECLS

#endif
