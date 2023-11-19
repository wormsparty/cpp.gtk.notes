#include <gtkmm.h>
#include <libsoup/soup.h>

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

class MyWindow : public Gtk::Window
{
private:
  Gtk::Button button_send;
  Gtk::Button button_quit;

public:
  MyWindow();
};

MyWindow::MyWindow()
{
  set_title("GTK test");
  set_default_size(640, 480);

  Gtk::Grid grid {};
  grid.set_row_spacing(5);
  grid.set_column_spacing(5);

  button_send.set_label("Send message");

  button_send.signal_clicked().connect([&]() {
    send_message();
  });

  button_quit.set_label("Quit");

  button_quit.signal_clicked().connect([&]() {
    get_application()->quit();
  });

  grid.attach(button_send, 0, 0, 1, 1);
  grid.attach(button_quit, 0, 1, 1, 1);

  set_child(grid);
}

int main (int argc, char **argv) {
  auto app = Gtk::Application::create("org.gtk.gtktest");
  return app->make_window_and_run<MyWindow>(argc, argv);
}

