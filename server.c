#include <stdio.h>

#include <stdlib.h>

#include <signal.h>

#include <string.h>

#include <unistd.h>

#include <netinet/in.h>

#include <sys/socket.h>

#include <unistd.h>

#include <sys/time.h>

#include <time.h>

#include <netdb.h>

#include <pthread.h> // gcc 옵션에 -lpthread 사용.

 

#define MAP_SIZE_ROW 17

#define MAP_SIZE_COL 17

 

#define MAX_BOMB_NUM 5

#define MAX_POWER_LEN 7

 

#define MAX_NAME_SIZE 100

#define MAX_USER_SIZE 4

 

#define UP 65

#define DOWN 66

#define RIGHT 67

#define LEFT 68

 

typedef struct player_information{

	char *player_name;

	int isAlive;

	int power;

	int bomb_num_max;

	int bomb_onmap;

	int row;

	int col;

}player_information;

 

typedef struct bomb_position{

	int row;

	int col;

	int index;

}bomb_position;

 

int	delay = 60;	/* how long to wait	*/

int	done = 0;

int power = 3;

 

int isEnd = 0;

int startPlay = 0;

 

int Max_Player_Number=0;

int current_player_number=0;

 

player_information Player_List[MAX_USER_SIZE];

bomb_position bomb_pos;

 

char	map[MAP_SIZE_ROW][MAP_SIZE_COL]=
						{'2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2',
						 '2',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ','1',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ','1',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2',' ',' ',' ',' ','1',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','2',
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
						 '2',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','2',
						 '2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2',};

 

int	bomb_map[MAP_SIZE_ROW][MAP_SIZE_COL];

						 

						 

void* do_chat(void *);

void *boom();

void explosion(int row,int col);

int set_ticker(int); 		

void drawMap();

 

pthread_t thread;

pthread_mutex_t mutex;

 

#define MAX_CLIENT 10

#define CHATDATA 1024

#define HOSTLEN 256

#define INVALID_SOCK -1 // 클라이언트 소켓 배열의 초기 값.

 

int list_c[MAX_CLIENT]; // 클라이언트들의 소켓 번호를 저장하기 위한 배열

 

char escape[] = "exit";

char greeting[] = "Welcome to chatting room\n";

char CODE200[] = "Sorry No More Connection\n";

 

int popClient(int s);

int pushClient(int c_socket);

void *do_chat(void *arg);

void on_alarm(int signum);

int openServer(int argc, char *argv[]);

 

int main(int argc, char *argv[]) {

	

	srand(time(NULL));

	signal(SIGALRM, on_alarm);

	printf("플레이어 수 입력 : ");

	scanf("%d",&Max_Player_Number);

	openServer(argc, argv);

	initPlayer();

	run();

 

	return 0;

}

 

 

void run()

{

	set_ticker(delay);

	while(!isEnd)

	{

		//

 

	}

}

 

void initPlayer()

{

	int i,j;

	

	for(i=0;i<MAP_SIZE_ROW;i++)

		for(j=0;j<MAP_SIZE_COL;j++)

				bomb_map[i][j]=9;

	

 

	for(i=0;i<MAX_USER_SIZE;i++){

		

		Player_List[i].player_name=NULL;

		Player_List[i].power=0;

		Player_List[i].bomb_num_max=0;

		Player_List[i].bomb_onmap=0;

		Player_List[i].isAlive=0;

		Player_List[i].row=0;

		Player_List[i].col=0;

 

	}

 

	for(i=0;i<Max_Player_Number;i++){

 

		Player_List[i].player_name=(char *)malloc(sizeof(char)*MAX_NAME_SIZE);

		Player_List[i].power=1;

		Player_List[i].bomb_num_max=1;

		Player_List[i].bomb_onmap=0;

		Player_List[i].isAlive=1;

 

		switch(i){

			case 0:

			strcpy(Player_List[i].player_name,"aaa");

			Player_List[i].row=1;

			Player_List[i].col=1;

			map[1][1]='a';

			break;

			case 1:

			strcpy(Player_List[i].player_name,"bbb");

			Player_List[i].row=15;

			Player_List[i].col=15; 

			map[15][15]='b';

			break;

			case 2:

			strcpy(Player_List[i].player_name,"ccc");

			Player_List[i].row=15;

			Player_List[i].col=1;

			map[15][1]='c';

			break;

			case 3:

			strcpy(Player_List[i].player_name,"ddd");

			Player_List[i].row=1;

			Player_List[i].col=15;

			map[1][15]='d';

			break;

		}

 

	}

 

	printf("Player Init 완료\n");

}

 

