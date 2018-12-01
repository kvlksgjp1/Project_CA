#include	<stdio.h>
#include	<curses.h>
#include	<signal.h>
#include	<fcntl.h>
#include 	<string.h>
#include    <sys/time.h>
#include	<unistd.h>
#include	<termio.h> 

#define MAP_SIZE_ROW 10
#define MAP_SIZE_COL 10
#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68

typedef struct player_position{
	int row;
	int col;
}player_position;

int	delay = 20;	/* how long to wait	*/
int	done  = 0;

player_position position;

char	map[MAP_SIZE_ROW][MAP_SIZE_COL]=
						{'2','2','2','2','2','2','2','2','2','2',
						 '2','C',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2','2','2','2','2','2','2','2','2','2',};



void set_cr_noecho_mode(void); 
int set_ticker(int); 		
void enable_kbd_signals();
void init();
void play();
void drawMap();
void update(int,int,char);
void end();

int main(void)
{	void	on_alarm(int);	/* handler for alarm	*/
	void	on_input(int);	/* handler for keybd    */
	

	init();
	play();
	end();

	return 0;
}

void on_input(int signum)
{		
	int 	c = getchar();		/* grab the char */

    if(c==27)
    {
        c=getchar();
        c=getchar();
    }

	if ( c == 'Q' || c == EOF )
		done = 1;
	else if ( c==' ' )
	{

		if(map[position.row][position.col]!='M')
			update(position.row,position.col,'M');

	}
	else if( c==LEFT )//LEFT
	{
		if(map[position.row][position.col-1]!='2' && map[position.row][position.col-1]!='M'){
			if(map[position.row][position.col]!='M'){
				update(position.row,position.col,' ');
			}
			position.col-=1;
			update(position.row,position.col,'C');
		}
	}
	else if( c==RIGHT )//RIGHT
	{
		if(map[position.row][position.col+1]!='2' && map[position.row][position.col+1]!='M'){
			if(map[position.row][position.col]!='M'){
				update(position.row,position.col,' ');
			}
			position.col+=1;
			update(position.row,position.col,'C');
		}
	}
	else if( c==UP )//UP
	{
		if(map[position.row-1][position.col]!='2' && map[position.row-1][position.col]!='M'){
			if(map[position.row][position.col]!='M'){
				update(position.row,position.col,' ');
			}
			position.row-=1;
			update(position.row,position.col,'C');
		}
	}
	else if( c==DOWN )//DOWN
	{
		if(map[position.row+1][position.col]!='2' && map[position.row+1][position.col]!='M'){
			if(map[position.row][position.col]!='M'){
				update(position.row,position.col,' ');
			}
			position.row+=1;
			update(position.row,position.col,'C');
		}
	}
}

void on_alarm(int signum)
{
	signal(SIGALRM, on_alarm);	/* reset, just in case	*/
	drawMap();
}

void enable_kbd_signals()
{
	int  fd_flags;

	fcntl(0, F_SETOWN, getpid());         
	fd_flags = fcntl(0, F_GETFL);         
	fcntl(0, F_SETFL, (fd_flags|O_ASYNC));
}

void set_cr_noecho_mode(void)
{
	struct termios ttystate;
	
	tcgetattr( 0, &ttystate );
	ttystate.c_lflag	&= ~ICANON;
	ttystate.c_lflag	&= ~ECHO;
	ttystate.c_cc[VMIN]	= 1;
	tcsetattr( 0, TCSANOW, &ttystate);
}


void init()
{
	initscr();		
	set_cr_noecho_mode(); 
	enable_kbd_signals();     
	signal(SIGIO, on_input);      
	signal(SIGALRM, on_alarm); 
	position.row=1;
	position.col=1;
	start_color();
}

void play()
{

	set_ticker(delay);
	while( !done )
	{		
		pause();
	}


}

void drawMap()
{
	int i,j;
    char cBlock=(char)0x2588;

	clear();
	init_pair(1,COLOR_RED,COLOR_BLACK);
	init_pair(2,COLOR_GREEN,COLOR_BLACK);
	init_pair(3,COLOR_BLUE,COLOR_BLACK);

	for(i=0;i<MAP_SIZE_ROW;i++)
	for(j=0;j<MAP_SIZE_COL;j++)
	{
		move(i,j*2);		          /* get into position      */
		if(map[i][j]==' ')
		{
			addch(' ');
			move(i,j*2+1);
			addch(' ');
		}
		else if(map[i][j]=='2')
		{
			attron(COLOR_PAIR(1));
			addch(cBlock);
			move(i,j*2+1);
			addch(cBlock);
			attroff(COLOR_PAIR(1));
		}
		else if(map[i][j]=='C')
		{
			attron(COLOR_PAIR(2));
			addch(' '|A_REVERSE);
			move(i,j*2+1);
			addch(' '|A_REVERSE);
			attron(COLOR_PAIR(2));
		}
		else if(map[i][j]=='M')
		{
			attron(COLOR_PAIR(3));
			addch(' '|A_REVERSE);
			move(i,j*2+1);
			addch(' '|A_REVERSE);
			attron(COLOR_PAIR(3));
		}	

	}
    //addch(' '|A_REVERSE);
    //addch(' '|A_REVERSE);
    //printw("0x2588: "); addch(cBlock); printw("\n");
	move(50,50);
	refresh();
}

void end()
{
	endwin();
}

void update(int row, int col, char value)
{
	map[row][col]=value;
}


int set_ticker( int n_msecs )
{
	struct itimerval new_timeset;
	long    n_sec, n_usecs;
	
	n_sec = n_msecs / 1000 ;			/* int part*/
	n_usecs = ( n_msecs % 1000 ) * 1000L ;  	/* remainder*/

	new_timeset.it_interval.tv_sec  = n_sec;        /* set reload       */
	new_timeset.it_interval.tv_usec = n_usecs;      /* new ticker value */
	new_timeset.it_value.tv_sec     = n_sec  ;      /* store this       */
	new_timeset.it_value.tv_usec    = n_usecs ;     /* and this         */

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}


