#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <sstream>
#include <chrono> 
#include "QuadTree.h"
using namespace std;


#define SCREEN_TAM_X 10000
#define SCREEN_TAM_Y 10000

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *events = NULL;

Point RandomPoint() {
    //int positionX = (rand() % 1000) + 4500;
    //int positionY = (rand() % 1000) + 4500;
    int positionX = (rand() % SCREEN_TAM_X);
    int positionY = (rand() % SCREEN_TAM_Y);
    return Point(positionX, positionY);
}

void AddRandomCircles(QuadTree& q, int num) {
    static int order = 0;

    for (int i = 0; i < num; i++) {
        Point p = RandomPoint();
        stringstream ss;
        ss << "C# z:" << order++ << "   [" << p.x() << " , " << p.y() << "]";
        int col[] = { 0, (75 + (rand() % 150)), 0 };

        q.AddCircle(new Circle(p, ss.str(), col, order));
    }
}

int main() {

    al_init();
    if (!al_init()) {
        cerr << "Init allegro error";
        return 1;
    }
    if (!al_install_keyboard()) {
        cerr << "Keyboard install error";
        return 1;
    }
    if (!al_init_primitives_addon()) {
        cerr << "InitPrimitives error";
        return 2;
    }
    if (!al_init_font_addon()) {
        cerr << "InitPrimitives error";
        return 2;
    }


    al_set_new_display_option(ALLEGRO_SINGLE_BUFFER, 1, ALLEGRO_REQUIRE);
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    display = al_create_display(1400, 1000);

    events = al_create_event_queue();
    al_register_event_source(events, al_get_keyboard_event_source());


    bool exit = false;
    bool gridEnabled = true;
    bool drawEnabled = true;
    //while (al_is_event_queue_empty(events)) {}
    ALLEGRO_EVENT ev;
    ALLEGRO_FONT* font = al_create_builtin_font();

    Circle* raycastedCircle = nullptr;
    Point rayCast(-1000, -1000);
    QuadTree q(SCREEN_TAM_X, SCREEN_TAM_Y);
    AddRandomCircles(q, 5);
    auto start = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    while (!exit) {

        cout << "";

        al_clear_to_color(al_map_rgb(16, 16, 16));
        if (drawEnabled) {
            q.ForEachCircle([&](Circle* c) {
                al_draw_filled_circle(c->center().x() / 10, c->center().y() / 10, CIRCLES_RADIUS / 10,
                    al_map_rgb(c->color()[0], c->color()[1], c->color()[2]));
            });

            if (gridEnabled) {
                q.ForEachQuad([&](QuadTree* q) {
                    al_draw_line(
                        (q->position().x()) / 10,
                        (q->position().y()) / 10,
                        (q->position().x() + q->length().x()) / 10,
                        (q->position().y()) / 10,
                        al_map_rgb(255, 255, 0), 1);
                    al_draw_line(
                        (q->position().x()) / 10,
                        (q->position().y()) / 10,
                        (q->position().x()) / 10,
                        (q->position().y() + q->length().y()) / 10,
                        al_map_rgb(255, 255, 0), 1);
                    al_draw_line(
                        (q->position().x() + q->length().x()) / 10,
                        (q->position().y()) / 10,
                        (q->position().x() + q->length().x()) / 10,
                        (q->position().y() + q->length().y()) / 10,
                        al_map_rgb(255, 255, 0), 1);
                    al_draw_line(
                        (q->position().x()) / 10,
                        (q->position().y() + q->length().y()) / 10,
                        (q->position().x() + q->length().x()) / 10,
                        (q->position().y() + q->length().y()) / 10,
                        al_map_rgb(255, 255, 0), 1);
                });
            }

            al_draw_filled_circle(rayCast.x() / 10, rayCast.y() / 10, CIRCLES_RADIUS / 10,
                al_map_rgba(64, 0, 0, 128));
            al_draw_filled_circle(rayCast.x() / 10, rayCast.y() / 10, CIRCLES_RADIUS / 2,
                al_map_rgba(32, 0, 0, 128));
            al_draw_filled_circle(rayCast.x() / 10, rayCast.y() / 10, CIRCLES_RADIUS / 100,
                al_map_rgb(255, 0, 0));
        }

        int textPos = 0;
        al_draw_text(font, al_map_rgb(255, 255, 255), 1020, textPos += 20, ALLEGRO_ALIGN_LEFT, "Instrucciones");
        al_draw_text(font, al_map_rgb(255, 255, 255), 1020, textPos += 20, ALLEGRO_ALIGN_LEFT, "ESC - Salir");
        al_draw_text(font, al_map_rgb(255, 255, 255), 1020, textPos += 20, ALLEGRO_ALIGN_LEFT, "D - Dibujado on/off");
        al_draw_text(font, al_map_rgb(255, 255, 255), 1020, textPos += 20, ALLEGRO_ALIGN_LEFT, "G - Grid on/off");
        al_draw_text(font, al_map_rgb(255, 255, 255), 1020, textPos += 20, ALLEGRO_ALIGN_LEFT, "R - Lanzar rayo aleatorio");
        al_draw_text(font, al_map_rgb(255, 255, 255), 1020, textPos += 20, ALLEGRO_ALIGN_LEFT, "T - Lanzar 1k rayos aleatorios");
        al_draw_text(font, al_map_rgb(255, 255, 255), 1020, textPos += 20, ALLEGRO_ALIGN_LEFT, "Y - Lanzar 1M rayos aleatorios");
        al_draw_text(font, al_map_rgb(255, 255, 255), 1020, textPos += 20, ALLEGRO_ALIGN_LEFT, "1 - Anade 1 bola");
        al_draw_text(font, al_map_rgb(255, 255, 255), 1020, textPos += 20, ALLEGRO_ALIGN_LEFT, "2 - Anade 10 bolas");
        al_draw_text(font, al_map_rgb(255, 255, 255), 1020, textPos += 20, ALLEGRO_ALIGN_LEFT, "3 - Anade 100 bolas");
        al_draw_text(font, al_map_rgb(255, 255, 255), 1020, textPos += 20, ALLEGRO_ALIGN_LEFT, "4 - Anade 1k bolas");
        al_draw_text(font, al_map_rgb(255, 255, 255), 1020, textPos += 20, ALLEGRO_ALIGN_LEFT, "5 - Anade 10k bolas (lento)");
        al_draw_text(font, al_map_rgb(255, 255, 255), 1020, textPos += 20, ALLEGRO_ALIGN_LEFT, "6 - Anade 100k bolas (muy lento)");
        textPos += 20;
        if (raycastedCircle != nullptr) {
            al_draw_text(font, al_map_rgb(255, 255, 255), 1020, textPos += 20, ALLEGRO_ALIGN_LEFT, "El circulo tocado es:");
            al_draw_text(font, al_map_rgb(255, 255, 255), 1020, textPos += 20, ALLEGRO_ALIGN_LEFT, raycastedCircle->name().c_str());
            stringstream ss;
            ss << "RayCast coords: [" << rayCast.x() << ", " << rayCast.y() << "]";
            al_draw_text(font, al_map_rgb(255, 255, 255), 1020, textPos += 20, ALLEGRO_ALIGN_LEFT, ss.str().c_str());
        } else {
            al_draw_text(font, al_map_rgb(255, 255, 255), 1020, textPos += 20, ALLEGRO_ALIGN_LEFT, "No se ha tocado ningun circulo");
            textPos += 20;
            textPos += 20;
        }

        textPos += 20;
        if (rayCast.x() >= 0) {
            stringstream ss;
            ss << "Tiempo: " << microseconds << " microsegundos";
            al_draw_text(font, al_map_rgb(255, 255, 255), 1020, textPos += 20, ALLEGRO_ALIGN_LEFT, ss.str().c_str());
        }

        al_flip_display();

        al_wait_for_event(events, &ev);

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_G:
                    gridEnabled = !gridEnabled;
                break;
                case ALLEGRO_KEY_D:
                    drawEnabled = !drawEnabled;
                break;
                case ALLEGRO_KEY_1:
                case ALLEGRO_KEY_PAD_1:
                    AddRandomCircles(q, 1);
                break;
                case ALLEGRO_KEY_2:
                case ALLEGRO_KEY_PAD_2:
                    AddRandomCircles(q, 10);
                break;
                case ALLEGRO_KEY_3:
                case ALLEGRO_KEY_PAD_3:
                    AddRandomCircles(q, 100);
                break;
                case ALLEGRO_KEY_4:
                case ALLEGRO_KEY_PAD_4:
                    AddRandomCircles(q, 1000);
                break;
                case ALLEGRO_KEY_5:
                case ALLEGRO_KEY_PAD_5:
                    AddRandomCircles(q, 10000);
                break;
                case ALLEGRO_KEY_6:
                case ALLEGRO_KEY_PAD_6:
                    AddRandomCircles(q, 100000);
                break;
                case ALLEGRO_KEY_R:
                    rayCast = RandomPoint();
                    start = std::chrono::high_resolution_clock::now();
                    raycastedCircle = q.RayCast(rayCast);
                    elapsed = std::chrono::high_resolution_clock::now() - start;

                    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
                break;
                case ALLEGRO_KEY_T:
                    start = std::chrono::high_resolution_clock::now();
                    for(int i = 0; i < 1000; ++i) {
                        rayCast = RandomPoint();
                        raycastedCircle = q.RayCast(rayCast);
                    }
                    elapsed = std::chrono::high_resolution_clock::now() - start;

                    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
                break;
                case ALLEGRO_KEY_Y:
                    start = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i < 1000000; ++i) {
                        rayCast = RandomPoint();
                        raycastedCircle = q.RayCast(rayCast);
                    }
                    elapsed = std::chrono::high_resolution_clock::now() - start;

                    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
                break;
                case ALLEGRO_KEY_ESCAPE:
                    exit = true;
                break;
            }
        }
    }

    return 0;
}