int openServer(int argc, char *argv[])

{

 

	int c_socket, s_socket;

	struct sockaddr_in s_addr, c_addr;

	struct hostent *hp;

	int len;

	int nfds = 0;

	int i, j, n;

	fd_set read_fds;

	int res;

	char hostname[HOSTLEN];

	int client_count=1;

 

	if (argc < 2) {

		printf("usage: %s port_number\n", argv[0]);

		exit(-1);

	}

    

	if (pthread_mutex_init(&mutex, NULL) != 0) {

		printf("Can not create mutex\n");

		return -1;

	}

    

	s_socket = socket(PF_INET, SOCK_STREAM, 0); // TCP 소켓 생성

	

	// 연결 요청을 수신할 주소와 포트 설정.

	bzero( (void *)&s_addr, sizeof(s_addr));

	gethostname(hostname,HOSTLEN);

	hp=gethostbyname(hostname);

	bcopy((void*)hp->h_addr, (void*)&s_addr.sin_addr, hp->h_length);

	//s_addr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY: IP주소

	s_addr.sin_family = AF_INET;

	s_addr.sin_port = htons(atoi(argv[1]));

 

	printf("주소와 포트설정 완료\n");

	

	// 위에서 만든 주소를 소켓에 연결.

	if (bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1) {

		printf("Can not Bind\n");

		return -1;

	}

    

	printf("바인딩 완료\n");

 

	// 운영체제에 개통 요청. 이 시점부터 연결을 받아들인다.

	if (listen(s_socket, MAX_CLIENT) == -1) {

		printf("listen Fail\n");

		return -1;

	}

    

	printf("연결 준비 완료\n");

	// 클라이언트 소켓 배열의 초기 값 세팅.

	for (i = 0; i < MAX_CLIENT; i++)

		list_c[i] = INVALID_SOCK;

    

	while(1) {

		// 클라이언트로부터의 연결 요청 수신. 듣기 소켓과 연결 소켓.

		len = sizeof(c_addr);

 

        printf("새로운 accept 시도중...\n");

		c_socket = accept(s_socket, (struct sockaddr *)&c_addr, &len);

        printf("%d번째 클라이언트와 연결 성공\n",client_count++);

 

		// 연결 소켓의 번호를 클라이언트 배열에 추가.

		res = pushClient(c_socket);

		if (res < 0) {

			write(c_socket, CODE200, strlen(CODE200));

			close(c_socket);

		} else {

			// 클라이언트 배열에 무사히 소켓 번호가 추가됐으면 환영 인사를 보내주고 채팅 스레드를 생성한다.

			write(c_socket, greeting, strlen(greeting));

			pthread_create(&thread, NULL, do_chat, (void *) c_socket);

		}

 

		current_player_number++;

		if(current_player_number==Max_Player_Number)

		{

			break;

		}

	}

	

	startPlay=1;

	printf("모든 클라이언트 접속 완료\n");

}

 

