/*
 * Snake (ported by jachermocilla@gmail.com)
 *
 * Original code lifted from:
 * http://www.simonhuggins.com/courses/cbasics/course_notes/snake.htm
 */


#include "../../sdk/dexsdk.h"

/* constants */
#define maxrow 25
#define maxcol 78
#define jump_key ' '
#define quit_key 'q'

/* prototypes */
void draw_line(int col, int row);
void print_road();
void print_HomeScreen();
void print_buggy();
void print_jump_buggy();
void print_obstacles();
void print_game_over();
void print_statistices();
void level_up();
void check_jump();

/* global variables */
char keypress;
int speed = 5;
int obstacle = 0;
int lives = 3;
int level = 1;
int score = 0;
int progress = 0;
int y = 20;


int main(){

  clrscr();
  print_HomeScreen();

  do{
    // Wait for the user to press space
    while(keypress != jump_key){
      keypress=(char)getch();
    }
    clrscr();

    // start the game
    while(keypress != quit_key){
      clrscr();
      print_road();

      if(kb_ready()){
        keypress=(char)getch();
      }
      if(keypress == jump_key){
        y--;
      }else if(y<20){
        y++;
      }else if(keypress == quit_key || lives == 0){
        print_game_over();
        break;
      }
      print_jump_buggy();
      keypress = '*';

      check_jump();

      print_obstacles(obstacle);
      delay(speed);
      score++;
      level_up();
    }

  }while(keypress != quit_key);

}

void check_jump(){
  if(y == 20 && obstacle == 60){
    lives --;
  }
}

void level_up(){
  progress ++;
  if(progress > 100){
    level++;
    progress = 0;
  }
}

void print_statistices(){
  gotoxy(17, 24);
  printf("L i v e s : %d   L e v e l : %d   S c o r e : %d\n", lives, level, score);
}

void print_game_over(){
  clrscr();
  gotoxy(30, 20);
  printf("G A M E  O V E R !\n");
  print_road();

}

void print_obstacles(){
  gotoxy(obstacle, 22);
  textcolor(RED);
  printf("_");
  if(obstacle == 72){
    obstacle = 0;
  }else{
    obstacle ++;
  }
}

void print_jump_buggy(){
  gotoxy(60,y);
  printf(" |  |\n");
  gotoxy(60,y+1);
  printf("O---O\n");
}

void print_HomeScreen(){
  gotoxy(25,7);textcolor(RED);
  printf("SSSS U  U NNNN"); gotoxy(25,8); textcolor(LIGHTRED);
  printf("S    U  U N  N"); gotoxy(25,9); textcolor(LIGHTRED);
  printf("SSSS U  U N  N"); gotoxy(25,10); textcolor(YELLOW);
  printf("   S U  U N  N"); gotoxy(25,11); textcolor(YELLOW);
  printf("SSSS UUUU N  N");

  gotoxy(25,13); textcolor(RED);
  printf("     BBBB U  U GGGG GGGG Y    Y"); gotoxy(25,14);textcolor(LIGHTRED);
  printf("     B  B U  U G    G     Y  Y"); gotoxy(25,15);textcolor(LIGHTRED);
  printf("     BBBB U  U G GG G GG   Y"); gotoxy(25,16);textcolor(YELLOW);
  printf("     B  B U  U G  G G  G  Y"); gotoxy(25,17);textcolor(YELLOW);
  printf("     BBBB UUUU GGGG GGGG Y"); gotoxy(30,20);textcolor(RED);
  printf("(P r e s s  S p a c e  t o  s t a r t . . .)\n");

  print_road();
}

void print_buggy(){
  gotoxy(60,20);
  printf(" |  |\n");
  gotoxy(60,21);
  printf("O---O\n");
}

void print_road(){
    textcolor(LIGHTRED);
    draw_line(0,21);
    textcolor(YELLOW);
    draw_line(0,22);

    print_statistices();
}

void draw_line(int col, int row){
  gotoxy(col,row);
  for (col=0;col<maxcol+2;col++) printf("=");
  printf("\n");
}
