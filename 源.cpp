#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<graphics.h>//图形化界面头文件
#include<windows.h>
#include<mmsystem.h>//添加音乐
#pragma comment(lib,"winmm.lib")

//#define ROW 22
//#define COL 30
//#define MINE_COUNT 10
#define IMAGESIZE 32
IMAGE imgs[12];
void finaldraw(int** map);
void loadresourse();
void show(int** map);
void init(int** map);
void draw(int** map);
void mouseMsg(ExMessage* msg, int** map);
void unfold(int** map, int row, int col);
int judge(int** map, int row, int col);
void sendBGM(int select);

int ROW = 10;
int COL = 10;
int MINE_COUNT = 10;

int main() {
	//start();
	//扫雷棋盘
	//创建窗口
	int** map = (int**)calloc(ROW, sizeof(int*));
	for (int i = 0; i < ROW; i++) {
		map[i] = (int*)calloc(COL, sizeof(int));
	}
	init(map);
	initgraph(IMAGESIZE * COL, IMAGESIZE * ROW);//EX_SHOWCONSOLE
	/*int** map = (int**)calloc(ROW,sizeof(int*));
	for (int i = 0; i < ROW; i++) {
		map[i] = (int*)calloc(COL ,sizeof(int));
	}
	init(map);*/
	//游戏主循环
	//show(map);
	//draw(map);
	draw(map);
	sendBGM(-2);//开机
	while (1) {
		ExMessage msg;
		while (peekmessage(&msg, EM_MOUSE)) {
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
				mouseMsg(&msg, map);
				int ret = judge(map, msg.y / IMAGESIZE, msg.x / IMAGESIZE);
				system("cls");
				if (ret == 1) {
					draw(map);
					//draw(map);
					//draw(map);
					sendBGM(2);
					int select = MessageBox(GetHWnd(), "再来一把？", "你赢了！", MB_OKCANCEL);
					if (select == IDOK) {
						//再来一把
						//重新初始化
						//start();
						init(map);
					}
					else {
						//退出
						exit(0);
					}
					//printf("你赢了！\n");
					//show(map);
					break;
				}
				else if (ret == -1) {
					//mciSendString("open ./sound/boom.mp3 alias BOOM1", NULL, 0, NULL);
					finaldraw(map);
					sendBGM(-1);
					int select = MessageBox(GetHWnd(), "重新开始？", "你输了！", MB_OKCANCEL);
					if (select == IDOK) {
						//再来一把
						//重新初始化
						//start();
						init(map);
					}
					else {
						//退出
						exit(0);
					}
					//printf("你输了！\n");
					show(map);
					break;
				}
				else {
					sendBGM(1);
					//printf("继续！\n");
					//show(map);
					break;
				}
			}
			draw(map);
		}
	}

	getchar();//防止闪退
	return 0;
}
//展示棋盘（命令行）
void show(int** map) {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			printf("%-3d ", map[i][j]);
		}
		printf("\n\n");
	}
}
//初始化棋盘
void init(int** map) {

	loadresourse();
	//随机设置MINE_COUNT个雷，用-1表示
	srand((unsigned)time(NULL));
	//memset(map, 0, sizeof(map));
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			map[i][j] = 0;
		}
	}
	for (int i = 0; i < MINE_COUNT;) {
		int rr = rand() % ROW;
		int rc = rand() % COL;
		if (map[rr][rc] == 0) {
			map[rr][rc] = -1;//防止雷的位置重复
			i++;
		}
	}
	//设置周围的数字
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (map[i][j] == -1) {
				for (int row = i - 1; row <= i + 1; row++) {
					for (int col = j - 1; col <= j + 1; col++) {
						//防止数组越界
						if (row >= 0 && col >= 0 && row < ROW && col < COL && map[row][col] != -1) {
							++map[row][col];//周围有雷，数字自增
						}
					}
				}
			}
		}
	}
	//加密格子，把雷盘隐藏
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			map[i][j] += 20;
		}
	}
}
void loadresourse() {
	for (int i = 0; i < 12; i++) {
		char imgPath[50] = { 0 };
		sprintf_s(imgPath, "./images/%d.png", i);
		loadimage(&imgs[i], imgPath, IMAGESIZE, IMAGESIZE);//这里要把字符集改为使用多字节
		//putimage(i * IMAGESIZE, 0, imgs + i);
	}
}
void draw(int** map) {
	//根据map里的数据贴对应的图片
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {

			if (map[i][j] >= 0 && map[i][j] <= 8) {
				putimage(j * IMAGESIZE, i * IMAGESIZE, &imgs[map[i][j]]);
			}
			else if (map[i][j] == -1) {
				putimage(j * IMAGESIZE, i * IMAGESIZE, &imgs[9]);
			}
			else if (map[i][j] >= 19 && map[i][j] <= 28) {
				putimage(j * IMAGESIZE, i * IMAGESIZE, &imgs[10]);
			}
			else if (map[i][j] >= 39) {
				putimage(j * IMAGESIZE, i * IMAGESIZE, &imgs[11]);
			}
		}
	}
}
void finaldraw(int** map) {
	//根据map里的数据贴对应的图片
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {

			if (map[i][j] >= 0 && map[i][j] <= 8) {
				putimage(j * IMAGESIZE, i * IMAGESIZE, &imgs[map[i][j]]);
			}
			else if (map[i][j] == -1 || map[i][j] == 19) {
				putimage(j * IMAGESIZE, i * IMAGESIZE, &imgs[9]);
			}
			else if (map[i][j] >= 19 && map[i][j] <= 28) {
				putimage(j * IMAGESIZE, i * IMAGESIZE, &imgs[10]);
			}
			else if (map[i][j] >= 39) {
				putimage(j * IMAGESIZE, i * IMAGESIZE, &imgs[11]);
			}
		}
	}
}
void fin_draw(int** map) {
	//根据map里的数据贴对应的图片
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {

			if (map[i][j] >= 0 && map[i][j] <= 8) {
				putimage(j * IMAGESIZE, i * IMAGESIZE, &imgs[map[i][j]]);
			}
			else if (map[i][j] == -1) {
				putimage(j * IMAGESIZE, i * IMAGESIZE, &imgs[9]);
			}
			else if (map[i][j] >= 19 && map[i][j] <= 28) {
				putimage(j * IMAGESIZE, i * IMAGESIZE, &imgs[10]);
			}
			else if (map[i][j] >= 39) {
				putimage(j * IMAGESIZE, i * IMAGESIZE, &imgs[11]);
			}
		}
	}
}
//鼠标交互
void mouseMsg(ExMessage* msg, int** map) {
	//sendBGM(1);
	//现根据鼠标点击的坐标求出对应的数组下标
	int _x = msg->x / IMAGESIZE;
	int _y = msg->y / IMAGESIZE;
	printf("%d %d\n", msg->x, msg->y);

	//左键打开格子

	if (msg->message == WM_LBUTTONDOWN) {

		if (map[_y][_x] >= 19 && map[_y][_x] <= 28) {
			map[_y][_x] -= 20;//表示点击过了，揭秘
			unfold(map, _y, _x);
			system("cls");
			//show(map);
		}
	}
	//右键标记格子
	else if (msg->message == WM_RBUTTONDOWN) {
		if (map[_y][_x] >= 19 && map[_y][_x] <= 28) {
			map[_y][_x] += 20;//表示右键了
			system("cls");
			//show(map);
		}
		else if (map[_y][_x] >= 39) {
			map[_y][_x] -= 20;
			system("cls");
			//show(map);
		}
	}
}

