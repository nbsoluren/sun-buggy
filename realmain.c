#include <ncurses.h>
#include <time.h> //this is for the nanosleep
#include <unistd.h>
int screenHeight;
int screenWidth;
float movement = 0;
float mov[3] = {1,2,3};

// UI

// Race car
char* rccar[6] = {
    "    o   ",
    "    o   ",
    "    o   ",
    " 00 o 00",
    "    o   ",
    "    o   "
};


// Sun Buggy title screen
char* homeScreen[84] = {
    "  S S S S    U         U    N N        N                                            ",
    "S            U         U    N  N       N                                            ",
    "  S          U         U    N   N      N                                            ",
    "   S         U         U    N    N     N                                            ",
    "     S       U         U    N     N    N                                            ",
    "       S     U         U    N      N   N                                            ",
    "        S    U         U    N       N  N                                            ",
    " S S S S       U U U U      N        N N                                            ",
    "                                                                                    ",
    "                                                                                    ",
    "             B B B B B     U        U      G G G G G       G G G G G     Y         Y",
    "             B        B    U        U    G          G    G          G     Y       Y ",
    "             B        B    U        U    G          G    G          G      Y     Y  ",
    "             B B B B B     U        U    G               G                  Y   Y   ",
    "             B        B    U        U    G    G G G G    G    G G G G        Y Y    ",
    "             B        B    U        U    G          G    G          G        Y      ",
    "             B        B    U        U    G         G     G         G        Y       ",
    "             B B B B B      U U U U        G G G G         G G G G       Y Y        ",
    "                                                                                    ",
};

// Press space to play
char* instruction_1[44] = {
    " P r e s s   S p a c e   T o   P l a y . . .",
};

// Obstacle road
char* road[5] = {
    "_____"
    // "    ######    ################        #################         #############   ####"
};

// Upper right corner text
char* readMe[66] = {
    "Sun-Buggy version 1.0.00, Copyright 2018.                         ",
    "Sun-Buggy comes with ABSOLUTELY NO WARRANTY; for details type 'w'.",
    "This is free software, and you are welcome to redistribute it     ",
    "under certain conditions: type 'c' for details                    "
};

// Game Over
char* gameoverScreen[18] = {
    " G A M E   O V E R"
};

// Blank
char* blank[1] = {
    " "
};

// Level/Lives/Score
char* statistics[35] = {
    "Level: 1    Lives: 3    Score: 0   "
};

// Goodluck, Space to jump
char* goodluckMessage[] = {
    "G o o d   l u c k   ( u s e   < S P A C E >   to   j u m p )"
};


// FUNCTION PROTOTYPES
void print_road(WINDOW *main_win);
void previewscreen(WINDOW *main_win);
int mainloop(WINDOW *main_win);
int render_and_move_car(WINDOW *main_win, int rc_state);
void drawImage(WINDOW *window, int y, int x, char **arr, int h, int w);
void gameover();

// MAIN FUNCTION
int main(){

    WINDOW *main_win;

    initscr();
	clear();
	noecho();
	cbreak();
    curs_set(0);

    getmaxyx(stdscr, screenHeight, screenWidth);
    main_win = newwin(screenHeight, screenWidth, 0, 0);
    keypad(main_win, TRUE);
    wtimeout(main_win, 0);
    timeout(0);

    previewscreen(main_win);
    mainloop(main_win);
    gameover(main_win);
    endwin();

}


// FUNCTIONS
void print_road(WINDOW *main_win){
    for(int x = 0; x < screenWidth; x++) {
        mvwprintw(main_win, screenHeight-3, x, "%c", '#');
        mvwprintw(main_win, screenHeight-2, x, "%c", '#');
    }
}

