/*
	����:  ����̰����
	����:  ÷��գ�QQ��3406077232��
	����:  Vs2019-C++����Ŀģ��-32λ
	��չ��:EasyX_20200315(beta)
	ʱ��:  2020-04
*/
#include <ctime>
#include <cstdlib>
#include <list>
#include <cmath>
#include <graphics.h>
#include <conio.h>
const int length = 5;		// �ߵ���С���ȵ�λ
const int BEGIN = 12;		// �ߵĳ�ʼ����
const int TIME = 25;		// �ߵ���ǰһ����С��λ�ļ��ʱ��
const int DIE_TIME = 700;	// ������ĵȴ�ʱ��
const int BIG_M = 3;		// ��ͼ����չ��С
const int LEFT_MAP = 50;	// ��ͼ�󲿵� x ����
const int TOP_MAP = 50;		// ��ͼ������ y ����
const int RIGHT_MAP = 550;	// ��ͼ�Ҳ��� x ����
const int BUTTOM_MAP = 550;	// ��ͼ�ײ��� y ����
const int BEGIN_X = 350;	// ��β�ĳ�ʼ x ����
const int BEGIN_Y = 360;	// ��β�ĳ�ʼ y ����

const RECT site = { LEFT_MAP,TOP_MAP,RIGHT_MAP,BUTTOM_MAP };	// ���صĽṹ��

typedef void(*fuction)();	// ���Ƶ�ͼ�ĺ���ָ��

int sc;			// ����
int now;		// ���¸��µ�ʱ��
int game_time;	// ��Ϸʱ��



void begin();
void end();
void core_algorithm();

void out_time(bool);
void out_score();
void out_ins();

void scenario_1();
void scenario_2();
void scenario_3();
void scenario_4();

void solidcircle(POINT, int);	// ���غ���


POINT point_rand();
POINT turn(int, POINT);


int soc();
int middle_rect(int, int, int);
int In_rect(RECT, POINT);


// ���ֵ�ͼ
fuction scenario[] = { scenario_1, scenario_2, scenario_3, scenario_4 };
int main()
{
	initgraph(800, 600);	//800 * 600�Ļ���
	while (1)
	{
		begin();
		end();
	}
}


// ��Ϸ��ʼʱ�ĳ���
void begin()
{

	// ȫ�ֱ�����ʼ�������
	game_time = 0;
	sc = 0;
	srand(time(0));
	setbkcolor(BLACK);
	cleardevice();
	settextcolor(WHITE);
	settextstyle(72, 0, L"����");
	outtextxy(getwidth() / 2 - textwidth(L"̰����С��Ϸ") / 2, 20, L"̰����С��Ϸ");
	settextcolor(RED);
	settextstyle(40, 0, L"����");
	const wchar_t* diff = L"����ģʽ";
	const wchar_t* easy = L"��ģʽ";
	const int str_left_diff = middle_rect(0, getwidth(), textwidth(easy));
	const int str_left_easy = middle_rect(0, getwidth(), textwidth(easy));
	const int str_right_diff = str_left_diff + textwidth(diff);
	const int str_right_easy = str_left_easy + textwidth(easy);
	outtextxy(str_left_diff, 350, diff);
	outtextxy(str_left_easy, 400, easy);
	MOUSEMSG mouse;

	// �������������ϣ��������»���
	// �������������ϣ��Ұ��������������أ���ʼ��Ϸ
	do
	{
		settextcolor(RED);
		settextstyle(40, 0, L"����", 0, 0, 1, false, false, false);
		outtextxy(str_left_easy, 400, easy);
		outtextxy(str_left_diff, 350, diff);
		do
		{
			mouse = GetMouseMsg();
		} while (!(In_rect({ str_left_easy, 400, str_right_easy, 400 + textheight(easy) }, { mouse.x ,mouse.y }) || In_rect({ str_left_diff, 350, str_right_diff, 350 + textheight(diff) }, { mouse.x, mouse.y })));
		// �Ƿ�������

		settextstyle(40, 0, L"����", 0, 0, 2, false, true, false);
		settextcolor(BLUE);
		if (mouse.y <= 350 + textheight(diff))
			outtextxy(str_left_diff, 350, diff);
		else
			outtextxy(str_left_easy, 400, easy);
		while ((In_rect({ str_left_easy, 400, str_right_easy, 400 + textheight(easy) }, { mouse.x, mouse.y }) || In_rect({ str_left_diff, 350, str_right_diff,350 + textheight(diff) }, { mouse.x, mouse.y })) && !mouse.mkLButton)
			mouse = GetMouseMsg();
		// �ͷŻ������ϵ�û�а����������Ϣ

	} while (!mouse.mkLButton);
	setbkcolor(BLACK);
	cleardevice();
	if (mouse.y <= 350 + textheight(diff))
	{
		// ѡ������ģʽ
		// �����ͼ
		int m = rand() % 3 + 1;
		scenario[m]();
	}
	else
		scenario[0]();	// ѡ���ģʽ
	core_algorithm();
	return;
}


