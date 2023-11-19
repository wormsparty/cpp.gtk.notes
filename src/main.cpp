#include <gtk/gtk.h>
#include <libsoup/soup.h>
#include <glib.h>

static bool send_message() {
    g_autoptr(SoupSession) session = soup_session_new ();
    g_autoptr(SoupMessage) message = soup_message_new (SOUP_METHOD_POST, "http://localhost:5000");

    const char* args = "foo=bar&one=two";

    g_autoptr(GBytes) args_b = g_bytes_new (args, strlen(args));
    soup_message_set_request_body_from_bytes(
        message, "application/x-www-form-urlencoded", args_b);

    GError *error = NULL;
    g_autoptr(GBytes) bytes = soup_session_send_and_read(session, message, nullptr, &error);

    if (error) {
        g_printerr ("Failed to POST: %s\n", error->message);
        g_error_free (error);
        return false;
    }

    g_print ("Received: %s\n", (char*)g_bytes_get_data(bytes, NULL));
    return true;
}

static void activate (GtkApplication* app, [[maybe_unused]] gpointer user_data) {
  auto window = GTK_WINDOW (gtk_application_window_new (app));
  gtk_window_set_title (GTK_WINDOW (window), "GTK test");
  gtk_window_set_default_size (GTK_WINDOW (window), 640, 480);

  GtkWidget *grid = gtk_grid_new();
  gtk_grid_set_row_spacing(GTK_GRID (grid), 5);
  gtk_grid_set_column_spacing(GTK_GRID (grid), 5);

  GtkWidget *button_send = gtk_button_new_with_label("Send message");
  GtkWidget *button_quit = gtk_button_new_with_label("Quit");

  g_signal_connect (G_OBJECT (button_send), "clicked",
                    G_CALLBACK (send_message), nullptr);

  g_signal_connect (G_OBJECT (button_quit), "clicked",
                    G_CALLBACK (+[]([[maybe_unused]] GtkWidget* widget, gpointer data) {
                        g_application_quit(G_APPLICATION(data));
                    }), G_OBJECT (app));

  gtk_grid_attach(GTK_GRID (grid), button_send, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID (grid), button_quit, 0, 1, 1, 1);

  gtk_window_set_child(window, grid);
  gtk_window_present (GTK_WINDOW (window));
}

int main (int argc, char **argv) {
  GtkApplication *app = gtk_application_new ("org.gtk.gtktest",
                                             G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), nullptr);
  int status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}

