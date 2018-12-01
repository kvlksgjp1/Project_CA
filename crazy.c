#include   <stdio.h>
#include   <curses.h>
#include   <signal.h>
#include   <fcntl.h>
#include    <string.h>
#include    <sys/time.h>
#include   <unistd.h>
#include   <termio.h> 
#include <pthread.h>
#include <stdlib.h>

#define MAP_SIZE_ROW 10
#define MAP_SIZE_COL 10
#define LEFT 75
#define RIGHT 72
#define UP 77
#define DOWN 80

typedef struct player_position{
   int row;
   int col;
}player_position;

int   delay = 20;   /* how long to wait   */
int   done  = 0;
int   power = 3;
player_position position;

char   map[MAP_SIZE_ROW][MAP_SIZE_COL]=
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

void *boom();
void set_cr_noecho_mode(void); 
int set_ticker(int);       
void enable_kbd_signals();
void init();
void play();
void drawMap();
void update(int,int,char);
void end();

int main(void)
{
   void   on_alarm(int);   /* handler for alarm   */
   void   on_input(int);   /* handler for keybd    */
   

   init();
   play();
   end();

   return 0;
}

void on_input(int signum)
{      
   int    c = getchar();      /* grab the char */
   pthread_t thr;
   int thr_id;
   int status;

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
   else if( c=='a' )//LEFT
   {
      if(map[position.row][position.col-1]!='2' && map[position.row][position.col-1]!='M'){
         if(map[position.row][position.col]!='M'){
            update(position.row,position.col,' ');
         }
         position.col-=1;
         update(position.row,position.col,'C');
      }
   }
   else if( c=='d' )//RIGHT
   {
      if(map[position.row][position.col+1]!='2' && map[position.row][position.col+1]!='M'){
         if(map[position.row][position.col]!='M'){
            update(position.row,position.col,' ');
         }
         position.col+=1;
         update(position.row,position.col,'C');
      }
   }
   else if( c=='w' )//UP
   {
      if(map[position.row-1][position.col]!='2' && map[position.row-1][position.col]!='M'){
         if(map[position.row][position.col]!='M'){
            update(position.row,position.col,' ');
         }
         position.row-=1;
         update(position.row,position.col,'C');
      }
   }
   else if( c=='s' )//DOWN
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
   signal(SIGALRM, on_alarm);   /* reset, just in case   */
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
   ttystate.c_lflag   &= ~ICANON;
   ttystate.c_lflag   &= ~ECHO;
   ttystate.c_cc[VMIN]   = 1;
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

   clear();
   for(i=0;i<MAP_SIZE_ROW;i++)
   for(j=0;j<MAP_SIZE_COL;j++)
   {
      move(i,j);                /* get into position      */
      addch(map[i][j]);        

   }           
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
	int i=0;
	player_position bomb = position;

	update(bomb.row,bomb.col,'M');
	sleep(1);
	update(bomb.row,bomb.col,'M');
	sleep(1);
	update(bomb.row,bomb.col,'M');
	sleep(1);

	update(bomb.row,bomb.col,'B');
	for(i=1;i<=power;i++){
		if(map[bomb.row][bomb.col+i]!='2')
			update(bomb.row,bomb.col+i,'B');
		else
			break;
		}

	for(i=1;i<=power;i++){
		if(map[bomb.row][bomb.col-i]!='2')
			update(bomb.row,bomb.col-i,'B');
		else
			break;
		}

	for(i=1;i<=power;i++){
		if(map[bomb.row+i][bomb.col]!='2')
			update(bomb.row+i,bomb.col,'B');
		else
			break;
		}

	for(i=1;i<=power;i++){
		if(map[bomb.row-i][bomb.col]!='2')
			update(bomb.row-i,bomb.col,'B');
		else
			break;
		}

	usleep(100000);
	
	update(bomb.row,bomb.col,' ');	
	for(i=1;i<=power;i++){
		if(map[bomb.row][bomb.col+i]!='2')
			update(bomb.row,bomb.col+i,' ');
		else
			break;
		}

	for(i=1;i<=power;i++){
		if(map[bomb.row][bomb.col-i]!='2')
			update(bomb.row,bomb.col-i,' ');
		else
			break;
		}

	for(i=1;i<=power;i++){
		if(map[bomb.row+i][bomb.col]!='2')
			update(bomb.row+i,bomb.col,' ');
		else
			break;
		}

	for(i=1;i<=power;i++){
		if(map[bomb.row-i][bomb.col]!='2')
			update(bomb.row-i,bomb.col,' ');
		else
			break;
		}
}

int set_ticker( int n_msecs )
{
   struct itimerval new_timeset;
   long    n_sec, n_usecs;
   
   n_sec = n_msecs / 1000 ;         /* int part*/
   n_usecs = ( n_msecs % 1000 ) * 1000L ;     /* remainder*/

   new_timeset.it_interval.tv_sec  = n_sec;        /* set reload       */
   new_timeset.it_interval.tv_usec = n_usecs;      /* new ticker value */
   new_timeset.it_value.tv_sec     = n_sec  ;      /* store this       */
   new_timeset.it_value.tv_usec    = n_usecs ;     /* and this         */

   return setitimer(ITIMER_REAL, &new_timeset, NULL);
}
