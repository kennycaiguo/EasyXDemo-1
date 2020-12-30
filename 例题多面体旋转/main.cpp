//////////////////////////////////////////////////////////////
// 程序名称：立体多面体旋转
// 编译环境：Mictosoft Visual Studio 2013, EasyX_20200315(beta)
// 作　　者：luoyh <2864292458@qq.com>
// 最后修改：2020-10-9
// 说明结合三维旋转球与 TIN 三角网生成
// 三维立体球参考：https://codebus.cn/bestans/a/3d-rotating-sphere
// TIN 三角网参考：https://codebus.cn/luoyh/a/tin-triangulation
//

#include <graphics.h>
#include <time.h>
#include <math.h>
#include <conio.h>

#define WIDTH     640											// 生成窗口的大小
#define HEIGHT    480 
#define MAXPOINT  25
#define	PI        acos(-1.0)

bool LineXY[(1 + MAXPOINT) * MAXPOINT / 2] = { false };			// 为了判断两点是否连线定义的一维数组

// 初始化连线
void initLineXY()
{
	for (int i = 0; i < (1 + MAXPOINT) * MAXPOINT / 2; i++)
	{
		LineXY[i] = false;
	}
}

// 判断这两个点是否连线
bool IsLinked(int p1, int p2)
{
	if (p1 >= p2)
		return LineXY[(1 + p1) * p1 / 2 + p2];
	else
		return LineXY[(1 + p2) * p2 / 2 + p1];
}

// 储存已经绘制过的线
void Link(int p1, int p2)
{
	if (p1 >= p2)
		LineXY[(1 + p1) * p1 / 2 + p2] = true;
	else
		LineXY[(1 + p2) * p2 / 2 + p1] = true;
}

// 用于计算空间两点的距离
double distance(double x1, double y1, double z1, double x2, double y2, double z2)
{
	return sqrt((double)((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2)));
}

// 绘制随机点
void drawpoint(int x, int y, COLORREF  z)
{
	setfillcolor(z);
	solidcircle(x, y, 1);
}

// 定义三维点
struct POINT3D
{
	double x;
	double y;
	double z;
};

POINT3D p3d[MAXPOINT];		// 所有的三维点
double viewZ = 3;			// 视点 z 轴坐标

// 初始化三维点
void InitPoint()
{
	// 产生随机种子
	srand((unsigned)time(NULL));
	// 产生球体表面的随机点（根据球体面积与其外切圆柱面积的关系）
	double rxy, a;
	for (int i = 0; i < MAXPOINT; i++)
	{
		p3d[i].z = 2.0 * rand() / RAND_MAX - 1;	// 求随机 z 坐标
		rxy = sqrt(1 - p3d[i].z * p3d[i].z);	// 计算三维矢量在 xoy 平面的投影长度
		a = 2 * PI * rand() / RAND_MAX;			// 产生随机角度
		p3d[i].x = cos(a) * rxy;
		p3d[i].y = sin(a) * rxy;
	}
}

// 使三维点按 x 轴旋转指定角度
void RotateX(POINT3D& p, double angle)
{
	double y = p.y;
	p.y = p.y * cos(angle) + p.z * sin(-angle);
	p.z = y * sin(angle) + p.z * cos(angle);
}

// 根据三个点坐标算出他们三个形成外接圆的圆心和半径
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

// 使三维点按 y 轴旋转指定角度
void RotateY(POINT3D& p, double angle)
{
	double x = p.x;
	p.x = p.x * cos(angle) + p.z * sin(-angle);
	p.z = x * sin(angle) + p.z * cos(angle);
}

// 使三维点按 z 轴旋转指定角度
void RotateZ(POINT3D& p, double angle)
{
	double x = p.x;
	p.x = p.x * cos(angle) + p.y * sin(-angle);
	p.y = x * sin(angle) + p.y * cos(angle);
}

