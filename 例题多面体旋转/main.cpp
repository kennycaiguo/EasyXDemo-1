//////////////////////////////////////////////////////////////
// �������ƣ������������ת
// ���뻷����Mictosoft Visual Studio 2013, EasyX_20200315(beta)
// �������ߣ�luoyh <2864292458@qq.com>
// ����޸ģ�2020-10-9
// ˵�������ά��ת���� TIN ����������
// ��ά������ο���https://codebus.cn/bestans/a/3d-rotating-sphere
// TIN �������ο���https://codebus.cn/luoyh/a/tin-triangulation
//

#include <graphics.h>
#include <time.h>
#include <math.h>
#include <conio.h>

#define WIDTH     640											// ���ɴ��ڵĴ�С
#define HEIGHT    480 
#define MAXPOINT  25
#define	PI        acos(-1.0)

bool LineXY[(1 + MAXPOINT) * MAXPOINT / 2] = { false };			// Ϊ���ж������Ƿ����߶����һά����

// ��ʼ������
void initLineXY()
{
	for (int i = 0; i < (1 + MAXPOINT) * MAXPOINT / 2; i++)
	{
		LineXY[i] = false;
	}
}

// �ж����������Ƿ�����
bool IsLinked(int p1, int p2)
{
	if (p1 >= p2)
		return LineXY[(1 + p1) * p1 / 2 + p2];
	else
		return LineXY[(1 + p2) * p2 / 2 + p1];
}

// �����Ѿ����ƹ�����
void Link(int p1, int p2)
{
	if (p1 >= p2)
		LineXY[(1 + p1) * p1 / 2 + p2] = true;
	else
		LineXY[(1 + p2) * p2 / 2 + p1] = true;
}

// ���ڼ���ռ�����ľ���
double distance(double x1, double y1, double z1, double x2, double y2, double z2)
{
	return sqrt((double)((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2)));
}

// ���������
void drawpoint(int x, int y, COLORREF  z)
{
	setfillcolor(z);
	solidcircle(x, y, 1);
}

// ������ά��
struct POINT3D
{
	double x;
	double y;
	double z;
};

POINT3D p3d[MAXPOINT];		// ���е���ά��
double viewZ = 3;			// �ӵ� z ������

// ��ʼ����ά��
void InitPoint()
{
	// �����������
	srand((unsigned)time(NULL));
	// ����������������㣨�������������������Բ������Ĺ�ϵ��
	double rxy, a;
	for (int i = 0; i < MAXPOINT; i++)
	{
		p3d[i].z = 2.0 * rand() / RAND_MAX - 1;	// ����� z ����
		rxy = sqrt(1 - p3d[i].z * p3d[i].z);	// ������άʸ���� xoy ƽ���ͶӰ����
		a = 2 * PI * rand() / RAND_MAX;			// ��������Ƕ�
		p3d[i].x = cos(a) * rxy;
		p3d[i].y = sin(a) * rxy;
	}
}

// ʹ��ά�㰴 x ����תָ���Ƕ�
void RotateX(POINT3D& p, double angle)
{
	double y = p.y;
	p.y = p.y * cos(angle) + p.z * sin(-angle);
	p.z = y * sin(angle) + p.z * cos(angle);
}

