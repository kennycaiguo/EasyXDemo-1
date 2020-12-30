/*
��Ŀ���ƣ�2048
�������ߣ�Visual Studio 2017��EasyX 20190415(beta)
��Ŀ���ͣ�Windows Consoal Application
���ߣ����
QQ��1101458726
Email��1101458726@qq.com
�������ڣ�2019-4-17
����޸ģ�2019-4-24
https://codebus.cn/luckyfeng/a/2048
*/

#include <easyx.h>
#include <string.h>
#include <map>
#include <sstream>
#include <time.h>
#include <conio.h>

#pragma comment( lib, "MSIMG32.LIB")


// �����״̬
enum State
{
	EXIST,		// ����
	DESTORY		// ����
};


// ��ά���������ڱ�ʾλ�û��ߴ�С
struct Vector2
{
	float x;
	float y;
};


// ���ź���
int sgn(float d)
{
	if (d < 0)	return -1;
	if (d > 0)	return  1;
	return 0;
}


class Block
{
private:
	State	currentState;		// ��ǰ��״̬
	State	targetState;		// �ƶ����״̬
	Vector2	size;
	Vector2	currentPos;			// ��ǰλ��
	Vector2	targetPos;			// Ŀ��λ��
	IMAGE* img;
	IMAGE* newImg;
	float	deltaPos;			// ÿ���ƶ�����λ��
	float	deltaSize;			// ÿ�������
	float	animationSpeed;		// �����ٶ�


public:
	Block(const Vector2& pos, IMAGE* img)
	{
		currentPos = targetPos = pos;
		currentState = targetState = EXIST;
		size = { 50,50 };
		this->img = this->newImg = img;

		deltaPos = 100;
		deltaSize = 40;
		animationSpeed = 20.0f;
	}


	void update(float deltaTime)
	{
		// �ı䷽���С��ͼƬ������ʱ����С����Ķ�����
		if (size.x < img->getwidth())
		{
			size.x = size.y = size.x + deltaSize * deltaTime * animationSpeed / 2;
			if (size.x > img->getwidth())
			{
				size.x = size.y = (float)img->getwidth();
			}
		}

		// ���·���λ��
		if (currentPos.x != targetPos.x || currentPos.y != targetPos.y)
		{
			int directionX = sgn(targetPos.x - currentPos.x);
			int directionY = sgn(targetPos.y - currentPos.y);

			currentPos.x += deltaPos * directionX * deltaTime * animationSpeed;
			// ���С�� 5 ��Ϊ��ͬһλ��
			if (fabs(currentPos.x - targetPos.x) < 5)
			{
				currentPos.x = targetPos.x;
			}

			currentPos.y += deltaPos * directionY * deltaTime * animationSpeed;
			if (fabs(currentPos.y - targetPos.y) < 5)
			{
				currentPos.y = targetPos.y;
			}
		}
		if (currentPos.x == targetPos.x && currentPos.y == targetPos.y)
		{
			currentState = targetState;
			img = newImg;
		}
	}


	void draw()
	{
		TransparentBlt(GetImageHDC(NULL), int(currentPos.x + (90 - size.x) / 2), int(currentPos.y + (90 - size.y) / 2),
			(int)size.x, (int)size.y, GetImageHDC(img), 0, 0, img->getwidth(), img->getheight(), BLACK);
	}


	// �ѷ���ӵ�ǰλ���ƶ���Ŀ��λ�ã��ƶ���ı�״̬
	void MoveTo(const Vector2& pos, IMAGE* newImg, State state = EXIST)
	{
		targetPos = pos;
		targetState = state;
		this->newImg = newImg;
	}


	State getState()
	{
		return currentState;
	}
};



int		map[4][4];				// 4 * 4 ��ͼ
Block* blockMap[4][4];		// ��������
int		score;					// �÷�
int		maxScore;				// ��ߵ÷�
int		currentMaxBlock;		// ��ǰ��󷽿�
int		maxBlock;				// ��ʷ��󷽿�
int		gameLoop;				// ��Ϸѭ��
float	keyTime = 0;			// �������
std::map<int, IMAGE> image;		// �洢��������ͼ��
bool	gameOver = false;		// ��Ϸ�Ƿ����
float	overTime;				// ��Ϸ�����󲻻������˳�ѭ�������ǵȴ� 0.5s ���¶���


