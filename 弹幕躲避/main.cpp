///////////////////////////////////////////////////
// 程序名称：弹幕躲避游戏
// 编译环境：Visual Studio 2017/2019，EasyX_20200520(beta)
// 作　　者：tonyddg <799052200@qq.com>
// 最后修改：2020-6-14
// https://codebus.cn/tonyddg/a/danmugame
//

#include"define.h"

int main()
{
	int uicmd;

	init();

	while (true)
	{
		datainit();

		uicmd = initui();

		if (uicmd == -1)break;

		if (uicmd > 0)Gaming(uicmd - 1);
		else Gaming(-1);
	}

	gend();
}

bool ChangeDisSize(int cx, int cy)
{
	LPDIRECTDRAW DXW;
	HRESULT ddrval;
	ddrval = DirectDrawCreate(NULL, &DXW, NULL);
	if (DD_OK != DD_OK) return FALSE;//创建DirectDraw对象失败返回
	ddrval = DXW->SetCooperativeLevel(NULL, DDSCL_NORMAL);
	if (DD_OK != DD_OK) return FALSE;// 取得窗口模式失败返回
	ddrval = DXW->SetDisplayMode(cx, cy, 16);
	if (DD_OK != DD_OK) return FALSE;//设置显示模式失败返回
	return true;
}

//生成一个向量
inline vec mv(double x, double y)
{
	vec obj;
	obj.x = x;
	obj.y = y;
	return obj;
}

vec operator+ (const vec& a, const vec& b)
{
	return mv(a.x + b.x, a.y + b.y);
}
vec operator- (const vec& a, const vec& b)
{
	return mv(a.x - b.x, a.y - b.y);
}
//数量积
vec operator* (const vec& a, double b)
{
	return mv(a.x * b, a.y * b);
}
//数量积
vec operator* (double b, const vec& a)
{
	return mv(a.x * b, a.y * b);
}
double operator* (const vec& a, const vec& b)
{
	return a.x * b.x + a.y * b.y;
}
//向量旋转(逆时针)(第二个参数必须为单位向量) 原理 欧拉变换  dl[np].ra = mv(cos(Pi / 20), sin(Pi / 20)); 循环作用一个速度向量40次 相当于画一个正40边型
vec operator% (const vec& a, const vec& b)
{
	return mv(a.x * b.x - a.y * b.y, a.y * b.x + a.x * b.y);
}
//取向量长度
double operator! (const vec& a)
{
	return sqrt(a.x * a.x + a.y * a.y);
}
//生成一个区间内的随机数 整数
inline int rangrand(int L, int R)
{
	return rand() % (R - L) + L;
}
//生成随机角度
inline double randangle()
{
	return 2 * Pi * ((double)rangrand(1, 360) / (double)360);
}

vec::vec()
{
	x = 0;
	y = 0;
}

void DrawDanmu(danmu obj)
{
	setfillcolor(obj.clr);
	solidcircle((int)obj.pos.x, (int)obj.pos.y, obj.r);
}

void WriteKbinf(int left, int top, int right, int bottom)
{
	int lx = min(right, w);
	int ly = min(bottom, h);
	for (int ix = max(left, 0); ix < lx; ix++)
	{
		for (int iy = max(top, 0); iy < ly; iy++)
		{
			KickBox[ix][iy] = true;
		}
	}
}

void CleanKb()
{
	for (int ix = 0; ix < w; ix++)
	{
		for (int iy = 0; iy < h; iy++)
		{
			KickBox[ix][iy] = false;
		}
	}
}

danmu::danmu()
{
	id = 0;//							信息
	r = 0;//							半径
	type = 0;//							类型
	clr = 0;//							颜色
	alv = false;
}

void NormalDeal(danmu &obj)
{
	obj.pos = obj.pos + obj.sped;

	if (!PassCheck(obj.pos))
	{
		obj.alv = false;
		return;
	}

	if (!is_replay)WriteKbinf((int)obj.pos.x - obj.r / 2, (int)obj.pos.y - obj.r / 2, (int)obj.pos.x + obj.r / 2, (int)obj.pos.y + obj.r / 2);
}

bool PassCheck(vec pos)
{
	return NumCheck(pos.y, 0, h) && NumCheck(pos.x, 0, w);
}

int FindBlank()
{
	for (int i = 1; i < maxdanmu; i++)
	{
		if (!dlst[i].alv)return i;
	}
	return 0;
}

int ListDeal()
{
	int count = 0;

	for (int i = 1; i < maxdanmu; i++)
	{
		if (dlst[i].alv)
		{
			count++;
			NormalDeal(dlst[i]);
		}
	}

	return count;
}