// ��������������������������γ����Բ��Բ�ĺͰ뾶
double CircleCenter(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double* x, double* y, double* z, double* r)
{
	double a1, b1, c1, d1;
	double a2, b2, c2, d2;
	double a3, b3, c3, d3;

	a1 = (y1 * z2 - y2 * z1 - y1 * z3 + y3 * z1 + y2 * z3 - y3 * z2);
	b1 = -(x1 * z2 - x2 * z1 - x1 * z3 + x3 * z1 + x2 * z3 - x3 * z2);
	c1 = (x1 * y2 - x2 * y1 - x1 * y3 + x3 * y1 + x2 * y3 - x3 * y2);
	d1 = -(x1 * y2 * z3 - x1 * y3 * z2 - x2 * y1 * z3 + x2 * y3 * z1 + x3 * y1 * z2 - x3 * y2 * z1);

	a2 = 2.0 * (x2 - x1);
	b2 = 2.0 * (y2 - y1);
	c2 = 2.0 * (z2 - z1);
	d2 = x1 * x1 + y1 * y1 + z1 * z1 - x2 * x2 - y2 * y2 - z2 * z2;

	a3 = 2.0 * (x3 - x1);
	b3 = 2.0 * (y3 - y1);
	c3 = 2.0 * (z3 - z1);
	d3 = x1 * x1 + y1 * y1 + z1 * z1 - x3 * x3 - y3 * y3 - z3 * z3;

	*x = -(b1 * c2 * d3 - b1 * c3 * d2 - b2 * c1 * d3 + b2 * c3 * d1 + b3 * c1 * d2 - b3 * c2 * d1)
		/ (a1 * b2 * c3 - a1 * b3 * c2 - a2 * b1 * c3 + a2 * b3 * c1 + a3 * b1 * c2 - a3 * b2 * c1);
	*y = (a1 * c2 * d3 - a1 * c3 * d2 - a2 * c1 * d3 + a2 * c3 * d1 + a3 * c1 * d2 - a3 * c2 * d1)
		/ (a1 * b2 * c3 - a1 * b3 * c2 - a2 * b1 * c3 + a2 * b3 * c1 + a3 * b1 * c2 - a3 * b2 * c1);
	*z = -(a1 * b2 * d3 - a1 * b3 * d2 - a2 * b1 * d3 + a2 * b3 * d1 + a3 * b1 * d2 - a3 * b2 * d1)
		/ (a1 * b2 * c3 - a1 * b3 * c2 - a2 * b1 * c3 + a2 * b3 * c1 + a3 * b1 * c2 - a3 * b2 * c1);
	*r = sqrt((x1 - *x) * (x1 - *x) + (y1 - *y) * (y1 - *y) + (z1 - *z) * (z1 - *z));
	return *r;
}

// ʹ��ά�㰴 y ����תָ���Ƕ�
void RotateY(POINT3D& p, double angle)
{
	double x = p.x;
	p.x = p.x * cos(angle) + p.z * sin(-angle);
	p.z = x * sin(angle) + p.z * cos(angle);
}

// ʹ��ά�㰴 z ����תָ���Ƕ�
void RotateZ(POINT3D& p, double angle)
{
	double x = p.x;
	p.x = p.x * cos(angle) + p.y * sin(-angle);
	p.y = x * sin(angle) + p.y * cos(angle);
}

// ����ά��ͶӰ����ά��Ļ�ϣ�����͸�ӣ�
POINT Projection(POINT3D p)
{
	POINT p2d;
	p2d.x = (int)(p.x * (viewZ / (viewZ - p.z)) * 200 + 0.5) + 320;
	p2d.y = (int)(p.y * (viewZ / (viewZ - p.z)) * 200 + 0.5) + 240;
	return p2d;
}