// �ж��Ƿ��п��ƶ��ķ�ʽ���з��� 1 ��û�з��� 0
// ���˼·���������Ϊ 0 �ĸ��ӣ������������ڵĸ���������ȣ��򷵻� 1
int Judge()
{
	// ������
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (map[i][j] == 0 || map[i][j] == map[i][j + 1] || map[i][j + 1] == 0)
				return 1;
		}
	}

	// ������
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (map[j][i] == 0 || map[j][i] == map[j + 1][i] || map[j + 1][i] == 0)
				return 1;
		}
	}

	return 0;
}


// ����
void Up()
{
	int moveFlag = 0;	// ��¼�Ƿ���й��ƶ�
	int mergeFlag = 0;	// ��¼�Ƿ�ϲ���

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int k, z;

			// �ҵ�һ����Ϊ 0 �ķ��������ƶ������ж��Ƿ���Ժ��±ߵķ���ϲ�
			for (k = j; k < 4; k++)
				if (map[k][i] != 0)
					break;

			// Ѱ���ұ߲�Ϊ 0 �ķ���
			for (z = k + 1; z < 4; z++)
				if (map[z][i] != 0)
					break;

			// ��ǰ���з� 0 ����
			if (k < 4)
			{
				if (z < 4 && map[k][i] == map[z][i])
				{
					// ���Ժϲ�
					int value = map[k][i] + map[z][i];
					map[k][i] = map[z][i] = 0;
					map[j][i] = value;

					// ��������
					Block* temp = blockMap[k][i];
					blockMap[k][i] = NULL;
					blockMap[j][i] = temp;
					blockMap[j][i]->MoveTo({ 25.0f + 100 * i,225.0f + 100 * j }, &image[map[j][i]]);
					blockMap[z][i]->MoveTo({ 25.0f + 100 * i,225.0f + 100 * (j + 1) }, &image[map[z][i]], DESTORY);

					// ���·���
					score += map[j][i];
					if (score > maxScore) maxScore = score;

					// ���·���
					if (value > currentMaxBlock) currentMaxBlock = value;
					if (currentMaxBlock > maxBlock) maxBlock = currentMaxBlock;

					mergeFlag = 1;
				}
				else
				{
					// �����Ժϲ�
					int value = map[k][i];
					map[k][i] = 0;
					map[j][i] = value;

					// �ж��Ƿ�����ƶ�
					if (k != j)
					{
						moveFlag = 1;

						// ��������
						Block* temp = blockMap[k][i];
						blockMap[k][i] = NULL;
						blockMap[j][i] = temp;
						blockMap[j][i]->MoveTo({ 25.0f + 100 * i,225.0f + 100 * j }, &image[map[j][i]]);
					}
				}
			}
			else		// �ж���һ��
			{
				break;
			}
		}
	}

	// ����������ƶ���ϲ����������һ�� 2 �� 4
	if (moveFlag || mergeFlag)
	{
		int index;	// ���λ�õ�����

		// ֱ�������һ����λ���˳�ѭ��
		do
		{
			index = rand() % 4;
		} while (map[3][index] != 0);

		// 80% ���� 2 , 20% ���� 4
		int num = rand() % 10;
		if (num < 8)
		{
			map[3][index] = 2;
			blockMap[3][index] = new Block({ 25.0f + 100 * index, 225.0f + 100 * 3 }, &image[2]);
		}
		else
		{
			map[3][index] = 4;
			blockMap[3][index] = new Block({ 25.0f + 100 * index, 225.0f + 100 * 3 }, &image[4]);
		}
	}
}


