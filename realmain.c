#include <ncurses.h>
#include <time.h> //this is for the nanosleep
#include <unistd.h>
int screenHeight;
int screenWidth;

char* rccar[6] = {
    "    o   ",
    "    o   ",
    "    o   ",
    " 00 o 00",
    "    o   ",
    "    o   "    
};

void print_road(WINDOW *main_win);
int mainloop(WINDOW *main_win);
int render_and_move_car(WINDOW *main_win, int rc_state);
void drawImage(WINDOW *window, int y, int x, char **arr, int h, int w);


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

    mainloop(main_win);


    endwin();
  
}


void print_road(WINDOW *main_win){
    for(int x = 0; x < screenWidth; x++) {
        mvwprintw(main_win, screenHeight-2, x, "%c", '=');
    }
}

int mainloop(WINDOW *main_win){
    int rc_state = 0;
  
    while(1){
        wclear(main_win);
        print_road(main_win);

        char ch;
        do{
            ch = getch();
            if(ch == ' ' && rc_state == 0) rc_state = 1;
        } while(ch != ERR);

        rc_state = render_and_move_car(main_win, rc_state);
        wrefresh(main_win);

        usleep(1000*200);
    }
}

int render_and_move_car(WINDOW *main_win, int rc_state) {
    int ground = screenHeight-3-6;

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