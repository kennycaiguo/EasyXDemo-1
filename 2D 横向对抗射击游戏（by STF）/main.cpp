/*
 * 作者：STF(QQ：2292683261)
 * 编译环境：Visual Studio 2019，EasyX_20200315(beta)
 * 这只是个小游戏，所以全部内联
 * 第一版发布日期：2016-6-17
 * 第二版发布日期：2020-3-22
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
int generate_line();					// 若返回 -1，表示生成线条失败

int create_p_b();						// 创建自机的子弹
int create_e_b();						// 创建敌机的子弹

int destroy_p_b(int index);
int destroy_e_b(int index);				// 删除一个子弹

#define FRAMERATE 20					// 画面刷新的周期(ms)
#define FIRERATE 350					// 射击间隔时间
#define E_FIRERATE 350					// 敌人射击间隔
#define BLEED_TIME 150					// 受伤闪烁时间

#define BACKGROUND 80					// 绘制背景线条的周期

#define MAX_LINES 75					// 最多同屏背景线条数目
#define MAX_PLAYER_BULLETS 40			// 最多同屏自机子弹数目
#define MAX_ENEMY_BULLETS 40			// 最多同屏敌机子弹数目


int player_pos[2] = { 30,30 };						// 自机位置xy
int enemy_bullet[MAX_ENEMY_BULLETS][2];				// 敌人的子弹位置
int player_bullet[MAX_PLAYER_BULLETS][2];			// 自机的子弹位置
int enemy_pos[2] = { 580,240 };						// 敌机位置
bool p_b_slots[MAX_PLAYER_BULLETS] = { false };		// 用于判断 player_bullet 的某个位置是否可用
bool e_b_slots[MAX_ENEMY_BULLETS] = { false };
int number_p_b = 0, number_e_b = 0;					// 记录自机和敌机的子弹数，减少遍历压力

int player_health = 100, enemy_health = 100;

bool isBleeding_p = false, isBleeding_e = false;	// 用于实现命中后的闪烁效果


int background_line[MAX_LINES][3];					// 背景的线条，三个参数分别是 x、y、长度
bool line_slots[MAX_LINES] = { false };
int number_lines = 0;								// 记录背景线条数目


clock_t begin_time = 0;

int main()
{
	initgraph(640, 550, 4);

	srand((unsigned)time(NULL));

	settextcolor(RGB(0, 254, 0));
	settextstyle(30, 0, L"微软雅黑");
	outtextxy(50, 200, L"方向键移动, Z 攻击, 左 Shift 切换低速模式");

	bool win = false, dead = false;

	clock_t firerate = clock();						// 射击控制
	clock_t e_firerate = clock();					// 控制敌机的射击
	clock_t runtime = clock();						// 用于控制画面刷新频率
	clock_t bleed_p = clock(), bleed_e = clock();	// 用于实现受伤闪烁
	clock_t backgroundline_generate = clock();		// 用于生成背景线条
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
			hp_bar();// 画血条
			show_player();
			show_enemy();

			int n_p_b = 1, n_e_b = 1;					// 计数,遍历子弹，刷新位置
			int p_b_toprocess = number_p_b, e_b_toprocess = number_e_b;	// 需要处理的子弹数
			for (int i = 0; i < MAX_PLAYER_BULLETS && (n_p_b <= p_b_toprocess || n_e_b <= e_b_toprocess); ++i)
			{
				if (n_p_b <= p_b_toprocess)				// 如果子弹已经处理完就不处理了
				{
					if (p_b_slots[i] == true)
					{
						++n_p_b;
						player_bullet[i][0] += 3;
						setfillcolor(RGB(150, 180, 210));
						if (player_bullet[i][0] >= 635)
						{
							destroy_p_b(i);	// 到达了屏幕最右端
						}

						// 碰撞检测，两个矩形
						if ((player_bullet[i][0] + 5 >= enemy_pos[0] - 20 && player_bullet[i][0] - 5 <= enemy_pos[0] + 20) && (player_bullet[i][1] - 5 < enemy_pos[1] + 40 && player_bullet[i][1] + 5 > enemy_pos[1] - 40))
							// 击中敌人
						{
							destroy_p_b(i);
							enemy_health -= 8;
							isBleeding_e = true;
							bleed_e = clock();
						}

						fillrectangle(player_bullet[i][0] - 5, player_bullet[i][1] - 5, player_bullet[i][0] + 5, player_bullet[i][1] + 5);		// 画子弹
					}

				}

				if (n_e_b <= e_b_toprocess)				// 敌人的子弹
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

						// 碰撞检测，两个矩形
						if (enemy_bullet[i][0] - 5 < player_pos[0] + 25 && enemy_bullet[i][0] + 5 > player_pos[0] - 25 && enemy_bullet[i][1] - 5 < player_pos[1] + 25 && enemy_bullet[i][1] + 5 > player_pos[1] - 25)
						{
							// 击中自机
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

			if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)	// 按住 Shift 减速
			{
				leftshift = true;
			}
			else
			{
				leftshift = false;
			}

			if (GetAsyncKeyState(VK_UP) & 0x8000)
				// 玩家移动
			{
				if (player_pos[1] >= 28)
					if (leftshift)
						player_pos[1] -= 2;				// y 的正方向是向下的
					else
						player_pos[1] -= 5;
			}
			if (clock() - firerate >= FIRERATE && GetAsyncKeyState('Z') & 0x8000)
				// 玩家开火
			{
				firerate = clock();
				create_p_b();
			}
			if (GetAsyncKeyState(VK_DOWN) & 0x8000)
				// 玩家移动
			{
				if (player_pos[1] <= 452)
					if (leftshift)
						player_pos[1] += 2;
					else
						player_pos[1] += 5;
			}
			if (GetAsyncKeyState(VK_LEFT) & 0x8000)
				// 玩家移动
			{
				if (player_pos[0] >= 30)
					if (leftshift)
						player_pos[0] -= 2;
					else
						player_pos[0] -= 5;
			}
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
				// 玩家移动
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


			if (clock() - bleed_p >= BLEED_TIME)		// 受伤时间结束后关闭受伤闪烁效果
			{
				isBleeding_p = false;
			}

			if (clock() - bleed_e >= BLEED_TIME)		// 受伤时间结束后关闭受伤闪烁效果
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
		settextstyle(35, 0, L"黑体");
		outtextxy(150, 200, L"你打败了boss!你赢了！！");
	}
	else
	{
		settextcolor(RGB(254, 0, 0));
		settextstyle(35, 0, L"黑体");
		outtextxy(140, 200, L"你被boss打败了！");
	}
	FlushBatchDraw();
	Sleep(5000);
	EndBatchDraw();
	return 0;
}






void hp_bar()
{
	setlinecolor(RGB(255, 255, 255));
	line(0, 481, 640, 481);										// 一条分割线
	settextstyle(20, 0, L"黑体");
	outtextxy(10, 485, L"BOSS的生命值:");
	outtextxy(10, 520, L"玩家的生命值:");
	setfillcolor(RGB(0, 255, 1));
	setlinecolor(WHITE);
	rectangle(160, 515, 560, 540);								// 血条外框
	setfillcolor(RGB(0, 255, 1));
	setlinecolor(RGB(255, 255, 255));
	if (player_health > 0)
		fillrectangle(160, 515, 160 + player_health * 4, 540);	// 玩家血条
	setlinecolor(WHITE);
	rectangle(160, 485, 560, 510);								// 敌人血条外框
	setfillcolor(RGB(230, 0, 1));
	setlinecolor(RGB(255, 255, 255));
	if (enemy_health > 0)
		fillrectangle(160, 485, 160 + enemy_health * 4, 510);	// 敌人血条

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
	static bool angle_v;		// 控制敌机的竖直移动方向，true 为向上，到边缘就换向
	static bool angle_h;		// 控制敌机的水平移动方向，true 为向左，到边缘就换向
	static clock_t interval;	// 定时随机换向

	if (clock() - interval >= 2000)
	{
		interval = clock();
		if (rand() % 2)			// 一般的概率换向
			angle_v = !angle_v;
		if (rand() % 2)
			angle_h = !angle_h;
	}

	if (angle_v == true)		// 到了地图边缘就调头
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
	int n_b_l = number_lines;		// 待处理线条数目
	for (int i = 0; i < MAX_LINES && (n_b_l > 0); ++i)
	{
		if (line_slots[i] == true)
		{
			if (background_line[i][0] + background_line[i][2] <= 0)		// 说明线条出了屏幕
			{
				--number_lines;
				line_slots[i] = false;
			}
			else
			{
				background_line[i][0] -= 10;				// 线条移动
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
			background_line[i][0] = 640;				// 线条出现于屏幕最右边
			background_line[i][1] = rand() % 480;		// 线条高度随机
			background_line[i][2] = 10 + rand() % 50;	// 线条长度随机在 10-50 像素之间

			break;
		}
	}
	return 0;
}

int create_p_b()
{
	if (number_p_b > MAX_PLAYER_BULLETS)			// 空间不够
		return -1;
	for (int i = 0; i < MAX_PLAYER_BULLETS; ++i)	// 搜索 slots，寻找空位
	{
		if (p_b_slots[i] == false)
		{
			p_b_slots[i] = true;
			player_bullet[i][0] = player_pos[0] + 45;
			player_bullet[i][1] = player_pos[1];	// 创建子弹
			++number_p_b;
			break;
		}
	}
	return 0;
}

int create_e_b()
{
	if (number_e_b > MAX_ENEMY_BULLETS)				// 空间不够
		return -1;
	for (int i = 0; i < MAX_ENEMY_BULLETS; ++i)		// 搜索 slots，寻找空位
	{
		if (e_b_slots[i] == false)
		{
			e_b_slots[i] = true;
			enemy_bullet[i][0] = enemy_pos[0] - 45;
			enemy_bullet[i][1] = enemy_pos[1];		// 创建子弹
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