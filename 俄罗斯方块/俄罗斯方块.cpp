#include<iostream>
#include<Windows.h>
#include<graphics.h>
#include<stdio.h>
#include<time.h>
#include<conio.h>

#define BLOCK_COUNT  5 //方块数组列
#define BLOCK_LINE   5 //方块数组行
#define BLOCK_COLUM  5 //方块个数
#define BLOCK_SIZEOF 20 //小方块像素

#define START_X  130  //起始x坐标
#define START_Y  30   //起始y坐标

#define KEY_UP      72 //上
#define KEY_RIGHT   77 //右
#define KEY_DOWN    80//下
#define KEY_LEFT    75 //左
#define KEY_SPACE   32 //空格

typedef enum{
	BLOCK_UP,
	BLOCK_RIGHT,
	BLOCK_DOWNT,
	BLOCK_LEFT
}block_dir_t;

typedef enum{
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
}move_dir_t;

int score=0;//分数
int grade=0;//等级

int NextIndex=-1;
int BlockIndex=-1;

int minX=30;
int minY=30;

int color[BLOCK_COUNT]={
	GREEN,CYAN,MAGENTA,BROWN,YELLOW
};

int visit[30][15];
int markColor[30][15];//表示颜色

int speed=500;//速度

using namespace std;

int block[BLOCK_COUNT*4][BLOCK_LINE][BLOCK_COLUM]={
// | 形方块
	{ 0,0,0,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,0,0,0,0,
	0,1,1,1,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,0,0,0,0,
	0,1,1,1,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	// L 形方块
	{ 0,0,0,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,1,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,0,0,0,0,
	0,1,1,1,0,
	0,1,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,1,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,0,0,1,0,
	0,1,1,1,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	// 田 形方块
	{ 0,0,0,0,0,
	0,1,1,0,0,
	0,1,1,0,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,1,1,0,0,
	0,1,1,0,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,1,1,0,0,
	0,1,1,0,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,1,1,0,0,
	0,1,1,0,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	// T 形方块
	{ 0,0,0,0,0,
	0,1,1,1,0,
	0,0,1,0,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,0,0,1,0,
	0,0,1,1,0,
	0,0,0,1,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,0,1,0,0,
	0,1,1,1,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,1,0,0,0,
	0,1,1,0,0,
	0,1,0,0,0,
	0,0,0,0,0 },

	// Z 形方块
	{ 0,0,0,0,0,
	0,1,1,0,0,
	0,0,1,1,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,0,1,0,0,
	0,1,1,0,0,
	0,1,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,1,1,0,0,
	0,0,1,1,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,0,1,0,0,
	0,1,1,0,0,
	0,1,0,0,0,
	0,0,0,0,0 },
};

void welcome(void){
	initgraph(550,660);//初始化画布

	//设置窗口标题
	HWND window=GetHWnd();//获取窗口
	SetWindowText(window,_T("俄罗斯方块  (￣(oo)￣)妞妞版"));//设置窗口名称

	//设置文本字体样式
	setfont(40,0,_T("微软雅黑"));
	setcolor(WHITE);
	outtextxy(205,200,_T("俄罗斯方块"));

	setfont(25,0,_T("楷体"));
	setcolor(WHITE);
	outtextxy(198,300,_T("妞妞为老婆订做"));
	
	Sleep(3000);//休眠3秒
}

//初始化游戏场景
void initGameScene(void){
	char str[16];//储存分数

	//清除屏幕
	cleardevice();

	rectangle(27,27,336,635);
	rectangle(29,29,334,633);
	rectangle(370,50,515,195);

	setfont(24,0,_T("楷体"));
	setcolor(LIGHTGRAY);
	outtextxy(405,215,_T("下一个"));

	setcolor(RED);
	outtextxy(405,280,_T("分数:"));
	sprintf(str,"%d",score);
	outtextxy(415,310,str);

	outtextxy(370,340,"老婆我爱你");

	outtextxy(405,375,_T("等级:"));
	sprintf(str,"%d",grade);
	outtextxy(425,405,str);

	//操作说明
	setcolor(LIGHTBLUE);
	outtextxy(370,440,"老婆我爱你");
	outtextxy(370,475,"操作说明:");
	outtextxy(390,500,"↑:旋转");
	outtextxy(390,525,"↓:下降");
	outtextxy(390,550,"←:左移");
	outtextxy(390,575,"→:右移"); 
	outtextxy(390,600,"空格:暂停");
}

void clearBlock(void){
	setcolor(BLACK);//设置为黑色,擦除方块
	setfont(23,0,"楷体");

	for(int i=0;i<BLOCK_LINE;i++){
		for(int j=0;j<BLOCK_COLUM;j++){
			int x=391+j*BLOCK_SIZEOF;
			int y=71+i*BLOCK_SIZEOF;
			outtextxy(x,y,"■");
		}
	}

}

void clearBlock(int x,int y,block_dir_t dir){
	setcolor(BLACK);
	int id=BlockIndex*4+dir;
	y+=START_Y;

	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			if(block[id][i][j]==1){
				outtextxy(x+BLOCK_SIZEOF*j,y+i*BLOCK_SIZEOF,"■");
			}
		}
	}

}

//在右上角绘制下一个方块
void drawBlock(int across=0,int erect=0){
	setcolor(color[NextIndex]);//设置小方块颜色
	setfont(23,0,"楷体");

	for(int i=0;i<BLOCK_LINE;i++){
		for(int j=0;j<BLOCK_COLUM;j++){
			if(block[NextIndex*4][i][j]==1){
			int x=across+j*BLOCK_SIZEOF;
			int y=erect+i*BLOCK_SIZEOF;
			outtextxy(x,y,"■");
			}
		}
	}

}

void drawBlock(int x,int y,int blockIndex,block_dir_t dir){
	setcolor(color[blockIndex]);//设置小方块颜色
	setfont(23,0,"楷体");
	int id=blockIndex*4+dir;

	for(int i=0;i<BLOCK_LINE;i++){
		for(int j=0;j<BLOCK_COLUM;j++){
			if(block[id][i][j]==1){
			int x2=x+j*BLOCK_SIZEOF;
			int y2=y+i*BLOCK_SIZEOF;
			outtextxy(x2,y2,"■");
			}
		}
	}
}

void nextblock(void){
	clearBlock();//清除右上角的方块

	//随机出现方块
	srand(time(NULL));//使用时间函数的返回值来做随机种子
	NextIndex=rand()%BLOCK_COUNT;

	drawBlock(391,71);
}

int moveable(int x0,int y0,move_dir_t moveDir,block_dir_t blockDir){
	int id=BlockIndex*4+blockDir;//第几个方块

	int x=(y0-minY)/BLOCK_SIZEOF;
	int y=(x0-minX)/BLOCK_SIZEOF;
	int ret=1;
	
	if(moveDir==MOVE_DOWN){
		for(int i=0;i<BLOCK_LINE;i++){
			for(int j=0;j<BLOCK_COLUM;j++){
				if(block[id][i][j]==1 &&
					(x+i+1>=30 ||  visit[x+i+1][y+j]==1)){
						ret=0;
				}
			}
		}
	}else if(moveDir==MOVE_LEFT){
		for(int i=0;i<BLOCK_LINE;i++){
			for(int j=0;j<BLOCK_COLUM;j++){
				if(block[id][i][j]==1 &&
					(y+j==0 || visit[x+i][y+j-1]==1)){
						ret=0;
				}
			}
		}
	}else if(moveDir==MOVE_RIGHT){
		for(int i=0;i<BLOCK_LINE;i++){
			for(int j=0;j<BLOCK_COLUM;j++){
				if(block[id][i][j]==1 &&
					(y+j+1>=15 || visit[x+i][y+j+1]==1)){
					    ret=0;
				}

			}
		}
	}


	return ret;
}

void failCheck(){
	if(!moveable(START_X,START_Y,MOVE_DOWN,BLOCK_UP)){
		setcolor(WHITE);
		setfont(45,0,"楷体");
		outtextxy(60,300,"老婆加油哦");
		Sleep(1000);
		system("pause");
		closegraph();
		exit(0);
	}

}

void wait(int interval){
	int count=interval/10;

	for(int i=0;i<count;i++){
		Sleep(10);
		if(kbhit()){
			return; 
		}
	}
}

int rotatable(int x,int y,block_dir_t dir){
	int xIndex=(y-minY)/20;
	int yIndex=(x-minX)/20;

	if(!moveable(x,y,MOVE_DOWN,dir)){
		return 0;
	}

	int id=BlockIndex*4+dir;

	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			if(block[id][i][j]==1 &&
				(yIndex+j<0 || yIndex+j>=15 || visit[xIndex+i][yIndex+j]==1)){
					return 0;
			}
		}
	}

	return 1;
}

