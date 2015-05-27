
#include "gol.h"

#include <unistd.h>

extern int usleep (__useconds_t __useconds);

int main2(int argn, char ** argc){
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