//点击空白格子，展开其他位置，直到遇到非空白格子
void unfold(int** map, int row, int col) {

	//row和col是当前点击位置
	if (map[row][col] == 0) {
		for (int i = row - 1; i <= row + 1; i++) {
			for (int j = col - 1; j <= col + 1; j++) {
				if (i >= 0 && i < ROW && j >= 0 && j < COL
					&& map[i][j] >= 19 && map[i][j] <= 28) {
					map[i][j] -= 20;
					unfold(map, i, j);
				}
			}
		}
	}

}
//游戏结束判定
int judge(int** map, int row, int col) {

	//点到了雷，输了，结束
	if (map[row][col] == -1)//点完后减去了20，回到开始的-1了
	{
		return -1;
	}

	//点完了格子，赢了，结束
	//赢了返回1
	//游戏胜利的条件：统计点开的数量
	int cnt = 0;
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (map[i][j] >= 0 && map[i][j] <= 8) {
				cnt++;
			}
		}
	}
	if (ROW * COL - cnt == MINE_COUNT) {
		return 1;//打开的格子数等于总格子数-雷的个数
	}
	//没结束返回0
	return 0;


}
void sendBGM(int select) {
	switch (select)
	{
	case -2://开场
		mciSendString("open ./sound/start.wav alias START", NULL, 0, NULL);
		mciSendString("play START wait", NULL, 0, NULL);
		mciSendString("close START", 0, 0, 0);
		break;
	case -1://爆炸
		mciSendString("open ./sound/boom.mp3 alias BOOM1", NULL, 0, NULL);
		mciSendString("play BOOM1 wait", NULL, 0, NULL);
		mciSendString("close BOOM1", 0, 0, 0);
		break;
	case 0://标记
	case 1://正常点开
		mciSendString("open ./sound/点击.wav alias click", NULL, 0, NULL);
		mciSendString("play click", NULL, 0, NULL);
		Sleep(100);
		mciSendString("close click", 0, 0, 0);
		break;
	case 2://胜利
		mciSendString("open ./sound/success.wav alias WIN", NULL, 0, NULL);
		mciSendString("play WIN wait", NULL, 0, NULL);
		mciSendString("close WIN", 0, 0, 0);
		break;
	}
}