/*
  Game: SUN BUGGY
  Authors: Jhon Jerme Dela Cruz && Nicole Soluren
  Course: CMSC 125 Operating Systems
*/

// LIBRARIES
#include "../../sdk/dexsdk.h"

// CONSTANTS
#define maxrow 25
#define maxcol 78
#define jump_key ' '
#define quit_key 'q'
#define restart_key 'r'

// FUNCTION PROTOTYPES
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
void save_score();
void sort_high_scores();

// GLOBAL VARIABLES
char keypress;
int speed = 5;
int obstacle = 0;
int lives = 3;
int level = 1;
int score = 0;
int progress = 0;
int y = 20;
int blink = 0;
int highscore[3] = {0,0,0};

// MAIN FUNCTION
int main(){

  clrscr(); // clear the whole screen

  do{ // run the game

    // Wait for the user to press space
    while(keypress != jump_key){
      print_HomeScreen(); // print sunbuggy home screen
      if(kb_ready()){
        keypress=(char)getch();
      }
      delay(10);
    }

    // clear the screen to start the game play
    clrscr();

    while(keypress != quit_key){ // while the player does not quit
      clrscr(); // clear screen
      print_road(); // print road

      if(kb_ready()){ // get sunbuggy movement
        keypress=(char)getch();
      }

      if(keypress == jump_key){ // if user jumps
        y--; // decrease y
      }else if(y<20){
        y++;
      }else if(keypress == quit_key || lives == 0){
        save_score();
        print_game_over();
        keypress = quit_key;
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

// FUNCTIONS
void sort_high_scores(){
  int i, j, temp;
  for(i=0; i<3; i++){
    for(j=i+1; j<3; j++){
      if(highscore[i] > highscore[j]){
        temp = highscore[i];
        highscore[i] = highscore[j];
        highscore[j] = temp;
      }
    }
  }
}

void save_score(){
  int i = 0;

  clrscr();

  if(highscore[0] == 0 || highscore[1] == 0 || highscore[2] == 0){
    if(highscore[0] == 0){
      highscore[0] = score;
    }else if(highscore[1] == 0){
      highscore[1] = score;
    }else{
      highscore[2] = score;
    }
  }else{
    for(i=0; i<3; i++){
      if(score < highscore[i]){
        highscore[i] = score;
        break;
      }
    }
  }

  gotoxy(35, 12); textcolor(YELLOW);
  printf("Score: %d\n", score);


  sort_high_scores();

  gotoxy(34,5); textcolor(RED);
  printf("HIGH SCORES\n");
  for(i=0; i<3; i++){
    gotoxy(36,7+i); textcolor(YELLOW);
    printf("[%d] %d\n", i, highscore[i]);
  }
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

  gotoxy(30, 19);
  printf("press 'r' to restart\n");
  gotoxy(31, 20);
  printf("press 'q' to exit\n");
  print_road();

  textcolor(RED);
  gotoxy(31, 17);
  printf("G A M E  O V E R !\n");

  keypress = '*';

  do{
      keypress = (char)getch();
  }while(keypress != quit_key && keypress != restart_key );

  if(keypress == restart_key){
    speed = 5;
    obstacle = 0;
    lives = 3;
    score = 0;
    level = 1;
    main();
  }

  clrscr();

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
  char color0 = RED;
  char color1 = LIGHTRED;
  char color2 = YELLOW;

  if(blink < 2){
    blink++;
  }else{
    blink = 0;
  }

  if(blink == 0){
    color0 = RED;
    color1 = LIGHTRED;
    color2 = YELLOW;
  }else if(blink == 1){
    color2 = RED;
    color0 = LIGHTRED;
    color1 = YELLOW;
  }else{
    color1 = RED;
    color2 = LIGHTRED;
    color0 = YELLOW;
  }

  gotoxy(25,5);textcolor(color0);
  printf("SSSS U  U NNNN"); gotoxy(25,6); textcolor(color0);
  printf("S    U  U N  N"); gotoxy(25,7); textcolor(color0);
  printf("SSSS U  U N  N"); gotoxy(25,8); textcolor(color1);
  printf("   S U  U N  N"); gotoxy(25,9); textcolor(color1);
  printf("SSSS UUUU N  N");

  gotoxy(25,11); textcolor(color1);
  printf("     BBBB U  U GGGG GGGG Y    Y"); gotoxy(25,12);textcolor(color1);
  printf("     B  B U  U G    G     Y  Y"); gotoxy(25,13);textcolor(color2);
  printf("     BBBB U  U G GG G GG   Y"); gotoxy(25,14);textcolor(color2);
  printf("     B  B U  U G  G G  G   Y"); gotoxy(25,15);textcolor(color2);
  printf("     BBBB UUUU GGGG GGGG   Y");gotoxy(30,18);

  if(blink == 1){
    textcolor(RED);
  }else if(blink == 2){
    textcolor(LIGHTRED);
  }else{
    textcolor(YELLOW);
  }
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