void Gaming(int practicemode)
{
	srand(123);

	bool gaming = true;

	double debug_nfps = 0;
	int debug_told = 0;
	int debug_rfps = 0;
	int nlevel = 0;
	int nstage = 0;

	bool is_getbonus = false;

	int sfps = 0;

	int timedis = 0;
	int timerest = 0;
	int runtime = 0;

	int timedis2 = 0;

	IMAGE gamesence;
	Resize(&gamesence, w, h);

	timer debug_timer;
	timer game_timer;

	keycheck ke(VK_ESCAPE);
	keycheck kx('X');

	bool is_useboom = false;

	bool is_updata = false;

	bool levelchange = true;

	char st = 0;

	ifstream file;

	rpyuni tmprpy;
	infunio tmprpyinf;

	if (is_replay)
	{
		file.open(rppath, ios::binary | ios::in);

		file.read((char*)&tmprpyinf, sizeof(tmprpyinf));

		if (tmprpyinf.checks != tmprpyinf.setcheck())
		{
			MessageBox(NULL, _T("回放错误!"), _T("错误"), MB_OK);
			gaming = false;
			return;
		}
	}
	else
	{
		rpyinftop = 0;
	}

	if (practicemode != -1)
	{
		nstage = practicemode;
	}

	while (gaming)
	{
		timedis = game_timer.read();

		if (game_timer.check(FPSN))
		{
			runtime = (timedis + timerest) / FPSN;
			timerest = (timedis + timerest) % FPSN;

			//逻辑部分 时间不够则循环补帧
			for (int i = 0; i < runtime; i++)
			{
				sfps++;

				CleanKb();

				debug_told = ListDeal();
				nlevel = ScriptChoose(sfps, nstage);

				if (nlevel >= 0)
				{
					if (is_replay)
					{
						file.read((char*)&tmprpy, sizeof(tmprpy));
						player_pos.x = tmprpy.x;
						player_pos.y = tmprpy.y;
						st = tmprpy.stat;
						if (st == 'c')
						{
							ScriptChoose(sfps, nstage, true);
						}
						else if (st == 'x')
						{
							CleanDanmu();
							rbmtm--;
							is_useboom = true;
						}
						else if (st == 's')
						{
							playerspeed = plyspeed_slow;
						}
						else if (st == 'n')
						{
							playerspeed = plyspeed_fast;
						}
						else if (st == '#')
						{
							gaming = false;
							dhui(nstage, is_replay);
						}
						else if (file.eof() && st != '#')
						{
							MessageBox(NULL, _T("回放错误!"), _T("错误"), MB_OK);
							gaming = false;
							return;
						}
					}
					else
					{
						playerdeal();
						tmprpy.x = player_pos.x;
						tmprpy.y = player_pos.y;
						if (kx.getstat() == 1 && rbmtm > 0)
						{
							CleanDanmu();
							rbmtm--;
							is_useboom = true;
						}
						if (is_useboom)
						{
							is_useboom = false;
							tmprpy.stat = 'x';
						}
						else if (playerspeed == plyspeed_slow)
						{
							tmprpy.stat = 's';
						}
						else tmprpy.stat = 'n';

						tmprpylist[rpyinftop] = tmprpy;
						rpyinftop++;
					}
					score += (int)(pow(nlevel, 1.5) * ((double)nstage * 0.2 + 1));
					levelchange = true;
				}
				else if (levelchange)
				{
					levelchange = false;
					is_getbonus = false;
					if (!is_replay)
					{
						tmprpy.x = player_pos.x;
						tmprpy.y = player_pos.y;
						if (practicemode != -1 || nstage == -1)
						{
							tmprpy.stat = '#';
						}
						else tmprpy.stat = 'c';

						tmprpylist[rpyinftop] = tmprpy;
						rpyinftop++;
					}
				}

				if (nlevel >= bonuslevel && !is_getbonus)//		达到第六级别奖励一点使用技能的机会
				{
					rbmtm++;
					is_getbonus = true;
				}

				if (practicemode != -1 && nlevel < -2)
				{
					gaming = false;
					dhui(nstage, is_replay);
				}

				if (nstage == -1)
				{
					gaming = false;
					dhui(nstage, is_replay);
				}
			}
			is_updata = true;
		}

		if (!is_updata && is_limit) Sleep(1);
		else
		{
			//暂停界面
			if (ke.getstat() == 1)
			{
				game_timer.stop();
				debug_timer.stop();

				if (stui() == 1)gaming = false;

				game_timer.start();
				debug_timer.start();
			}

			//绘图部分
			SetWorkingImage(&gamesence);
			setbkcolor(BLACK);
			cleardevice();

			for (int i = 1; i < maxdanmu; i++)
			{
				if (dlst[i].alv)
				{
					DrawDanmu(dlst[i]);
				}
			}

			DrawPlayer();

			timedis2 = debug_timer.read();
			debug_rfps++;
			if (debug_timer.check(1000))
			{
				debug_nfps = (double)(debug_rfps) / (double)(timedis2) * 1000;
				debug_rfps = 0;
			}

			SetWorkingImage();
			setbkcolor(LIGHTBLUE);

			cleardevice();
			putimage(40, 40, &gamesence);

			ShowDebuginf(debug_nfps, debug_told, nlevel, nstage);

			FlushBatchDraw();//								更新图像

			is_updata = false;
		}
	}

	if (is_replay)
	{
		file.close();
	}
	else
	{
		tmprpy.x = player_pos.x;
		tmprpy.y = player_pos.y;
		tmprpy.stat = '#';

		tmprpylist[rpyinftop] = tmprpy;
		rpyinftop++;
	}
}

void init()
{
	initgraph(640, 480);

	setbkmode(TRANSPARENT);
	setbkcolor(LIGHTBLUE);
	setfillcolor(LIGHTGREEN);
	setlinecolor(BLACK);
	settextcolor(WHITE);

	is_fullscran = GetPrivateProfileInt(_T("display"), _T("fullscran"), 0, (_T(".\\") + (wstring)inipath).c_str());
	is_limit = GetPrivateProfileInt(_T("display"), _T("limit"), 1, (_T(".\\") + (wstring)inipath).c_str());
	GetPrivateProfileString(_T("set"), _T("path"), nulpath, datapath, 40, (_T(".\\") + (wstring)inipath).c_str());

	GetPrivateProfileString(_T("intro"), _T("text"), _T(""), intrtext, 2000, (_T(".\\") + (wstring)inipath).c_str());

	int i = 0;
	while (intrtext[i] != _T('\0'))
	{
		if (intrtext[i] == _T('&'))intrtext[i] = _T('\n');
		i++;
	}

	if (is_fullscran)FullScreen(true);

	BeginBatchDraw();
}

