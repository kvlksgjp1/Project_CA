#include<stdio.h>
#include<time.h>
#include<unistd.h>
#include<signal.h>

int arr[10][10] = {0,};
int power = 1; //물풍선 세기 초기값
int x,y;


void count();
void edge();
void map_print();
void pop(); 	//물풍선 터지는 것
void pop_init(int power, int x, int y); // 물풍선 터진 거 초기
void user();				// 물풍선 세기 입력
void coordinate();
int main(void)
{
	int i,j;

	edge();
	// 물풍선 터지기 전 좌표 입력
	coordinate();

	printf("----물풍선 좌표 상황-----\n");
	// 물풍선 좌표 찍힌 상황 print
	map_print();

	

	
	
	//물풍선 터지는 작업
	count();	

	printf("\n-----물풍선 터지고 난 후 상황-----\n");
	// 물풍선 터지고 난 후 상황 print
	map_print();

	user();
	count();
	map_print();
	pop_init(power, x, y);
}

void edge()
{	
	int i,j;
	for(i=0;i<10;i++){	
		arr[0][i] = 2;
		arr[9][i] = 2;
	}

	for(i=0;i<10;i++){
		arr[i][0] = 2;
		arr[i][9] = 2;
	}
}

void pop()
{
	int i,j;
	arr[x][y] = 3;
	for(i=1;i<=power;i++){
		if(arr[x-i][y] !=2)
			arr[x-i][y] = 3; //left
		else
			printf("벽 부딪힘\n");
		if(arr[x+i][y] !=2)
			arr[x+i][y] = 3; //right
		else
			printf("벽 부딪힘\n");
		if(arr[x][y-i] !=2)
			arr[x][y-i] = 3; //up
		else
			printf("벽 부딪힘\n");
		if(arr[x][y+i] !=2)
			arr[x][y+i] = 3; //down
		else
			printf("벽 부딪힘\n");
	}
}

void pop_init(int power, int x, int y)
{
	int i,j;
	arr[x][y] = 3;
	for(i=1;i<=power;i++){
		if(arr[x-i][y] !=2)
			arr[x-i][y] = 0; //left
		if(arr[x+i][y] !=2)
			arr[x+i][y] = 0; //right
		if(arr[x][y-i] !=2)
			arr[x][y-i] = 0; //up
		if(arr[x][y+i] !=2)
			arr[x][y+i] = 0; //down
	}
}

void map_print()
{
	int i,j;
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			printf("%3d", arr[i][j]);
		}
	puts("");
	}
}

void user()
{
	int value;
	printf("\npower : ");
	scanf("%d", &power);
}

void coordinate()
{
	printf("좌표 입력:");
	scanf("%d %d", &x, &y);
	
	if(x!=0 && x != 9 && y !=0 && y != 9)
		arr[x][y] = 3;
	else	{
		printf("벽입니다. 다시 입력하십시오\n");
		coordinate();
	}
}


void count()
{
	signal(SIGALRM, pop);
	alarm(3);
	pause();
}
