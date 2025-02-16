#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<graphics.h>//ͼ�λ�����ͷ�ļ�
#include<windows.h>
#include<mmsystem.h>//�������
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
	//ɨ������
	//��������
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
	//��Ϸ��ѭ��
	//show(map);
	//draw(map);
	draw(map);
	sendBGM(-2);//����
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
					int select = MessageBox(GetHWnd(), "����һ�ѣ�", "��Ӯ�ˣ�", MB_OKCANCEL);
					if (select == IDOK) {
						//����һ��
						//���³�ʼ��
						//start();
						init(map);
					}
					else {
						//�˳�
						exit(0);
					}
					//printf("��Ӯ�ˣ�\n");
					//show(map);
					break;
				}
				else if (ret == -1) {
					//mciSendString("open ./sound/boom.mp3 alias BOOM1", NULL, 0, NULL);
					finaldraw(map);
					sendBGM(-1);
					int select = MessageBox(GetHWnd(), "���¿�ʼ��", "�����ˣ�", MB_OKCANCEL);
					if (select == IDOK) {
						//����һ��
						//���³�ʼ��
						//start();
						init(map);
					}
					else {
						//�˳�
						exit(0);
					}
					//printf("�����ˣ�\n");
					show(map);
					break;
				}
				else {
					sendBGM(1);
					//printf("������\n");
					//show(map);
					break;
				}
			}
			draw(map);
		}
	}

	getchar();//��ֹ����
	return 0;
}
//չʾ���̣������У�
void show(int** map) {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			printf("%-3d ", map[i][j]);
		}
		printf("\n\n");
	}
}
//��ʼ������
void init(int** map) {

	loadresourse();
	//�������MINE_COUNT���ף���-1��ʾ
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
			map[rr][rc] = -1;//��ֹ�׵�λ���ظ�
			i++;
		}
	}
	//������Χ������
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (map[i][j] == -1) {
				for (int row = i - 1; row <= i + 1; row++) {
					for (int col = j - 1; col <= j + 1; col++) {
						//��ֹ����Խ��
						if (row >= 0 && col >= 0 && row < ROW && col < COL && map[row][col] != -1) {
							++map[row][col];//��Χ���ף���������
						}
					}
				}
			}
		}
	}
	//���ܸ��ӣ�����������
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
		loadimage(&imgs[i], imgPath, IMAGESIZE, IMAGESIZE);//����Ҫ���ַ�����Ϊʹ�ö��ֽ�
		//putimage(i * IMAGESIZE, 0, imgs + i);
	}
}
void draw(int** map) {
	//����map�����������Ӧ��ͼƬ
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
	//����map�����������Ӧ��ͼƬ
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
	//����map�����������Ӧ��ͼƬ
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
//��꽻��
void mouseMsg(ExMessage* msg, int** map) {
	//sendBGM(1);
	//�ָ�������������������Ӧ�������±�
	int _x = msg->x / IMAGESIZE;
	int _y = msg->y / IMAGESIZE;
	printf("%d %d\n", msg->x, msg->y);

	//����򿪸���

	if (msg->message == WM_LBUTTONDOWN) {

		if (map[_y][_x] >= 19 && map[_y][_x] <= 28) {
			map[_y][_x] -= 20;//��ʾ������ˣ�����
			unfold(map, _y, _x);
			system("cls");
			//show(map);
		}
	}
	//�Ҽ���Ǹ���
	else if (msg->message == WM_RBUTTONDOWN) {
		if (map[_y][_x] >= 19 && map[_y][_x] <= 28) {
			map[_y][_x] += 20;//��ʾ�Ҽ���
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

//����հ׸��ӣ�չ������λ�ã�ֱ�������ǿհ׸���
void unfold(int** map, int row, int col) {

	//row��col�ǵ�ǰ���λ��
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
//��Ϸ�����ж�
int judge(int** map, int row, int col) {

	//�㵽���ף����ˣ�����
	if (map[row][col] == -1)//������ȥ��20���ص���ʼ��-1��
	{
		return -1;
	}

	//�����˸��ӣ�Ӯ�ˣ�����
	//Ӯ�˷���1
	//��Ϸʤ����������ͳ�Ƶ㿪������
	int cnt = 0;
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (map[i][j] >= 0 && map[i][j] <= 8) {
				cnt++;
			}
		}
	}
	if (ROW * COL - cnt == MINE_COUNT) {
		return 1;//�򿪵ĸ����������ܸ�����-�׵ĸ���
	}
	//û��������0
	return 0;


}
void sendBGM(int select) {
	switch (select)
	{
	case -2://����
		mciSendString("open ./sound/start.wav alias START", NULL, 0, NULL);
		mciSendString("play START wait", NULL, 0, NULL);
		mciSendString("close START", 0, 0, 0);
		break;
	case -1://��ը
		mciSendString("open ./sound/boom.mp3 alias BOOM1", NULL, 0, NULL);
		mciSendString("play BOOM1 wait", NULL, 0, NULL);
		mciSendString("close BOOM1", 0, 0, 0);
		break;
	case 0://���
	case 1://�����㿪
		mciSendString("open ./sound/���.wav alias click", NULL, 0, NULL);
		mciSendString("play click", NULL, 0, NULL);
		Sleep(100);
		mciSendString("close click", 0, 0, 0);
		break;
	case 2://ʤ��
		mciSendString("open ./sound/success.wav alias WIN", NULL, 0, NULL);
		mciSendString("play WIN wait", NULL, 0, NULL);
		mciSendString("close WIN", 0, 0, 0);
		break;
	}
}