void datainit()
{
	cleardevice();
	CleanKb();

	player_pos = mv(200 + player_r / 2, 390);
	nfps = 0;
	deadwait = 0;
	rbmtm = boomtime;
	score = 0;

	is_replay = false;

	for (int i = 0; i < maxdanmu; i++)
	{
		dlst[i].alv = false;
	}
}

void gend()
{
	EndBatchDraw();

	closegraph();
}

int GetCommand()
{
	int c = 0;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)							c |= CMD_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)						c |= CMD_RIGHT;
	if (GetAsyncKeyState(VK_UP) & 0x8000)							c |= CMD_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)							c |= CMD_DOWN;

	if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)		                playerspeed = plyspeed_slow;
	else playerspeed = plyspeed_fast;

	return c;
}

void DispatchCommand(int _cmd)
{
	if (_cmd & CMD_UP)			OnUp();
	if (_cmd & CMD_DOWN)		OnDown();
	if (_cmd & CMD_LEFT)		OnLeft();
	if (_cmd & CMD_RIGHT)		OnRight();
}

void OnUp()
{
	player_pos.y = max(player_pos.y - playerspeed, player_r);
}
void OnDown()
{
	player_pos.y = min(player_pos.y + playerspeed, h - player_r);
}
void OnRight()
{
	player_pos.x = min(player_pos.x + playerspeed, w - player_r);
}
void OnLeft()
{
	player_pos.x = max(player_pos.x - playerspeed, player_r);
}

bool CheckKbinf(int left, int top, int right, int bottom)
{
	int lx = min(right, w);
	int ly = min(bottom, h);
	for (int ix = max(left, 0); ix < lx; ix++)
	{
		for (int iy = max(top, 0); iy < ly; iy++)
		{
			if (KickBox[ix][iy])return false;
		}
	}
	return true;
}

void playerdeal()
{
	DispatchCommand(GetCommand());
}

int GameScript_0(int sfps)//普通
{
	int level = sfps / 600 + 1;//							每10秒难度上升一级
	int p;

	if (sfps % (60 / level + 10) == 0)
	{
		for (int i = 0; i < level * 2; i++)
		{
			p = FindBlank();
			dlst[p].alv = true;
			dlst[p].clr = RED;
			dlst[p].pos = mv((double)rangrand(10, w - 10), (double)3);
			dlst[p].sped = mv(0, (double)(1 + level / 3 + (double)rangrand(0, 5) * 0.3));
			dlst[p].r = 3;
		}
	}
	return level;
}

int GameScript_2(int sfps)//波
{
	int level = sfps / 600 + 1;//							每10秒难度上升一级
	int p;
	static double angle = 0;
	double bp;

	if (sfps % 5 == 0)
	{
		bp = 40 * cos(agl_to_rec(angle));
		for (int i = 0; i <= 420; i += 70)
		{
			if (percent_true(50 + 2 * level))
			{
				p = FindBlank();
				dlst[p].alv = true;
				dlst[p].clr = RED;
				dlst[p].pos = mv(0, i + bp);
				dlst[p].sped = mv((double)(1.5 + level * 0.1), 0);
				dlst[p].r = 3;
			}
		}
		angle += 20;
	}
	return level;
}

int GameScript_1(int sfps)//圈
{
	static vec basesa = mv(cos(Pi / 10), sin(Pi / 10));
	static int count = 0;

	int level = sfps / 600 + 1;//							每10秒难度上升一级
	int p;

	count++;
	count %= 18;
	double srec = agl_to_rec((double)count);

	vec tmpspeed = mv(cos(srec), sin(srec));

	if (sfps % (40 - level * 3) == 0)
	{
		for (int i = 0; i < 20; i++)
		{
			p = FindBlank();
			dlst[p].alv = true;
			dlst[p].clr = RED;
			dlst[p].pos = mv(201.5, 201.5);
			dlst[p].sped = tmpspeed * (double)(1 + level * 0.1);
			dlst[p].r = 3;

			tmpspeed = tmpspeed % basesa;
		}
	}
	return level;
}

int GameScript_3(int sfps)//弧
{
	int level = sfps / 600 + 1;//							每10秒难度上升一级
	double baseagl = randangle();

	int p1, p2, p3;
	vec opv1, opv2, opv3;

	int r = rangrand(5, 10);//								弧的半径
	double ofsa = (double)rangrand(30, 100) / (double)500;
	double speed;

	if (sfps % (40 - level * 3) == 0)
	{
		for (int i = 0; i < 12; i++)
		{
			speed = 1 + rangrand(0, 5) * 0.1;

			opv1 = rec_to_vec(baseagl);
			opv2 = rec_to_vec(baseagl + ofsa);
			opv3 = rec_to_vec(baseagl - ofsa);

			p1 = FindBlank();
			dlst[p1].alv = true;
			dlst[p1].clr = WHITE;
			dlst[p1].pos = mv(201.5, 201.5) + opv1 * r;
			dlst[p1].sped = opv1 * speed;
			dlst[p1].r = 3;

			p2 = FindBlank();
			dlst[p2].alv = true;
			dlst[p2].clr = WHITE;
			dlst[p2].pos = mv(201.5, 201.5) + opv2 * r;
			dlst[p2].sped = opv2 * speed;
			dlst[p2].r = 3;

			p3 = FindBlank();
			dlst[p3].alv = true;
			dlst[p3].clr = WHITE;
			dlst[p3].pos = mv(201.5, 201.5) + opv3 * r;
			dlst[p3].sped = opv3 * speed;
			dlst[p3].r = 3;

			baseagl += Pi / (double)6;
		}
	}

	return level;
}

