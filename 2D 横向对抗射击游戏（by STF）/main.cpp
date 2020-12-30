/*
 * ���ߣ�STF(QQ��2292683261)
 * ���뻷����Visual Studio 2019��EasyX_20200315(beta)
 * ��ֻ�Ǹ�С��Ϸ������ȫ������
 * ��һ�淢�����ڣ�2016-6-17
 * �ڶ��淢�����ڣ�2020-3-22
 * https://codebus.cn/contributor/a/stf-2d-shooting
 */
#include <easyx.h>
#include <cstdlib>
#include <ctime>


void hp_bar();
void show_player();
void show_enemy();
void move_enemy();
void draw_background();
int generate_line();					// ������ -1����ʾ��������ʧ��

int create_p_b();						// �����Ի����ӵ�
int create_e_b();						// �����л����ӵ�

int destroy_p_b(int index);
int destroy_e_b(int index);				// ɾ��һ���ӵ�

#define FRAMERATE 20					// ����ˢ�µ�����(ms)
#define FIRERATE 350					// ������ʱ��
#define E_FIRERATE 350					// ����������
#define BLEED_TIME 150					// ������˸ʱ��

#define BACKGROUND 80					// ���Ʊ�������������

#define MAX_LINES 75					// ���ͬ������������Ŀ
#define MAX_PLAYER_BULLETS 40			// ���ͬ���Ի��ӵ���Ŀ
#define MAX_ENEMY_BULLETS 40			// ���ͬ���л��ӵ���Ŀ


int player_pos[2] = { 30,30 };						// �Ի�λ��xy
int enemy_bullet[MAX_ENEMY_BULLETS][2];				// ���˵��ӵ�λ��
int player_bullet[MAX_PLAYER_BULLETS][2];			// �Ի����ӵ�λ��
int enemy_pos[2] = { 580,240 };						// �л�λ��
bool p_b_slots[MAX_PLAYER_BULLETS] = { false };		// �����ж� player_bullet ��ĳ��λ���Ƿ����
bool e_b_slots[MAX_ENEMY_BULLETS] = { false };
int number_p_b = 0, number_e_b = 0;					// ��¼�Ի��͵л����ӵ��������ٱ���ѹ��

int player_health = 100, enemy_health = 100;

bool isBleeding_p = false, isBleeding_e = false;	// ����ʵ�����к����˸Ч��


int background_line[MAX_LINES][3];					// ���������������������ֱ��� x��y������
bool line_slots[MAX_LINES] = { false };
int number_lines = 0;								// ��¼����������Ŀ


clock_t begin_time = 0;

