#include <stdio.h>
#include <graphics.h>
#include "tools.h"
#include <time.h>
#include<string.h>
#include<conio.h>


#include <mmsystem.h>
#pragma comment (lib,"winmm.lib")


#define WIN_WIDTH 1024			//开始界面宽度
#define WIN_HEIGHT 546			//开始界面高度
#define activepicture 4			//动态的帧数
#define totalfloor 50

#define Goods 3					//	总的背景图片数
#define DR 5					//门

enum Key{yellow,blue,red};
enum Monsters{ redPao, greenPao, smallbat, skeleton, witch };


IMAGE BackGround, Button[2];	//初始界面
IMAGE MapBackGround[Goods];			//游戏地图背景
int Mapdata[2][13][13] = {		{	{1,1,1,1,1,1,1,1,1,1,1,1,1},
									{1,9,0,40,41,40,0,0,0,0,0,0,1},
									{1,1,1,1,1,1,1,1,1,1,1,0,1},
									{1,0,0,0,2,0,1,0,10,0,1,0,1},
									{1,0,43,0,1,0,1,0,10,0,1,0,1},
									{1,1,2,1,1,0,1,1,1,2,1,0,1},
									{1,0,0,0,1,0,2,42,44,42,1,0,1},
									{1,0,43,0,1,0,1,1,1,1,1,0,1},
									{1,1,2,1,1,0,0,0,0,0,0,0,1},
									{1,0,0,0,1,1,4,1,1,1,2,1,1},
									{1,0,43,10,1,12,0,0,1,0,0,0,1},
									{1,0,0,10,1,0,0,0,1,0,0,0,1},
									{1,1,1,1,1,1,1,1,1,1,1,1,1}} ,
								{
									{1,1,1,1,1,1,1,1,1,1,1,1,1},//1墙
									{1,8,0,3,0,0,0,0,0,0,0,0,1},//0路
									{1,0,0,1,1,0,43,0,43,0,1,1,1},//8下楼9上楼
									{1,0,1,1,1,1,1,0,1,1,1,1,1},//2黄门
									{1,0,1,10,0,2,0,0,0,2,0,0,1},//3蓝门
									{1,0,1,10,10,1,0,0,0,1,0,0,1},//4红门
									{1,0,1,1,1,1,0,0,0,1,1,1,1},
									{1,0,1,0,0,2,0,0,0,2,0,0,1},
									{1,0,1,0,0,1,0,0,0,1,0,0,1},
									{1,0,1,1,1,1,0,0,0,1,1,1,1},
									{1,0,1,0,0,2,0,0,0,2,0,0,1},
									{1,9,1,0,0,1,0,0,0,1,0,0,1},
									{1,1,1,1,1,1,1,1,1,1,1,1,1}}
};	


IMAGE myDoors[DR];	//门
IMAGE keypicture[red + 1];//钥匙
IMAGE monsterpicture[witch + 1];//野怪
IMAGE Stair[2];


IMAGE Myself[4][activepicture];//0向下走，1向左，2向右，3向上

int mode = 0;					//游戏模式1、简单	2、中等		3、困难
int k = -1;						
static int floor = 0;			//当前楼层

struct myTool {
	bool fly;
	bool book;
	bool conversation;
	bool axe;
	bool sacred_water;
	bool boom;
	bool bang;
	bool dragon_kill;
	bool snowstorm;
	bool upfly;
	bool downfly;
	bool luckymoney;
};

struct Hero {
	char name[20] ;
	int x, y;
	int attact;
	int defend;
	int hp;
	int money;
	int mykey[DR];
	int speed;
	myTool *tool;
};

struct Monster {
	int attact;
	int defend;
	int money;
	int hp;
}mons[witch + 1];


void monsterset(Monster& x,int a,int d,int m,int hp) {
	x.attact = a;
	x.defend = d;
	x.hp = hp;
	x.money = m;
}

void monsterInit() {
	//enum Monsters{redPao,greenPao,smallbat,skeleton,witch};
	monsterset(mons[greenPao], 10, 10, 10, 100);
	monsterset(mons[redPao], 10, 10, 10, 100);
	monsterset(mons[skeleton], 10, 10, 10, 100);
	monsterset(mons[witch], 10, 10, 10, 100);
	monsterset(mons[smallbat], 10, 10, 10, 100);
}


void GameInit() {


	loadimage(&BackGround, "res/Menu/menubkg1.png");
	loadimage(&Stair[0], "res/Door/8.png");
	loadimage(&Stair[1], "res/Door/9.png");
	monsterInit();//初始化野怪
	
	char name[64];
	for (int i = 0; i < Goods; i++) {
		sprintf_s(name, sizeof(name), "res/Map/%d.jpg", i);
		loadimage(&MapBackGround[i], name);
	}
	for (int i = 0; i < DR; i++) {
		sprintf_s(name, sizeof(name), "res/Door/%d.png", i);
		loadimage(&myDoors[i], name);
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < activepicture; j++) {
			sprintf_s(name, sizeof(name), "res/Player/%d%d.png", i, j);
			loadimage(&Myself[i][j], name);
		}
	}
	for (int i = 0; i <= red; i++) {
		sprintf_s(name, sizeof(name), "res/Good/%d.png", i+10);
		loadimage(&keypicture[i], name);
	}
	for (int i = 0; i <= witch; i++) {
		sprintf_s(name, sizeof(name), "res/Npc/%d.png", i + 40);
		loadimage(&monsterpicture[i], name);
	}


	initgraph(WIN_WIDTH, WIN_HEIGHT, 1);





	//设置字体
	LOGFONT f;

	gettextstyle(&f);
	f.lfHeight = 50;
	f.lfWeight = 40;
	strcpy_s(f.lfFaceName, "Segoe UI Black");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setcolor(BLACK);
}