// ����
void Down()
{
	int moveFlag = 0;	// ��¼�Ƿ���й��ƶ�
	int mergeFlag = 0;	// ��¼�Ƿ�ϲ���

	for (int i = 0; i < 4; i++)
	{
		for (int j = 3; j > 0; j--)
		{
			int k, z;

			// �ҵ�һ����Ϊ 0 �ķ��������ƶ������ж��Ƿ���Ժ��ϱߵķ���ϲ�
			for (k = j; k >= 0; k--)
				if (map[k][i] != 0)
					break;

			// Ѱ���ұ߲�Ϊ 0 �ķ���
			for (z = k - 1; z >= 0; z--)
				if (map[z][i] != 0)
					break;

			// ��ǰ���з� 0 ����
			if (k >= 0)
			{
				if (z >= 0 && map[k][i] == map[z][i])
				{
					// ���Ժϲ�
					int value = map[k][i] + map[z][i];
					map[k][i] = map[z][i] = 0;
					map[j][i] = value;

					// ��������
					Block* temp = blockMap[k][i];
					blockMap[k][i] = NULL;
					blockMap[j][i] = temp;
					blockMap[j][i]->MoveTo({ 25.0f + 100 * i,225.0f + 100 * j }, &image[map[j][i]]);
					blockMap[z][i]->MoveTo({ 25.0f + 100 * i,225.0f + 100 * (j - 1) }, &image[map[z][i]], DESTORY);

					// ���·���
					score += map[j][i];
					if (score > maxScore) maxScore = score;

					// ���·���
					if (value > currentMaxBlock) currentMaxBlock = value;
					if (currentMaxBlock > maxBlock) maxBlock = currentMaxBlock;

					mergeFlag = 1;
				}
				else
				{
					// �����Ժϲ�
					int value = map[k][i];
					map[k][i] = 0;
					map[j][i] = value;

					// �ж��Ƿ�����ƶ�
					if (k != j)
					{
						moveFlag = 1;
						// ��������
						Block* temp = blockMap[k][i];
						blockMap[k][i] = NULL;
						blockMap[j][i] = temp;
						blockMap[j][i]->MoveTo({ 25.0f + 100 * i,225.0f + 100 * j }, &image[map[j][i]]);
					}
				}
			}
			else		// �ж���һ��
			{
				break;
			}
		}
	}

	// ����������ƶ���ϲ����������һ�� 2 �� 4
	if (moveFlag || mergeFlag)
	{
		int index;	// ���λ�õ�����

		// ֱ�������һ��Ϊ 0 ��λ���˳�ѭ��
		do
		{
			index = rand() % 4;
		} while (map[0][index] != 0);

		// 80% ���� 2 , 20% ���� 4
		int num = rand() % 10;
		if (num < 8)
		{
			map[0][index] = 2;
			blockMap[0][index] = new Block({ 25.0f + 100 * index,225.0f + 100 * 0 }, &image[2]);
		}
		else
		{
			map[0][index] = 4;
			blockMap[0][index] = new Block({ 25.0f + 100 * index,225.0f + 100 * 0 }, &image[4]);
		}
	}
}


// ����
void Left()
{
	int moveFlag = 0;	// ��¼�Ƿ���й��ƶ�
	int mergeFlag = 0;	// ��¼�Ƿ�ϲ���

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int k, z;

			// �ҵ�һ����Ϊ 0 �ķ��������ƶ������ж��Ƿ���Ժ��ұߵķ���ϲ�
			for (k = j; k < 4; k++)
				if (map[i][k] != 0)
					break;

			// Ѱ���ұ߲�Ϊ 0 �ķ���
			for (z = k + 1; z < 4; z++)
				if (map[i][z] != 0)
					break;

			// ��ǰ���з� 0 ����
			if (k < 4)
			{
				if (z < 4 && map[i][k] == map[i][z])
				{
					// ���Ժϲ�
					int value = map[i][k] + map[i][z];
					map[i][k] = map[i][z] = 0;
					map[i][j] = value;

					// ��������
					Block* temp = blockMap[i][k];
					blockMap[i][k] = NULL;
					blockMap[i][j] = temp;
					blockMap[i][j]->MoveTo({ 25.0f + 100 * j,225.0f + 100 * i }, &image[value]);
					blockMap[i][z]->MoveTo({ 25.0f + 100 * (j + 1),225.0f + 100 * i }, &image[map[z][i]], DESTORY);

					// ���·���
					score += map[i][j];
					if (score > maxScore) maxScore = score;

					// ���·���
					if (value > currentMaxBlock) currentMaxBlock = value;
					if (currentMaxBlock > maxBlock) maxBlock = currentMaxBlock;

					mergeFlag = 1;
				}
				else
				{
					// �����Ժϲ�
					int value = map[i][k];
					map[i][k] = 0;
					map[i][j] = value;

					// �ж��Ƿ�����ƶ�
					if (k != j)
					{
						moveFlag = 1;
						// ��������
						Block* temp = blockMap[i][k];
						blockMap[i][k] = NULL;
						blockMap[i][j] = temp;
						blockMap[i][j]->MoveTo({ 25.0f + 100 * j,225.0f + 100 * i }, &image[value]);
					}
				}
			}
			else		// �ж���һ��
			{
				break;
			}
		}
	}

	// ����������ƶ���ϲ����������һ�� 2 �� 4
	if (moveFlag || mergeFlag)
	{

		int index;	// ���λ�õ�����

		// ֱ�������һ��Ϊ 0 ��λ���˳�ѭ��
		do
		{
			index = rand() % 4;
		} while (map[index][3] != 0);

		// 80% ���� 2 , 20% ���� 4
		int num = rand() % 10;
		if (num < 8)
		{
			map[index][3] = 2;
			blockMap[index][3] = new Block({ 25.0f + 100 * 3,225.0f + 100 * index }, &image[2]);
		}
		else
		{
			map[index][3] = 4;
			blockMap[index][3] = new Block({ 25.0f + 100 * 3,225.0f + 100 * index }, &image[4]);
		}
	}
}


