///////////////////////////////////////////////
// 程序名称：画板
// 编译环境：Visual C++ 2017，EasyX_20180727(beta)
// 作者：冰雪女神百亚
// QQ：1169351944
// 邮箱：cloudea@163.com
// 最后修改：2019-1-18
// https://codebus.cn/contributor/a/bxnsby-drawing-board
//
#include<string>
#include<graphics.h>
using namespace std;

// 点
struct Point
{
	int x;
	int y;

	Point() { x = -1; y = -1; }

	Point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	bool operator==(const Point& p)
	{
		return x == p.x && y == p.y;
	}

	Point& operator=(const Point& p)
	{
		x = p.x; y = p.y;
		return *this;
	}
};

// 所有控件的父类
struct Tool
{
	int left;
	int top;
	int width;
	int height;

	Tool() {}

	Tool(int left, int top, int width, int height)
	{
		this->left = left;
		this->top = top;
		this->width = width;
		this->height = height;
	}

	virtual double call(MOUSEMSG) = 0;

	bool isIn(const Point& p)
	{
		if (p.x >= left && p.x <= left + width && p.y >= top && p.y <= top + height)
		{
			return true;
		}
		return false;
	}
};

// 作为矩形用
struct Tool_Range :Tool
{
	Tool_Range() {}

	Tool_Range(int left, int top, int width, int height) :Tool(left, top, width, height) {}

	double call(MOUSEMSG m)
	{
		return 0;
	}
};


// 可画画的区域
struct Tablet :Tool
{
	int size;
	COLORREF color;

	Tablet() {}

	Tablet(int left, int top, int width, int height, int size, COLORREF color) :Tool(left, top, width, height)
	{
		this->size = size;
		this->color = color;
		isDown = false;
	}

	double call(MOUSEMSG m)
	{
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (isIn(Point(m.x, m.y)))
			{
				isDown = true;
				begPos = Point(m.x, m.y);
			}
		}

		if (m.uMsg == WM_LBUTTONUP)
		{
			isDown = false;
		}

		if (m.uMsg == WM_MOUSEMOVE && isDown)
		{
			if (isIn(begPos) && isIn(Point(m.x, m.y)))		// 在区域内
			{
				setlinestyle(PS_ENDCAP_ROUND, size);
				setlinecolor(color);
				HRGN rgn = CreateRectRgn(left, top, left + width, top + height);
				setcliprgn(rgn);
				DeleteObject(rgn);
				line(begPos.x, begPos.y, m.x, m.y);
				setcliprgn(NULL);
			}
			begPos = Point(m.x, m.y);
		}

		return 1;
	}

private:
	bool isDown;
	Point begPos;
};

// 简单标签
struct Label :Tool
{
	wstring s;
	COLORREF color;
	bool isCenteral;

	Label() {}

	Label(int left, int top, int width, int height, wstring s, COLORREF color, bool isCenteral = false) :Tool(left, top, width, height)
	{
		this->color = color;
		this->s = s;
	}

	double call(MOUSEMSG m)
	{
		setfillcolor(0xefefef);
		solidrectangle(left, top, left + width, top + height);
		settextcolor(color);
		setbkmode(TRANSPARENT);
		settextstyle(24, 15, L"Courier");
		RECT rect = { left, top, left + width, top + height };
		if (isCenteral)
			drawtext(s.c_str(), &rect, DT_CENTER | DT_TOP | DT_WORDBREAK);
		else
			drawtext(s.c_str(), &rect, DT_LEFT | DT_TOP | DT_WORDBREAK);

		return 1;
	}

	bool isClick(MOUSEMSG m)
	{
		if (m.uMsg == WM_LBUTTONUP)
		{
			if (isIn(Point(m.x, m.y)))
			{
				return true;
			}
		}
		return false;
	}
};

// 圆形按钮
struct Circle :Tool
{
	COLORREF color;
	COLORREF color_over;
	bool isClick;

	Circle()
	{
		isClick = false;
	}

	Circle(int left, int top, int width, int height, COLORREF color, COLORREF color_over) :Tool(left, top, width, height)
	{
		this->color = color;
		this->color_over = color_over;
		isClick = false;
	}

	double call(MOUSEMSG m) { return 1; }

	bool call(MOUSEMSG m, Tablet& tab)
	{
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (isIn(Point(m.x, m.y)))
			{
				isClick = true;
				tab.color = color;
			}
		}

		if (m.uMsg == WM_LBUTTONUP)
		{
			if (isIn(Point(m.x, m.y)))
			{
				isClick = true;
			}
		}

		if (m.uMsg == WM_MOUSEMOVE)
		{
			if (isIn(Point(m.x, m.y)))
			{
				isOver = true;
			}
			else
			{
				isOver = false;
			}
		}

		setfillcolor(color);	// always have a color
		if (isOver) setfillcolor(color_over);
		setlinecolor(0xffffff);
		if (isClick)
		{
			setlinestyle(PS_ENDCAP_ROUND, 3);
		}
		else
		{
			setlinestyle(PS_ENDCAP_ROUND, 0);
		}
		fillellipse(left, top, left + width, top + height);
		return isClick;
	}

private:
	bool isOver;
};