void *do_chat(void *arg) {

 

   int c_socket = (int)arg;

   char chatData[CHATDATA];

   char character;

   int i, n;

   int who;

   char *ptr;

   pthread_t thr;

   int thr_id;

 

   set_ticker(delay);

 

   while (1) {

      if (startPlay)

      {

         memset(chatData, 0, sizeof(chatData));

 

         // 소켓으로부터 읽어온 데이터가 있으면 전체 클라이언트에 메시지를 보낸다.

         if ((n = read(c_socket, chatData, sizeof(chatData))) > 0) {

            printf("받은 데이터 : %s\n", chatData);

            /*for (i = 0; i < MAX_CLIENT; i++){

               if (list_c[i] != INVALID_SOCK) {

                write(list_c[i], chatData, n);

               }

            }*/

 

            if (contain(chatData, "aaa"))

            {

               who = 0;

               character = 'a';

            }

            else if (contain(chatData, "bbb"))

            {

               who = 1;

               character = 'b';

            }

            else if (contain(chatData, "ccc"))

            {

               who = 2;

               character = 'c';

            }

            else if (contain(chatData, "ddd"))

            {

               who = 3;

               character = 'd';

            }

 

            if (contain(chatData, "LEFT") && Player_List[who].isAlive == 1)

            {

               if (map[Player_List[who].row][Player_List[who].col - 1] != '2' && map[Player_List[who].row][Player_List[who].col - 1] != 'M' && map[Player_List[who].row][Player_List[who].col - 1] != '1') {

                  if (map[Player_List[who].row][Player_List[who].col] != 'M' && map[Player_List[who].row][Player_List[who].col] != '1') {

                     if (map[Player_List[who].row][Player_List[who].col - 1] == '3') {

                        if (Player_List[who].power <= MAX_POWER_LEN)

                        {

                           Player_List[who].power++;

                           //power++;

                        }

                     }

                     else if (map[Player_List[who].row][Player_List[who].col - 1] == '4') {

                        if (Player_List[who].bomb_num_max <= MAX_BOMB_NUM)

                        {

                           Player_List[who].bomb_num_max++;

                        }

                     }

                     update(Player_List[who].row, Player_List[who].col, ' ');

                  }

                  Player_List[who].col -= 1;

                  update(Player_List[who].row, Player_List[who].col, character);

               }

            }

            else if (contain(chatData, "RIGHT") && Player_List[who].isAlive == 1)

            {

               if (map[Player_List[who].row][Player_List[who].col + 1] != '2' && map[Player_List[who].row][Player_List[who].col + 1] != 'M' && map[Player_List[who].row][Player_List[who].col + 1] != '1') {

                  if (map[Player_List[who].row][Player_List[who].col] != 'M' && map[Player_List[who].row][Player_List[who].col] != '1') {

                     if (map[Player_List[who].row][Player_List[who].col + 1] == '3') {

                        if (Player_List[who].power <= MAX_POWER_LEN)

                        {

                           Player_List[who].power++;

                           //power++;

                        }

                     }

                     else if (map[Player_List[who].row][Player_List[who].col + 1] == '4') {

                        if (Player_List[who].bomb_num_max <= MAX_BOMB_NUM)

                        {

                           Player_List[who].bomb_num_max++;

                        }

                     }

                     update(Player_List[who].row, Player_List[who].col, ' ');

                  }

                  Player_List[who].col += 1;

                  update(Player_List[who].row, Player_List[who].col, character);

               }

            }

            else if (contain(chatData, "UP") && Player_List[who].isAlive == 1)

            {

               if (map[Player_List[who].row - 1][Player_List[who].col] != '2' && map[Player_List[who].row - 1][Player_List[who].col] != 'M' && map[Player_List[who].row - 1][Player_List[who].col] != '1') {

                  if (map[Player_List[who].row][Player_List[who].col] != 'M' && map[Player_List[who].row][Player_List[who].col] != '1') {

                     if (map[Player_List[who].row - 1][Player_List[who].col] == '3') {

                        if (Player_List[who].power <= MAX_POWER_LEN)

                        {

                           Player_List[who].power++;

                           //power++;

                        }

 

                     }

                     else if (map[Player_List[who].row - 1][Player_List[who].col] == '4') {

                        if (Player_List[who].bomb_num_max <= MAX_BOMB_NUM)

                        {

                           Player_List[who].bomb_num_max++;

                        }

 

                     }

                     update(Player_List[who].row, Player_List[who].col, ' ');

                  }

                  Player_List[who].row -= 1;

                  update(Player_List[who].row, Player_List[who].col, character);

               }

            }

            else if (contain(chatData, "DOWN") && Player_List[who].isAlive == 1)

            {

               if (map[Player_List[who].row + 1][Player_List[who].col] != '2' && map[Player_List[who].row + 1][Player_List[who].col] != 'M' && map[Player_List[who].row + 1][Player_List[who].col] != '1') {

                  if (map[Player_List[who].row][Player_List[who].col] != 'M' && map[Player_List[who].row][Player_List[who].col] != '1') {

                     if (map[Player_List[who].row + 1][Player_List[who].col] == '3') {

                        if (Player_List[who].power <= MAX_POWER_LEN)

                        {

                           Player_List[who].power++;

                           //power++;

                        }

 

                     }

                     else if (map[Player_List[who].row + 1][Player_List[who].col] == '4') {

                        if (Player_List[who].bomb_num_max <= MAX_BOMB_NUM)

                        {

                           Player_List[who].bomb_num_max++;

                        }

 

                     }

                     update(Player_List[who].row, Player_List[who].col, ' ');

                  }

                  Player_List[who].row += 1;

                  update(Player_List[who].row, Player_List[who].col, character);

               }

            }

            else if (contain(chatData, "BOMB") && Player_List[who].isAlive == 1)

            {

               if (map[Player_List[who].row][Player_List[who].col] != 'M') {

 

                  if (Player_List[who].bomb_onmap < Player_List[who].bomb_num_max) {

                     bomb_pos.row = Player_List[who].row;

                     bomb_pos.col = Player_List[who].col;

                     bomb_pos.index = who;

                     bomb_map[bomb_pos.row][bomb_pos.col] = who;

                     Player_List[who].bomb_onmap++;

                     thr_id = pthread_create(&thr, NULL, boom, NULL);

 

                     if (thr_id < 0)

                     {

                        perror("thread create error : ");

                        exit(0);

                     }

                  }

                  //pthread_join(thr,(void **)&status);

               }

            }

            else if (contain(chatData, "WALL") && Player_List[who].isAlive == 1)

            {

               update(Player_List[who].row, Player_List[who].col, '1');

            }

 

            sendMap();

            // 종료 문자열이 포함되어 있으면 해당 클라이언트를 배열에서 지운다.

            if (strstr(chatData, escape) != NULL) {

               popClient(c_socket);

               break;

            }

         }

      }

   }

}

 

