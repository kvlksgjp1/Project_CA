#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h> // gcc 옵션에 -lpthread 사용.

void* do_chat(void *);

pthread_t thread;
pthread_mutex_t mutex;

#define MAX_CLIENT	10
#define CHATDATA	1024
#define INVALID_SOCK -1 // 클라이언트 소켓 배열의 초기 값.

int list_c[MAX_CLIENT]; // 클라이언트들의 소켓 번호를 저장하기 위한 배열

char escape[] = "exit";
char greeting[] = "Welcome to chatting room\n";
char CODE200[] = "Sorry No More Connection\n";

int popClient(int s);
int pushClient(int c_socket);
void *do_chat(void *arg);

int main(int argc, char *argv[]) {
	int c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	struct hostent *hp;
	int len;
	int nfds = 0;
	int i, j, n;
	fd_set read_fds;
	int res;

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
	memset(&s_addr, 0, sizeof(s_addr));
	gethostname(hostname,HOSTLEN);
	hp=gethostbyname(hostname);
	bcopy((void*)hp->h_addr, (void*)&saddr.sin_addr, hp->h_length);
	//s_addr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY: IP주소
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(atoi(argv[1]));
	
	// 위에서 만든 주소를 소켓에 연결.
	if (bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1) {
		printf("Can not Bind\n");
		return -1;
	}
    
	// 운영체제에 개통 요청. 이 시점부터 연결을 받아들인다.
	if (listen(s_socket, MAX_CLIENT) == -1) {
		printf("listen Fail\n");
		return -1;
	}
    
	// 클라이언트 소켓 배열의 초기 값 세팅.
	for (i = 0; i < MAX_CLIENT; i++)
		list_c[i] = INVALID_SOCK;
    
	while(1) {
		// 클라이언트로부터의 연결 요청 수신. 듣기 소켓과 연결 소켓.
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *)&c_addr, &len);
        
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
	}
}

void *do_chat(void *arg) {

	int c_socket = (int) arg;
	char chatData[CHATDATA];
	int i, n;
	
	while(1) {
		memset(chatData, 0, sizeof(chatData));
		
		// 소켓으로부터 읽어온 데이터가 있으면 전체 클라이언트에 메시지를 보낸다.
		if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
			for (i = 0; i < MAX_CLIENT; i++){
				if (list_c[i] != INVALID_SOCK) {
					write(list_c[i], chatData, n);
				}
			}
            
			// 종료 문자열이 포함되어 있으면 해당 클라이언트를 배열에서 지운다.
			if (strstr(chatData, escape) != NULL) {
				popClient(c_socket);
				break;
			}
		}
	}
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
