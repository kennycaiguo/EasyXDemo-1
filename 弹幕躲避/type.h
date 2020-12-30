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

//////////////////////////////////��ͼ����

const int w = 400;
const int h = 400;

//////////////////////////////////

//////////////////////////////////��ѧ����

struct vec
{
	double x;
	double y;

	vec();
};// ������

const double Pi = 3.1415926536;

/////////////////////////////////

/////////////////////////////////��ʱ��

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

////////////////////////////////�������

struct keycheck
{
	int vKey;
	int stat;//							״̬ 0�ɿ� 1���� 2��������

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

/////////////////////////////////��Ļ����

const int maxdanmu = 2500;
const int ttlstage = 5;//	�ؿ���
struct danmu
{
	vec pos;//							λ��
	vec sped;//							�ٶ�

	DWORD id;//							��Ϣ
	int r;//							�뾶
	DWORD type;//						����
	DWORD clr;//						��ɫ

	bool alv;

	danmu();
};

struct rpyuni //�ط����ݵ�Ԫ
{
	double x, y;
	char stat;
};

danmu dlst[maxdanmu];

/////////////////////////////////

/////////////////////////////////��ײ����

bool KickBox[w][h];

/////////////////////////////////

////////////////////////////////��Ϸ���Ʋ���

bool is_limit = true;//					�Ƿ�ֱͬ��
bool is_fullscran = false;//			�Ƿ�ȫ��

bool is_replay = false;

vec player_pos;
double playerspeed;
int rbmtm;//							ʣ�༼��ʹ�ô���
int score;//							����

const int boomtime = 3;
const int bonuslevel = 6;//				��������

const double plyspeed_fast = 8;
const double plyspeed_slow = 4;

const int player_r = 8;
const double pd_r = 4;
const DWORD player_clr = GREEN;

const int FPSN = 16;//					֡��� 1000/ÿ��֡��

#define	CMD_UP			1
#define	CMD_DOWN		2
#define	CMD_LEFT		4
#define	CMD_RIGHT		8

int deadrelax = 100;

//										����scripchoose
int nfps = 0;
int deadwait = 0;

wstring rppath;

const int MAXRPYINF = 144000;

rpyuni tmprpylist[MAXRPYINF];
int rpyinftop = 0;

//////////////////////////////

//////////////////////////////��������

struct infunio
{
	TCHAR name[20];//					���� ���ó���10���ַ� ���ܳ���* \

	DWORD type;//						��� ��ò���0��Ϊ��� ���� 1024Ϊ��׼ģʽ ��������Ϊ��Ӧ�Ĺؿ�
	DWORD score;//						����
	time_t t;//							ʱ��

	DWORD checks;//						������

	DWORD setcheck()//					��ȡ�����
	{
		checks = (type * 11 + score * 4 + (DWORD)lstrlen(name) * 514 + (DWORD)(t % 142857)) % 128;
		return checks;
	}

};//��Ϣ��Ԫ

struct scoreset
{
	TCHAR name[20];//					����
	DWORD score;//						����
	time_t t;//							ʱ��
};//									���������

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