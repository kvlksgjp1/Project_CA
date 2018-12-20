#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <curses.h>
#include <termios.h>
#include <string.h>
#include <signal.h>
#define an(x) attron(COLOR_PAIR(x))
#define af(x) attroff(COLOR_PAIR(x))
#define MAP_SIZE_ROW 17
#define MAP_SIZE_COL 17
typedef struct player_information{
	char *player_name;
	int isAlive;
	int power;
	int bomb_num_max;
	int bomb_available;
	int row;
	int col;
}player_information;
player_information Player_List[4]={{"aa",0,1,1,1,3,1},{"bb",1,1,1,1,3,2},{"cc",0,1,1,1,3,3},{"dd",1,1,1,1,3,4}};
char	map[MAP_SIZE_ROW][MAP_SIZE_COL]=
						{'2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2','a','b','c','d','1',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ','1',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ','1',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ','M','B',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2',};

void main()
{
	int i,j;
    char cBlock=(char)0x2588;
	
	initscr();
	start_color();
	refresh();
	clear();
	init_pair(1,COLOR_RED,COLOR_BLACK);
	init_pair(2,COLOR_GREEN,COLOR_BLACK);
	init_pair(3,COLOR_BLUE,COLOR_BLACK);
	init_pair(4,COLOR_CYAN,COLOR_BLACK);/* bomb pop color added */
	init_pair(5,COLOR_YELLOW,COLOR_BLACK); //wall color
	init_pair(6,COLOR_BLACK,COLOR_WHITE); //wall color
	init_pair(7,COLOR_BLACK,COLOR_RED);
	init_pair(8,COLOR_BLACK,COLOR_YELLOW);
	init_pair(9,COLOR_BLACK,COLOR_CYAN);
	init_pair(10,COLOR_BLACK,COLOR_MAGENTA);
	init_pair(11,COLOR_BLACK,COLOR_GREEN);
	attron(COLOR_PAIR(10));
	for(i=-9;i<MAP_SIZE_COL;i++){
		addstr("  ");
	}
	move(1,0);
	addstr("  ");
	move(1, MAP_SIZE_COL*2+16);
	addstr("  ");
	attroff(COLOR_PAIR(10));
	printw("\n");

	      printw("   ");an(6);printw("   ");af(6);printw("    ");an(6);printw("   ");af(6);printw("  ");an(6);printw("     ");af(6);printw("           ");an(9);printw("     ");af(9);printw("  ");an(7);printw("     ");af(7);printw("         \n");
	      printw("   ");an(6);printw(" ");af(6);printw("  ");an(6);printw(" ");af(6);printw("  ");an(6);printw(" ");af(6);printw("   ");an(6);printw(" ");af(6);printw("   ");an(6);printw(" ");af(6);printw("           ");an(9);printw("  ");an(6);printw("  ");an(9);printw("   ");an(7);printw("  ");an(6);printw("  ");an(7);printw("     ");af(7);printw("       \n");
	      printw("   ");an(6);printw(" ");af(6);printw("  ");an(6);printw(" ");af(6);printw("  ");an(6);printw(" ");af(6);printw("   ");an(6);printw(" ");af(6);printw("   ");an(6);printw(" ");af(6);printw("          ");an(9);printw("  ");an(6);printw("  ");an(9);printw("   ");an(7);printw("  ");an(6);printw("  ");an(7);printw("       ");af(7);printw("     \n");
	      printw("   ");an(6);printw(" ");af(6);printw("  ");an(6);printw(" ");af(6);printw("  ");an(6);printw(" ");af(6);printw("   ");an(6);printw(" ");af(6);printw("   ");an(6);printw(" ");af(6);printw("           ");an(9);printw("       ");an(7);printw("         ");af(7);printw("      \n");
	      printw("   ");an(6);printw("   ");af(6);printw("    ");an(6);printw("   ");af(6);printw("    ");an(6);printw(" ");af(6);printw("             ");an(9);printw("     ");af(9);printw("  ");an(7);printw("     ");af(7);printw("         \n");
	      printw("                                                    \n");
	      printw("      ");an(11);printw(" ");af(11);printw("     ");an(11);printw("     ");af(11);printw("       ");an(11);printw("  ");af(11);printw("     ");an(11);printw(" ");af(11);printw("     ");an(11);printw("    ");af(11);printw("    ");an(11);printw("     ");af(11);printw("  \n");
	      printw("     ");an(11);printw("   ");af(11);printw("    ");an(11);printw("  ");af(11);printw("   ");an(11);printw("  ");af(11);printw("    ");an(11);printw("  ");af(11);printw("     ");an(11);printw("   ");af(11);printw("    ");an(11);printw("  ");af(11);printw(" ");an(11);printw("  ");af(11);printw("   ");an(11);printw("  ");af(11);printw("     \n");
	      printw("    ");an(11);printw("  ");af(11);printw(" ");an(11);printw("  ");af(11);printw("   ");an(11);printw("  ");af(11);printw("    ");an(11);printw("  ");af(11);printw("  ");an(11);printw("  ");af(11);printw("     ");an(11);printw("  ");af(11);printw(" ");an(11);printw("  ");af(11);printw("   ");an(11);printw("  ");af(11);printw("  ");an(11);printw("  ");af(11);printw("  ");an(11);printw("  ");af(11);printw("     \n");
	      printw("   ");an(11);printw("  ");af(11);printw("   ");an(11);printw("  ");af(11);printw("  ");an(11);printw("  ");af(11);printw("    ");an(11);printw("  ");af(11);printw(" ");an(11);printw("  ");af(11);printw("     ");an(11);printw("  ");af(11);printw("   ");an(11);printw("  ");af(11);printw("  ");an(11);printw("  ");af(11);printw("   ");an(11);printw("  ");af(11);printw(" ");an(11);printw("  ");af(11);printw("     \n");
	      printw("  ");an(11);printw("         ");af(11);printw(" ");an(11);printw("  ");af(11);printw("   ");an(11);printw("  ");af(11);printw("  ");an(11);printw("  ");af(11);printw("    ");an(11);printw("         ");af(11);printw(" ");an(11);printw("  ");af(11);printw("   ");an(11);printw("  ");af(11);printw(" ");an(11);printw("     ");af(11);printw("  \n");
	      printw("  ");an(11);printw("  ");af(11);printw("     ");an(11);printw("  ");af(11);printw(" ");an(11);printw("      ");af(11);printw("   ");an(11);printw("  ");af(11);printw("    ");an(11);printw("  ");af(11);printw("     ");an(11);printw("  ");af(11);printw(" ");an(11);printw("  ");af(11);printw("   ");an(11);printw("  ");af(11);printw(" ");an(11);printw("  ");af(11);printw("     \n");
	      printw("  ");an(11);printw("  ");af(11);printw("     ");an(11);printw("  ");af(11);printw(" ");an(11);printw("  ");af(11);printw("   ");an(11);printw("  ");af(11);printw("   ");an(11);printw("  ");af(11);printw("   ");an(11);printw("  ");af(11);printw("     ");an(11);printw("  ");af(11);printw(" ");an(11);printw("  ");af(11);printw("  ");an(11);printw("  ");af(11);printw("  ");an(11);printw("  ");af(11);printw("     \n");
	      printw("  ");an(11);printw("  ");af(11);printw("     ");an(11);printw("  ");af(11);printw(" ");an(11);printw("  ");af(11);printw("    ");an(11);printw("  ");af(11);printw("   ");an(11);printw("  ");af(11);printw("  ");an(11);printw("  ");af(11);printw("     ");an(11);printw("  ");af(11);printw(" ");an(11);printw("  ");af(11);printw(" ");an(11);printw("  ");af(11);printw("   ");an(11);printw("  ");af(11);printw("     \n");
	      printw("  ");an(11);printw("  ");af(11);printw("     ");an(11);printw("  ");af(11);printw(" ");an(11);printw("  ");af(11);printw("     ");an(11);printw("  ");af(11);printw("   ");an(11);printw("  ");af(11);printw(" ");an(11);printw("  ");af(11);printw("     ");an(11);printw("  ");af(11);printw(" ");an(11);printw("    ");af(11);printw("    ");an(11);printw("     ");af(11);printw("  \n");
	      printw("                                                    \n");
	      printw("                                        waiting...  \n");

	attron(COLOR_PAIR(10));
	addstr("  ");
	attroff(COLOR_PAIR(10));
    	printw("\n");
	
	an(10);printw("  ");move(i+2,MAP_SIZE_COL*2+16);printw("  \n");
	      printw("                                                    \n");
	      printw("                                  A                 \n");
	      printw("          ");an(7);printw("    ");an(10);printw("                   ");an(8);printw("   ");an(10);printw("       B        \n");
	      printw("          ");an(7);printw("    ");an(10);printw("                  ");an(8);printw("     ");an(10);printw("     ");an(8);printw("   ");an(10);printw("       \n");
	      printw("          ");an(7);printw("    ");an(10);printw("                   ");an(8);printw("   ");an(10);printw("     ");an(8);printw("     ");an(10);printw("      \n");
	      printw("    ");an(7);printw("                ");an(10);printw("                 C    ");an(8);printw("   ");an(10);printw("       \n");
	      printw("    ");an(7);printw("                ");an(10);printw("                ");an(8);printw("   ");an(10);printw("             \n");
	      printw("          ");an(7);printw("    ");an(10);printw("                     ");an(8);printw("     ");an(10);printw("            \n");
	      printw("          ");an(7);printw("    ");an(10);printw("                      ");an(8);printw("   ");an(10);printw("     .....   \n");
	      printw("          ");an(7);printw("    ");an(10);printw("       L      R              .......  \n");
	      printw("                    ");an(9);printw("   ");an(10);printw("    ");an(9);printw("   ");an(10);printw("              .....   \n");
	      printw("                                        *DOT ARCADE*\n");
	

	move(LINES-1,COLS-1);
	refresh();
	getchar();
	endwin();
}

