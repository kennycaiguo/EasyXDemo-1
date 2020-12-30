//��Ŀ����		�������Ͳ��̬
//����			����1704
//email			1479245050@qq.com
//��ϵ��ʽ		qq 1479245050
//����			������Ƿ������εĶ���Σ������ں궨��ֱ�Ӹı������ͻ�����С�ȣ�ֱ�Ӹ��ܲ�ͬ���Ӿ�Ч����
//���뻷��		win10 64λ + vs2017 + easyx��ver: 20180727(beta)
// https://codebus.cn/featherstar/a/draw-pentagram

#include <graphics.h>
#include <conio.h>
#include <math.h>

// �궨��
#define PI				3.1415926535897932384626433832795
#define Lthickness		2										//	���ߴ�ϸ
#define	Pnum			30										//	���������
#define	side			4										//	�����λ
#define FPS				50										//	֡��
#define	BeginDelay		1000
#define	EndDelay		1000
#define	AllTurnDeg		4*PI									//	�ܹ���ת�Ƕ�
#define	EveryTurnDeg	PI/FPS/2								//	ÿ����ת�Ƕ�
#define	Width			640										//	������
#define	Highth			480										//	����߶�

// ȫ�ֱ�������
struct pentagram
{
	POINT	p[10];
}Ptm[Pnum];
int temp;
int ifre;														//	�Ƿ�����
int times;														//	��ת����

																//	TODO	��	��������
void init();													//	���õ�λ
void rotating();												//	��ת
void connectp();												//	�滭

																//	TODO	��	���캯��
int main()
{
	initgraph(Width, Highth);
	setbkcolor(WHITE);
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND, Lthickness, NULL, 0);

	BeginBatchDraw();
	while (1)
	{
		init();
		connectp();
		FlushBatchDraw();
		Sleep(BeginDelay);
		while (times < (4 * FPS + 1))
		{
			cleardevice();
			rotating();
			connectp();
			FlushBatchDraw();
			Sleep(1000 / FPS);
		}
		Sleep(EndDelay);
	}
	EndBatchDraw();
	_getch();
	closegraph();
	return 0;
}

// ���õ�λ
void init()
{
	cleardevice();
	times = 0;
	for (int num = 0; num < Pnum; num++) {
		Ptm[num].p[0] = { 0,(side * 4 * (num + 1)) };
		Ptm[num].p[1].x = (LONG)(Ptm[num].p[0].x * cos(-PI / 5) - Ptm[num].p[0].y / 2 * sin(-PI / 5));
		Ptm[num].p[1].y = (LONG)(Ptm[num].p[0].x * sin(-PI / 5) + Ptm[num].p[0].y / 2 * cos(-PI / 5));
		for (int num1 = 0; num1 < 5; num1++)
		{
			Ptm[num].p[(num1 * 2)].x = (LONG)(Ptm[num].p[0].x * cos(-2 * num1 * PI / 5) - Ptm[num].p[0].y * sin(-2 * num1 * PI / 5));
			Ptm[num].p[(num1 * 2)].y = (LONG)(Ptm[num].p[0].x * sin(-2 * num1 * PI / 5) + Ptm[num].p[0].y * cos(-2 * num1 * PI / 5));
			Ptm[num].p[(num1 * 2 + 1)].x = (LONG)(Ptm[num].p[1].x * cos(-2 * num1 * PI / 5) - Ptm[num].p[1].y * sin(-2 * num1 * PI / 5));
			Ptm[num].p[(num1 * 2 + 1)].y = (LONG)(Ptm[num].p[1].x * sin(-2 * num1 * PI / 5) + Ptm[num].p[1].y * cos(-2 * num1 * PI / 5));
		}
	}
}

// �滭
void connectp()
{
	for (int num = 0; num < Pnum; num++)
		for (int num1 = 0; num1 < 10; num1++) {
			temp = ((num1 + 1) < 10) ? (num1 + 1) : 0;
			line(Ptm[num].p[num1].x + Width / 2, -Ptm[num].p[num1].y + Highth / 2, Ptm[num].p[temp].x + Width / 2, -Ptm[num].p[temp].y + Highth / 2);
		}
}

// ��ת
void rotating()
{
	for (int num = 0; num < Pnum; num++) {
		//((num%2)==0?(-1):1) �ɲ����ж���ת����
		Ptm[num].p[0].x = (LONG)(0 * cos(((num % 2) == 0 ? (-1) : 1) * times * EveryTurnDeg) - (side * 4 * (num + 1)) * sin(((num % 2) == 0 ? (-1) : 1) * times * EveryTurnDeg));
		Ptm[num].p[0].y = (LONG)(0 * sin(((num % 2) == 0 ? (-1) : 1) * times * EveryTurnDeg) + (side * 4 * (num + 1)) * cos(((num % 2) == 0 ? (-1) : 1) * times * EveryTurnDeg));
		Ptm[num].p[1].x = (LONG)(Ptm[num].p[0].x * cos(-PI / 5) - Ptm[num].p[0].y / 2 * sin(-PI / 5));
		Ptm[num].p[1].y = (LONG)(Ptm[num].p[0].x * sin(-PI / 5) + Ptm[num].p[0].y / 2 * cos(-PI / 5));
		for (int num1 = 0; num1 < 5; num1++) {
			Ptm[num].p[(num1 * 2)].x = (LONG)(Ptm[num].p[0].x * cos(-2 * num1 * PI / 5) - Ptm[num].p[0].y * sin(-2 * num1 * PI / 5));
			Ptm[num].p[(num1 * 2)].y = (LONG)(Ptm[num].p[0].x * sin(-2 * num1 * PI / 5) + Ptm[num].p[0].y * cos(-2 * num1 * PI / 5));
			Ptm[num].p[(num1 * 2 + 1)].x = (LONG)(Ptm[num].p[1].x * cos(-2 * num1 * PI / 5) - Ptm[num].p[1].y * sin(-2 * num1 * PI / 5));
			Ptm[num].p[(num1 * 2 + 1)].y = (LONG)(Ptm[num].p[1].x * sin(-2 * num1 * PI / 5) + Ptm[num].p[1].y * cos(-2 * num1 * PI / 5));
		}
	}
	times++;
}