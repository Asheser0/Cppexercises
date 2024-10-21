#include <iostream>
#include <easyx.h>
#include <vector>
#include <ctime>
using namespace std;
/*精灵*/
class Sprite
{
public:
	Sprite() :Sprite(0, 0) {};
	Sprite(int x, int y) :m_x(x), m_y(y), m_color(RED) {};
	void draw()
	{
		//填充颜色
		setfillcolor(m_color);
		fillrectangle(m_x, m_y, m_x + 10, m_y + 10);
	};
	void moveBy(int dx, int dy)
	{
		m_x += dx;
		m_y += dy;
	}
	bool collision(const Sprite& other)
	{
		return m_x == other.m_x && other.m_y == m_y;
	}

protected:
	int m_x;
	int m_y;
	COLORREF m_color;

};

/*蛇类*/
class Snake : public Sprite
{
public:
	Snake():Snake(0,0){}
	Snake(int x,int y):Sprite(x,y),dir(VK_RIGHT)
	{
		nodes.push_back(Sprite(20, 0));
		nodes.push_back(Sprite(10, 0));
		nodes.push_back(Sprite(0, 0));
	}
	void draw() 
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i].draw();
		
		}
	}

	void bodymove()
	{
		for (size_t i = nodes.size() - 1; i > 0; i--)
		{
			nodes[i] = nodes[i - 1];
		}


		switch (dir)
		{
		case VK_UP:
			nodes[0].moveBy(0, -10);
			break;
		case VK_DOWN:
			nodes[0].moveBy(0, 10);
	
			break;
		case VK_LEFT:
			nodes[0].moveBy(-10, 0);
		
			break;
		case VK_RIGHT:
				nodes[0].moveBy(10, 0);
			break;
		}
		
	}
	bool collision(const Sprite& other)
	{
		return nodes[0].collision(other);
	}
	void incrment()
	{
		nodes.push_back(Sprite());
	}


private:
	std::vector<Sprite>nodes;
public:
	int dir;

};


/*food*/
class Food :public Sprite
{
public:
	Food():Sprite(0,0)
	{
		changePos();
	}
	void draw() 
	{
		setfillcolor(m_color);
		solidellipse(m_x, m_y, m_x + 10, m_y+10);
	}
	void changePos()
	{

		m_x= rand() % 64 * 10;
		m_y= rand() % 48 * 10;

	}

};

/*游戏场景*/
class GameScene
{
public:
	GameScene() 
	{
	
	};
	void run()
	{
		BeginBatchDraw();

		cleardevice();
		
		snake.draw();
		food.draw();
		EndBatchDraw();

		snakeEatFood();

		snake.bodymove();

		ExMessage msg = { 0 };
		while (peekmessage(&msg, EM_KEY))
		{
			onMsg(msg);
		}
	}
	void onMsg(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
		{
			snake.dir = msg.vkcode;

		}
	}

	void snakeEatFood()
	{
		if (snake.collision(food))
		{
			snake.incrment();
			food.changePos();
		}
	
	}


private:
	Snake snake;
	Food food;
};
int main()
{
	initgraph(640, 480,EW_SHOWCONSOLE);
	srand(time(nullptr));
	GameScene scene;
	while (1)
	{
		scene.run();
		Sleep(100);
	}
	
	getchar();

	return 0;
}