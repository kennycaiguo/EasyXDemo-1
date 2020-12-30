///////////////////////////////////////////////
// 程序名称：支持 Alpha 信息图片最近邻内插
// 编译环境：Visual C++ 2019，EasyX_20200902
// 作者：豆焰（beanflame）
// QQ：2962672241
// 邮箱：beanflame@163.com
// 最后修改：2020-10-18
//

#include <easyx.h>	// 引用图形库头文件
#include <conio.h>
#include <cmath>
#include <iostream>

using namespace std;


// transparentimage 函数：
// 请参考 https://codeabc.cn/yangw/post/transparent-putimage 中的第五项：
// 根据 png 的 alpha 信息实现半透明贴图（基于直接操作显示缓冲区）
void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg)
{
	DWORD* dst = GetImageBuffer(dstimg);
	DWORD* src = GetImageBuffer(srcimg);
	int src_width = srcimg->getwidth();
	int src_height = srcimg->getheight();
	int dst_width = (dstimg == NULL ? getwidth() : dstimg->getwidth());
	int dst_height = (dstimg == NULL ? getheight() : dstimg->getheight());
	int iwidth = (x + src_width > dst_width) ? dst_width - x : src_width;
	int iheight = (y + src_height > dst_height) ? dst_height - y : src_height;
	if (x < 0) { src += -x;				iwidth -= -x;	x = 0; }
	if (y < 0) { src += src_width * -y;	iheight -= -y;	y = 0; }
	dst += dst_width * y + x;
	for (int iy = 0; iy < iheight; iy++)
	{
		for (int ix = 0; ix < iwidth; ix++)
		{
			int sa = ((src[ix] & 0xff000000) >> 24);
			int sr = ((src[ix] & 0xff0000) >> 16);
			int sg = ((src[ix] & 0xff00) >> 8);
			int sb = src[ix] & 0xff;
			int dr = ((dst[ix] & 0xff0000) >> 16);
			int dg = ((dst[ix] & 0xff00) >> 8);
			int db = dst[ix] & 0xff;
			dst[ix] = ((sr + dr * (255 - sa) / 255) << 16)
				| ((sg + dg * (255 - sa) / 255) << 8)
				| (sb + db * (255 - sa) / 255);
		}
		dst += dst_width;
		src += src_width;
	}
}





void NearestNeighborImage(IMAGE* dstimg, IMAGE* srcimg, int width, int height)
{
	// 初始化新图
	dstimg->Resize(width, height);
	DWORD* dst = GetImageBuffer(dstimg);
	DWORD* src = GetImageBuffer(srcimg);

	int src_width = srcimg->getwidth();
	int src_height = srcimg->getheight();

	int dst_width = (dstimg == NULL ? getwidth() : dstimg->getwidth());
	int dst_height = (dstimg == NULL ? getheight() : dstimg->getheight());

	// 比例
	float h_scale_rate = (float)src_height / height;
	float w_scale_rate = (float)src_width / width;

	for (int iy = 0; iy < height; iy++)
	{
		for (int ix = 0; ix < width; ix++)
		{
			// 比例计算原图相应坐标中
			int i_scale = h_scale_rate * iy;
			int j_scale = w_scale_rate * ix;

			DWORD* p = GetImageBuffer(srcimg);
			int psc_width = srcimg->getwidth();
			int psc_height = srcimg->getheight();

			for (int iy = 0; iy < i_scale; iy++)
			{
				p += psc_width;
			}

			dst[ix] = p[j_scale];
		}
		dst += dst_width;
		src += src_width;
	}
}



int main()
{
	int width = 800;
	int height = 600;

	// 初始化图形窗口
	initgraph(width, height);
	BeginBatchDraw();

	IMAGE RedSweetPepper, src, yyy;
	loadimage(&RedSweetPepper, _T("red_sweet_pepper.png"));	// 加载图片
	loadimage(&src, _T("red_sweet_pepper.png"), 64 * 2, 64 * 2);	// 加载放大图片
	while (true)
	{
		// 清屏
		FlushBatchDraw();

		// 绘制背景
		setfillcolor(RGB(255, 255, 255));
		fillrectangle(0, 0, width, height);

		// 最近邻内插
		NearestNeighborImage(&yyy, &RedSweetPepper, 64 * 2, 64 * 2);

		// 贴图
		putimage(0, 0, &src);
		transparentimage(NULL, 300, 0, &src);

		putimage(0, 200, &yyy);
		transparentimage(NULL, 300, 200, &yyy);
	}

	EndBatchDraw();
	closegraph();
	return 0;
}