// ��������ĳ���
void end()
{
	setbkcolor(BLACK);
	cleardevice();
	settextcolor(WHITE);
	settextstyle(72, 0, L"����", 0, 0, 0, false, false, false);

	// �������
	wchar_t num[7] = { 0 };
	swprintf(num, 7, L"������%d", sc);
	outtextxy(getwidth() / 2 - textwidth(num) / 2, 50, num);
	FlushMouseMsgBuffer();
	MOUSEMSG n;
	do
		n = GetMouseMsg();
	while (!n.mkLButton);
	return;
}


// ��Ϸ�ĺ����㷨
void core_algorithm()
{
	// ��Ϸ��ʼ�趨
	// ��ʼ����

	int dir = 1;	// ����
	POINT food = point_rand();
	setfillcolor(RED);
	solidcircle(food, 2);
	POINT tmp[BEGIN];
	for (int i = 0; i < BEGIN; i++)
		tmp[i] = { BEGIN_X + length * i, BEGIN_Y };
	std::list<POINT> snack;	// �����ߵ�list
	std::list<POINT>::iterator it = snack.begin();
	for (int i = 0; i < BEGIN; i++)
		snack.push_back(tmp[i]);
	POINT b = *snack.begin();
	it = snack.begin();
	it++;
	setfillcolor(RGB(0, 187, 18));
	while (it != snack.end())
	{
		POINT e = *it;
		solidrectangle(min(e.x, b.x) - 2, max(e.y, b.y) + 2, max(e.x, b.x) + 2, min(e.y, b.y) - 2);
		b = e;
		it++;
	}
	bool die = false;
	// ��̬��������������
	POINT head = snack.back(), tail = snack.front();
	bool quit = true;

	// ����ַ�������
	while (_kbhit())
		_getch();
play:
	now = time(0);
	out_score();
	out_time(true);
	out_ins();
	while (!die)
	{
		while (!_kbhit() || quit == false)
		{
			// ��ǰһ��

			out_time(false);
			quit = true;
			bool eat = false;
			if (head.x >= RIGHT_MAP || head.x <= LEFT_MAP || head.y >= BUTTOM_MAP || head.y <= TOP_MAP)
			{
				die = true;
				Sleep(DIE_TIME);
				goto jd;
			}
			POINT th;
			th = turn(dir, head);
			if (getpixel(th.x, th.y) == RGB(0, 187, 18) || getpixel(th.x, th.y) == BLUE)
				die = true;
			snack.push_back(th);
			setfillcolor(RGB(0, 187, 18));
			solidrectangle(min(th.x, head.x) - 2, min(th.y, head.y) - 2, max(th.x, head.x) + 2, max(th.y, head.y) + 2);

			// �ж��Ƿ�Ե�ʳ��
			if (getpixel(food.x, food.y) != RED)
			{
				eat = true;
				sc++;
				out_score();

				//ʳ���������
				food = point_rand();
				setfillcolor(RED);
				solidcircle(food.x, food.y, 2);
			}
			if (die)
			{
				Sleep(DIE_TIME);
				goto jd;
			}

			//����β��
			head = th;
			it = snack.begin();
			it++;
			if (!eat)
			{
				clearrectangle(min(tail.x, it->x) - 2, min(it->y, tail.y) - 2, max(tail.x, it->x) + 2, max(it->y, tail.y) + 2);
				tail = *it;
				snack.pop_front();
			}
			setfillcolor(RGB(0, 187, 18));
			Sleep(TIME);
		}
		int share = _getch();
		if (share == 224)
		{
			quit = false;
			share = _getch();
			switch (share)
			{
			case 75:
				if (dir != 1)dir = 2;  break;
			case 77:
				if (dir != 2)dir = 1;  break;
			case 72:
				if (dir != 3)dir = 4;  break;
			case 80:
				if (dir != 4)dir = 3;  break;
			}
		}
		if (share == 27)
			goto suspended;
	}
jd:
	snack.clear();
	cleardevice();
	return;
suspended:
	if (soc() == 0)
		goto jd;
	else
		goto play;
}