void mark (int x,int y,int blockIndex,block_dir_t dir){
	int id=blockIndex*4+dir;
	int x2=(y-minY)/20;
	int y2=(x-minX)/20;

	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			if(block[id][i][j]==1){
				visit[x2+i][y2+j]=1;
				markColor[x2+i][y2+j]=color[blockIndex];
			}
		}
	}


}


void movedown(void){
	int x=START_X;
	int y=START_Y;
	int curSpeed=speed;
	int k=0;
	block_dir_t blockDir=BLOCK_UP;

	failCheck();//检查游戏是否结束

	while(1){
		if(kbhit()){
			int key=getch();
			if(key==KEY_SPACE){
				getch();
			}

		}

		clearBlock(x,k,blockDir);

		if(kbhit()){
			int key=getch();

			if(key==KEY_UP){
				block_dir_t nextDir=(block_dir_t)((blockDir+1)%4);
				if(rotatable(x,y+k,nextDir)){
					blockDir=nextDir;
				}
			}else if(key==KEY_DOWN){
				curSpeed=100;
			}else if(key==KEY_LEFT){
				if(moveable(x,y+k+20,MOVE_LEFT,blockDir)){
					x-=20;
				}
			}else if(key==KEY_RIGHT){
				if(moveable(x,y+k+20,MOVE_RIGHT,blockDir)){
				x+=20;
				}
			}

		}

		k+=20;

		//绘制方块
		drawBlock(x,y+k,BlockIndex,blockDir);

		wait(curSpeed);

		//固化
		if(!moveable(x,y+k,MOVE_DOWN,blockDir)){
			mark(x,y+k,BlockIndex,blockDir);
			break;
		}
	}
}

