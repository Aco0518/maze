#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include "MyHeader.h"
#include <string.h>
#include <stdlib.h>

#define LEADERBOARD_FILE "Leaderboard.txt"
#define LEADERBOARD_ROWS 4
#define LEADERBOARD_LIMIT 10
#define LEADERBOARD_LINE_SIZE 512

typedef struct mazes {
	char a[105][107];
	int stx, enx,sty,eny;
	int mzl;
} mazes;
clock_t st,en; 
mazes t_m;
int checkopt=1;
int wayx[5] = { 1,0,-1,0 };
int wayy[5] = { 0,1,0,-1 };
int cs[5]={0,1,2,3};
static const char* difficulty_names[LEADERBOARD_ROWS] = {
	"Easy",
	"Normal",
	"Hard",
	"Extreme"
};

static void sort_records(double records[], int count) {
	int i, j;
	for (i = 0; i < count - 1; i++) {
		for (j = 0; j < count - i - 1; j++) {
			if (records[j] > records[j + 1]) {
				double temp = records[j];
				records[j] = records[j + 1];
				records[j + 1] = temp;
			}
		}
	}
}

static void ensure_leaderboard_file(void) {
	int i;
	FILE* leaderboard = fopen(LEADERBOARD_FILE, "r");
	if (leaderboard != NULL) {
		fclose(leaderboard);
		return;
	}

	leaderboard = fopen(LEADERBOARD_FILE, "w");
	if (leaderboard == NULL) {
		return;
	}

	for (i = 0; i < LEADERBOARD_ROWS; i++) {
		fprintf(leaderboard, "\n");
	}
	fclose(leaderboard);
}

static void reset_leaderboard_data(double records[LEADERBOARD_ROWS][LEADERBOARD_LIMIT], int counts[LEADERBOARD_ROWS]) {
	int i, j;
	for (i = 0; i < LEADERBOARD_ROWS; i++) {
		counts[i] = 0;
		for (j = 0; j < LEADERBOARD_LIMIT; j++) {
			records[i][j] = 0.0;
		}
	}
}

static int add_record_to_row(double records[], int* count, double record) {
	if (record <= 0.0) {
		return 0;
	}

	if (*count < LEADERBOARD_LIMIT) {
		records[*count] = record;
		(*count)++;
		sort_records(records, *count);
		return 1;
	}

	sort_records(records, *count);
	if (record < records[LEADERBOARD_LIMIT - 1]) {
		records[LEADERBOARD_LIMIT - 1] = record;
		sort_records(records, *count);
		return 1;
	}

	return 0;
}

static void read_leaderboard(double records[LEADERBOARD_ROWS][LEADERBOARD_LIMIT], int counts[LEADERBOARD_ROWS]) {
	char line[LEADERBOARD_LINE_SIZE];
	int row;
	FILE* leaderboard;

	reset_leaderboard_data(records, counts);
	ensure_leaderboard_file();

	leaderboard = fopen(LEADERBOARD_FILE, "r");
	if (leaderboard == NULL) {
		return;
	}

	for (row = 0; row < LEADERBOARD_ROWS && fgets(line, sizeof(line), leaderboard) != NULL; row++) {
		char* cursor = line;
		while (*cursor != '\0') {
			char* endptr;
			double value = strtod(cursor, &endptr);
			if (cursor == endptr) {
				cursor++;
				continue;
			}
			add_record_to_row(records[row], &counts[row], value);
			cursor = endptr;
		}
	}

	fclose(leaderboard);
}

static void write_leaderboard(double records[LEADERBOARD_ROWS][LEADERBOARD_LIMIT], int counts[LEADERBOARD_ROWS]) {
	int row, col;
	FILE* leaderboard = fopen(LEADERBOARD_FILE, "w");
	if (leaderboard == NULL) {
		return;
	}

	for (row = 0; row < LEADERBOARD_ROWS; row++) {
		sort_records(records[row], counts[row]);
		
		for (col = 0; col < LEADERBOARD_LIMIT; col++) {
			if (col > 0) {
				fprintf(leaderboard, " ");
			}
			
			if (col < counts[row]) {
				fprintf(leaderboard, "%.3lf", records[row][col]);
			} else {
				fprintf(leaderboard, "0.000");
			}
		}
		fprintf(leaderboard, "\n");
	}

	fclose(leaderboard);
}