void ShowDebuginf(double fps, int told, int level, int stage)
{
	outtextxy(480, 40, (_T("fps: ") + to_wstring(fps)).c_str());
	outtextxy(480, 55, (_T("told: ") + to_wstring(told)).c_str());
	if (level > 0)outtextxy(480, 70, (_T("level: ") + to_wstring(level)).c_str());
	else outtextxy(480, 70, (_T("relax: ") + to_wstring(-level - 2)).c_str());
	outtextxy(480, 85, (_T("stage: ") + to_wstring(stage + 1)).c_str());
	outtextxy(480, 100, (_T("boom: ") + to_wstring(rbmtm)).c_str());
	outtextxy(480, 115, (_T("score: ") + to_wstring(score)).c_str());
}

inline double agl_to_rec(double agl)
{
	return agl / 360 * 2 * Pi;
}

inline bool percent_true(int rent)
{
	return (rand() % 100) < rent;
}

void DrawPlayer()
{
	setfillcolor(player_clr);
	solidcircle((int)player_pos.x, (int)player_pos.y, player_r);
	if (playerspeed == plyspeed_slow)
	{
		setfillcolor(WHITE);
		setlinecolor(BLACK);
		fillcircle((int)player_pos.x, (int)player_pos.y, (int)pd_r);
	}
}

int ScriptChoose(int sfps, int &nstage, bool into)
{
	int level = 0;

	if (is_replay)
	{
		if (into)
		{
			nstage++;
			nfps = sfps;
			deadwait = deadrelax;
			CleanDanmu();
			player_pos = mv(200 + player_r / 2, 390);
			if (nstage == ttlstage)
			{
				return -2;//									表示游戏结束
			}
		}
	}
	else if (!CheckKbinf((int)player_pos.x - player_r / 2, (int)player_pos.y - player_r / 2, (int)player_pos.x + player_r / 2, (int)player_pos.y + player_r / 2))
	{
		nstage++;
		nfps = sfps;
		deadwait = deadrelax;
		CleanDanmu();
		playerspeed = plyspeed_fast;
		player_pos = mv(200 + player_r / 2, 390);
		if (nstage == ttlstage)
		{
			return -2;//										表示游戏结束
		}
	}

	if (deadwait != 0)
	{
		deadwait--;
		if (nstage != ttlstage)return -deadwait - 2;//			表示休息模式 返回值为-deadwait-2
		else
		{
			nstage = -1;
			return 0;
		}
	}

	if (nstage == 0)
	{
		level = GameScript_0(sfps - nfps);
	}
	else if (nstage == 1)
	{
		level = GameScript_1(sfps - nfps);
	}
	else if (nstage == 2)
	{
		level = GameScript_2(sfps - nfps);
	}
	else if (nstage == 3)
	{
		level = GameScript_3(sfps - nfps);
	}
	else if (nstage == 4)
	{
		level = GameScript_4(sfps - nfps);
	}
	else
	{
		nstage = -1;
		return 0;
	}

	return level;
}

void CleanDanmu()
{
	for (int i = 0; i < maxdanmu; i++)
	{
		dlst[i].alv = false;
	}
}

