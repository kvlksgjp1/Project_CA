#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netdb.h>
#include <strings.h>
#include <curses.h>
#include <fcntl.h>
#include <termio.h> 
#include <pthread.h>
#include <signal.h>

#define CHATDATA 1024
#define MAP_SIZE_ROW 10
#define MAP_SIZE_COL 10

#define MAX_USER_SIZE 4
#define Max_Player_Number 

#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68

typedef struct player_information{
	char *player_name;
	int isAlive;
	int power;
	int bomb_num_max;
	int bomb_available;
	int row;
	int col;
}player_information;

player_information Player_List[4];

char	map[MAP_SIZE_ROW][MAP_SIZE_COL]=
						{'2','2','2','2','2','2','2','2','2','2',
						 '2',' ','G','A','M','E',' ',' ',' ','2',
						 '2',' ',' ','S','T','A','R','T',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2','2','2','2','2','2','2','2','2','2',};

void *do_send_chat(void *);
void *do_receive_chat(void *);
void set_cr_noecho_mode(void);
void enable_kbd_signals();
void drawMap();

pthread_t thread_1, thread_2;

char escape[] = "exit";
char nickname[20];
int c_socket;
int done=0;

int connectServer(int argc, char *argv[]);
void play();

int main (int argc, char *argv[]) {
	
	connectServer(argc, argv);
	
	initPlayer();
	play();
	end();
}

void play()
{

	while( !done )
	{		
		pause();
	}

}

int connectServer(int argc, char *argv[])
{
	struct sockaddr_in c_addr;
	struct hostent *hp;
	int len;
	char chatData[CHATDATA];
	char buf[CHATDATA];
	int nfds;
	fd_set read_fds;
	int n;

	if (argc < 3) {
		printf("usage : %s ip_address port_number\n", argv[0]);
		exit(-1);
	}
    
	c_socket = socket(PF_INET, SOCK_STREAM, 0);
    
	memset(&c_addr, 0, sizeof(c_addr));
	hp=gethostbyname(argv[1]);
	bcopy(hp->h_addr,(struct sockaddr*)&c_addr.sin_addr,hp->h_length);
	//c_addr.sin_addr.s_addr = inet_addr(argv[1]); // 서버 주소를 설정한다.
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = htons(atoi(argv[2]));
    
	printf("Input Nickname : ");
	scanf("%s", nickname);
	getchar();
	// 닉네임을 입력받고 설정한 주소를 통해 연결을 시도한다.
	if (connect(c_socket, (struct sockaddr *)&c_addr, sizeof(c_addr)) == -1) {
		printf("Can not connect\n");
		return -1;
	}

	// 연결한 소켓을 이용하여 채팅 송수신 함수를 스레드로 생성 후 실행한다.
	pthread_create(&thread_1, NULL, do_send_chat, (void *) c_socket);
	pthread_create(&thread_2, NULL, do_receive_chat, (void *) c_socket);
    
	init();
	//drawMap();

	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
    
	close(c_socket);
}

void *do_send_chat(void *arg) {
	char chatData[CHATDATA];
	char buf[CHATDATA];
	int n;
	int c_socket = (int) arg;

	while(1) {/*
		memset(buf, 0, sizeof(buf));
		// buf에 값이 들어오면 이 값을 닉네임과 합쳐서 서버와 연결된 소켓으로 송신한다.
		if ((n = read(0, buf, sizeof(buf))) > 0) {
			sprintf(chatData, "[%s]%s", nickname, buf);
			write(c_socket, chatData, strlen(chatData));
			
			// 종료 문자열이 들어오면 스레드를 삭제한다.
			if (!strncmp(buf, escape, strlen(escape))) {
				pthread_kill(thread_2, SIGINT);
				break;
			}
		}*/
	}
}

void initPlayer()
{
	int i;


	for(i=0;i<MAX_USER_SIZE;i++){

		Player_List[i].player_name=NULL;
		Player_List[i].power=0;
		Player_List[i].bomb_num_max=0;
		Player_List[i].bomb_available=0;
		Player_List[i].isAlive=0;
		Player_List[i].row=0;
		Player_List[i].col=0;

	}

	printf("Player Init 완료\n");
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

void enable_kbd_signals()
{
	int  fd_flags;

	fcntl(0, F_SETOWN, getpid());         
	fd_flags = fcntl(0, F_GETFL);         
	fcntl(0, F_SETFL, (fd_flags|O_ASYNC));
}

void on_input(int signum)
{		
	int 	c = getchar();		/* grab the char */
	char chatData[CHATDATA];

    if(c==27)
    {
        c=getchar();
        c=getchar();
    }
	
	//printf("%c caught\n",c);
	if ( c == 'Q' || c == EOF )
		done = 1;
	else if ( c==' ' )
	{
		sprintf(chatData, "[%s]%s", nickname, "BOMB");
		write(c_socket, chatData, strlen(chatData));
	}
	else if( c==LEFT )//LEFT
	{
		sprintf(chatData, "[%s]%s", nickname, "LEFT");
		write(c_socket, chatData, strlen(chatData));
	}
	else if( c==RIGHT )//RIGHT
	{
		sprintf(chatData, "[%s]%s", nickname, "RIGHT");
		write(c_socket, chatData, strlen(chatData));
	}
	else if( c==UP )//UP
	{
		sprintf(chatData, "[%s]%s", nickname, "UP");
		write(c_socket, chatData, strlen(chatData));
	}
	else if( c==DOWN )//DOWN
	{
		sprintf(chatData, "[%s]%s", nickname, "DOWN");
		write(c_socket, chatData, strlen(chatData));
	}
	else if( c=='q' )//DOWN
	{
		sprintf(chatData, "[%s]%s", nickname, "WALL");
		write(c_socket, chatData, strlen(chatData));
	}
}

void init()
{
	printf("init수행\n");
	initscr();
	set_cr_noecho_mode();
	signal(SIGIO, on_input);     
	enable_kbd_signals();     
	start_color();
	
	//drawMap();
}

void *do_receive_chat(void *arg) {
	char chatData[CHATDATA];
	int n;
	int c_socket = (int) arg;
    int i,j;
	int count=0;
	char *str;
	char *pos;
	char dates[50];
	int row;
	int col;

	while(1) {
		memset(chatData, 0, sizeof(chatData));
		// 읽어온 값이 있으면 값을 출력한다.
		if ((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
			//printf("서버로 부터 받은 데이터\r\n");
			//printf("%s\r\n",chatData);
			//for(i=0;i<50;i++)
				//printf("[%d] : %c\r\n",i,chatData[i]);
			//write(1, chatData, n);// 1은 표준 출력을 의미한다.
			if(contain(chatData,"Q")){
			pos=strchr(chatData,'Q');
			*pos='\0';}

			if(contain(chatData,"[0]"))
				{
				sscanf(chatData,"[0]%s %d %d %d %d %d %d",dates,&Player_List[0].isAlive,&Player_List[0].power,&Player_List[0].bomb_num_max,&Player_List[0].bomb_available,&Player_List[0].row,&Player_List[0].col);
				}
			else if(contain(chatData,"[1]"))
				{
				sscanf(chatData,"[1]%s %d %d %d %d %d %d",dates,&Player_List[1].isAlive,&Player_List[1].power,&Player_List[1].bomb_num_max,&Player_List[1].bomb_available,&Player_List[1].row,&Player_List[1].col);
				}
			else if(contain(chatData,"[2]"))
				{
				sscanf(chatData,"[2]%s %d %d %d %d %d %d",dates,&Player_List[2].isAlive,&Player_List[2].power,&Player_List[2].bomb_num_max,&Player_List[2].bomb_available,&Player_List[2].row,&Player_List[2].col);
				}
			else if(contain(chatData,"[3]"))
				{
				sscanf(chatData,"[3]%s %d %d %d %d %d %d",dates,&Player_List[3].isAlive,&Player_List[3].power,&Player_List[3].bomb_num_max,&Player_List[3].bomb_available,&Player_List[3].row,&Player_List[3].col);
				}
			else{
				count=0;
				for(i=0;i<MAP_SIZE_ROW;i++)
					for(j=0;j<MAP_SIZE_COL;j++)
						map[i][j]=chatData[count++];
			}

/*
			for(i=0;i<4;i++)
			{
				switch(i)
				{
					case 0:
						str=strstr(chatData,"[0]");
						printf("this:%s\n",str);
						break;
					case 1:
						str=strstr(chatData,"[1]");
						break;
					case 2:
						str=strstr(chatData,"[2]");
						break;
					case 3:
						str=strstr(chatData,"[3]");
						break;

				}
				//read(c_socket, chatData, sizeof(chatData));
				if(str!=NULL){
				sscanf(str+3,"player_name:%s isAlive:%d power:%d bomb_num_max:%d bomb_available:%d row:%d col:%d",Player_List[i].player_name,&Player_List[i].isAlive,&Player_List[i].power,&Player_List[i].bomb_num_max,&Player_List[i].bomb_available,&Player_List[i].row,&Player_List[i].col);
				printf("%s",Player_List[i].player_name);}
			}
*/

				drawMap();
				/*
				for(i=0;i<MAP_SIZE_ROW;i++)
					{
						for(j=0;j<MAP_SIZE_COL;j++)
						{
							printf("%c",map[i][j]);
						}
					printf("\n");
					}*/
		
		}
	}
}

void end()
{
	endwin();
}

int contain(char *s1, char *find)
{	
	char *ptr;

	ptr=strstr(s1,find);
	if(ptr!=NULL)
		return 1;

	return 0;
}

void drawMap()
{
	int i,j;
    char cBlock=(char)0x2588;

	clear();
	init_pair(1,COLOR_RED,COLOR_BLACK);
	init_pair(2,COLOR_GREEN,COLOR_BLACK);
	init_pair(3,COLOR_BLUE,COLOR_BLACK);
	init_pair(4,COLOR_CYAN,COLOR_BLACK);/* bomb pop color added */
	init_pair(5,COLOR_YELLOW,COLOR_BLACK); //wall color

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
		else if(map[i][j]=='a')
		{
			attron(COLOR_PAIR(2));
			addch('P'|A_REVERSE);
			move(i,j*2+1);
			addch('1'|A_REVERSE);
			attron(COLOR_PAIR(2));
		}
		else if(map[i][j]=='b')
		{
			attron(COLOR_PAIR(2));
			addch('P'|A_REVERSE);
			move(i,j*2+1);
			addch('2'|A_REVERSE);
			attron(COLOR_PAIR(2));
		}
		else if(map[i][j]=='c')
		{
			attron(COLOR_PAIR(2));
			addch('P'|A_REVERSE);
			move(i,j*2+1);
			addch('3'|A_REVERSE);
			attron(COLOR_PAIR(2));
		}
		else if(map[i][j]=='d')
		{
			attron(COLOR_PAIR(2));
			addch('P'|A_REVERSE);
			move(i,j*2+1);
			addch('4'|A_REVERSE);
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
		else if(map[i][j] == '1')
				{
					attron(COLOR_PAIR(5));
					addch(' '|A_REVERSE);
					move(i,j*2+1);
					addch(' '|A_REVERSE);
					attron(COLOR_PAIR(5));

		}

	}
    //addch(' '|A_REVERSE);
    //addch(' '|A_REVERSE);
    //printw("0x2588: "); addch(cBlock); printw("\n");
	for(i=0;i<4;i++)
		printw("\n%d -> row:%d col:%d alive:%d\n",i,Player_List[i].row,Player_List[i].col,Player_List[i].isAlive);
	move(LINES-1,COLS-1);
	refresh();
}
