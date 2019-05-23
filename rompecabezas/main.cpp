#include "pch.h"
#include "Gpa.h"

int main()
{
	ALLEGRO_DISPLAY *display = NULL;
	srand(time(NULL));
	if (!al_init()) {
		std::cout << "failed to inciarVariables allegro!\n";
		return -1;
	}

	display = al_create_display(640, 480);
	if (!display) {
		std::cout << "failed to create display!\n";
		return -1;
	}

	al_install_keyboard();
	al_install_mouse();

	al_init_image_addon();
	al_init_primitives_addon();

	al_init_font_addon();
	al_init_ttf_addon();

	Gpa::TraerSingleton().configution();
	Gpa::TraerSingleton().inciarVariables();
	Gpa::TraerSingleton().cargarTodo();

	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	ALLEGRO_TIMER *timer = al_create_timer(1.0f);

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	bool done = false;
	al_start_timer(timer);

	while (!done) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(event_queue, &evento);
		if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			done = true;
		Gpa::TraerSingleton().refrescar(evento, &done);
		if (Gpa::TraerSingleton().rePintar) {
			Gpa::TraerSingleton().pintarPantalla(display);
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
			Gpa::TraerSingleton().rePintar = false;
		}

		if (evento.type == ALLEGRO_EVENT_TIMER) {
			// contador
		}
	}

	Gpa::TraerSingleton().eliminarTodo();
	al_destroy_display(display);

	return 0;
}