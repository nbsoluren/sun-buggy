#include <ncurses.h>
#include <time.h> //this is for the nanosleep
#include <unistd.h>
int screenHeight;
int screenWidth;
float movement = 0;

char* rccar[6] = {
    "    o   ",
    "    o   ",
    "    o   ",
    " 00 o 00",
    "    o   ",
    "    o   "    
};

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

char* instruction_1[44] = {
    " P r e s s   S p a c e   T o   P l a y . . .",
};

char* road[84] = {
    "    ######    ################        #################         #############   ####"   
};

char* readMe[66] = {
    "Sun-Buggy version 1.0.00, Copyright 2018.                         ",
    "Sun-Buggy comes with ABSOLUTELY NO WARRANTY; for details type 'w'.",
    "This is free software, and you are welcome to redistribute it     ",
    "under certain conditions: type 'c' for details                    "
};

char* gameoverScreen[18] = {
    " G A M E   O V E R"
};

char* blank[1] = {
    " "
};

char* statistics[35] = {
    "Level: 1    Lives: 3    Score: 0   "
};

void print_road(WINDOW *main_win);
void previewscreen(WINDOW *main_win);
int mainloop(WINDOW *main_win);
int render_and_move_car(WINDOW *main_win, int rc_state);
void drawImage(WINDOW *window, int y, int x, char **arr, int h, int w);
void gameover();

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


void print_road(WINDOW *main_win){
    for(int x = 0; x < screenWidth; x++) {
        mvwprintw(main_win, screenHeight-3, x, "%c", '#');
        mvwprintw(main_win, screenHeight-2, x, "%c", '#');
    }
}

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

int mainloop(WINDOW *main_win){
    int rc_state = 0;
    char ch;

    while(1){
        wclear(main_win);
        print_road(main_win);

        do{
            ch = getch();
            if(ch == ' ' && rc_state == 0) rc_state = 1;
            if(ch == 'q') break;
        } while(ch != ERR);

        if(ch == 'q') break;

        rc_state = render_and_move_car(main_win, rc_state);
        wrefresh(main_win);

        usleep(1000*200);
    }
}

int render_and_move_car(WINDOW *main_win, int rc_state) {
    int ground = screenHeight-3-6;
    
     drawImage(main_win, screenHeight-3, movement, road, 1, 84);
     drawImage(main_win, screenHeight-1, screenWidth*(int) 2.0/5.0, statistics, 1, 35);
    
     movement += 5;
     if(movement==200){
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

void drawImage(WINDOW *main_win, int y, int x, char **arr, int h, int w) {
    for (int current_y = 0; current_y < h; current_y++) 
        for (int current_x = 0; current_x < w; current_x++){
            mvwprintw(main_win,current_y + y, current_x + x, "%c", arr[current_y][current_x]);
        }
        
}

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