void startUI() {

	loadimage(&Button[0], "res/Menu/button.png");
	loadimage(&Button[1], "res/Menu/button1.png");

	char scoreText1[] = { "简单模式" };

	ExMessage msg;
	while (1) {

		BeginBatchDraw();
		putimage(0, 0, &BackGround);
		putimagePNG(375, 190, mode ? &Button[0] : &Button[1]);
		outtextxy(420, 195, scoreText1);
		EndBatchDraw();


		if (peekmessage(&msg)) {
			if (msg.x > 392 && msg.x < 652 && msg.y>190 && msg.y < 250) {
				if (mode == 0) {
					mode = 1;
					continue;
				}
				else if (msg.message == WM_LBUTTONDOWN) {
					break;
				}

			}
			else mode = 0;
		}
	}


}//启动菜单

void InitWindow(Hero& me) {
	
	BeginBatchDraw();
	

	putimagePNG(265, 25, &MapBackGround[0]);
	putimagePNG(760, 25, &MapBackGround[1]);
	putimagePNG(113, 25, &MapBackGround[2]);
	for (int i = 1; i < 12; i++) {
		for (int j = 1; j < 12; j++) {
			if ((k = Mapdata[floor][i][j]) != 0&&k <= DR) {
				putimagePNG(j * 45 + 220, i * 45 -20, &myDoors[k-1]);
			}
			else if (k == 8 || k == 9) {
				putimagePNG(j * 45 + 220, i * 45 - 20, &Stair[k-8]);
			}
			else if (k > 9 && k < 13) {
				putimagePNG(j * 45 + 220, i * 45 - 20, &keypicture[k-10]);
			}
			else if (k > 39 && k < 45) {
				putimagePNG(j * 45 + 220, i * 45 - 20, &monsterpicture[k - 40]);
			}
		}
	}

	int row = me.x * 45 + 220;

	int col = me.y * 45 -20;

	putimagePNG(row,col, &Myself[3][3]);

	EndBatchDraw();
	//printf("%d\n", me.x);
}

void HeroInit(Hero& me) {
	//初始化英雄

	me.attact = 100;
	me.defend = 100;
	memset(me.mykey, 0, sizeof(int [5]));
	me.mykey[4] = -1;
	me.x = 6;
	me.y = 11;
	me.speed = 20;
	me.tool = new myTool;
	memset(me.tool, 0, sizeof(myTool));

	
}

void userplayer(Hero& me, int x, int y) {

	int num = Mapdata[floor][x][y];

	if (num != 1) {
		if (num == 0) {
			me.x = y;
			me.y = x;
		} 
		else if (num == 8) {
			floor--;
			me.x = y;
			me.y = x;
		}
		else if (num == 9) {
			floor++;
			me.x = y;
			me.y = x;
		}
		else if (num >1 && num < 6) {
			if (me.mykey[num-1] != 0) {
				Mapdata[floor][x][y] = 0;
				me.mykey[num-1]--;
				me.x = y;
				me.y = x;
			}
		}
		else if (num > 9 && num < 13) {
			Mapdata[floor][x][y] = 0;
			me.mykey[num - 9]++;
			me.x = y;
			me.y = x;
		}
		else if (num > 39 && num < 45) {
			int ac = me.attact;
			int de = mons[num - 40].defend;
			int h = mons[num - 40].hp;
			if (ac > de) {
				int count = h / (ac - de) + 1;
				ac = mons[num - 40].attact;
				de = me.defend;
				if (de < ac) {
					me.hp -= (ac - de)*count;
					if (me.hp <= 0) {
						//游戏结束
					}

				}
				me.money += mons[num - 40].money;
				Mapdata[floor][x][y] = 0;
				me.x = y;
				me.y = x;
			}

		}


	}
}

void HeroMove(Hero& me) {

	int orientation = _getch();
	if (orientation == 87 || orientation == 119 || orientation == 72) {//up

		userplayer(me, me.y - 1, me.x);
	
	}else if (orientation == 68 || orientation == 115 || orientation == 80) {//down

		userplayer(me, me.y+1, me.x);

	}else if (orientation == 65 || orientation == 97 || orientation == 75) {//left

		userplayer(me, me.y, me.x - 1);

	}else if (orientation == 83 || orientation == 100 || orientation == 77) {//right

		userplayer(me, me.y, me.x + 1);

	}
}



int main() {


	GameInit();
	Hero me;

	HeroInit(me);

	startUI();
	
	while (1) {
		InitWindow(me);
		HeroMove(me);
	}

	/*while (1) {
		int orientation = _getch();
		printf("%d\n", orientation);
	}
	system("pause");*/

	return 0;
}


