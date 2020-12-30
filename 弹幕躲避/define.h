#pragma once
#include"type.h"

///////////////////////////////////数学部分

//范围检查
#define NumCheck(Obj,L,R) ((Obj >= L) && (Obj < R))

//生成一个向量
inline vec mv(double x, double y);

vec operator+ (const vec& a, const vec& b);
vec operator- (const vec& a, const vec& b);
vec operator* (const vec& a, double b);
vec operator* (double b, const vec& a);
double operator* (const vec& a, const vec& b);
vec operator% (const vec& a, const vec& b);
double operator! (const vec& a);

inline vec rec_to_vec(double rec);

//生成一个区间内的随机数 整数
inline int rangrand(int L, int R);

//生成随机角度(返回弧度)
inline double randangle();

//角度转弧度
inline double agl_to_rec(double agl);

//概率返回真
inline bool percent_true(int rent);

////////////////////////////////////

///////////////////////////////////绘画部分

//修改屏幕分辨率
bool ChangeDisSize(int, int);

//绘制弹幕
void DrawDanmu(danmu obj);

//绘制玩家
void DrawPlayer();

//设置全屏 true全屏 false回复
bool FullScreen(bool is_fullscran);
//////////////////////////////////////

/////////////////////////////////碰撞部分

//插入矩形碰撞区
void WriteKbinf(int left, int top, int right, int bottom);

//清空
void CleanKb();

//查找是否有为真的区域(有的话为假)
bool CheckKbinf(int left, int top, int right, int bottom);
/////////////////////////////////

/////////////////////////////////弹幕部分

//通常处理
void NormalDeal(danmu &obj);

//越界检查
bool PassCheck(vec pos);

//寻找空位  确保寻找到空位后立刻对空位进行激活(alv = true)再再次运行此函数
int FindBlank();

//全弹幕处理
int ListDeal();

//清空所有弹幕
void CleanDanmu();
/////////////////////////////////

/////////////////////////////////游戏部分

//游戏主循环 practicemode为练习模式对应的层数 为-1则无
void Gaming(int practicemode);
//环境初始化
void init();
//数据初始化
void datainit();
//结束游戏
void gend();
/////////////////////////////////

/////////////////////////////////控制部分

void OnUp();
void OnDown();
void OnRight();
void OnLeft();

int GetCommand();
void DispatchCommand(int _cmd);

void playerdeal();
/////////////////////////////////

/////////////////////////////////剧本部分

//游戏剧本 pm 是否启用练习模式
int ScriptChoose(int sfps, int &nstage, bool into = false);
int GameScript_0(int sfps);
int GameScript_1(int sfps);
int GameScript_2(int sfps);
int GameScript_3(int sfps);
int GameScript_4(int sfps);
/////////////////////////////////

/////////////////////////////////DEBUG部分

void ShowDebuginf(double fps, int told, int level, int stage);
////////////////////////////////

///////////////////////////////UI部分

//主界面UI 返回值意义:0正常开始 >0:练习模式的关卡 -1退出 -2-所选关卡 回放
int initui();
//选关UI
int clui();
//暂停ui
int stui();
//死亡界面
void dhui(int stage, bool is_replay);
//说明界面
void itui();
//排行榜
void rkui();
//画面设置界面
void opui();
//回放界面
int ryui();
///////////////////////////////

//////////////////////////////录像,成绩记录与处理

//检测检验码
inline bool checheck(infunio* obj, DWORD che);
//复制字符串
void CharTran(TCHAR obj[], const TCHAR frm[]);
//比较字符串
bool CharEqual(TCHAR obj[], TCHAR frm[]);
//输出 模式0输出dat文件 1添加进.mdat文件中 2添加进InfList中
void OutputUnio(infunio obj, const TCHAR* path);
//输入 .mdat 模式0在原有列表上添加
int InputMess(const TCHAR* path);
//生成特定类别的序列 0为所有类型
void MakeTypelist(DWORD mt);
//显示列表
void MakeTable(wstringstream * ss, int be = 0);
//sort的比较函数
bool comp(scoreset a, scoreset b);
//获取录像信息
bool getrpyinf(rpyinf *obj, const TCHAR* path);
//获取录像
int rpysearch(const TCHAR *wildcardPath);
//处理录像信息
void rpydeal(wstringstream * ss, int be = 0);

//////////////////////////////