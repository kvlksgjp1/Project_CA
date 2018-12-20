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
	for(i=0;i<MAP_SIZE_ROW;i++){
	attron(COLOR_PAIR(10));
	addstr("  ");
	attroff(COLOR_PAIR(10));
	addstr("  ");
	for(j=0;j<MAP_SIZE_COL;j++)
	{
		
		//move(LINES/2 - MAP_SIZE_ROW/2 + i,2+j*2);		          /* get into position      */
		if(map[i][j]==' ')
		{
			addch(' ');
			addch(' ');
		}
		else if(map[i][j]=='2')
		{
			attron(COLOR_PAIR(1));
			addch(cBlock);
			addch(cBlock);
			attroff(COLOR_PAIR(1));
		}
		else if(map[i][j]=='a')
		{
			attron(COLOR_PAIR(2));
			addch('P'|A_REVERSE);
			addch('1'|A_REVERSE);
			attroff(COLOR_PAIR(2));
		}
		else if(map[i][j]=='b')
		{
			attron(COLOR_PAIR(2));
			addch('P'|A_REVERSE);
			addch('2'|A_REVERSE);
			attroff(COLOR_PAIR(2));
		}
		else if(map[i][j]=='c')
		{
			attron(COLOR_PAIR(2));
			addch('P'|A_REVERSE);

			addch('3'|A_REVERSE);
			attroff(COLOR_PAIR(2));
		}
		else if(map[i][j]=='d')
		{
			attron(COLOR_PAIR(2));
			addch('P'|A_REVERSE);

			addch('4'|A_REVERSE);
			attroff(COLOR_PAIR(2));
		}
		else if(map[i][j]=='M')
		{
			attron(COLOR_PAIR(3));
			addch(' '|A_REVERSE);

			addch(' '|A_REVERSE);
			attroff(COLOR_PAIR(3));
		}	
		else if(map[i][j]=='B')
				{
					attron(COLOR_PAIR(4));
					addch(' '|A_REVERSE);
					addch(' '|A_REVERSE);
					attroff(COLOR_PAIR(4));
		} 
		else if(map[i][j] == '1')
				{
					attron(COLOR_PAIR(5));
					addch(' '|A_REVERSE);
					addch(' '|A_REVERSE);
					attroff(COLOR_PAIR(5));

		}
		

	}
	switch (i){
	case 0:
		printw("  #P1");
		Player_List[0].isAlive ? (an(5),addstr(" ALIVE "),af(5)) : (an(1),addstr(" DEAD  "),af(1));
		break;
	case MAP_SIZE_ROW/3:
		printw("  #P2");
		Player_List[1].isAlive ? (an(5),addstr(" ALIVE "),af(5)) : (an(1),addstr(" DEAD  "),af(1));
		break;
	case MAP_SIZE_ROW/3*2:
		printw("  #P3");
		Player_List[2].isAlive ? (an(5),addstr(" ALIVE "),af(5)) : (an(1),addstr(" DEAD  "),af(1));
		break;
	case MAP_SIZE_ROW-1:
		printw("  #P4");
		Player_List[3].isAlive ? (an(5),addstr(" ALIVE "),af(5)) : (an(1),addstr(" DEAD  "),af(1));
		break;
	default:
		move(i+2, MAP_SIZE_COL*2+16);
	}
	
	attron(COLOR_PAIR(10));
	addstr("  ");
	attroff(COLOR_PAIR(10));
    	printw("\n");
	}
	an(10);printw("  ");move(i+2,MAP_SIZE_COL*2+16);printw("  \n");
	      printw("                                                    \n");
	      printw("                                 A                  \n");
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