int main()
{
	initgraph(640, 550, 4);

	srand((unsigned)time(NULL));

	settextcolor(RGB(0, 254, 0));
	settextstyle(30, 0, L"΢���ź�");
	outtextxy(50, 200, L"������ƶ�, Z ����, �� Shift �л�����ģʽ");

	bool win = false, dead = false;

	clock_t firerate = clock();						// �������
	clock_t e_firerate = clock();					// ���Ƶл������
	clock_t runtime = clock();						// ���ڿ��ƻ���ˢ��Ƶ��
	clock_t bleed_p = clock(), bleed_e = clock();	// ����ʵ��������˸
	clock_t backgroundline_generate = clock();		// �������ɱ�������
	Sleep(3000);
	BeginBatchDraw();

	bool leftshift = false;

	begin_time = clock();
	while (true)
	{

		if (clock() - runtime >= FRAMERATE)
		{
			runtime = clock();
			cleardevice();
			draw_background();
			hp_bar();// ��Ѫ��
			show_player();
			show_enemy();

			int n_p_b = 1, n_e_b = 1;					// ����,�����ӵ���ˢ��λ��
			int p_b_toprocess = number_p_b, e_b_toprocess = number_e_b;	// ��Ҫ������ӵ���
			for (int i = 0; i < MAX_PLAYER_BULLETS && (n_p_b <= p_b_toprocess || n_e_b <= e_b_toprocess); ++i)
			{
				if (n_p_b <= p_b_toprocess)				// ����ӵ��Ѿ�������Ͳ�������
				{
					if (p_b_slots[i] == true)
					{
						++n_p_b;
						player_bullet[i][0] += 3;
						setfillcolor(RGB(150, 180, 210));
						if (player_bullet[i][0] >= 635)
						{
							destroy_p_b(i);	// ��������Ļ���Ҷ�
						}

						// ��ײ��⣬��������
						if ((player_bullet[i][0] + 5 >= enemy_pos[0] - 20 && player_bullet[i][0] - 5 <= enemy_pos[0] + 20) && (player_bullet[i][1] - 5 < enemy_pos[1] + 40 && player_bullet[i][1] + 5 > enemy_pos[1] - 40))
							// ���е���
						{
							destroy_p_b(i);
							enemy_health -= 8;
							isBleeding_e = true;
							bleed_e = clock();
						}

						fillrectangle(player_bullet[i][0] - 5, player_bullet[i][1] - 5, player_bullet[i][0] + 5, player_bullet[i][1] + 5);		// ���ӵ�
					}

				}

				if (n_e_b <= e_b_toprocess)				// ���˵��ӵ�
				{
					if (e_b_slots[i] == true)
					{
						++n_e_b;
						enemy_bullet[i][0] -= 3;
						setfillcolor(RGB(255, 180, 20));
						if (enemy_bullet[i][0] < 5)
						{
							destroy_e_b(i);
						}

						// ��ײ��⣬��������
						if (enemy_bullet[i][0] - 5 < player_pos[0] + 25 && enemy_bullet[i][0] + 5 > player_pos[0] - 25 && enemy_bullet[i][1] - 5 < player_pos[1] + 25 && enemy_bullet[i][1] + 5 > player_pos[1] - 25)
						{
							// �����Ի�
							isBleeding_p = true;
							destroy_e_b(i);
							player_health -= 8;
							bleed_p = clock();
						}
						fillrectangle(enemy_bullet[i][0] - 5, enemy_bullet[i][1] - 5, enemy_bullet[i][0] + 5, enemy_bullet[i][1] + 5);
					}

				}
			}

			if (win || dead)
				break;
			FlushBatchDraw();

			move_enemy();

			if (player_health <= 0)
				dead = true;
			if (enemy_health <= 0)
			{
				win = true;
			}

			if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)	// ��ס Shift ����
			{
				leftshift = true;
			}
			else
			{
				leftshift = false;
			}

			if (GetAsyncKeyState(VK_UP) & 0x8000)
				// ����ƶ�
			{
				if (player_pos[1] >= 28)
					if (leftshift)
						player_pos[1] -= 2;				// y �������������µ�
					else
						player_pos[1] -= 5;
			}
			if (clock() - firerate >= FIRERATE && GetAsyncKeyState('Z') & 0x8000)
				// ��ҿ���
			{
				firerate = clock();
				create_p_b();
			}
			if (GetAsyncKeyState(VK_DOWN) & 0x8000)
				// ����ƶ�
			{
				if (player_pos[1] <= 452)
					if (leftshift)
						player_pos[1] += 2;
					else
						player_pos[1] += 5;
			}
			if (GetAsyncKeyState(VK_LEFT) & 0x8000)
				// ����ƶ�
			{
				if (player_pos[0] >= 30)
					if (leftshift)
						player_pos[0] -= 2;
					else
						player_pos[0] -= 5;
			}
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
				// ����ƶ�
			{
				if (player_pos[0] <= 320)
					if (leftshift)
						player_pos[0] += 2;
					else
						player_pos[0] += 5;
			}


			if (clock() - e_firerate >= E_FIRERATE)
			{
				e_firerate = clock();
				create_e_b();
			}


			if (clock() - bleed_p >= BLEED_TIME)		// ����ʱ�������ر�������˸Ч��
			{
				isBleeding_p = false;
			}

			if (clock() - bleed_e >= BLEED_TIME)		// ����ʱ�������ر�������˸Ч��
			{
				isBleeding_e = false;
			}

			if (clock() - backgroundline_generate >= BACKGROUND)
			{
				backgroundline_generate = clock();
				generate_line();
			}
		}
	}
	if (win)
	{
		settextcolor(RGB(0, 254, 0));
		settextstyle(35, 0, L"����");
		outtextxy(150, 200, L"������boss!��Ӯ�ˣ���");
	}
	else
	{
		settextcolor(RGB(254, 0, 0));
		settextstyle(35, 0, L"����");
		outtextxy(140, 200, L"�㱻boss����ˣ�");
	}
	FlushBatchDraw();
	Sleep(5000);
	EndBatchDraw();
	return 0;
}






void hp_bar()
{
	setlinecolor(RGB(255, 255, 255));
	line(0, 481, 640, 481);										// һ���ָ���
	settextstyle(20, 0, L"����");
	outtextxy(10, 485, L"BOSS������ֵ:");
	outtextxy(10, 520, L"��ҵ�����ֵ:");
	setfillcolor(RGB(0, 255, 1));
	setlinecolor(WHITE);
	rectangle(160, 515, 560, 540);								// Ѫ�����
	setfillcolor(RGB(0, 255, 1));
	setlinecolor(RGB(255, 255, 255));
	if (player_health > 0)
		fillrectangle(160, 515, 160 + player_health * 4, 540);	// ���Ѫ��
	setlinecolor(WHITE);
	rectangle(160, 485, 560, 510);								// ����Ѫ�����
	setfillcolor(RGB(230, 0, 1));
	setlinecolor(RGB(255, 255, 255));
	if (enemy_health > 0)
		fillrectangle(160, 485, 160 + enemy_health * 4, 510);	// ����Ѫ��

}