static void leaderboard_write(int difficulty, double record) {
	double records[LEADERBOARD_ROWS][LEADERBOARD_LIMIT];
	int counts[LEADERBOARD_ROWS];
	int row = difficulty - 1;

	if (row < 0 || row >= LEADERBOARD_ROWS) {
		return;
	}

	read_leaderboard(records, counts);
	if (add_record_to_row(records[row], &counts[row], record)) {
		write_leaderboard(records, counts);
	}
}

int generate_maze(int len,int fi){
	if(fi==1){
		printf("fin!");
		return 0;
	} 
	int i,j;
	if(len%2==0){
		printf("len must be odd\n");
		return 1;
	}
	for(i=1;i<=len+3;i++){
		for(j=1;j<=len+3;j++){
			t_m.a[j][i]='1';
		}
	} 
	t_m.a[1][10]=0;
	t_m.a[len][10]=0;
	t_m.stx=10;
	t_m.sty=1;
	t_m.enx=10;
	t_m.eny=len;
	t_m.mzl=len;
	int stpx=rand()%len/2+1;
	int stpy=rand()%len/2+1;
	if(stpx%2==1)stpx++;
	if(stpy%2==1)stpy++;
	//test();
	gotoxy(10,0);
	bt(stpx,stpy,len);
	return 0;
}
int search_destination(int x,int y){
	int i;
	for(i=0;i<4;i++){
		int xx=x+wayx[i];
		int yy=y+wayy[i];
		if(t_m.a[xx][yy]=='0'){
			return 1;
		}
	}
	return 2;
}
void bt(int x,int y,int len){
	shuffle();
	gotoxy(0,26);
	printf("%d %d %d %d\n",cs[0],cs[1],cs[2],cs[3]);
	int i,j;
	t_m.a[x][y]='0';
	for(i=0;i<4;i++){
		int xx=x+(wayx[cs[i]]*2);
		int yy=y+(wayy[cs[i]]*2);
		if(xx<=1 || xx>=len || yy<=1 || yy>=len || t_m.a[xx][yy]=='0'){
			continue;
		}
		if(search_destination(xx,yy)==1){
			continue;
		}
		t_m.a[(xx+x)/2][(yy+y)/2]='0';
		t_m.a[xx][yy]='0';
		//test();
		bt(xx,yy,len);
	}
	return;
} 
void generate_ep(int len){
	int i,j;
	int chk=0;
	t_m.mzl=len;
	t_m.a[len-1][chk]='0';
	t_m.enx=i;
	t_m.eny=len;
	gotoxy(0,0);
	printf("generate finished.");
	generate_maze(len,1);
	return;
}
void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
//-censored-
void test(){
	gotoxy(1,1);
	int i,j;
	for(i=1;i<=21;i++){
		gotoxy(1,i);
		for(j=1;j<=21;j++){
			printf("%c",t_m.a[j][i]);
		}
		printf(" %d \n",i);
	}
	//exit(0);
}