// 将三维点投影到二维屏幕上（单点透视）
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
		cleardevice();		// 清除屏幕

		for (int i = 0; i < MAXPOINT; i++)
		{
			// 使该点围绕三个坐标轴做旋转运动
			RotateX(p3d[i], PI / 180);
			RotateY(p3d[i], PI / 170);
			RotateZ(p3d[i], PI / 160);
			// 根据点的深度，产生相应灰度的颜色
			c = (int)(p3d[i].z * 100) + 155;
			// 投影该点到屏幕上
			p2d = Projection(p3d[i]);
			// 画点
			drawpoint(p2d.x, p2d.y, RGB(c, c, c));
		}

		//	空间中已经有了点，将空间点按照 TIN 三角网进行连线
		int Z[MAXPOINT];								// 储存判断过的点的数组
		double max = WIDTH * HEIGHT * WIDTH;			// 用窗口的长和宽来定义一个相对大的数
		int lx;											// 储存临时 X 方向产生的变量
		int ly;											// 储存临时 Y 方向产生的变量
		int li;											// 用于储存临时判断过的点的下标数

		// 在随机生成的点中依次判断，找出距离最短的两个点
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
		// 这个地方可能需要该他的投影方式
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

			// 开始判断随机生成的每一个点
			for (int i = 0; i < MAXPOINT; i++)
			{
				m = 0;
				OK = false;

				// 判断这个点是否已经判断过，如果已经判断过，返回判断下一个点，如果不在，继续程序
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

				// 在已经确定的两个点和未确定的点进行计算它们形成三角形的的半径，并判断形成的圆内有无其它的点
				// 若无其它的点，则可以连线，如有其它的点，则进行判断下一个点
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

							// 判断圆内有无其它点，并且这个被判断的点不能为形成这个圆的这个三个点，如果有其它点，就跳出该循环
							if (distance(Xd, Yd, Zd, p3d[cc].x, p3d[cc].y, p3d[cc].z) <= Rd && cc != Z[k] && cc != Z[j] && cc != i)
							{
								OK = true;
								break;
							}
							cc++;
						}

						// 因为圆内有其它点，结束本次循环
						if (OK == true)
							continue;

						if (max >= rad && rad != 0)					// 在三个点围成圆内没有点找到半径最小的
						{
							lx = Z[j];
							ly = Z[k];

							if (rad >= WIDTH)
								continue;
							else
							{
								POINT p2d_1, p2d_2;

								if (IsLinked(i, lx) == false)		// 绘制线段，首先判断这个线段是否已经绘制过
								{
									Link(i, lx);
									c = (int)(p3d[i].z * 100) + 155;
									setlinecolor(RGB(c, c, c));
									p2d_1 = Projection(p3d[i]);
									p2d_2 = Projection(p3d[lx]);
									line(p2d_1.x, p2d_1.y, p2d_2.x, p2d_2.y);
									/*Sleep(100);*/					// 如果需要查看绘制过程，去掉注释
								}
								if (IsLinked(i, ly) == false)
								{
									Link(i, ly);
									c = (int)(p3d[ly].z * 100) + 155;
									setlinecolor(RGB(c, c, c));
									p2d_1 = Projection(p3d[i]);
									p2d_2 = Projection(p3d[ly]);
									line(p2d_1.x, p2d_1.y, p2d_2.x, p2d_2.y);
									/*Sleep(100);*/					// 如果需要查看绘制过程，去掉注释
								}
								if (IsLinked(lx, ly) == false)
								{
									Link(lx, ly);
									c = (int)(p3d[lx].z * 100) + 155;
									setlinecolor(RGB(c, c, c));
									p2d_1 = Projection(p3d[lx]);
									p2d_2 = Projection(p3d[ly]);
									line(p2d_1.x, p2d_1.y, p2d_2.x, p2d_2.y);
									/*Sleep(100);*/					// 如果需要查看绘制过程，去掉注释
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
		Sleep(10);													// 延时 10 毫秒
	}

	EndBatchDraw();
	closegraph();
	return 0;
}