void update(int row, int col, char value)

{

	map[row][col]=value;

}

 

int contain(char *s1, char *find)

{	

	char *ptr;

 

	ptr=strstr(s1,find);

	if(ptr!=NULL)

		return 1;

 

	return 0;

}

 

void sendMap()

{

	int i,j;

	char str[100];

	char fullStr[500]="";

	int count=0;

 

 

	for (i = 0; i < MAX_CLIENT; i++){

				if (list_c[i] != INVALID_SOCK) {

					//write(list_c[i], mapdata, sizeof(100));

					//strcat(fullStr,map);

					write(list_c[i], map, 289);

						//sprintf(str,"[%d]player_name:%s isAlive:%d power:%d bomb_num_max:%d bomb_available:%d row:%d col:%d",j,Player_List[j].player_name,Player_List[j].isAlive,Player_List[j].power,Player_List[j].bomb_num_max,Player_List[j].bomb_available,Player_List[j].row,Player_List[j].col);

 

					sendData(list_c[i]);

						//printf("보낸 데이터\n");

						//printf("%s\n",str);

						//write(list_c[i], str, sizeof(str)*4);

				}

			}

 

 

	/*for(i=0;i<MAP_SIZE_ROW;i++)

		{for(j=0;j<MAP_SIZE_COL;j++)

			{printf("%c",map[i][j]);}

		printf("\n");}*/

 

	//printf("맵 전송 완료\n");

}

 

void sendData(int client){

 

	int i,j;

	char str[100];

	char fullStr[500]="";

	int count=0;

 

					for(j=0;j<4;j++){

	usleep(20000);

sprintf(str,"[%d]%s %d %d %d %d %d %d",j,Player_List[j].player_name,Player_List[j].isAlive,Player_List[j].power,Player_List[j].bomb_num_max,Player_List[j].bomb_onmap,Player_List[j].row,Player_List[j].col);

 

						//sprintf(str,"[%d]player_name:%s isAlive:%d power:%d bomb_num_max:%d bomb_available:%d row:%d col:%d",j,Player_List[j].player_name,Player_List[j].isAlive,Player_List[j].power,Player_List[j].bomb_num_max,Player_List[j].bomb_available,Player_List[j].row,Player_List[j].col);

 

						//printf("\n%s\n",str);

						//strcat(fullStr,str);

 

						//printf("보낸 데이터\n");

						//printf("%s\n",str);

						write(client, str, sizeof(str));

 

			}

 

 

	//printf("데이터 전송 완료\n");

}

 

void on_alarm(int signum)

{

	signal(SIGALRM, on_alarm);	/* reset, just in case	*/

	sendMap();

}

 

int pushClient(int c_socket) {

	int i;

	

	// 클라이언트 배열에 추가하기 전에 뮤텍스를 이용하여 공유자원 접근을 제한한다.

	for (i = 0; i < MAX_CLIENT; i++) {

		pthread_mutex_lock(&mutex);

		if (list_c[i] == INVALID_SOCK) {

			list_c[i] = c_socket;

			pthread_mutex_unlock(&mutex);

			return i;

		}

		pthread_mutex_unlock(&mutex);

	}

    

	if (i == MAX_CLIENT)

		return -1;

}

 

int popClient(int s) {

	int i;

 

	close(s);

 

	// push할 때와 마찬가지로 공유자원 접근을 제한 후 클라이언트를 지웁니다.

	for (i = 0; i < MAX_CLIENT; i++) {

		pthread_mutex_lock(&mutex);

		if (s == list_c[i]) {

			list_c[i] = INVALID_SOCK;

			pthread_mutex_unlock(&mutex);

			break;

		}

		pthread_mutex_unlock(&mutex);

	}

	return 0;

}

 

void *boom() /*2018.11.30 KJY add*/

