#include	<stdio.h>
#include	<curses.h>
#include	<signal.h>
#include	<fcntl.h>
#include 	<string.h>
#include    <sys/time.h>
#include	<unistd.h>
#include	<termio.h>
#include 	<pthread.h>
#include	 <stdlib.h>
#include     	 <time.h>

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
int   power = 3;

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

void explosion(int,int);
void *boom();
void set_cr_noecho_mode(void); 
int set_ticker(int); 		
void enable_kbd_signals();
void init();
void play();
void drawMap();
void update(int,int,char);
void end();
void wall();
int main(void)
{	void	on_alarm(int);	/* handler for alarm	*/
	void	on_input(int);	/* handler for keybd    */
	
	wall();
	init();
	play();
	end();

	return 0;
}

void on_input(int signum)
{		
	int 	c = getchar();		/* grab the char */
	pthread_t thr;
   	int thr_id;
   	int status;


    if(c==27)
    {
        c=getchar();
        c=getchar();
    }

	if ( c == 'Q' || c == EOF )
		done = 1;
	else if ( c==' ' )
 {
      if(map[position.row][position.col]!='M'){
         thr_id=pthread_create(&thr, NULL, boom, NULL);
	   if (thr_id < 0)
   	 {
    	    perror("thread create error : ");
    	    exit(0);
    	}
	//pthread_join(thr,(void **)&status);
	}
   }
	else if( c==LEFT )//LEFT
	{
		if(map[position.row][position.col-1]!='2' && map[position.row][position.col-1]!='M' && map[position.row][position.col-1] !='1'){
			if(map[position.row][position.col]!='M'){
				update(position.row,position.col,' ');
			}
			position.col-=1;
			update(position.row,position.col,'C');
		}
	}
	else if( c==RIGHT )//RIGHT
	{
		if(map[position.row][position.col+1]!='2' && map[position.row][position.col+1]!='M'&& map[position.row][position.col+1] !='1'){
			if(map[position.row][position.col]!='M'){
				update(position.row,position.col,' ');
			}
			position.col+=1;
			update(position.row,position.col,'C');
		}
	}
	else if( c==UP )//UP
	{
		if(map[position.row-1][position.col]!='2' && map[position.row-1][position.col]!='M'&& map[position.row-1][position.col] !='1'){
			if(map[position.row][position.col]!='M'){
				update(position.row,position.col,' ');
			}
			position.row-=1;
			update(position.row,position.col,'C');
		}
	}
	else if( c==DOWN )//DOWN
	{
		if(map[position.row+1][position.col]!='2' && map[position.row+1][position.col]!='M'&& map[position.row+1][position.col] !='1'){
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
	char bBlock = (char)0x08; //일단 아무 숫자 넣음
	clear();
	init_pair(5,COLOR_YELLOW,COLOR_BLACK); //wall color
	init_pair(1,COLOR_RED,COLOR_BLACK);
	init_pair(2,COLOR_GREEN,COLOR_BLACK);
	init_pair(3,COLOR_BLUE,COLOR_BLACK);
	init_pair(4,COLOR_CYAN,COLOR_BLACK);/* bomb pop color added */

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
		else if(map[i][j] == '1')
		{
			attron(COLOR_PAIR(5));
			addch(' '|A_REVERSE);
			move(i,j*2+1);
			addch(' '|A_REVERSE);
			attron(COLOR_PAIR(5));

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
		else if(map[i][j]=='B')
		{
			attron(COLOR_PAIR(4));
			addch(' '|A_REVERSE);
			move(i,j*2+1);
			addch(' '|A_REVERSE);
			attron(COLOR_PAIR(4));
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


void *boom() /*2018.11.30 KJY add*/
{
	player_position bomb = position;
	update(position.row,position.col,'M');

	sleep(3);

	if(map[bomb.row][bomb.col]=='M')/* if it doesn't explode with earlier other thread*/
		explosion(bomb.row,bomb.col);/* bomb at thread's original position */
}

void explosion(int row,int col){
	int i=0;

	update(row,col,'B');
	for(i=1;i<=power;i++){
		if(map[row][col+i]!='2')
		{
			if(map[row][col+i]=='M')
				{explosion(row,col+i);
				break;}	
			update(row,col+i,'B');}
		else
			break;
		}

	for(i=1;i<=power;i++){
		if(map[row][col-i]!='2')
		{
			if(map[row][col-i]=='M')
				{explosion(row,col-i);
				break;}	
			update(row,col-i,'B');}
		else
			break;
		}

	for(i=1;i<=power;i++){
		if(map[row+i][col]!='2')
		{
			if(map[row+i][col]=='M')
				{explosion(row+i,col);
				break;}	
			update(row+i,col,'B');}
		else
			break;
		}

	for(i=1;i<=power;i++){
		if(map[row-i][col]!='2')
		{
			if(map[row-i][col]=='M')
				{explosion(row-i,col);
				break;}	
			update(row-i,col,'B');}
		else
			break;
		}

	usleep(100000);

	update(row,col,' ');	
	for(i=1;i<=power;i++){
		if(map[row][col+i]!='2')
		{
			update(row,col+i,' ');}
		else
			break;
		}

	for(i=1;i<=power;i++){
		if(map[row][col-i]!='2')
		{	
			update(row,col-i,' ');}
		else
			break;
		}

	for(i=1;i<=power;i++){
		if(map[row+i][col]!='2')
		{
			update(row+i,col,' ');}
		else
			break;
		}

	for(i=1;i<=power;i++){
		if(map[row-i][col]!='2')
		{
			update(row-i,col,' ');}
		else
			break;
		}
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

void wall()
{
	srand(time(NULL));
	int i,j;
	int random_r, random_c;
	for(i=0; i<10;i++){
		random_r = rand() % MAP_SIZE_ROW;
		random_c = rand() % MAP_SIZE_COL;
		
		if(map[random_r][random_c] != '2')
			map[random_r][random_c] ='1';
	}

}