// ת���ߵķ���
POINT turn(int dir, POINT k)
{
	switch (dir)
	{
	case 1: { POINT t = { k.x + length,k.y }; return t; }
	case 2: { POINT t = { k.x - length,k.y }; return t; }
	case 3: { POINT t = { k.x ,k.y + length }; return t; }
	case 4: { POINT t = { k.x ,k.y - length }; return t; }
	}
}


// ʳ���������
POINT point_rand()
{
	int tx, ty;
	// ��ֹʳ����������
	// ��ֹʳ���ڵ�ͼ��
	// ��ֹʳ����ǽ����
	do
	{
		tx = (rand() % (((RIGHT_MAP - LEFT_MAP) / length)) + 1) * length + LEFT_MAP;
		ty = (rand() % (((BUTTOM_MAP - TOP_MAP) / length)) + 1) * length + TOP_MAP;
	} while (getpixel(tx, ty) != BLACK);
	POINT t = { tx,ty };
	return t;
}


// ����ͬ���������� x �� y �����ϳ� POINT ���͵Ĳ���
void solidcircle(POINT k, int l)
{
	int tx = k.x;
	int ty = k.y;
	solidcircle(tx, ty, l);
}


// ��ͨ��ͼ
void scenario_1()
{
	setlinestyle(PS_SOLID, 5);
	setlinecolor(RGB(243, 248, 241));
	rectangle(LEFT_MAP - BIG_M, TOP_MAP - BIG_M, RIGHT_MAP + BIG_M, BUTTOM_MAP + BIG_M);
}


// ʮ�ֵ�ͼ
void scenario_2()
{
	int crossing_big = 50;
	setlinestyle(PS_SOLID, 5);
	setlinecolor(BLUE);
	line((RIGHT_MAP - LEFT_MAP) / 2 + LEFT_MAP, TOP_MAP + crossing_big, (RIGHT_MAP - LEFT_MAP) / 2 + LEFT_MAP, BUTTOM_MAP - crossing_big);
	line(LEFT_MAP + crossing_big, (BUTTOM_MAP - TOP_MAP) / 2 + TOP_MAP, RIGHT_MAP - crossing_big, (BUTTOM_MAP - TOP_MAP) / 2 + TOP_MAP);
	setlinecolor(RGB(243, 248, 241));
	rectangle(LEFT_MAP - BIG_M, TOP_MAP - BIG_M, RIGHT_MAP + BIG_M, BUTTOM_MAP + BIG_M);
}


// ·�ڵ�ͼ
void scenario_3()
{
	int space_big = (RIGHT_MAP - LEFT_MAP) / 4;
	int crossing_big = 50;
	setlinestyle(PS_SOLID, 5);
	setlinecolor(BLUE);
	line(LEFT_MAP, TOP_MAP + space_big * 1, RIGHT_MAP - crossing_big, TOP_MAP + space_big * 1);
	line(LEFT_MAP + crossing_big, TOP_MAP + space_big * 2, RIGHT_MAP, TOP_MAP + space_big * 2);
	line(LEFT_MAP, TOP_MAP + space_big * 3, RIGHT_MAP - crossing_big, TOP_MAP + space_big * 3);
	setlinecolor(RGB(243, 248, 241));
	rectangle(LEFT_MAP - BIG_M, TOP_MAP - BIG_M, RIGHT_MAP + BIG_M, BUTTOM_MAP + BIG_M);
}


