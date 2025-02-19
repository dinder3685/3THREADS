#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#define WIDTH 100
#define HEIGHT 30
#define GRAPH_WIDTH 30
#define GRAPH_HEIGHT 10

int temperature, humidity, pressure;
int temp_data[GRAPH_WIDTH], hum_data[GRAPH_WIDTH], pres_data[GRAPH_WIDTH];

pthread_mutex_t lock;

void shift_and_add(int *data, int value) {
    for (int i = 1; i < GRAPH_WIDTH; i++) {
        data[i - 1] = data[i];
    }
    data[GRAPH_WIDTH - 1] = value;
}

void *update_temperature(void *arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        temperature = rand() % 50;
        shift_and_add(temp_data, temperature);
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
    return NULL;
}

void *update_humidity(void *arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        humidity = rand() % 101;
        shift_and_add(hum_data, humidity);
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
    return NULL;
}

void *update_pressure(void *arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        pressure = 950 + rand() % 101;
        shift_and_add(pres_data, pressure - 950);
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
    return NULL;
}

void draw_gauge(int y, int x, int value, int max, char *label, int color_pair) {
    int length = 20;
    int filled = (value * length) / max;
    attron(COLOR_PAIR(color_pair));
    mvprintw(y, x, "%s: [", label);
    for (int i = 0; i < length; i++) {
        if (i < filled)
            printw("#");
        else
            printw(" ");
    }
    printw("] %d", value);
    attroff(COLOR_PAIR(color_pair));
}

void draw_graph(int y, int x, int *data, int max, char *label, int color_pair) {
    attron(COLOR_PAIR(color_pair));
    mvprintw(y, x, "%s", label);
    for (int i = 0; i < GRAPH_WIDTH; i++) {
        for (int j = 0; j < GRAPH_HEIGHT; j++) {
            if (GRAPH_HEIGHT - j <= (data[i] * GRAPH_HEIGHT) / max)
                mvprintw(y + j + 1, x + i, "#");
            else
                mvprintw(y + j + 1, x + i, " ");
        }
    }
    attroff(COLOR_PAIR(color_pair));
}

void display_data() {
    initscr();
    curs_set(0);
    noecho();
    nodelay(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);

    while (1) {
        clear();
        box(stdscr, 0, 0);
        attron(COLOR_PAIR(4));
        mvprintw(2, (WIDTH - 40) / 2, "==== REAL-TIME SENSOR DASHBOARD ====");
        attroff(COLOR_PAIR(4));
        
        pthread_mutex_lock(&lock);
        draw_gauge(5, 10, temperature, 50, "Temperature (C)", 1);
        draw_gauge(7, 10, humidity, 100, "Humidity (%)", 2);
        draw_gauge(9, 10, pressure - 950, 100, "Pressure (hPa)", 3);
        draw_graph(5, 40, temp_data, 50, "Temp Graph", 1);
        draw_graph(16, 40, hum_data, 100, "Hum Graph", 2);
        draw_graph(27, 40, pres_data, 100, "Pres Graph", 3);
        pthread_mutex_unlock(&lock);

        mvprintw(HEIGHT - 3, (WIDTH - 30) / 2, "Press 'q' to exit");
        refresh();
        usleep(500000);
        
        if (getch() == 'q') {
            break;
        }
    }
    endwin();
}

int main() {
    srand(time(NULL));
    pthread_mutex_init(&lock, NULL);
    pthread_t temp_thread, hum_thread, pres_thread;

    pthread_create(&temp_thread, NULL, update_temperature, NULL);
    pthread_create(&hum_thread, NULL, update_humidity, NULL);
    pthread_create(&pres_thread, NULL, update_pressure, NULL);

    display_data();

    pthread_join(temp_thread, NULL);
    pthread_join(hum_thread, NULL);
    pthread_join(pres_thread, NULL);
    pthread_mutex_destroy(&lock);

    return 0;
}
