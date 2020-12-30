#pragma once
#include <easyx.h>
#include <ddraw.h>
#include <cmath>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>
#pragma comment (lib,"ddraw.lib")
using namespace std;

//////////////////////////////////绘图部分

const int w = 400;
const int h = 400;

//////////////////////////////////

//////////////////////////////////数学部分

struct vec
{
	double x;
	double y;

	vec();
};// 向量类

const double Pi = 3.1415926536;

/////////////////////////////////

/////////////////////////////////计时器

struct timer
{
	clock_t Thalf;
	clock_t Tstart;
	bool is_running;
	inline void reset()
	{
		Tstart = clock();
		is_running = true;
		Thalf = 0;
	}

	inline clock_t read()
	{
		if (is_running)return clock() - Tstart;
		else return Thalf;
	}

	inline bool check(clock_t t)
	{
		if (read() >= t)
		{
			reset();
			return true;
		}
		else return false;
	}

	inline void stop()
	{
		Thalf = read();
		is_running = false;
	}

	inline void start()
	{
		Tstart = clock() - Thalf;
		is_running = true;
	}

	timer()
	{
		reset();
	}
};

/////////////////////////////////

////////////////////////////////按键检测

struct keycheck
{
	int vKey;
	int stat;//							状态 0松开 1按下 2持续按下

	keycheck(int gvKey)
	{
		vKey = gvKey;
		stat = 0;
		getstat();
	}

	int getstat()
	{
		if (GetAsyncKeyState(vKey) & 0x8000)
		{
			if (stat == 0)
			{
				stat = 1;
			}
			else stat = 2;
		}
		else stat = 0;

		return stat;
	}
};

////////////////////////////////

/////////////////////////////////弹幕部分

const int maxdanmu = 2500;
const int ttlstage = 5;//	关卡数
struct danmu
{
	vec pos;//							位置
	vec sped;//							速度

	DWORD id;//							信息
	int r;//							半径
	DWORD type;//						类型
	DWORD clr;//						颜色

	bool alv;

	danmu();
};

struct rpyuni //回放数据单元
{
	double x, y;
	char stat;
};

danmu dlst[maxdanmu];

/////////////////////////////////

/////////////////////////////////碰撞部分

bool KickBox[w][h];

/////////////////////////////////

////////////////////////////////游戏控制部分

bool is_limit = true;//					是否垂直同步
bool is_fullscran = false;//			是否全屏

bool is_replay = false;

vec player_pos;
double playerspeed;
int rbmtm;//							剩余技能使用次数
int score;//							分数

const int boomtime = 3;
const int bonuslevel = 6;//				奖励层数

const double plyspeed_fast = 8;
const double plyspeed_slow = 4;

const int player_r = 8;
const double pd_r = 4;
const DWORD player_clr = GREEN;

const int FPSN = 16;//					帧间隔 1000/每秒帧数

#define	CMD_UP			1
#define	CMD_DOWN		2
#define	CMD_LEFT		4
#define	CMD_RIGHT		8

int deadrelax = 100;

//										辅助scripchoose
int nfps = 0;
int deadwait = 0;

wstring rppath;

const int MAXRPYINF = 144000;

rpyuni tmprpylist[MAXRPYINF];
int rpyinftop = 0;

//////////////////////////////

//////////////////////////////排名部分

struct infunio
{
	TCHAR name[20];//					姓名 不得超过10个字符 不能出现* \

	DWORD type;//						类别 最好不将0作为类别 正数 1024为标准模式 其余数字为对应的关卡
	DWORD score;//						分数
	time_t t;//							时间

	DWORD checks;//						检验码

	DWORD setcheck()//					获取检测码
	{
		checks = (type * 11 + score * 4 + (DWORD)lstrlen(name) * 514 + (DWORD)(t % 142857)) % 128;
		return checks;
	}

};//信息单元

struct scoreset
{
	TCHAR name[20];//					姓名
	DWORD score;//						分数
	time_t t;//							时间
};//									名字与分数

const int LISTMAX = 100;

infunio InfList[LISTMAX];
int ListTop = 0;

scoreset ScrList[LISTMAX];
int ScoreListtop = 0;

TCHAR datapath[50] = _T("local");
TCHAR inipath[50] = _T("gdat");
const TCHAR nulpath[] = _T("local");

TCHAR intrtext[2000];

struct rpyinf
{
	TCHAR path[20];
	infunio inf;
};

rpyinf RpyList[LISTMAX];
int RpyTop = 0;

//////////////////////////////