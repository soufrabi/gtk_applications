#include <gtk/gtk.h>

static void app_activate(GApplication *app, gpointer *user_data) {
    g_printerr("Please pass a filename argument.\n");
}

static void app_open(GApplication *app, GFile **files, gint n_files,
                     gchar *hint) {
    GtkWidget *win;
    GtkWidget *nb;
    GtkWidget *lab;
    GtkNotebookPage *nbp;
    GtkWidget *scr;
    GtkWidget *tv;
    GtkTextBuffer *tb;
    char *contents;
    gsize length;
    char *filename;
    GError *err = NULL;
    int i;

    win = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_default_size(GTK_WINDOW(win), 400, 300);
    gtk_window_set_title(GTK_WINDOW(win), "File Viewer");

    nb = gtk_notebook_new();
    gtk_window_set_child(GTK_WINDOW(win), nb);

    for (i = 0; i < n_files; i++) {
        g_print("Processing File number %d.\n", i);

        if (g_file_load_contents(files[i], NULL, &contents, &length, NULL,
                                 &err)) {
            scr = gtk_scrolled_window_new();
            tv = gtk_text_view_new();
            tb = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv));
            gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(tv), GTK_WRAP_WORD_CHAR);
            gtk_text_view_set_editable(GTK_TEXT_VIEW(tv), FALSE);
            gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scr), tv);
            gtk_text_buffer_set_text(tb, contents, length);
            g_free(contents);

            if ((filename = g_file_get_basename(files[i])) != NULL) {

                lab = gtk_label_new(filename);
                g_free(filename);
            }else {
                lab = gtk_label_new("[No Name]");
            }
            
            gtk_notebook_append_page (GTK_NOTEBOOK(nb),scr,lab);
            nbp = gtk_notebook_get_page(GTK_NOTEBOOK(nb),scr);
            g_object_set(nbp, "tab-expand", TRUE,NULL);

        } else {
            g_printerr("%s\n", err->message);
            /* g_error_free(err); */
            g_clear_error(&err);
        }
    }

    if (1){
            gtk_window_present(GTK_WINDOW(win));
    }else {
            gtk_window_destroy(GTK_WINDOW(win));
    }
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;
    // accepts cli arguments but only filenames
    app = gtk_application_new("com.github.To", G_APPLICATION_HANDLES_OPEN);
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
    g_signal_connect(app, "open", G_CALLBACK(app_open), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
