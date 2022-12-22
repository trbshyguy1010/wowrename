#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <dirent.h>

// the actual foni
char *strremove(char *str, const char *sub) {
    size_t len = strlen(sub);
    if (len > 0) {
        char *p = str;
        while ((p = strstr(p, sub)) != NULL) {
            memmove(p, p + len, strlen(p + len) + 1);
        }
    }
    return str;
}

GtkWidget *g_lbl_display;
char* folder_path;

int ren() {
    printf("%s\n", folder_path);
    char* fp = folder_path;
    char* fp2;
    DIR *dir;
    struct dirent *entry;
    char oldname[1024], newname[1038]; // ...idk man
    char newname_finalized[1024];
    char oldname_finalized[1038];
    dir = opendir(folder_path);
    if (!dir) {
        perror("Error 1: Invalid Directory");
        g_free(folder_path);
        return 1;
    }
    strcat(fp, "/");
    int i = 0;
    while ((entry = readdir(dir)) != NULL) {
        char ich[300];
        char chasch[0];

        if (strncmp(entry->d_name, ".", 1) == 0
            || strncmp(entry->d_name, "..", 2) == 0) {
                continue;
        }

        // store old variables for later use
        strcpy(oldname, entry->d_name);
        strcpy(newname, entry->d_name);

        // add the string (thats a lot of strcpy() functions)
        strcpy(oldname_finalized, oldname);
        sprintf(ich, "%d", i);
        strcat(ich, "_");
        strcat(ich, newname);
        strcpy(newname, ich);
        i++;
        strcpy(newname_finalized, newname);

        // out with the old
        strcat(fp, oldname);
        strcpy(oldname, fp);

        // do this funny thing...
        fp2 = strremove(fp, oldname_finalized);

        // in with the new
        strcat(fp2, newname);
        strcpy(newname, fp2);

        // renaming algorithm, wow very cool...
        int ren_res = rename(oldname, newname);
        if (ren_res != 0) {
            perror("Error 2: renaming");
            return 2;
        } else { printf("File %d...Success!\n", i);}
        fp2 = strremove(fp, newname_finalized);
        strcpy(fp, fp2);
    }
    closedir(dir);
    return 0;
}

void btn_submit_clicked() {
    printf("Clicked!\n");
    gtk_label_set_text(GTK_LABEL(g_lbl_display), "Renaming in Progress...\n");
    if (ren() == 0) {
        gtk_label_set_text(GTK_LABEL(g_lbl_display), "Renamed Successfully\n");
    } else {
        gtk_label_set_text(GTK_LABEL(g_lbl_display), "There was an error during rename...\n");
    }
}

void on_file_set(GtkFileChooserButton *btn, gpointer user_data) {
    folder_path = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(btn));
    printf("Selected folder %s\n", folder_path);
    gtk_label_set_text(GTK_LABEL(g_lbl_display), folder_path);
}

int main (int argc, char **argv)
{
    gtk_init(&argc, &argv);

    GtkBuilder *builder;
    GtkWidget *window;
    GtkWidget *g_btn_submit;
    GtkWidget *folder_sel;
    GdkPixbuf *icon;

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "wow_rename.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "mainWindow"));
    g_signal_connect(window, "destroy", gtk_main_quit, NULL);
    gtk_window_set_title(GTK_WINDOW(window), "WowRename v0.1a");

    icon = gdk_pixbuf_new_from_file("wowrenameico.ico", NULL);
    gtk_window_set_icon(GTK_WINDOW(window), icon);

    g_lbl_display = GTK_WIDGET(gtk_builder_get_object(builder, "log_label"));
    folder_sel = GTK_WIDGET(gtk_builder_get_object(builder, "folder_select"));
    g_signal_connect(G_OBJECT(folder_sel), "file-set", G_CALLBACK(on_file_set), NULL);

    g_btn_submit = GTK_WIDGET(gtk_builder_get_object(builder, "rename_btn"));
    g_signal_connect(G_OBJECT(g_btn_submit), "clicked", G_CALLBACK(btn_submit_clicked), (gpointer)window);

    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);

    gtk_widget_show(window);
    gtk_main();
    return 0;
}