void show_player()
{
	if (isBleeding_p)
		setfillcolor(RGB(255, 0, 0));
	else
		setfillcolor(RGB(150, 180, 210));
	fillrectangle(player_pos[0] - 25, player_pos[1] - 25, player_pos[0] + 25, player_pos[1] + 25);
	setfillcolor(RGB(100, 200, 180));
	fillrectangle(player_pos[0], player_pos[1] + 5, player_pos[0] + 40, player_pos[1] - 5);
}

void show_enemy()
{
	if (isBleeding_e)
		setfillcolor(RGB(255, 0, 0));
	else
		setfillcolor(RGB(0, 130, 125));
	fillrectangle(enemy_pos[0] - 20, enemy_pos[1] - 40, enemy_pos[0] + 20, enemy_pos[1] + 40);
	setfillcolor(RGB(100, 200, 180));
	fillrectangle(enemy_pos[0], enemy_pos[1] + 5, enemy_pos[0] - 40, enemy_pos[1] - 5);
}

void move_enemy()
{
	static bool angle_v;		// ���Ƶл�����ֱ�ƶ�����true Ϊ���ϣ�����Ե�ͻ���
	static bool angle_h;		// ���Ƶл���ˮƽ�ƶ�����true Ϊ���󣬵���Ե�ͻ���
	static clock_t interval;	// ��ʱ�������

	if (clock() - interval >= 2000)
	{
		interval = clock();
		if (rand() % 2)			// һ��ĸ��ʻ���
			angle_v = !angle_v;
		if (rand() % 2)
			angle_h = !angle_h;
	}

	if (angle_v == true)		// ���˵�ͼ��Ե�͵�ͷ
		enemy_pos[1] -= 3;
	else
		enemy_pos[1] += 3;
	if (angle_h == true)
		enemy_pos[0] -= 3;
	else
		enemy_pos[0] += 3;


	if (enemy_pos[1] >= 440)
		angle_v = true;
	else if (enemy_pos[1] <= 40)
		angle_v = false;


	if (enemy_pos[0] >= 580)
		angle_h = true;
	else if (enemy_pos[0] <= 380)
		angle_h = false;

}

void draw_background()
{
	setlinecolor(WHITE);
	int n_b_l = number_lines;		// ������������Ŀ
	for (int i = 0; i < MAX_LINES && (n_b_l > 0); ++i)
	{
		if (line_slots[i] == true)
		{
			if (background_line[i][0] + background_line[i][2] <= 0)		// ˵������������Ļ
			{
				--number_lines;
				line_slots[i] = false;
			}
			else
			{
				background_line[i][0] -= 10;				// �����ƶ�
				line(background_line[i][0], background_line[i][1], background_line[i][0] + background_line[i][2], background_line[i][1]);
			}
			--n_b_l;
		}

	}
}

int generate_line()
{
	if (number_lines >= MAX_LINES)
		return -1;
	++number_lines;
	for (int i = 0; i < MAX_LINES; ++i)
	{
		if (line_slots[i] == false)
		{
			line_slots[i] = true;
			background_line[i][0] = 640;				// ������������Ļ���ұ�
			background_line[i][1] = rand() % 480;		// �����߶����
			background_line[i][2] = 10 + rand() % 50;	// ������������� 10-50 ����֮��

			break;
		}
	}
	return 0;
}

int create_p_b()
{
	if (number_p_b > MAX_PLAYER_BULLETS)			// �ռ䲻��
		return -1;
	for (int i = 0; i < MAX_PLAYER_BULLETS; ++i)	// ���� slots��Ѱ�ҿ�λ
	{
		if (p_b_slots[i] == false)
		{
			p_b_slots[i] = true;
			player_bullet[i][0] = player_pos[0] + 45;
			player_bullet[i][1] = player_pos[1];	// �����ӵ�
			++number_p_b;
			break;
		}
	}
	return 0;
}

int create_e_b()
{
	if (number_e_b > MAX_ENEMY_BULLETS)				// �ռ䲻��
		return -1;
	for (int i = 0; i < MAX_ENEMY_BULLETS; ++i)		// ���� slots��Ѱ�ҿ�λ
	{
		if (e_b_slots[i] == false)
		{
			e_b_slots[i] = true;
			enemy_bullet[i][0] = enemy_pos[0] - 45;
			enemy_bullet[i][1] = enemy_pos[1];		// �����ӵ�
			++number_e_b;
			break;
		}
	}
	return 0;
}

int destroy_p_b(int index)
{
	if (index > MAX_PLAYER_BULLETS - 1)
		return -2;
	if (p_b_slots[index] == false)
		return -1;
	p_b_slots[index] = false;
	--number_p_b;
	return 0;
}

int destroy_e_b(int index)
{
	if (index > MAX_ENEMY_BULLETS - 1)
		return -2;
	if (e_b_slots[index] == false)
		return -1;
	e_b_slots[index] = false;
	--number_e_b;
	return 0;
}