// �ر���ͼ
void scenario_4()
{
	const int LONG = 150;
	const int ENT = 50;
	setlinestyle(PS_SOLID, 5);
	setlinecolor(BLUE);
	POINT bunker[] = {
						{(RIGHT_MAP - LEFT_MAP) / 2 + LEFT_MAP + ENT / 2 + LONG,(BUTTOM_MAP - TOP_MAP) / 2 + TOP_MAP + ENT / 2 },
						{(RIGHT_MAP - LEFT_MAP) / 2 + LEFT_MAP + ENT / 2,(BUTTOM_MAP - TOP_MAP) / 2 + TOP_MAP + ENT / 2},
						{(RIGHT_MAP - LEFT_MAP) / 2 + LEFT_MAP + ENT / 2,(BUTTOM_MAP - TOP_MAP) / 2 + TOP_MAP + ENT / 2 + LONG},
						{(RIGHT_MAP - LEFT_MAP) / 2 + LEFT_MAP - ENT / 2,(BUTTOM_MAP - TOP_MAP) / 2 + TOP_MAP + ENT / 2 + LONG},
						{(RIGHT_MAP - LEFT_MAP) / 2 + LEFT_MAP - ENT / 2,(BUTTOM_MAP - TOP_MAP) / 2 + TOP_MAP + ENT / 2},
						{(RIGHT_MAP - LEFT_MAP) / 2 + LEFT_MAP - ENT / 2 - LONG,(BUTTOM_MAP - TOP_MAP) / 2 + TOP_MAP + ENT / 2},
						{(RIGHT_MAP - LEFT_MAP) / 2 + LEFT_MAP - ENT / 2 - LONG,(BUTTOM_MAP - TOP_MAP) / 2 + TOP_MAP - ENT / 2},
						{(RIGHT_MAP - LEFT_MAP) / 2 + LEFT_MAP - ENT / 2,(BUTTOM_MAP - TOP_MAP) / 2 + TOP_MAP - ENT / 2},
						{(RIGHT_MAP - LEFT_MAP) / 2 + LEFT_MAP - ENT / 2,(BUTTOM_MAP - TOP_MAP) / 2 + TOP_MAP - ENT / 2 - LONG},
						{(RIGHT_MAP - LEFT_MAP) / 2 + LEFT_MAP + ENT / 2,(BUTTOM_MAP - TOP_MAP) / 2 + TOP_MAP - ENT / 2 - LONG},
						{(RIGHT_MAP - LEFT_MAP) / 2 + LEFT_MAP + ENT / 2,(BUTTOM_MAP - TOP_MAP) / 2 + TOP_MAP - ENT / 2},
						{(RIGHT_MAP - LEFT_MAP) / 2 + LEFT_MAP + ENT / 2 + LONG,(BUTTOM_MAP - TOP_MAP) / 2 + TOP_MAP - ENT / 2}
	};
	polyline(bunker, 12);
	setlinecolor(RGB(243, 248, 241));
	rectangle(LEFT_MAP - BIG_M, TOP_MAP - BIG_M, RIGHT_MAP + BIG_M, BUTTOM_MAP + BIG_M);
}


// ��Ϸ��ͣ�����ж��Ǽ�������ֹͣ
int soc()
{
	setbkcolor(RGB(161, 175, 201));
	const wchar_t* stop = L"ֹͣ��Ϸ";
	const wchar_t* Cont = L"������Ϸ";
	IMAGE now(getwidth(), getheight());
	getimage(&now, 0, 0, getwidth() - 1, getwidth() - 1);	// ���浱ǰͼ���Ա������Ϸʱ���¼���
	setfillcolor(RGB(161, 175, 201));
	solidrectangle(getwidth() / 2 - 200, getheight() / 2 - 250, getwidth() / 2 + 200, getheight() / 2 + 250);
	settextcolor(RGB(249, 239, 229));
	settextstyle(72, 0, L"����", 0, 0, 0, false, false, false);
	outtextxy(getwidth() / 2 - textwidth(L"��Ϸ��ͣ") / 2, 100, L"��Ϸ��ͣ");
	settextcolor(RED);
	settextstyle(40, 0, L"����", 0, 0, 1, false, false, false);
	const int str_left_stop = middle_rect(0, getwidth(), textwidth(stop));
	const int str_left_Cont = middle_rect(0, getwidth(), textwidth(Cont));
	const int str_right_stop = str_left_stop + textwidth(stop);
	const int str_right_Cont = str_left_Cont + textwidth(Cont);
	outtextxy(str_left_stop, 350, stop);
	outtextxy(str_left_Cont, 400, Cont);
	MOUSEMSG mouse;

	// �������������ϣ��������»���
	// �������������ϣ��Ұ��������������أ���ʼ��Ϸ
	do
	{
		settextcolor(RED);
		settextstyle(40, 0, L"����", 0, 0, 1, false, false, false);
		outtextxy(str_left_stop, 350, stop);
		outtextxy(str_left_Cont, 400, Cont);
		do
		{
			mouse = GetMouseMsg();
		} while (!(In_rect({ str_left_Cont, 400, str_right_Cont, 400 + textheight(Cont) }, { mouse.x, mouse.y }) || In_rect({ str_left_stop, 350, str_right_stop, 350 + textheight(stop) }, { mouse.x, mouse.y })));
		// �Ƿ�������

		settextstyle(40, 0, L"����", 0, 0, 2, false, true, false);
		settextcolor(BLUE);
		if (mouse.y <= 350 + textheight(stop))
			outtextxy(str_left_stop, 350, stop);
		else
			outtextxy(str_left_Cont, 400, Cont);
		while ((In_rect({ str_left_Cont, 400, str_right_Cont, 400 + textheight(Cont) }, { mouse.x, mouse.y }) || In_rect({ str_left_stop, 350, str_right_stop, 350 + textheight(stop) }, { mouse.x, mouse.y })) && !mouse.mkLButton)
			mouse = GetMouseMsg();
		// �ͷŻ������ϵ�û�а����������Ϣ

	} while (!mouse.mkLButton);
	setbkcolor(BLACK);
	if (mouse.y <= 350 + textheight(stop))
	{
		setbkcolor(WHITE);
		cleardevice();
		return 0;
	}
	else
	{
		putimage(0, 0, &now);
		return 1;
	}
}