int main()
{
	initgraph(WIDTH, HEIGHT);
	InitPoint();
	BeginBatchDraw();
	int c;
	POINT p2d;

	while (!_kbhit())
	{
		initLineXY();
		cleardevice();		// �����Ļ

		for (int i = 0; i < MAXPOINT; i++)
		{
			// ʹ�õ�Χ����������������ת�˶�
			RotateX(p3d[i], PI / 180);
			RotateY(p3d[i], PI / 170);
			RotateZ(p3d[i], PI / 160);
			// ���ݵ����ȣ�������Ӧ�Ҷȵ���ɫ
			c = (int)(p3d[i].z * 100) + 155;
			// ͶӰ�õ㵽��Ļ��
			p2d = Projection(p3d[i]);
			// ����
			drawpoint(p2d.x, p2d.y, RGB(c, c, c));
		}

		//	�ռ����Ѿ����˵㣬���ռ�㰴�� TIN ��������������
		int Z[MAXPOINT];								// �����жϹ��ĵ������
		double max = WIDTH * HEIGHT * WIDTH;			// �ô��ڵĳ��Ϳ�������һ����Դ����
		int lx;											// ������ʱ X ��������ı���
		int ly;											// ������ʱ Y ��������ı���
		int li;											// ���ڴ�����ʱ�жϹ��ĵ���±���

		// ��������ɵĵ��������жϣ��ҳ�������̵�������
		for (int i = 0; i < MAXPOINT; i++)
		{
			for (int j = i + 1; j < MAXPOINT; j++)
			{
				if (max > distance(p3d[i].x, p3d[i].y, p3d[i].z, p3d[j].x, p3d[j].y, p3d[j].z))
				{
					lx = i;
					ly = j;
					max = distance(p3d[i].x, p3d[i].y, p3d[i].z, p3d[j].x, p3d[j].y, p3d[j].z);
				}
			}
		}

		POINT p2d_1, p2d_2;
		p2d_1 = Projection(p3d[lx]);
		p2d_2 = Projection(p3d[ly]);
		// ����ط�������Ҫ������ͶӰ��ʽ
		line(p2d_1.x, p2d_1.y, p2d_2.x, p2d_2.y);
		Link(lx, ly);
		Z[0] = lx;
		Z[1] = ly;
		int n = 2;

		while (true)
		{
			if (n >= MAXPOINT)
				break;
			int m = 0;
			double rad, Xd, Yd, Zd, Rd;
			bool OK = false;
			max = WIDTH * HEIGHT * WIDTH;

			// ��ʼ�ж�������ɵ�ÿһ����
			for (int i = 0; i < MAXPOINT; i++)
			{
				m = 0;
				OK = false;

				// �ж�������Ƿ��Ѿ��жϹ�������Ѿ��жϹ��������ж���һ���㣬������ڣ���������
				while (true)
				{
					if (m >= n)
					{
						m = 0;
						break;
					}
					if (i == Z[m])
					{
						OK = true;
						break;
					}
					m++;
				}
				if (OK == true)
					continue;

				// ���Ѿ�ȷ�����������δȷ���ĵ���м��������γ������εĵİ뾶�����ж��γɵ�Բ�����������ĵ�
				// ���������ĵ㣬��������ߣ����������ĵ㣬������ж���һ����
				for (int j = 0; j < n; j++)
				{
					for (int k = 0; k < n; k++)
					{
						rad = CircleCenter(p3d[Z[j]].x, p3d[Z[j]].y, p3d[Z[j]].z, p3d[Z[k]].x, p3d[Z[k]].y, p3d[Z[k]].z, p3d[i].x, p3d[i].y, p3d[i].z, &Xd, &Yd, &Zd, &Rd);
						int cc = 0;
						OK = false;
						while (true)
						{
							if (cc >= MAXPOINT)
								break;

							// �ж�Բ�����������㣬����������жϵĵ㲻��Ϊ�γ����Բ����������㣬����������㣬��������ѭ��
							if (distance(Xd, Yd, Zd, p3d[cc].x, p3d[cc].y, p3d[cc].z) <= Rd && cc != Z[k] && cc != Z[j] && cc != i)
							{
								OK = true;
								break;
							}
							cc++;
						}

						// ��ΪԲ���������㣬��������ѭ��
						if (OK == true)
							continue;

						if (max >= rad && rad != 0)					// ��������Χ��Բ��û�е��ҵ��뾶��С��
						{
							lx = Z[j];
							ly = Z[k];

							if (rad >= WIDTH)
								continue;
							else
							{
								POINT p2d_1, p2d_2;

								if (IsLinked(i, lx) == false)		// �����߶Σ������ж�����߶��Ƿ��Ѿ����ƹ�
								{
									Link(i, lx);
									c = (int)(p3d[i].z * 100) + 155;
									setlinecolor(RGB(c, c, c));
									p2d_1 = Projection(p3d[i]);
									p2d_2 = Projection(p3d[lx]);
									line(p2d_1.x, p2d_1.y, p2d_2.x, p2d_2.y);
									/*Sleep(100);*/					// �����Ҫ�鿴���ƹ��̣�ȥ��ע��
								}
								if (IsLinked(i, ly) == false)
								{
									Link(i, ly);
									c = (int)(p3d[ly].z * 100) + 155;
									setlinecolor(RGB(c, c, c));
									p2d_1 = Projection(p3d[i]);
									p2d_2 = Projection(p3d[ly]);
									line(p2d_1.x, p2d_1.y, p2d_2.x, p2d_2.y);
									/*Sleep(100);*/					// �����Ҫ�鿴���ƹ��̣�ȥ��ע��
								}
								if (IsLinked(lx, ly) == false)
								{
									Link(lx, ly);
									c = (int)(p3d[lx].z * 100) + 155;
									setlinecolor(RGB(c, c, c));
									p2d_1 = Projection(p3d[lx]);
									p2d_2 = Projection(p3d[ly]);
									line(p2d_1.x, p2d_1.y, p2d_2.x, p2d_2.y);
									/*Sleep(100);*/					// �����Ҫ�鿴���ƹ��̣�ȥ��ע��
								}
								li = i;
							}
						}
					}
				}
			}
			Z[n] = li;
			n++;
		}
		FlushBatchDraw();
		Sleep(10);													// ��ʱ 10 ����
	}

	EndBatchDraw();
	closegraph();
	return 0;
}