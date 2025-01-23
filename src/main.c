#include <gtk/gtk.h>
#include "magazyn.h"
#include "gui.h"

static void activate(GtkApplication *app) {
    create_main_window(app);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.gtk.magazyn", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    load_inventory_from_file("src/savefile.txt");
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    free_inventory();

    return status;
}