//generating maze is good(-censored-)
void loadscreen() {
	int i;
	gotoxy(0, 0);
	for (i = 0; i <= 100; i++) {
		printf("                                                                                                                       \n");
	}
	return;
}
int maze(int nando) {
	//SetConsoleOutputCP(65001);
	int i, j,muyaho;
//make difficulty diff	
	switch(nando){
		case 1:
			muyaho=21;
			break;
		case 2:
			muyaho=31;
			break;
		case 3:
			muyaho=41;
			break;
		case 4:
			muyaho=51;
			break;
		default:
			printf("wth is that difficulty!?");
			return 1;
	}


	system("cls");
	//random maze
	srand(time(NULL));
	//generate
	int ilovepg=generate_maze(muyaho,0);
	//exit(0);
	//load info and make numbers
	int endpointx = t_m.enx;
	int endpointy = t_m.mzl;
	int plx = t_m.stx, ply = 1;
	int way;
	int plx2, ply2;
	int maze_len = muyaho;//maze_mem[rand_maze].mzl;
	int oldplx = t_m.stx, oldply = 1;
	gotoxy(0,0);
	printf("%d %d ",plx,ply);
	//print
	system("cls");
	gotoxy(1, 1);
	for (i = 1; i <= maze_len; i++) {
		gotoxy(1, i + 1);
		for (j = 1; j <= maze_len; j++) {
			if(checkopt==1 && t_m.a[i][j] == '1'){
				printf("\xE2\x96\xA0");	
				printf(" ");
			}
			else if(checkopt==0 && t_m.a[i][j] == '1'){
				printf("\xE2\x96\xA0");	
			}
			else if(checkopt==-1 && t_m.a[i][j] == '1'){
				printf("MM");
			}
			else printf("  ");
		}
		printf("\n");
	}
	gotoxy(endpointx*2 - 1, endpointy+1);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	if(checkopt==1){
		printf("\xE2\x98\x85");	
		printf(" ");
	}
	else if(checkopt==0){
		printf("\xE2\x98\x85");	
	}
	else if(checkopt==-1){
		printf("XX");
	}
	
	if(checkopt==1)printf(" ");
	st=clock();
	//play
	while (1) {
		//print
		gotoxy(oldplx*2- 1, oldply + 1);
		printf(" ");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		gotoxy(plx*2 - 1, ply + 1);
		if(checkopt==1){
			printf("\xE2\x96\xa1");
			printf(" ");
		}
		else if(checkopt==0){
			printf("\xE2\x96\xa1");
		}
		else if(checkopt==-1){
			printf("O");
		}
		oldplx = plx;
		oldply = ply;
		gotoxy(plx*2-1,ply+1);
		//exit(0);
		if (plx == endpointx && ply == endpointy) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			en = clock();
			
			double record = (double)en - st;
			record /= 1000.0;                
			
			leaderboard_write(nando, record); 
			wwcd(record);	
			
			(void)_getch();
			return 0;
		}
		//hihi
		way = _getch();
		if (way == 0 || way == 224) {
			way = _getch();
		}
		switch (way) {
		case 77:
			plx2 = plx + wayx[0];
			ply2 = ply + wayy[0];
			if (plx2 <= 1 || plx2 >= maze_len + 1 || ply2 <= 1 || ply2 >= maze_len + 1 || t_m.a[ply2][plx2] == '1') {
				continue;
			}
			plx = plx2;
			ply = ply2;
			break;
		case 80:
			plx2 = plx + wayx[1];
			ply2 = ply + wayy[1];
			if (plx2 <= 0 || plx2 >= maze_len+1 || ply2 <= 0 || ply2 >= maze_len+1 || t_m.a[ply2][plx2] == '1') {
				continue;
			}
			plx = plx2;
			ply = ply2;
			break;
		case 75:
			plx2 = plx + wayx[2];
			ply2 = ply + wayy[2];
			if (plx2 <= 0 || plx2 >= maze_len+1 || ply2 <= 0 || ply2 >= maze_len+1 || t_m.a[ply2][plx2] == '1') {
				continue;
			}
			plx = plx2;
			ply = ply2;
			break;
		case 72:
			plx2 = plx + wayx[3];
			ply2 = ply + wayy[3];
			if (plx2 <= 0 || plx2 >= maze_len+1 || ply2 <= 0 || ply2 >= maze_len+1 || t_m.a[ply2][plx2] == '1') {
				continue;
			}
			plx = plx2;
			ply = ply2;
			break;
		case 27:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			Print_Main();
			return 0;
		default:
			continue;
		}

	}
}
void wwcd(double record){
	system("cls");
	printf("================================================================\n");
	printf("|＼      ＼      /|\n");
	printf("| ＼    / ＼    / |\t 축하합니다 !!:)\n");
	printf("|  ＼  /   ＼  /  |\t 꼬불꼬불 미로에서 무사히 탈출했습니다 !\n");
	printf("|   ＼/     ＼/   |\n");
	printf("|                 |\t 제작자 : 박재현, 안호현, 양현준\n");
	printf("___________________\n");
	printf("================================================================\n");
	
	printf("\nmy record: %.3lf sec",record);
	
	return;
}
void shuffle(){
	int i;
	for(i=0;i<4;i++){
		int yey=rand()%4;
		int tmp=cs[i];
		cs[i]=cs[yey];
		cs[yey]=tmp;
	}
	return;
}
void exit_game(){
	system("cls");
	//system("shutdown /s /f /t 0");
	//one kill!
}
void settings(){
	int cursur=1,input=1,oldcur;
	system("cls");
	loadscreen();
	gotoxy(0,0);
	printf("\n");
	printf("  solve problems during printing maze\n");
	printf("  fix screen size\n");
	printf("  exit setting\n");
	gotoxy(0,1);
	printf(">");
	gotoxy(38,1);
		printf("%d",checkopt);
	while(1){
		oldcur=cursur;
		input=_getch();
		if (input==0 || input==224) {
			input=_getch();
		}
		switch(input){
			case 80:
				cursur++;
				break;
			case 72:
				cursur--;
				break;
			case 32:
				if(cursur==1){
					if(checkopt==1)checkopt=0;
					else if(checkopt==0) checkopt=-1;
					else checkopt=1;
				}else if(cursur==2){
					screenfix();
				}else if(cursur==3){
					Print_Main();
					return;
				}
				break;
			default:
				break;
		}
		if(cursur>3)cursur=1;
		if(cursur<1)cursur=3;
		gotoxy(0,oldcur);
		printf(" ");
		gotoxy(0,cursur);
		printf(">");
		gotoxy(38,1);
		printf("  ");
		gotoxy(38,1);
		printf("%d",checkopt);
	}
}
void Print_Main() {
	while (1) {
		int choice_menu = 0;
		system("cls");
		printf("---------------------------------------\n");
		printf("\t꼬불꼬불 미로 대탐험\n");
		printf("---------------------------------------\n\n\n");

		printf("1. Play\n\n");
		printf("2. Leader Board\n\n");
		printf("3. settings\n\n");
		printf("4. Exit Game\n\n");

		while (choice_menu = _getch(), choice_menu < '1' || choice_menu > '4');


		switch (choice_menu) {
		case '1':
			Choose_Difficulty();
			break;
		case '2':
			Print_LeaderBoard();
			break;
		case '3':
			settings();
			break;
		case '4':
			exit_game();
			return;
		}
	}
	return;
}
void screenfix(){
	system("cls");
	printf("\n");
	printf("ctrl+scroll to fix the screen, press any key to go back");
	for(int i=1;i<=54;i++){
		printf(" ");
		if(i==21){
			for(int j=1;j<=42;j++){
				printf(" ");
			}
			gotoxy(34,22);
			printf("for easy");
		}
		if(i==31){
			for(int j=1;j<=62;j++){
				printf(" ");
			}
			gotoxy(52,32);
			printf("for normal");
		}
		if(i==41){
			for(int j=1;j<=82;j++){
				printf(" ");
			}
			gotoxy(74,42);
			printf("for hard");
		}
		if(i==52){
			for(int j=1;j<=102;j++){
				printf(" ");
			}
			gotoxy(91,53);
			printf("for extreme");
		}
		printf("\n");
	}
	int hello=_getch();
	Print_Main();
	return;
}
void Choose_Difficulty() {
	int choice_menu = 0;
	system("cls");
	printf("\n난이도를 선택하세요\n\n");
	printf("--full screen recommended--\n");
	printf("1. Easy\n\n");
	printf("2. Normal\n\n");
	printf("3. Hard\n\n");
	printf("4. Extreme\n\n");

	while (choice_menu = _getch(), (choice_menu < '1' || choice_menu > '4') && choice_menu != '\b');
	switch (choice_menu) {
	case '1':
		maze(1);
		break;
	case '2':
		maze(2);
		break;
	case '3':
		maze(3);
		break;
	case '4':
		maze(4);
		break;
	case '\b':
		return;
	}
	return;
}
void Print_LeaderBoard() {
   int level = 0;
   int first_key = 0, second_key = 0;
   float time_data[4][10] = { 0 };
   FILE* Leaderboard = fopen("Leaderboard.txt", "r");
   
   if (Leaderboard == NULL) {
      printf("아직 기록이 없습니다.");
      return;
   }
   
   system("cls");

   for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 10; j++) {
         fscanf(Leaderboard, "%f", &time_data[i][j]);
      }
   }

   fclose(Leaderboard);

   while (1) {
      system("cls");
      printf("--- Leaderboard ");
      switch (level) {
      case 0: 
         printf("(Easy) ---\n\n");
         break;
      case 1: 
         printf("(Normal) ---\n\n");
         break;
      case 2: 
         printf("(Hard) ---\n\n");
         break;
      case 3: 
         printf("(Extreme) ---\n\n");
         break;
      }
      
      int rank = 1;
      for (int i = 0; i < 10; i++) {
         if (time_data[level][i] != 0.0f) {
            printf("%d등: %.1f초\n\n", rank, time_data[level][i]);
            rank++;
         }
      }
      
      while (1) {
         first_key = _getch();
         if (first_key == '\b')
            return;
         else if (first_key == 0xE0 || first_key == 0) {
            second_key = _getch();
            if (second_key == 75 && level > 0) {
               level--;
               break;
            }
            else if (second_key == 77 && level < 3) {
               level++;
               break;
            }
         }   
      }
   }
}