bool FullScreen(bool is_fullscran)
{
	static int ow = GetSystemMetrics(SM_CXSCREEN);
	static int oh = GetSystemMetrics(SM_CYSCREEN);
	HWND hWnd = GetHWnd();

	LONG l_WinStyle = GetWindowLong(hWnd, GWL_STYLE);   /* 获取窗口信息 */

	if (!is_fullscran)
	{
		ChangeDisSize(ow, oh);

		SetWindowLong(hWnd, GWL_STYLE, (l_WinStyle | WS_POPUP | WS_MAXIMIZE) | WS_CAPTION | WS_THICKFRAME | WS_BORDER);
		SetWindowPos(hWnd, HWND_TOPMOST, 100, 100, 0, 0, SWP_NOOWNERZORDER | SWP_SHOWWINDOW | SWP_NOSIZE);

	}
	else
	{
		ChangeDisSize(getwidth(), getheight());

		SetWindowLong(hWnd, GWL_STYLE, (l_WinStyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOOWNERZORDER | SWP_SHOWWINDOW | SWP_NOSIZE);

		FlushBatchDraw();
	}

	return is_fullscran;
}

int initui()
{
	settextcolor(BLACK);

	RECT r[6] = { {30,397,110,427},{130,397,210,427},{230,397,310,427},{330,397,410,427},{430,397,510,427},{530,397,610,427} };
	const wstring str[6] = { _T("开始游戏"),_T("排行榜"),_T("观看回放"),_T("画面设置"),_T("说明"),_T("退出游戏") };

	int iic = 1;
	int ans = -2;

	IMAGE bk;

	keycheck kl(VK_LEFT);
	keycheck kr(VK_RIGHT);
	keycheck ke(VK_RETURN);

	wstringstream ss;

	loadimage(&bk, _T("Title.jpg"));

	while (ans == -2)
	{
		if (kl.getstat() == 1)
		{
			iic--;
			iic = max(iic, 1);
		}

		if (kr.getstat() == 1)
		{
			iic++;
			iic = min(iic, 6);
		}

		if (ke.getstat() == 1)
		{
			if (iic == 6)
			{
				ans = -1;
			}
			else if (iic == 1)
			{
				ans = clui();
				if (ans == -1)ans = -2;
			}
			else if (iic == 5)
			{
				itui();
			}
			else if (iic == 2)
			{
				rkui();
			}
			else if (iic == 4)
			{
				opui();
			}
			else if (iic == 3)
			{
				ans = ryui();
				if (ans == -1)ans = -2;
			}
		}

		cleardevice();

		putimage(0, 0, &bk);

		for (int i = 0; i < 6; i++)
		{
			drawtext(str[i].c_str(), r + i, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}

		rectangle(r[iic - 1].left, r[iic - 1].top, r[iic - 1].right, r[iic - 1].bottom);

		FlushBatchDraw();

		Sleep(5);

	}

	settextcolor(WHITE);
	return ans;
}

int clui()
{
	int cic = 0;

	keycheck kl(VK_LEFT);
	keycheck kr(VK_RIGHT);
	keycheck ke(VK_RETURN);
	keycheck kes(VK_ESCAPE);

	RECT r1 = { 300, 400, 339, 430 };
	RECT r2 = { 100, 200, 539, 350 };

	const wstring intr[6] = { _T("| 标准模式 >"),_T("< 第一关,简单的直线弹幕 >") ,_T("< 第二关,简单的圆形弹幕 >") ,_T("< 第三关,简单的正弦形弹幕 >") ,_T("< 第四关,简单的弧形弹幕 >"),_T("< 第五关,简单的自机狙 |") };

	bool is_exit = true;

	settextcolor(WHITE);

	while (is_exit)
	{
		cleardevice();

		drawtext(intr[cic].c_str(), &r2, DT_CENTER | DT_VCENTER | DT_LEFT | DT_WORDBREAK);
		drawtext(to_wstring(cic).c_str(), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		FlushBatchDraw();

		Sleep(5);

		if (kl.getstat() == 1)
		{
			cic--;
			cic = max(cic, 0);
		}

		if (kr.getstat() == 1)
		{
			cic++;
			cic = min(cic, ttlstage);
		}

		if (ke.getstat() == 1)
		{
			is_exit = false;
		}

		if (kes.getstat() == 1)
		{
			is_exit = false;
			cic = -1;
		}
	}

	settextcolor(BLACK);

	return cic;
}

inline vec rec_to_vec(double rec)
{
	return mv(cos(rec), sin(rec));
}

int stui()//							1继续游戏 2退出游戏
{
	settextcolor(BLACK);

	int iic = 1;
	int ans = -2;

	const wstring intr[3] = { _T("游戏暂停"), _T("继续游戏") ,_T("回到菜单") };

	RECT r[3] = { { 100, 200, 200, 220 } ,{ 100, 270, 200, 290 } ,{ 100, 340, 200, 360 } };

	IMAGE bk;

	keycheck ku(VK_UP);
	keycheck kd(VK_DOWN);
	keycheck ke(VK_RETURN);
	keycheck kes(VK_ESCAPE);

	while (ans == -2)
	{
		if (ku.getstat() == 1)
		{
			iic--;
			iic = max(iic, 1);
		}

		if (kd.getstat() == 1)
		{
			iic++;
			iic = min(iic, 2);
		}

		if (ke.getstat() == 1)
		{
			if (iic == 1)
			{
				ans = 0;
			}
			else if (iic == 2)
			{
				ans = 1;
			}
		}

		if (kes.getstat() == 1)
		{
			ans = 0;
		}

		cleardevice();

		for (int i = 1; i < 3; i++)solidrectangle(100, 200 + i * 70, 200, 220 + i * 70);
		rectangle(100, 200 + iic * 70, 200, 220 + iic * 70);

		for (int i = 0; i < 3; i++)drawtext(intr[i].c_str(), r + i, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		FlushBatchDraw();

		Sleep(5);
	}

	settextcolor(WHITE);

	return ans;

}

void dhui(int stage, bool is_replay)
{
	cleardevice();

	RECT r1 = { 40, 40, 600, 300 };
	RECT r2 = { 40, 300, 600, 400 };
	RECT r[3] = { { 280,400,360,420 } ,{ 280,425,360,445 } ,{ 280,450,360,470 } };
	const wstring str1 = _T("满身疮痍");
	const wstring str[3] = { _T("记录回放与分数"),_T("记录分数"),_T("回到主菜单") };
	keycheck ku(VK_UP);
	keycheck kd(VK_DOWN);
	keycheck ke(VK_RETURN);
	keycheck kes(VK_ESCAPE);
	infunio tmp;
	int iic = 1;
	TCHAR tmpstr[20];
	ofstream rpy;
	const wstring strrpy = _T(".rpy");

	if (!is_replay)
	{
		while (true)
		{
			if (ku.getstat() == 1)
			{
				iic--;
				iic = max(iic, 1);
			}

			if (kd.getstat() == 1)
			{
				iic++;
				iic = min(iic, 3);
			}

			if (ke.getstat() == 1)
			{
				if (iic == 3)
				{
					return;
				}
				else if (iic == 2)
				{
					InputBox(tmp.name, 10, _T("请输入你的名字"), _T("记录"));
					tmp.score = score;
					if (stage == -1)tmp.type = 1024;
					else tmp.type = stage;
					tmp.t = time(NULL);

					OutputUnio(tmp, datapath);

					return;
				}
				else if (iic == 1)
				{
					InputBox(tmp.name, 10, _T("请输入你的名字"), _T("记录"));
					tmp.score = score;
					if (stage == -1)tmp.type = 1024;
					else tmp.type = stage;
					tmp.t = time(NULL);

					OutputUnio(tmp, datapath);

					InputBox(tmpstr, 10, _T("请输入回放的名字"), _T("记录"));

					rpy.open((tmpstr + strrpy).c_str(), ios::binary);

					rpy.write((char*)&tmp, sizeof(tmp));

					for (int i = 0; i < rpyinftop; i++)
					{
						rpy.write((char*)&tmprpylist[i], sizeof(tmprpylist[i]));
					}

					rpy.close();

					return;
				}
			}

			cleardevice();

			settextcolor(WHITE);
			drawtext(str1.c_str(), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			drawtext((_T("得分:") + to_wstring(score)).c_str(), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			settextcolor(BLACK);
			for (int i = 0; i < 3; i++)
			{
				solidrectangle(r[i].left, r[i].top, r[i].right, r[i].bottom);
				drawtext(str[i].c_str(), r + i, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}

			rectangle(r[iic - 1].left, r[iic - 1].top, r[iic - 1].right, r[iic - 1].bottom);

			FlushBatchDraw();

			Sleep(5);
		}
	}
	else
	{
		cleardevice();

		settextcolor(WHITE);

		drawtext(str1.c_str(), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext((_T("得分:") + to_wstring(score)).c_str(), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		settextcolor(BLACK);
		fillrectangle(r[2].left, r[2].top, r[2].right, r[2].bottom);
		drawtext(str[2].c_str(), r + 2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		FlushBatchDraw();

		while (ke.getstat() != 1)Sleep(1);
	}
}

void itui()
{
	RECT r = { 10,10,630,470 };
	keycheck ke(VK_ESCAPE);

	int i = 0;

	settextcolor(WHITE);

	cleardevice();

	drawtext(intrtext, &r, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_EXPANDTABS);

	FlushBatchDraw();

	while (ke.getstat() != 1)Sleep(1);

	settextcolor(BLACK);
}

void rkui()
{
	wstringstream ss;

	InputMess(datapath);

	int cic = 0;

	keycheck kl(VK_LEFT);
	keycheck kr(VK_RIGHT);
	keycheck kd(VK_DOWN);
	keycheck ku(VK_UP);
	keycheck kes(VK_ESCAPE);

	RECT r1 = { 200, 50, 440, 100 };
	RECT r2 = { 100, 110, 539, 470 };

	const wstring intr[6] = { _T("| 标准模式 >"),_T("< 第一关 >") ,_T("< 第二关 >") ,_T("< 第三关 >") ,_T("< 第四关 >"),_T("< 第五关 |") };

	bool is_exit = true;

	int rkp = 0;

	settextcolor(WHITE);

	MakeTypelist(1024);
	MakeTable(&ss);

	while (is_exit)
	{
		cleardevice();

		drawtext(intr[cic].c_str(), &r1, DT_CENTER | DT_VCENTER | DT_LEFT | DT_WORDBREAK);
		drawtext(ss.str().c_str(), &r2, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_EXPANDTABS);

		FlushBatchDraw();

		Sleep(5);

		if (kl.getstat() == 1)
		{
			cic--;
			cic = max(cic, 0);

			if (cic != 0)MakeTypelist(cic);
			else MakeTypelist(1024);
			MakeTable(&ss);

			rkp = 0;
		}

		if (kr.getstat() == 1)
		{
			cic++;
			cic = min(cic, ttlstage);

			if (cic != 0)MakeTypelist(cic);
			else MakeTypelist(1024);
			MakeTable(&ss);

			rkp = 0;
		}

		if (kd.getstat() == 1 && ScoreListtop > 1)
		{
			rkp++;
			rkp = min(rkp, ScoreListtop - 1);

			MakeTable(&ss, rkp);
		}

		if (ku.getstat() == 1 && ScoreListtop > 1)
		{
			rkp--;
			rkp = max(rkp, 0);

			MakeTable(&ss, rkp);
		}

		if (kes.getstat() == 1)
		{
			is_exit = false;
		}
	}

	settextcolor(BLACK);
}

bool comp(scoreset a, scoreset b)//sort的比较函数
{
	return a.score > b.score;
}

inline bool checheck(infunio* obj, DWORD che)//检测检验码
{
	return obj->setcheck() == che;
}

void CharTran(TCHAR obj[], const TCHAR frm[])//复制字符串
{
	int i = -1;
	do
	{
		i++;
		obj[i] = frm[i];
	} while (frm[i] != '\0');
}

bool CharEqual(const TCHAR obj[], const TCHAR frm[])//比较字符串
{
	int t = lstrlen(obj);
	if (t == lstrlen(frm))
	{
		for (int i = 0; i < t; i++)
		{
			if (obj[i] != frm[i])return false;
		}
	}
	else return false;
	return true;
}

void OutputUnio(infunio obj, const TCHAR* path)//				输出
{
	ofstream file;

	if (CharEqual(path, nulpath))
	{
		file.open(inipath, ios::app | ios::binary);
	}
	else
	{
		file.open(path, ios::app | ios::binary);
	}

	file.write((char*)(&obj), sizeof(obj));

	file.close();
}

int InputMess(const TCHAR* path)//								输入
{
	ifstream file;
	char c = 0;

	if (CharEqual(path , nulpath))
	{
		file.open(inipath, ios::binary);
		while (c != '#')
		{
			file.get(c);
		}
	}
	else
	{
		file.open(path, ios::binary);
	}

	int wrongdat = 0;

	ListTop = 0;

	while (!file.eof())
	{
		file.read((char *)(InfList + ListTop), sizeof(InfList[ListTop]));
		if (InfList[ListTop].checks == InfList[ListTop].setcheck())ListTop++;
		else
		{
			wrongdat++;
		}
		if (ListTop >= LISTMAX)return wrongdat;
	}

	file.close();

	return wrongdat;
}

void MakeTypelist(DWORD mt)//								生成特定类别的序列
{
	ScoreListtop = 0;
	for (int i = 0; i <= ListTop; i++)
	{
		if (InfList[i].type == mt || mt == 0)
		{
			CharTran(ScrList[ScoreListtop].name, InfList[i].name);
			ScrList[ScoreListtop].score = InfList[i].score;
			ScrList[ScoreListtop].t = InfList[i].t;
			ScoreListtop++;

			if (ScoreListtop >= LISTMAX)return;
		}
	}
	sort(ScrList, ScrList + ScoreListtop, comp);
}

void MakeTable(wstringstream * ss, int be)//				显示列表
{
	ss->str(_T(""));
	ss->clear();

	TCHAR tmp[100];
	int end = 0;
	bool rest = true;

	*ss << _T("位次") << _T('\t') << _T("姓名") << _T("\t") << _T("分数") << _T("\t\t") << _T("时间") << _T("\n");

	if (ScoreListtop > be + 10)
	{
		end = be + 10;
	}
	else
	{
		end = ScoreListtop;
		rest = false;
	}

	for (int i = be; i < end; i++)
	{
		_wctime_s(tmp, 50, &ScrList[i].t);
		*ss << i + 1 << _T("\t") << ScrList[i].name << _T("\t") << ScrList[i].score << _T("\t\t") << tmp << _T("\n");
	}
	if (rest)*ss << _T("v");
}

void opui()
{
	settextcolor(WHITE);

	int iic = 1;
	int ans = -2;

	const wstring intr[3] = { _T("图像设置"), _T("全屏: ") ,_T("垂直同步: ") };

	RECT r[3] = { { 270, 200, 370, 220 } ,{ 270, 270, 370, 290 } ,{ 270, 340, 370, 360 } };

	IMAGE bk;

	keycheck ku(VK_UP);
	keycheck kd(VK_DOWN);
	keycheck ke(VK_RETURN);
	keycheck kes(VK_ESCAPE);

	while (true)
	{
		if (ku.getstat() == 1)
		{
			iic--;
			iic = max(iic, 1);
		}

		if (kd.getstat() == 1)
		{
			iic++;
			iic = min(iic, 2);
		}

		if (ke.getstat() == 1)
		{
			if (iic == 1)
			{
				if (is_fullscran)
				{
					FullScreen(false);
					is_fullscran = false;
				}
				else
				{
					FullScreen(true);
					is_fullscran = true;
				}
			}
			else if (iic == 2)
			{
				if (is_limit)
				{
					is_limit = false;
				}
				else
				{
					is_limit = true;
				}
			}
		}

		if (kes.getstat() == 1)
		{
			settextcolor(BLACK);

			if (is_fullscran)WritePrivateProfileString(_T("display"), _T("fullscran"), _T("1"), (_T(".\\") + (wstring)inipath).c_str());
			else WritePrivateProfileString(_T("display"), _T("fullscran"), _T("0"), (_T(".\\") + (wstring)inipath).c_str());

			if (is_limit)WritePrivateProfileString(_T("display"), _T("limit"), _T("1"), (_T(".\\") + (wstring)inipath).c_str());
			else WritePrivateProfileString(_T("display"), _T("limit"), _T("0"), (_T(".\\") + (wstring)inipath).c_str());

			return;
		}

		cleardevice();

		for (int i = 1; i < 3; i++)solidrectangle(r[i].left, r[i].top, r[i].right, r[i].bottom);
		rectangle(r[iic].left, r[iic].top, r[iic].right, r[iic].bottom);

		drawtext(intr[0].c_str(), r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		if (is_fullscran)drawtext((intr[1] + _T("ON")).c_str(), r + 1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		else drawtext((intr[1] + _T("OFF")).c_str(), r + 1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		if (is_limit)drawtext((intr[2] + _T("ON")).c_str(), r + 2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		else drawtext((intr[2] + _T("OFF")).c_str(), r + 2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		FlushBatchDraw();

		Sleep(5);
	}
}

int ryui()
{
	wstringstream ss;

	InputMess(datapath);

	keycheck ke(VK_RETURN);
	keycheck kd(VK_DOWN);
	keycheck ku(VK_UP);
	keycheck kes(VK_ESCAPE);

	RECT r1 = { 200, 50, 440, 90 };
	RECT r2 = { 10, 90, 629, 470 };

	bool is_exit = true;

	int rkp = 0;
	int chs = 0;

	int ans = -1;

	settextcolor(WHITE);

	rpysearch(_T("*.rpy"));
	rpydeal(&ss, 0);

	while (is_exit)
	{
		cleardevice();

		if (RpyTop == 0)
		{
			drawtext(_T("没有找到有效回放文件"), &r1, DT_CENTER | DT_VCENTER | DT_LEFT | DT_WORDBREAK);

			FlushBatchDraw();

			Sleep(5);

			if (kes.getstat() == 1)
			{
				is_exit = false;
			}
		}
		else
		{
			drawtext(_T("选择回放"), &r1, DT_CENTER | DT_VCENTER | DT_LEFT | DT_WORDBREAK);
			drawtext(ss.str().c_str(), &r2, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_EXPANDTABS | DT_TABSTOP | 0x0F00);
			rectangle(5, 120 + (chs - rkp) * 32, 635, 140 + (chs - rkp) * 32);

			FlushBatchDraw();

			Sleep(5);

			if (kd.getstat() == 1)
			{
				chs++;
				chs = min(chs, RpyTop - 1);

				rpydeal(&ss, rkp);
			}

			if (ku.getstat() == 1)
			{
				chs--;
				chs = max(chs, 0);

				rpydeal(&ss, rkp);
			}

			if (kes.getstat() == 1)
			{
				is_exit = false;
			}

			if (ke.getstat() == 1)
			{
				if (RpyList[chs].inf.type != 1024)ans = RpyList[chs].inf.type;
				else ans = 0;
				is_replay = true;
				rppath = RpyList[chs].path;
				is_exit = false;
			}

			if (chs - rkp == 9 && rkp < RpyTop - 1)
			{
				rkp++;
				rpydeal(&ss, rkp);
			}
			else if (chs - rkp == 1 && rkp > 0)
			{
				rkp--;
				rpydeal(&ss, rkp);
			}
		}
	}

	settextcolor(BLACK);

	return ans;
}

int rpysearch(const TCHAR *wildcardPath)
{
	RpyTop = 0;
	int ErrorCont = 0;

	HANDLE hFile = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA pNextInfo;

	hFile = FindFirstFile(wildcardPath, &pNextInfo);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return FALSE;
	}

	char infPath[MAX_PATH] = { 0 };
	if (pNextInfo.cFileName[0] != '.')
	{
		if (getrpyinf(RpyList + RpyTop, pNextInfo.cFileName))
		{
			RpyTop++;
		}
		else ErrorCont++;



	}

	while (FindNextFile(hFile, &pNextInfo))
	{
		if (pNextInfo.cFileName[0] == '.')
		{
			continue;
		}

		if (getrpyinf(RpyList + RpyTop, pNextInfo.cFileName))
		{
			RpyTop++;

			if (RpyTop >= LISTMAX)return ErrorCont;
		}
		else ErrorCont++;
	}

	return ErrorCont;
}

bool getrpyinf(rpyinf * obj, const TCHAR * path)
{
	ifstream file(path);

	CharTran(obj->path, path);
	file.read((char*)&obj->inf, sizeof(obj->inf));

	if (obj->inf.checks == obj->inf.setcheck())return true;
	else return false;
}

void rpydeal(wstringstream * ss, int be)
{
	ss->str(_T(""));
	ss->clear();

	TCHAR tmp[100];
	int end = 0;
	bool rest = true;

	const wstring str[5] = { _T("标准"),_T("第一关"),_T("第二关"),_T("第三关"),_T("第四关") };

	*ss << _T("文件名") << _T("\t") << _T("玩家名") << _T("\t") << _T("关卡") << _T("\t") << _T("分数") << _T("\t") << _T("时间") << _T("\n");
	*ss << _T("-------------------------------------------------------------------------------------------------------------------------------\n");
	if (RpyTop > be + 10)
	{
		end = be + 10;
	}
	else
	{
		end = RpyTop;
		rest = false;
	}

	for (int i = be; i < end; i++)
	{
		_wctime_s(tmp, 50, &RpyList[i].inf.t);
		if (RpyList[i].inf.type != 1024)*ss << RpyList[i].path << _T("\t") << RpyList[i].inf.name << _T("\t") << str[RpyList[i].inf.type] << _T("\t") << RpyList[i].inf.score << _T("\t") << tmp << _T("\n");
		else *ss << RpyList[i].path << _T("\t") << RpyList[i].inf.name << _T("\t") << str[0] << _T("\t") << RpyList[i].inf.score << _T("\t") << tmp << _T("\n");
	}
	if (rest)*ss << _T(" ");
}

int GameScript_4(int sfps)
{
	int level = sfps / 600 + 1;//							每10秒难度上升一级
	int p;
	double agl;
	const vec basesa = mv(cos(Pi / 5), sin(Pi / 5));

	vec tmpspeed = mv(1, 0);

	if (sfps % (30 / level + 10) == 0)
	{
		for (int i = 0; i < level * 3 + 2; i++)
		{
			p = FindBlank();
			dlst[p].alv = true;
			dlst[p].clr = RED;
			dlst[p].pos = mv(201.5, 201.5);

			agl = atan2((player_pos.y - 201.5), (player_pos.x - 201.5)) + agl_to_rec(rangrand(-10 - level * 4, 10 + level * 4));

			dlst[p].sped = mv(cos(agl), sin(agl)) * (0.5 + rangrand(0, 5) * 0.3);
			dlst[p].r = 3;
		}
	}

	if (sfps % 120 == 0)
	{
		for (int i = 0; i < 10; i++)
		{
			p = FindBlank();
			dlst[p].alv = true;
			dlst[p].clr = WHITE;
			dlst[p].pos = mv(201.5, 201.5);
			dlst[p].sped = tmpspeed * 0.7;
			dlst[p].r = 3;

			tmpspeed = tmpspeed % basesa;
		}
	}

	return level;
}