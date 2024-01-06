#include <glib.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

struct window_info_t {
    GtkWindow* window;
    GtkInfoBar* bar;
    GtkLabel* message_label;
};

static bool send_message() {
    g_autoptr(GSocketClient) client = g_socket_client_new();
    GError* error = nullptr;

    g_autoptr(GSocketConnection) connection = g_socket_client_connect_to_host(
        client, "localhost", 5000, nullptr, &error);

    if (error != nullptr) {
      g_error (error->message);
      g_clear_error(&error);
    } else {
      g_print ("Connection successful!\n");

      //GInputStream* istream = g_io_stream_get_input_stream (G_IO_STREAM (connection));
      GOutputStream* ostream = g_io_stream_get_output_stream (G_IO_STREAM (connection));
  
      auto msg = "Hello, server!";
      g_output_stream_write(ostream, msg, strlen(msg), nullptr, &error);

      if (error != nullptr) {
        g_error (error->message);
        g_clear_error(&error);
      } else {
	g_print("Message sent successfully\n");
      }
    }

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
        g_autoptr(GFile) folderpath = gtk_file_chooser_get_file(chooser);
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
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW (textbox), GTK_WRAP_WORD_CHAR);

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

    auto toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    auto open_button = gtk_button_new_from_icon_name("document-open");

    g_signal_connect (G_OBJECT (open_button), "clicked",
        G_CALLBACK (+[](GtkWidget*, gpointer) {
            g_print ("Button 1\n");
        }), app);
    
    auto close_button = gtk_button_new_from_icon_name("window-close");
    
    g_signal_connect (G_OBJECT (close_button), "clicked",
        G_CALLBACK (+[](GtkWidget*, gpointer) {
            g_print ("Button 2\n");
        }), app);

    gtk_box_append(GTK_BOX (toolbar), open_button);
    gtk_box_append(GTK_BOX (toolbar), close_button);

    auto scrolled_window = gtk_scrolled_window_new ();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW (scrolled_window), textbox);

    gtk_widget_set_hexpand(bar, true);
    gtk_widget_set_hexpand(toolbar, true);
    gtk_widget_set_hexpand(button_send, true);
    gtk_widget_set_hexpand(button_folder, true);
    gtk_widget_set_hexpand(button_quit, true);
    gtk_widget_set_hexpand(textbox, true);
    gtk_widget_set_vexpand(textbox, true);

    gtk_grid_attach(grid, bar, 0, 0, 1, 1);
    gtk_grid_attach(grid, toolbar, 0, 1, 1, 1);
    gtk_grid_attach(grid, button_send, 0, 2, 1, 1);
    gtk_grid_attach(grid, button_folder, 0, 3, 1, 1);
    gtk_grid_attach(grid, button_quit, 0, 4, 1, 1);
    gtk_grid_attach(grid, scrolled_window, 0, 5, 1, 1);

    gtk_window_set_child(window, GTK_WIDGET (grid));
    gtk_window_present (window);
}

int main (int argc, char **argv) {
    g_autoptr(GtkApplication) app = gtk_application_new ("org.gtk.gtktest",
                                               G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect (app, "activate", G_CALLBACK (activate), nullptr);

    return g_application_run (G_APPLICATION (app), argc, argv);
}

