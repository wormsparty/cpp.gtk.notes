#include <gtk/gtk.h>
#include <libsoup/soup.h>
#include <glib.h>

struct window_info_t {
    GtkWindow* window;
    GtkInfoBar* bar;
    GtkLabel* message_label;
};

static bool send_message() {
    g_autoptr(SoupSession) session = soup_session_new ();
    g_autoptr(SoupMessage) message = soup_message_new (SOUP_METHOD_POST, "http://localhost:5000");

    const char* args = "foo=bar&one=two";

    g_autoptr(GBytes) args_b = g_bytes_new (args, strlen(args));
    soup_message_set_request_body_from_bytes(
        message, "application/x-www-form-urlencoded", args_b);

    GError *error = nullptr;
    g_autoptr(GBytes) bytes = soup_session_send_and_read(session, message, nullptr, &error);

    if (error) {
        g_printerr ("Failed to POST: %s\n", error->message);
        g_error_free (error);
        return false;
    }

    g_print ("Received: %s\n", (char*)g_bytes_get_data(bytes, nullptr));
    return true;
}

static void show_error_message(window_info_t* infos, const char* msg) {
    gtk_label_set_text (infos->message_label, msg);
    gtk_info_bar_set_message_type (infos->bar, GTK_MESSAGE_ERROR);
    gtk_widget_show (GTK_WIDGET (infos->bar));
}

static void folder_choice_closed(GtkFileChooser* chooser,
                                 GtkResponseType response,
                                 window_info_t* infos) {
    if (response == GTK_RESPONSE_ACCEPT) {
        g_autoptr(GFile) folderpath = gtk_file_chooser_get_current_folder(chooser);
        gchar* foldername = g_file_get_path(folderpath);
        g_print("%s\n", foldername);
        g_free(foldername);
    } else {
        show_error_message(infos, "Cancelled");
    }
}

static void folder_choice_opened(GtkWidget*, window_info_t* infos) {
    auto chooser = gtk_file_chooser_native_new("Choose a folder",
        GTK_WINDOW(infos->window),
        GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
        "_Open", "_Cancel");

    g_signal_connect(chooser, "response",
        G_CALLBACK(folder_choice_closed), infos);

    gtk_native_dialog_set_modal(GTK_NATIVE_DIALOG (chooser), true);
    gtk_native_dialog_show(GTK_NATIVE_DIALOG (chooser));
}

static void activate(GtkApplication* app, gpointer) {
    auto window = GTK_WINDOW (gtk_application_window_new (app));
    gtk_window_set_title (window, "GTK test");
    gtk_window_set_default_size (window, 640, 480);

    auto grid = GTK_GRID (gtk_grid_new());
    gtk_grid_set_row_spacing(grid, 5);
    gtk_grid_set_column_spacing(grid, 5);

    auto bar = gtk_info_bar_new ();
    gtk_widget_set_visible(bar, false);
    auto message_label = GTK_LABEL (gtk_label_new (""));
    gtk_info_bar_add_child (GTK_INFO_BAR (bar), GTK_WIDGET (message_label));
    gtk_info_bar_add_button (GTK_INFO_BAR (bar), "_OK", GTK_RESPONSE_OK);

    GtkWidget *button_send = gtk_button_new_with_label("Send message");
    GtkWidget *button_folder = gtk_button_new_with_label("Choose folder");
    GtkWidget *button_quit = gtk_button_new_with_label("Quit");
    auto textbox = gtk_text_view_new();

    g_signal_connect(G_OBJECT (bar), "response",
                     G_CALLBACK (gtk_widget_hide), nullptr);

    g_signal_connect (G_OBJECT (button_send), "clicked",
                      G_CALLBACK (send_message), nullptr);

    g_signal_connect (G_OBJECT (button_quit), "clicked",
        G_CALLBACK (+[](GtkWidget*, gpointer app) {
            g_application_quit(G_APPLICATION(app));
        }), app);

    auto infos = new window_info_t;
    infos->window = window;
    infos->bar = GTK_INFO_BAR (bar);
    infos->message_label = message_label;

    g_signal_connect (G_OBJECT (button_folder), "clicked",
        G_CALLBACK (folder_choice_opened), infos);

    auto scrolled_window = gtk_scrolled_window_new ();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW (scrolled_window), textbox);

    gtk_widget_set_hexpand(bar, true);
    gtk_widget_set_hexpand(button_send, true);
    gtk_widget_set_hexpand(button_folder, true);
    gtk_widget_set_hexpand(button_quit, true);
    gtk_widget_set_hexpand(textbox, true);
    gtk_widget_set_vexpand(textbox, true);

    gtk_grid_attach(grid, bar, 0, 0, 1, 1);
    gtk_grid_attach(grid, button_send, 0, 1, 1, 1);
    gtk_grid_attach(grid, button_folder, 0, 2, 1, 1);
    gtk_grid_attach(grid, button_quit, 0, 3, 1, 1);
    gtk_grid_attach(grid, scrolled_window, 0, 4, 1, 1);

    gtk_window_set_child(window, GTK_WIDGET (grid));
    gtk_window_present (window);
}

int main (int argc, char **argv) {
    GtkApplication *app = gtk_application_new ("org.gtk.gtktest",
                                               G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect (app, "activate", G_CALLBACK (activate), nullptr);

    int status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}