// ����
void Right()
{
	int moveFlag = 0;	// ��¼�Ƿ���й��ƶ�
	int mergeFlag = 0;	// ��¼�Ƿ�ϲ���

	for (int i = 0; i < 4; i++)
	{
		for (int j = 3; j > 0; j--)
		{
			int k, z;

			// �ҵ�һ����Ϊ 0 �ķ��������ƶ������ж��Ƿ���Ժ���ߵķ���ϲ�
			for (k = j; k >= 0; k--)
				if (map[i][k] != 0)
					break;

			// Ѱ���ұ߲�Ϊ 0 �ķ���
			for (z = k - 1; z >= 0; z--)
				if (map[i][z] != 0)
					break;

			// ��ǰ���з� 0 ����
			if (k >= 0)
			{
				if (z >= 0 && map[i][k] == map[i][z])
				{
					// ���Ժϲ�
					int value = map[i][k] + map[i][z];
					map[i][k] = map[i][z] = 0;
					map[i][j] = value;

					// ��������
					Block* temp = blockMap[i][k];
					blockMap[i][k] = NULL;
					blockMap[i][j] = temp;
					blockMap[i][j]->MoveTo({ 25.0f + 100 * j,225.0f + 100 * i }, &image[value]);
					blockMap[i][z]->MoveTo({ 25.0f + 100 * (j - 1),225.0f + 100 * i }, &image[map[z][i]], DESTORY);

					// ���·���
					score += map[i][j];
					if (score > maxScore) maxScore = score;

					// ���·���
					if (value > currentMaxBlock) currentMaxBlock = value;
					if (currentMaxBlock > maxBlock) maxBlock = currentMaxBlock;

					mergeFlag = 1;
				}
				else
				{
					// �����Ժϲ�
					int value = map[i][k];
					map[i][k] = 0;
					map[i][j] = value;

					// �ж��Ƿ�����ƶ�
					if (k != j)
					{
						moveFlag = 1;
						// ��������
						Block* temp = blockMap[i][k];
						blockMap[i][k] = NULL;
						blockMap[i][j] = temp;
						blockMap[i][j]->MoveTo({ 25.0f + 100 * j,225.0f + 100 * i }, &image[value]);
					}
				}
			}
			else		// �ж���һ��
			{
				break;
			}
		}
	}

	// ����������ƶ���ϲ����������һ�� 2 �� 4
	if (moveFlag || mergeFlag)
	{
		int index;	// ���λ�õ�����
		do
		{
			index = rand() % 4;
		} while (map[index][0] != 0);

		// 80% ���� 2 , 20% ���� 4
		int num = rand() % 10;
		if (num < 8)
		{
			map[index][0] = 2;
			blockMap[index][0] = new Block({ 25.0f + 100 * 0,225.0f + 100 * index }, &image[2]);
		}
		else
		{
			map[index][0] = 4;
			blockMap[index][0] = new Block({ 25.0f + 100 * 0,225.0f + 100 * index }, &image[4]);
		}
	}
}


void Update(float deltaTime)
{
	// ���·���
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (blockMap[i][j] != NULL)
			{
				blockMap[i][j]->update(deltaTime);
				if (blockMap[i][j]->getState() == DESTORY)
				{
					delete blockMap[i][j];
					blockMap[i][j] = NULL;
				}
			}
		}
	}

	if (gameOver)
	{
		overTime -= deltaTime;
		if (overTime <= 0)
			gameLoop = 0;
	}

	keyTime += deltaTime;
	// 0.2s ���԰���һ��
	if (keyTime < 0.2f || gameOver)
		return;

	if ((GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState('W') & 0x8000))			// ��
	{
		Up();
		if (!Judge())
		{
			gameOver = true;
		}
		keyTime = 0;
	}
	else if ((GetAsyncKeyState(VK_DOWN) & 0x8000) || (GetAsyncKeyState('S') & 0x8000))	// ��
	{
		Down();
		if (!Judge())
		{
			gameOver = true;
		}
		keyTime = 0;
	}
	else if ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000))	// ��
	{
		Left();
		if (!Judge())
		{
			gameOver = true;
		}
		keyTime = 0;
	}
	else if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState('D') & 0x8000))	// ��
	{
		Right();
		if (!Judge())
		{
			gameOver = true;
		}
		keyTime = 0;
	}
}