// ���ʱ��
void out_time(bool is_0)
{
	int time_ = time(0);
	if (time_ != now || is_0 == true)
	{
		setbkcolor(BLACK);
		game_time += (time_ - now);
		now = time_;
		wchar_t str_time[12] = { 0 };
		swprintf(str_time, 12, L"ʱ�䣺%2d:%2d:%2d", game_time / (60 * 60), game_time % (60 * 60) / 60, game_time % 60);
		for (int i = 0; i < 10; i++)
			if (str_time[i] == ' ')
				str_time[i] = '0';
		settextstyle(30, 0, L"΢���ź�", 0, 0, 0, false, false, false);
		settextcolor(RGB(201, 221, 35));
		outtextxy(getwidth() - textwidth(str_time) - 30, 30, str_time);
	}
	return;
}

// �������
void out_score()
{
	wchar_t str_sc[12] = { 0 };
	wchar_t tmp[] = L"ʱ�䣺00:00:00";
	swprintf(str_sc, 12, L"������%d", sc);
	settextstyle(30, 0, L"΢���ź�", 0, 0, 0, false, false, false);
	settextcolor(RGB(201, 221, 35));
	outtextxy(getwidth() - textwidth(tmp) - 30, 30 + textheight(tmp), str_sc);
	return;
}

// ���˵��
void out_ins()
{
	wchar_t tmp[] = L"ʱ�䣺00:00:00";
	const int out_begin_y = 30 + textheight(tmp) * 2;
	settextstyle(30, 0, L"΢���ź�", 0, 0, 0, false, false, false);
	settextcolor(RGB(201, 221, 35));
	wchar_t str_ins1a[] = L"�������";
	wchar_t str_ins1b[] = L"�����ߵķ���";
	wchar_t str_ins2a[] = L"Esc ����";
	wchar_t str_ins2b[] = L"��ͣ��Ϸ";
	outtextxy(getwidth() - textwidth(tmp) - 30, out_begin_y + 30 * 0, str_ins1a);
	outtextxy(getwidth() - textwidth(tmp) + 10, out_begin_y + 30 * 1, str_ins1b);
	outtextxy(getwidth() - textwidth(tmp) - 30, out_begin_y + 30 * 2, str_ins2a);
	outtextxy(getwidth() - textwidth(tmp) + 10, out_begin_y + 30 * 3, str_ins2b);
	return;
}

// ���س���Ϊ k ���߶ε��е�ǡ��Ϊ left-right �߶��е�ʱ��˵�� x ����
int middle_rect(int left, int right, int k)
{
	return ((right - left) / 2 - k / 2) + left;
}

// �жϵ��Ƿ��ھ�����
int In_rect(RECT rect, POINT point)
{
	if (point.x > rect.left && point.x < rect.right && point.y > rect.top && point.y < rect.bottom)
		return 1;
	else
		return 0;
}