/*
    Flash title screen
    while player does not press
    space bar
*/
void previewscreen(WINDOW *main_win){
    char ch;
    int blink = 0;

    while(ch != ' '){
        drawImage(main_win, screenHeight-50, screenWidth*(int) 0.0/4.0, readMe, 4, 66);
        drawImage(main_win, screenHeight-40, screenWidth*(int) 1.0/4.0, homeScreen, 19, 84);
        drawImage(main_win, screenHeight-9, screenWidth*(int) 3.0/4.0, rccar, 6, 8);
        drawImage(main_win, screenHeight-9, screenWidth*(int) 3.0/4.0, rccar, 6, 8);
        print_road(main_win);
        wrefresh(main_win);
        if(blink%2 == 0){
            drawImage(main_win, screenHeight-20, screenWidth*(int) 2.0/4.0, instruction_1, 1, 44);
        }
        ch = getch();
    }
}

/*
    Main game
*/
int mainloop(WINDOW *main_win){
    int rc_state = 0, goodluck_checker = 0;
    char ch;

    while(1){
        wclear(main_win);
        print_road(main_win);

        if(goodluck_checker == 0){
            drawImage(main_win, screenHeight-20, screenWidth*(int) 2.0/4.0, goodluckMessage, 1, 60);
        }

        do{
            ch = getch();
            if(ch == ' ' && rc_state == 0){
                rc_state = 1;
                goodluck_checker = 1;
            }
            if(ch == 'q') break;
        } while(ch != ERR);

        if(ch == 'q') break; // quit game

        if(rc_state == 0 && movement <= (screenWidth*(int) 3.0/4.0) + 5 && movement >= (screenWidth*(int) 3.0/4.0)){ // dead game
            break;
        }
        if(rc_state == 0 && movement <= (screenWidth*(int) 3.0/4.0) + 5 && movement >= (screenWidth*(int) 3.0/4.0)){ // dead game
            break;
        }
        rc_state = render_and_move_car(main_win, rc_state);
        wrefresh(main_win);

        usleep(1000*200);
    }
}

/*
    Race car movement (jump)
*/
int render_and_move_car(WINDOW *main_win, int rc_state) {
    int ground = screenHeight-3-6;


      for (int i = 0; i<3; i++){
        drawImage(main_win, screenHeight-3, mov[i],road,1,5);
        if(i == 1) mov[i] += 3;
        if(i == 2) mov[i] += 4;
        if(i == 3) mov[i] += 6;

        if(mov[i] >= screenWidth){
            mov[i] = 0;
        }

      }

     drawImage(main_win, screenHeight-3, movement, road, 1, 5);
     drawImage(main_win, screenHeight-1, screenWidth*(int) 2.0/5.0, statistics, 1, 35);

     movement += 5;
     if(movement >= screenWidth){
         movement = 0;
     }


    switch(rc_state){
        case 0:
            drawImage(main_win, ground, screenWidth*(int) 3.0/4.0, rccar, 6, 8);
            return 0;
            break;
        case 1:
            drawImage(main_win, ground-1, screenWidth*(int) 3.0/4.0, rccar, 6, 8);
            return 2;
            break;
        case 2:
            drawImage(main_win, ground-2, screenWidth*(int) 3.0/4.0, rccar, 6, 8);
            return 3;
            break;
        case 3:
            drawImage(main_win, ground-1, screenWidth*(int) 3.0/4.0, rccar, 6, 8);
            return 0;
            break;
    }
}

/*
    Drawing Images
*/
void drawImage(WINDOW *main_win, int y, int x, char **arr, int h, int w) {
    for (int current_y = 0; current_y < h; current_y++)
        for (int current_x = 0; current_x < w; current_x++){
            mvwprintw(main_win,current_y + y, current_x + x, "%c", arr[current_y][current_x]);
        }

}

/*
    Flash Game Over screen
*/
void gameover(WINDOW *main_win){
    int blink = 0;
    while(1){
        print_road(main_win);
        wrefresh(main_win);
        if(blink%5 == 0){
            drawImage(main_win, screenHeight-20, screenWidth*(int) 2.0/4.0, gameoverScreen, 1, 18);
        }else{
            wclear(main_win);
        }
        usleep(1000*200);
        blink++;
    }
}
