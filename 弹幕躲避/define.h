#pragma once
#include"type.h"

///////////////////////////////////��ѧ����

//��Χ���
#define NumCheck(Obj,L,R) ((Obj >= L) && (Obj < R))

//����һ������
inline vec mv(double x, double y);

vec operator+ (const vec& a, const vec& b);
vec operator- (const vec& a, const vec& b);
vec operator* (const vec& a, double b);
vec operator* (double b, const vec& a);
double operator* (const vec& a, const vec& b);
vec operator% (const vec& a, const vec& b);
double operator! (const vec& a);

inline vec rec_to_vec(double rec);

//����һ�������ڵ������ ����
inline int rangrand(int L, int R);

//��������Ƕ�(���ػ���)
inline double randangle();

//�Ƕ�ת����
inline double agl_to_rec(double agl);

//���ʷ�����
inline bool percent_true(int rent);

////////////////////////////////////

///////////////////////////////////�滭����

//�޸���Ļ�ֱ���
bool ChangeDisSize(int, int);

//���Ƶ�Ļ
void DrawDanmu(danmu obj);

//�������
void DrawPlayer();

//����ȫ�� trueȫ�� false�ظ�
bool FullScreen(bool is_fullscran);
//////////////////////////////////////

/////////////////////////////////��ײ����

//���������ײ��
void WriteKbinf(int left, int top, int right, int bottom);

//���
void CleanKb();

//�����Ƿ���Ϊ�������(�еĻ�Ϊ��)
bool CheckKbinf(int left, int top, int right, int bottom);
/////////////////////////////////

/////////////////////////////////��Ļ����

//ͨ������
void NormalDeal(danmu &obj);

//Խ����
bool PassCheck(vec pos);

//Ѱ�ҿ�λ  ȷ��Ѱ�ҵ���λ�����̶Կ�λ���м���(alv = true)���ٴ����д˺���
int FindBlank();

//ȫ��Ļ����
int ListDeal();

//������е�Ļ
void CleanDanmu();
/////////////////////////////////

/////////////////////////////////��Ϸ����

//��Ϸ��ѭ�� practicemodeΪ��ϰģʽ��Ӧ�Ĳ��� Ϊ-1����
void Gaming(int practicemode);
//������ʼ��
void init();
//���ݳ�ʼ��
void datainit();
//������Ϸ
void gend();
/////////////////////////////////

/////////////////////////////////���Ʋ���

void OnUp();
void OnDown();
void OnRight();
void OnLeft();

int GetCommand();
void DispatchCommand(int _cmd);

void playerdeal();
/////////////////////////////////

/////////////////////////////////�籾����

//��Ϸ�籾 pm �Ƿ�������ϰģʽ
int ScriptChoose(int sfps, int &nstage, bool into = false);
int GameScript_0(int sfps);
int GameScript_1(int sfps);
int GameScript_2(int sfps);
int GameScript_3(int sfps);
int GameScript_4(int sfps);
/////////////////////////////////

/////////////////////////////////DEBUG����

void ShowDebuginf(double fps, int told, int level, int stage);
////////////////////////////////

///////////////////////////////UI����

//������UI ����ֵ����:0������ʼ >0:��ϰģʽ�Ĺؿ� -1�˳� -2-��ѡ�ؿ� �ط�
int initui();
//ѡ��UI
int clui();
//��ͣui
int stui();
//��������
void dhui(int stage, bool is_replay);
//˵������
void itui();
//���а�
void rkui();
//�������ý���
void opui();
//�طŽ���
int ryui();
///////////////////////////////

//////////////////////////////¼��,�ɼ���¼�봦��

//��������
inline bool checheck(infunio* obj, DWORD che);
//�����ַ���
void CharTran(TCHAR obj[], const TCHAR frm[]);
//�Ƚ��ַ���
bool CharEqual(TCHAR obj[], TCHAR frm[]);
//��� ģʽ0���dat�ļ� 1��ӽ�.mdat�ļ��� 2��ӽ�InfList��
void OutputUnio(infunio obj, const TCHAR* path);
//���� .mdat ģʽ0��ԭ���б������
int InputMess(const TCHAR* path);
//�����ض��������� 0Ϊ��������
void MakeTypelist(DWORD mt);
//��ʾ�б�
void MakeTable(wstringstream * ss, int be = 0);
//sort�ıȽϺ���
bool comp(scoreset a, scoreset b);
//��ȡ¼����Ϣ
bool getrpyinf(rpyinf *obj, const TCHAR* path);
//��ȡ¼��
int rpysearch(const TCHAR *wildcardPath);
//����¼����Ϣ
void rpydeal(wstringstream * ss, int be = 0);

//////////////////////////////