// ����������ʽ����ɫ
void settext(int height, int weight, UINT color)
{
	settextstyle(height, 0, _T("Arial"), 0, 0, weight, false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH);
	settextcolor(color);
}


// ��ָ�����������ھ�������ַ���
void printtext(LPCTSTR s, int left, int top, int right, int width)
{
	RECT r = { left, top, right, width };
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}


// ���ƽ���
void Draw()
{
	// ��ʷ��󷽿�
	TransparentBlt(GetImageHDC(NULL), 12, 30, 90, 90, GetImageHDC(&image[maxBlock]), 0, 0, 90, 90, 0x9eaebb);

	setfillcolor(0x9eaebb);
	// ���Ƶ�ǰ����
	solidroundrect(112, 30, 264, 119, 10, 10);
	settext(28, 800, 0xdbe6ee);
	printtext(_T("SCORE"), 112, 40, 264, 69);
	std::wstringstream ss;
	ss << score;
	settext(44, 800, WHITE);
	printtext(ss.str().c_str(), 112, 70, 264, 114);
	ss.str(_T(""));

	// ������߷���
	solidroundrect(275, 30, 427, 119, 10, 10);
	settext(28, 800, 0xdbe6ee);
	printtext(_T("BEST"), 275, 40, 427, 69);
	ss << maxScore;
	settext(44, 800, WHITE);
	printtext(ss.str().c_str(), 275, 70, 427, 114);
	ss.str(_T(""));

	// ������ʾ��Ϣ
	settextcolor(BLACK);
	ss << "Join the numbers and get to the " << currentMaxBlock * 2 << " tile!";
	settext(24, 800, 0x707b83);
	printtext(ss.str().c_str(), 0, 120, 439, 211);

	// ���Ʒ���װ�
	solidroundrect(12, 212, 427, 627, 10, 10);

	// ���Ʒ���
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			putimage(25 + 100 * j, 225 + 100 * i, &image[0]);
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (blockMap[i][j] != NULL)
				blockMap[i][j]->draw();
		}
	}
}


// ��ʼ����Ϸ
void Init()
{
	srand((unsigned int)time(NULL));		// ��ʼ�����������

	memset(map, 0, 4 * 4 * sizeof(int));	// �ѵ�ͼ��ʼ��Ϊ 0
	memset(blockMap, 0, 4 * 4 * sizeof(Block*));

	score = 0;
	gameLoop = 1;
	gameOver = false;
	overTime = 0.5f;
	currentMaxBlock = 2;
	map[0][0] = 2;
	map[0][1] = 2;
	blockMap[0][0] = new Block({ 25,225 }, &image[2]);
	blockMap[0][1] = new Block({ 125,225 }, &image[2]);

	setbkcolor(WHITE);
	setbkmode(TRANSPARENT);
}


// ��Ϸ�������� ���� 1 ��ʾ������Ϸ ���� 0 ��ʾ������Ϸ
int OverInterface()
{
	// ������߼�¼
	std::wstringstream ss;
	ss << maxScore;
	WritePrivateProfileString(_T("2048"), _T("MaxScore"), ss.str().c_str(), _T(".\\data.ini"));
	ss.str(_T(""));
	ss << maxBlock;
	WritePrivateProfileString(_T("2048"), _T("MaxBlock"), ss.str().c_str(), _T(".\\data.ini"));

	setbkmode(TRANSPARENT);
	setbkcolor(0x8eecff);
	cleardevice();

	// Game Over
	settext(60, 1000, 0x696f78);
	printtext(_T("Game Over!"), 0, 0, 439, 199);

	// ������󷽿�
	TransparentBlt(GetImageHDC(NULL), 175, 150, 90, 90, GetImageHDC(&image[currentMaxBlock]), 0, 0, 90, 90, 0x9eaebb);

	// ReStart
	setfillcolor(0x9dadba);
	solidroundrect(120, 310, 319, 389, 10, 10);
	settext(36, 1000, WHITE);
	printtext(_T("ReStart"), 120, 310, 319, 389);
	// Exit
	solidroundrect(120, 460, 319, 539, 10, 10);
	printtext(_T("Exit"), 120, 460, 319, 539);

	FlushBatchDraw();

	FlushMouseMsgBuffer();

	while (1)
	{
		while (MouseHit())
		{
			MOUSEMSG msg = GetMouseMsg();
			if (msg.mkLButton)
			{
				int x = msg.x;
				int y = msg.y;
				if (x >= 120 && x <= 319 && y >= 310 && y <= 389)
					return 1;
				if (x >= 120 && x <= 319 && y >= 460 && y <= 539)
					return 0;
			}
		}
		Sleep(100);
	}
	return 1;
}


