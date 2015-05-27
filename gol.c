/* es gibt Zellen
 * wenn eine Zelle mehr als 4 Nachbarn hat, stirbt sie im nächsten Schritt
 * sonst produziert sie im nächsten Schritt (irgendwo) einen Nachbarn
 *
 * es gibt ein Spielfeld, das aus Zellen besteht
 * eine Zelle hat 2 Zustände, lebend und tot
 */

#include "gol.h"

#include <stdio.h>
#include <unistd.h>

typedef unsigned char bool; // 0 = FALSE, nonzero = TRUE

#define MAX_X 64
#define MAX_Y 32
#define true 1
#define false 0

typedef enum { dead = 0, alive = 1, num_states = 2 } cell_state;

int current_field = 0; // das spielfeld, von dem gerade gelesen wird
bool field[num_states][MAX_X][MAX_Y]; // geht von 0..MAX_X-1, 0..MAX_Y-1

// limitiere val auf den Bereich [val_min, val_max]
int limit(int val, int val_min, int val_max) {
	if(val > val_max) return val_max;
	if(val < val_min) return val_min;
	return val;
}

// test, ob Zelle lebt (liest in current_field)
cell_state is_alive(int x, int y){
	if((x >= 0) & (x < MAX_X) & (y >= 0) & (y < MAX_Y)) {
		return field[current_field%2][x][y];
	} else {
		return dead;
	}
}

// Zelle unlebendig setzen (schreibt NICHT in current_field)
void kill_cell(int x, int y){
	if((x > 0) & (x < MAX_X) & (y > 0) & (y < MAX_Y)) {
		field[(current_field+1)%2][x][y] = 0;
	}
}

// Zelle lebendig setzen (schreibt NICHT in current_field)
void create_cell(int x, int y){
	if((x > 0) & (x < MAX_X) & (y > 0) & (y < MAX_Y)) {
		field[(current_field+1)%2][x][y] = 1;
	}
}

// Zelle kopieren (von current_field ins andere)
void copy_cell(int x, int y){
	if((x > 0) & (x < MAX_X) & (y > 0) & (y < MAX_Y)) {
		field[(current_field+1)%2][x][y] = field[current_field%2][x][y];
	}
}

// Nachbarn zählen und anzahl lebendiger Zellen zurückgeben
int count_neighbours(int x, int y){
	/* x-1  x  x+1
	 *  o   o   o   y-1
	 *  o (x,y) o   y
	 *  o   o   o   y+1
	 */
	int cnt = 0;
	for(int x_neighbour = x-1; x_neighbour <= x+1; x_neighbour++)
		for(int y_neighbour = y-1; y_neighbour <= y+1; y_neighbour++)
			if(is_alive(x_neighbour,y_neighbour))
				cnt++;
	if(is_alive(x,y)) cnt--; // da wir uns selbst in der Mitte mitzählen
	return cnt;
}


void reset_current_field() {
	for(int y = 0; y < MAX_Y; y++){
		for(int x = 0; x < MAX_X; x++) {
			kill_cell(x,y);
		}
	}
}

// Spielfeld initialisieren
void reset_field() {
	current_field = 0;
	reset_current_field();
	current_field = 1;
	reset_current_field();
	current_field = 0;
}

// ein paar Felder lebendig machen (Variante 1)
void init_field_var1() {
	field[0][2][3] = 1;
	field[0][2][4] = 1;
	field[0][2][5] = 1;
}

// ein paar Felder lebendig machen (Variante 2)
void init_field_var2() {
	field[0][12][5] = 1;

	field[0][13][3] = 1;
	field[0][13][5] = 1;

	field[0][14][4] = 1;
	field[0][14][5] = 1;
}

// ein paar Felder lebendig machen (Variante 3)
void init_field_var3() {
	field[0][12][4] = 1;
    field[0][15][4] = 1;
    field[0][16][4] = 1;

	field[0][2][5] = 1;
    field[0][3][5] = 1;

	field[0][3][4] = 1;
	field[0][3][5] = 1;
}

// gibt das Feld einfach aus
void visualize_field(){
	printf("\n");
	for(int y = 0; y < MAX_Y; y++){
		for(int x = 0; x < MAX_X; x++) {
			if(is_alive(x,y)){
				printf("#");
			} else {
				printf(".");
			}
		}
		printf("\n");
	}
}

void visualize_field_debug(){
	printf("\n");
	for(int y = 0; y < MAX_Y; y++){
		for(int x = 0; x < MAX_X; x++) {
			int cnt = count_neighbours(x,y);
			if(is_alive(x,y)){
				printf("#%i ", cnt);
			} else {
				printf(" %i ", cnt);
			}
		}
		printf("\n");
	}
}

// nächsten Schritt berechnen
void update_field(){
	for(int x = 0; x < MAX_X; x++) {
		for(int y = 0; y < MAX_Y; y++){
			int cnt = count_neighbours(x,y);
			copy_cell(x,y); // Zelle in das neue Spielfeld rüberkopieren
			if(is_alive(x,y)) {
				if((cnt < 2) || (cnt > 3)) { // Zelle lebt und ist einsam oder überbevölkert
					kill_cell(x,y);
				}
			} else if(cnt == 3){ // zelle ist tot und hat genau 3 nachbarn
				create_cell(x,y);
			}
		}
	}
	current_field = (current_field+1)%2; // anderes Feld aktiv schalten
}

extern int usleep (__useconds_t __useconds);

int main(int argn, char ** argc){
	reset_field(); // Spielfeld initialisieren
	init_field_var2();  // ein paar Felder lebendig machen

	int key = 0;
	while(key != 'q'){
		visualize_field();
		//visualize_field_debug();
		//key = getchar(); // ein Zeichen als Eingabe abfangen
		update_field();
		usleep(200e3);
	}

	return 0;
}