{

	bomb_position bomb = bomb_pos;

	update(bomb.row,bomb.col,'M');

 

	sleep(3);

 

	if(bomb_map[bomb.row][bomb.col]!=9)/* if it doesn't explode with earlier other thread*/

		explosion(bomb.row,bomb.col);/* bomb at thread's original position */

}

 

void explosion(int row,int col){

	int i=0;

	int temp;

	update(row,col,'B');

 

	for(i=1;i<=Player_List[bomb_map[row][col]].power;i++){

		if(map[row][col+i]!='1'&&map[row][col+i]!='2'&&map[row][col+i]!='3'&&map[row][col+i]!='4')

		{

			if(map[row][col+i]=='M')

			{

				explosion(row,col+i);

				break;

				

			}	

			update(row,col+i,'B');

		}

		else

			break;

	}

 

	for(i=1;i<=Player_List[bomb_map[row][col]].power;i++){

		if(map[row][col-i]!='1'&&map[row][col-i]!='2'&&map[row][col-i]!='3'&&map[row][col-i]!='4')

		{

			if(map[row][col-i]=='M')

				{

					explosion(row,col-i);

					break;

				}	

			update(row,col-i,'B');

		}

		else

			break;

	}

 

	for(i=1;i<=Player_List[bomb_map[row][col]].power;i++){

		if(map[row+i][col]!='1'&&map[row+i][col]!='2'&&map[row+i][col]!='3'&&map[row+i][col]!='4')

		{

			if(map[row+i][col]=='M')

				{

					explosion(row+i,col);

					break;

				}

			update(row+i,col,'B');

		}

		else

			break;

	}

 

	for(i=1;i<=Player_List[bomb_map[row][col]].power;i++){

		if(map[row-i][col]!='1'&&map[row-i][col]!='2'&&map[row-i][col]!='3'&&map[row-i][col]!='4')

		{

			if(map[row-i][col]=='M')

				{

					explosion(row-i,col);

					break;

				}

			update(row-i,col,'B');

		}

		else

			break;

	}

 

	usleep(100000);

	

	update(row,col,' ');

 

   for(i=1;i<=Player_List[bomb_map[row][col]].power;i++){

      if (map[row][col + i] != '2')

      {      

         temp = rand() % 3;

         if (map[row][col + i] != '1') {

            update(row, col + i, ' ');

         }

         else if (map[row][col + i] == '1') {

            if (temp == 0) {

               update(row, col + i, '3');

               break;

            }

            else if (temp == 1) {

               update(row, col + i, '4');

               break;

            }

            else  {

               update(row, col + i, ' ');

               break;

            }

         }

      }

      else

         break;

      }

 

   for(i=1;i<=Player_List[bomb_map[row][col]].power;i++){

      if(map[row][col-i]!='2')

      {   

         temp = rand() % 3;

         if (map[row][col - i] != '1') {

            update(row, col - i, ' ');

         }

         else if (map[row][col - i] == '1') {

            if (temp == 0) {

               update(row, col - i, '3');

               break;

            }

            else if (temp == 1) {

               update(row, col - i, '4');

               break;

            }

            else  {

               update(row, col - i, ' ');

               break;

            }

         }

      }

      else

         break;

      }

 

   for(i=1;i<=Player_List[bomb_map[row][col]].power;i++){

      if(map[row+i][col]!='2')

      {

         temp = rand() % 3;

         if (map[row+i][col] != '1') {

            update(row+i, col, ' ');

         }

         else if (map[row+i][col] == '1') {

            if (temp == 0) {

               update(row + i, col, '3');

               break;

            }

            else if (temp == 1) {

               update(row + i, col, '4');

               break;

            }

            else  {

               update(row + i, col, ' ');

               break;

            }

         }

      }

      else

         break;

      }

 

   for(i=1;i<=Player_List[bomb_map[row][col]].power;i++){

      if(map[row-i][col]!='2')

      {

         temp = rand() % 3;

         if (map[row-i][col] != '1') {

            update(row-i, col, ' ');

         }

         else if (map[row-i][col] == '1') {

            if (temp == 0) {

               update(row - i, col,'3');

               break;

            }

            else if (temp == 1) {

               update(row - i, col, '4');

               break;

            }

            else  {

               update(row - i, col, ' ');

               break;

            }

         }

      }

      else

         break;

      }

 

 

	/*if(map[Player_List[bomb_map[row][col]].row][Player_List[bomb_map[row][col]].col]==' ')

		Player_List[bomb_map[row][col]].isAlive=0;*/

 

	pthread_mutex_lock(&mutex);

	Player_List[bomb_map[row][col]].bomb_onmap--;

	bomb_map[row][col]=9;

	pthread_mutex_unlock(&mutex);

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