//滑动条
struct Slider :Tool
{
	double percent;
	int radius;
	COLORREF bkcolor;
	COLORREF fkcolor;

	Slider() { }

	Slider(int left, int top, int width, int height, double percent = 0, int radius = 0, COLORREF bkcolor = 0x000000, COLORREF fkcolor = 0xffffff) :Tool(left, top, width, height)
	{
		this->percent = percent;
		this->radius = radius;
		this->bkcolor = bkcolor;
		this->fkcolor = fkcolor;
		setRange();
	}

	double call(MOUSEMSG m)
	{
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x >= left - radius && m.x <= left + width + radius && m.y >= top && m.y <= top + height)
			{
				isDrag = true;
				deltaY = range.top - m.y;
			}
		}

		if (m.uMsg == WM_LBUTTONUP)
		{
			isDrag = false;
		}

		if (m.uMsg == WM_MOUSEMOVE && isDrag)
		{
			int t = deltaY + m.y;
			if (t < top)
			{
				percent = 0;
			}
			else if (t > top + height - radius)
			{
				percent = 1;
			}
			else
			{
				percent = (t - top) / (double)(height - radius);
			}
		}

		setRange();
		setfillcolor(0xefefef);
		solidrectangle(left - radius, top - (radius / 2 + 2), left + width + radius, top + height + (radius / 2 + 2));
		setfillcolor(bkcolor);
		solidroundrect(left, top, left + width, top + height, 3, 3);
		setfillcolor(fkcolor);
		solidellipse(range.left, range.top, range.left + radius, range.top + radius);
		return percent;
	}

private:
	bool isDrag = false;
	int deltaY = 0;
	Tool_Range range;

	void setRange()
	{
		double t = radius / 2.0;
		double y = height - radius;
		y *= percent;
		y = top + t + y;
		double x = left + width / 2.0;
		range.left = (int)(x - t);
		range.top = (int)(y - t);
		range.width = radius;
		range.height = radius;
	}
};

int main()
{
	const Point defaultWH(1100, 768);										// 默认宽高
	initgraph(defaultWH.x, defaultWH.y);									// 初始化
	BeginBatchDraw();
	setfillcolor(0xffffff);
	fillrectangle(0, 0, defaultWH.x, defaultWH.y);							// 默认画图板背景
	setfillcolor(0xefefef);
	fillrectangle(defaultWH.x - 100, 0, defaultWH.x, defaultWH.y);			// 默认工具栏背景

	// 定义控件及其样式
	Tablet mainTablet(0, 0, defaultWH.x - 100, defaultWH.y, 5, 0x000000);
	Label label1(defaultWH.x - 100 + 27, 15, 48, 24, L"工具", 0x666666);
	Label label2(defaultWH.x - 100 + 22, 740, 60, 20, L"1", 0x666666, true);
	Slider slider1(defaultWH.x - 100 + 48, 600, 5, 120, 0, 26, 0xc3c3c3, 0x666666);
	Circle Circles[9];
	for (int i = 0; i < 9; i++)
	{
		Circles[i].left = defaultWH.x - 100 + 26;
		Circles[i].top = 50 + i * 60;
		Circles[i].width = 50;
		Circles[i].height = 50;
	}
	Circles[0].color = 0x000000;	Circles[0].color_over = 0x2e2e2e;	Circles[0].isClick = true;
	Circles[1].color = 0xffffff;	Circles[1].color_over = 0xfafafa;
	Circles[2].color = 0x1f1ae6;	Circles[2].color_over = 0x332fe6;
	Circles[3].color = 0x7018ed;	Circles[3].color_over = 0x863eed;
	Circles[4].color = 0xed3eeb;	Circles[4].color_over = 0xed76ec;
	Circles[5].color = 0xad2f09;	Circles[5].color_over = 0xad5c43;
	Circles[6].color = 0xcfd12f;	Circles[6].color_over = 0xd0d15d;
	Circles[7].color = 0x11f41c;	Circles[7].color_over = 0x4ef456;
	Circles[8].color = 0x0c98d2;	Circles[8].color_over = 0x4dabd2;

	while (true)
	{
		while (MouseHit())
		{
			MOUSEMSG m = GetMouseMsg();

			//控件之间的交互关系
			mainTablet.call(m);
			double percent = slider1.call(m);
			int progress = (int)(1 + percent * percent * 299);
			mainTablet.size = progress;
			wchar_t protext[4];
			_itow_s(progress, protext, 10);
			label2.s = protext;
			int whichCircle = -1, whichCircleChoosed = -1;
			for (int i = 0; i < 9; i++)									// find last
			{
				if (Circles[i].isClick) whichCircleChoosed = i;
			}
			for (int i = 0; i < 9; i++)									// find last + new
			{
				if (Circles[i].call(m, mainTablet) && whichCircleChoosed != i)
					whichCircle = i;
			}
			if (whichCircle != -1)
			{
				Circles[whichCircle].isClick = true;
				Circles[whichCircleChoosed].isClick = false;
			}
			if (label1.isClick(m))
			{
				saveimage(L"easyxImage.bmp");
			}

		}
		label1.call(MOUSEMSG());
		label2.call(MOUSEMSG());
		FlushBatchDraw();
		Sleep(50);
	}

	system("pause");
	EndBatchDraw();
	closegraph();
}