void newblock(void){
	BlockIndex=NextIndex;

	//绘制刚从顶部下降的方块
	drawBlock(START_X,START_Y);

	Sleep(300);

	//在右上角绘制下一个方块
	nextblock();

	//方块降落
	movedown();
}

void down(int k){
	for(int i=k;i>0;i--){
		for(int j=0;j<15;j++){
			if(visit[i-1][j]){
				visit[i][j]=1;
				markColor[i][j]=markColor[i-1][j];
				setcolor(markColor[i][j]);
				outtextxy(minX+BLOCK_SIZEOF*j,minY+BLOCK_SIZEOF*i,"■");
			}else{
				visit[i][j]=0;
				setcolor(BLACK);
				outtextxy(minX+BLOCK_SIZEOF*j,minY+BLOCK_SIZEOF*i,"■");
			}
		}
	}

	setcolor(BLACK);
	for(int j=0;j<15;j++){
		visit[0][j];
		outtextxy(20*j+minX,minY,"■");
	}

}

void addScore(int lines){
	char str[32];

	setcolor(RED);
	score+=lines*10;
	sprintf(str,"%d",score);
	outtextxy(415,310,str);
}

void updateGrade(void){
	grade=score/50;
	char str[16];
	sprintf(str,"%d",grade);
	outtextxy(425,405,str);

	speed=500-grade*10;
	if(speed<=100){
		speed=100;
	}

}

void check(void){
	int clearLines=0;
	int i,j;
	for(i=29;i>=0;i--){
		for(j=0;j<15 && visit[i][j];j++);
		if(j>=15){
			down(i);
			i++;
			clearLines++;
		}
		
	}

	addScore(clearLines);

	updateGrade();

}

int main(void){
	welcome();//欢迎界面
	initGameScene();//游戏场景

	//产生新方块
	nextblock();
	Sleep(500);

	//初始化访问数组
	memset(visit,0,sizeof(visit));

	while(1){
		newblock();

		check();

	}

	system("pause");
	closegraph();
	return 0;
}