// �ͷ��ڴ�
void FreeMem()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (blockMap[i][j] != NULL)
				delete blockMap[i][j];
}


// �������ɷ���ͼƬ
//		img:		����ͼƬָ��
//		num:		�����ϵ�����
//		imgColor:	������ɫ
//		fontSize:	�����С
//		fontColor:	������ɫ	
void CreateImage(IMAGE* img, LPCTSTR num, COLORREF imgColor, int fontSize, COLORREF fontColor)
{
	SetWorkingImage(img);
	setbkmode(TRANSPARENT);
	setbkcolor(0x9eaebb);
	settext(fontSize, 1000, fontColor);
	setfillcolor(imgColor);
	settextcolor(fontColor);

	cleardevice();

	solidroundrect(0, 0, img->getwidth() - 1, img->getheight() - 1, 10, 10);

	RECT r = { 0,0,img->getwidth() - 1,img->getheight() - 1 };
	drawtext(num, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

// ����ͼƬ����
void Load()
{
	IMAGE temp(90, 90);

	CreateImage(&temp, _T(""), 0xb5becc, 72, WHITE);		image[0] = temp;
	CreateImage(&temp, _T("2"), 0xdbe6ee, 72, 0x707b83);		image[2] = temp;
	CreateImage(&temp, _T("4"), 0xc7e1ed, 72, 0x707b83);		image[4] = temp;
	CreateImage(&temp, _T("8"), 0x78b2f4, 72, WHITE);		image[8] = temp;
	CreateImage(&temp, _T("16"), 0x538ded, 72, WHITE);		image[16] = temp;
	CreateImage(&temp, _T("32"), 0x607df6, 72, WHITE);		image[32] = temp;
	CreateImage(&temp, _T("64"), 0x3958e9, 72, WHITE);		image[64] = temp;
	CreateImage(&temp, _T("128"), 0x6bd9f5, 56, WHITE);		image[128] = temp;
	CreateImage(&temp, _T("256"), 0x4bd0f2, 56, WHITE);		image[256] = temp;
	CreateImage(&temp, _T("512"), 0x2ac0e4, 56, WHITE);		image[512] = temp;
	CreateImage(&temp, _T("1024"), 0x13b8e3, 40, WHITE);		image[1024] = temp;
	CreateImage(&temp, _T("2048"), 0x00c5eb, 40, WHITE);		image[2048] = temp;
	CreateImage(&temp, _T("4096"), 0x3958e9, 40, WHITE);		image[4096] = temp;
	CreateImage(&temp, _T("8192"), 0x3958e9, 40, WHITE);		image[8192] = temp;

	SetWorkingImage(NULL);
}


// ������
int main()
{
	float deltaTime = 0;	// ÿ֡��ʱ

	initgraph(440, 650);
	Load();
	BeginBatchDraw();

	maxScore = 0;

	// ��ȡ��߷�
	maxScore = GetPrivateProfileInt(_T("2048"), _T("MaxScore"), 0, _T(".\\data.ini"));
	// ��ȡ��󷽿�
	maxBlock = GetPrivateProfileInt(_T("2048"), _T("MaxBlock"), 2, _T(".\\data.ini"));

	while (1)
	{
		Init();

		while (gameLoop)
		{
			clock_t start = clock();

			cleardevice();
			Update(deltaTime);
			Draw();
			FlushBatchDraw();
			Sleep(1);

			clock_t end = clock();
			deltaTime = (end - start) / 1000.0f;
		}

		FreeMem();

		if (OverInterface() == 0)
			break;

		FlushMouseMsgBuffer();
	}

	closegraph();
}