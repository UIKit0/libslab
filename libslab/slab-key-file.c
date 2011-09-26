#include <config.h>
#include <string.h>
#include <libslab/slab-key-file.h>

#define DESKTOP_ITEM_TERMINAL_EMULATOR_FLAG "TerminalEmulator"
#define ALTERNATE_DOCPATH_KEY               "DocPath"

/*
 * This guy has to track the file path we loaded from
 * which is required by lots of the later slab code.
 */
struct _SlabKeyFile {
  int ref;
  GKeyFile *kf;
  char *filename;
};

#define EVIL_KEY_GROUP "evilgroup"
#define EVIL_KEY_VALUE "evilfilename"

char *
slab_key_file_get_location (SlabKeyFile *keyfile)
{
	return g_strdup (keyfile->filename);
}

static SlabKeyFile *
slab_key_file_new_with (GKeyFile *kf, const char *path)
{
	SlabKeyFile *item = g_new0 (SlabKeyFile, 1);

	item->ref = 1;
	item->kf = kf;
	item->filename = g_strdup (path);

	return item;
}

SlabKeyFile *
slab_key_file_ref (SlabKeyFile *item)
{
	g_return_val_if_fail (item != NULL, NULL);
	item->ref++;
	return item;
}

void
slab_key_file_unref (SlabKeyFile *item)
{
  g_return_if_fail (item != NULL);
  if (--item->ref == 0) {
	g_key_file_free (item->kf);
	g_free (item->filename);
	item->kf = NULL;
	item->filename = NULL;
  }
}

SlabKeyFile *
slab_key_file_new (void)
{
	return slab_key_file_new_with (g_key_file_new(), NULL);
}

SlabKeyFile *
slab_key_file_new_from_path (const gchar *file_path)
{
	GKeyFile *kf;
	GError *error = NULL;

	if (! file_path)
		return NULL;

	kf = g_key_file_new();
	g_key_file_load_from_file (kf, file_path, 0, &error);

	if (! error)
		return slab_key_file_new_with (kf, file_path);
	else {
		g_error_free (error);
		error = NULL;
	}
	return NULL;
}

SlabKeyFile *
slab_key_file_new_from_unknown_id (const gchar *id)
{
	SlabKeyFile *item;
	gchar *basename;

	item = slab_key_file_new_from_path (id);
	if (item)
		return item;

	basename = g_strrstr (id, "/");
	if (basename) {
		basename++;

		item = slab_key_file_new_from_path (id);
		if (item)
			return item;
	}

#ifdef FIXME_MORE_PORTING
	item = gnme_desktop_item_new_from_file (id, 0, & error);

	if (! error)
		return item;
	else {
		g_error_free (error);
		error = NULL;
	}

	item = gnme_desktop_item_new_from_basename (id, 0, & error);

	if (! error)
		return item;
	else {
		g_error_free (error);
		error = NULL;
	}
#endif

	return NULL;
}

void
slab_key_file_save (SlabKeyFile *item, const gchar *path)
{
	gsize  length;
	gchar *flat_text = g_key_file_to_data (item->kf, &length, NULL);
	g_file_set_contents (path, flat_text, length, NULL);
	g_free (flat_text);
}

gboolean
slab_key_file_launch_default (SlabKeyFile *item)
{
	GError *error = NULL;

	if (! item)
		return FALSE;

	g_warning ("desktop item launch !"); /* FIXME more porting required */
	/*	gnme_desktop_item_launch (item, NULL, GNOME_DESKTOP_ITEM_LAUNCH_ONLY_ONE, & error); */

	if (error) {
		g_warning ("error launching %s [%s]\n",
			   slab_key_file_get_location (item),
			   error->message);

		g_error_free (error);

		return FALSE;
	}

	return TRUE;
}

gchar *
slab_key_file_get_docpath (SlabKeyFile *item)
{
	gchar *path;

	path = slab_key_file_get_locale (item, "DocPath");
	if (! path)
		path = slab_key_file_get_locale (item, ALTERNATE_DOCPATH_KEY);

	return path;
}

gboolean
slab_key_file_open_help (SlabKeyFile *item)
{
	gchar *doc_path;
	gchar *help_uri;

	GError *error = NULL;

	gboolean retval = FALSE;


	if (! item)
		return retval;

	doc_path = slab_key_file_get_docpath (item);

	if (doc_path) {
		help_uri = g_strdup_printf ("ghelp:%s", doc_path);

		if (!gtk_show_uri (libslab_get_current_screen (), help_uri, gtk_get_current_event_time (), &error)) {
			g_warning ("error opening %s [%s]\n", help_uri, error->message);

			g_error_free (error);

			retval = FALSE;
		}
		else
			retval = TRUE;

		g_free (help_uri);
		g_free (doc_path);
	}

	return retval;
}

gboolean
slab_key_file_is_a_terminal (const gchar *uri)
{
	SlabKeyFile *d_item;
	gchar *categories;

	gboolean is_terminal = FALSE;

	d_item = slab_key_file_new_from_unknown_id (uri);

	if (! d_item)
		return FALSE;

	categories = slab_key_file_get (d_item, G_KEY_FILE_DESKTOP_KEY_CATEGORIES);
	is_terminal = (categories && strstr (categories, DESKTOP_ITEM_TERMINAL_EMULATOR_FLAG));
	g_free (categories);

	slab_key_file_unref (d_item);

	return is_terminal;
}

gboolean
slab_key_file_is_logout (const gchar *uri)
{
	SlabKeyFile *d_item;
	gboolean is_logout = FALSE;
	gchar *item_name;

	d_item = slab_key_file_new_from_unknown_id (uri);

	if (! d_item)
		return FALSE;

	item_name = slab_key_file_get (d_item, G_KEY_FILE_DESKTOP_KEY_NAME);
	is_logout = strstr ("Logout", item_name) != NULL;
	g_free (item_name);

	slab_key_file_unref (d_item);

	return is_logout;
}

gboolean
slab_key_file_is_lockscreen (const gchar *uri)
{
	SlabKeyFile *d_item;
	gboolean is_logout = FALSE;
	gchar *item_name;

	d_item = slab_key_file_new_from_unknown_id (uri);

	if (! d_item)
		return FALSE;

	item_name = slab_key_file_get (d_item, G_KEY_FILE_DESKTOP_KEY_NAME);
	is_logout = strstr ("Lock Screen", item_name) != NULL;
	g_free (item_name);

	slab_key_file_unref (d_item);

	return is_logout;
}

char *
slab_key_file_get (SlabKeyFile *keyfile, const char *key)
{
	return g_key_file_get_value (keyfile->kf, G_KEY_FILE_DESKTOP_GROUP, key, NULL);
}

char *
slab_key_file_get_locale (SlabKeyFile *keyfile, const char *key)
{
	return g_key_file_get_locale_string (keyfile->kf, G_KEY_FILE_DESKTOP_GROUP,
					     key, NULL, NULL);
}

void
slab_key_file_set_string (SlabKeyFile *keyfile, const char *group,
			  const char *key, const char *value)
{
	g_key_file_set_string (keyfile->kf, group, key, value);
}
