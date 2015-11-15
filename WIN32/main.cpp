#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{	WNDCLASS wndclass;
	char strClassName[] = "hungry snake";
	char strWindowName[] = "贪吃蛇";
	HWND hwnd;
	MSG msg;

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(hInstance,IDC_ARROW);
	wndclass.hIcon = LoadIcon(hInstance,IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = strClassName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = 0;

	if(!RegisterClass(&wndclass))
	{	MessageBeep(0);
		return FALSE;
	}

	hwnd = CreateWindow(strClassName,
						strWindowName,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						NULL,
						NULL,
						hInstance,
						NULL);

	ShowWindow(hwnd,nCmdShow);

	UpdateWindow(hwnd);

	while(GetMessage(&msg,NULL,0,0))
	{	TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;

}

LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{	
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush;
	HPEN hPen;
	struct Snake
	{
		int dir;
		int x;
		int y;
	};
	static Snake snake[240000];
	static int snake_top = 1;
	static int X1 = 200,Y1 = 100;//屏幕上顶点坐标
	static int X2 = 800,Y2 = 500;//屏幕下顶点坐标
	static int deta = 10;//蛇单元格步长
	static int x_head = 500,y_head = 300;//初始化蛇头 整个屏幕的中部 蛇头为红色
	static int detawalk = 10;//初始化移动步长
	static bool dir[4];//初始化上、下、左、右四个方向
	static int all_dir = -1;
	static bool game_start = false,food = false;
	static int x_food,y_food;
	static int flag;
	static int speed = 200;
	static int count = 0;
	static int top_num;
	static char num[10];
	snake[0].x = x_head;
	snake[0].y = y_head;
	snake[0].dir = all_dir;
	switch(msg)
	{	
		case WM_LBUTTONDOWN:
			if(all_dir == -1)
			{
				all_dir = 0;
				dir[0] = true;
				game_start = true;
				x_head = 500;
				y_head = 300;
				food = false;
				snake_top = 1;
				num[0] = '0';
				count = 0;
			}
			InvalidateRect(hwnd,NULL,1);
			break;
		case WM_RBUTTONDOWN:
			if(all_dir != -1)
			{
				if(dir[all_dir] == false)
				{
					dir[all_dir] = true;
					game_start = true;
				}
				else
				{
					dir[all_dir] = false;
					game_start = false;
				}
			}
			InvalidateRect(hwnd,NULL,1);
			break;
		case WM_CHAR:
			if(game_start)
			switch(wParam)
				{	
					case '0':
						speed = 50;
						break;
					case '1':
						speed = 100;
						break;
					case '2':
						speed = 200;
						break;
					case '3':
						speed = 400;
						break;
					case 'w':
					case 'W':
						dir[all_dir] = false;
						all_dir = 0;
						dir[all_dir] = true;
						break;
					case 's':
					case 'S':
						dir[all_dir] = false;
						all_dir = 1;
						dir[all_dir] = true;
						break;
					case 'a':
					case 'A':
						dir[all_dir] = false;
						all_dir = 2;
						dir[all_dir] = true;
						break;
					case 'd':
					case 'D':
						dir[all_dir] = false;
						all_dir = 3;
						dir[all_dir] = true;
						break;
			}
			InvalidateRect(hwnd,NULL,1);
			break;
		case WM_PAINT:
			hdc = BeginPaint(hwnd,&ps);
			if(all_dir != -1&&game_start == false)
			{
				SetTextColor(hdc,RGB(0,255,0));
				TextOut(hdc,910,55,"游戏暂停中！",12);
				TextOut(hdc,910,75,"点击右键开始",12);
				SetTextColor(hdc,RGB(0,0,0));
			}
			TextOut(hdc,910,95,"速度控制：",10);
			TextOut(hdc,910,115,"键盘1：快速",11);
			TextOut(hdc,910,135,"键盘2：中速",11);
			TextOut(hdc,910,155,"键盘3：慢速",11);
			TextOut(hdc,910,175,"键盘0：超快速",13);
			SetTextColor(hdc,RGB(255,0,0));
			TextOut(hdc,910,255,"590 制作",8);
			TextOut(hdc,10,55,"贪吃蛇",6);
			SetTextColor(hdc,RGB(0,0,0));
			TextOut(hdc,10,75,"具体操作：",10);
			TextOut(hdc,10,95,"鼠标：",6);
			TextOut(hdc,10,115,"左键控制游戏开始",16);
			TextOut(hdc,10,135,"右键控制游戏暂停",16);
			TextOut(hdc,10,155,"键盘：",6);
			TextOut(hdc,10,175,"    上",6);
			TextOut(hdc,10,195,"左下右",6);
			TextOut(hdc,10,215,"    W",5);
			TextOut(hdc,10,235,"A  S  D",7);
			TextOut(hdc,10,255,"初始化方向：向上",16);
			TextOut(hdc,10,275,"初始化速度：中速",16);
			TextOut(hdc,10,295,"已吃食物数：",12);
			SetTextColor(hdc,RGB(255,0,0));
			if(top_num == 0)top_num = 1;
			TextOut(hdc,10,315,num,top_num);
			SetTextColor(hdc,RGB(0,0,0));
			hPen = CreatePen(PS_INSIDEFRAME,1,RGB(0,0,0));
			SelectObject(hdc,hPen);
			Rectangle(hdc,X1-10,Y1-10,X2+10,Y2+10);
			Rectangle(hdc,X1,Y1,X2,Y2);
			DeleteObject(hPen);

			/*画蛇身*/
			hBrush = CreateSolidBrush(RGB(255,0,0));
			SelectObject(hdc,hBrush);
			int i;
			for(i = 1;i < snake_top;i++)
			{
				Rectangle(hdc,snake[i].x,snake[i].y,snake[i].x + deta,snake[i].y + deta);

			}
			DeleteObject(hBrush);
			/*画蛇头*/
			hBrush = CreateSolidBrush(RGB(0,0,0));
			SelectObject(hdc,hBrush);
			Rectangle(hdc,snake[0].x,snake[0].y,snake[0].x+deta,snake[0].y+deta);		
			DeleteObject(hBrush);
			if(game_start == true&&food == false)
			{
				food = true;
				srand( (unsigned)time( NULL ) );
				//随机生成0-40、0-60之间的一个数
				int temp_x = rand()%60;
				int temp_y = rand()%40;
				x_food = 200 + temp_x*10;
				y_food = 100 + temp_y*10;
			}
			if(food == true)
			{
				hBrush = CreateSolidBrush(RGB(0,0,255));
				SelectObject(hdc,hBrush);
				Rectangle(hdc,x_food,y_food,x_food+deta,y_food+deta);		
				DeleteObject(hBrush);
			}
			if(game_start)
			{
				int i;
				if(dir[0])
				{
					x_head += 0;
					y_head += -deta;
				}
				if(dir[1])
				{
					x_head += 0;
					y_head += deta;
				}
				if(dir[2])
				{
					x_head += -deta;
					y_head += 0;
				}
				if(dir[3])
				{
					x_head += deta;
					y_head += 0;
				}
				flag = 0;
				for(i = 1;i < snake_top;i++)
				{
					if(x_head == snake[i].x&&y_head == snake[i].y)
					{
						flag = 1;
					}
				}
				if(flag == 0)
				{
					for(i = snake_top-1;i >= 1;i--)
					{
						snake[i].x = snake[i - 1].x;
						snake[i].y = snake[i - 1].y;
						snake[i].dir = snake[i - 1].dir;
					}
					snake[0].x = x_head;
					snake[0].y = y_head;
					snake[0].dir = all_dir;
				}
				if(x_head > X2-10|| x_head < X1||y_head > Y2-10|| y_head < Y1||flag==1)
				{
					dir[all_dir] = false;
					all_dir = -1;
					game_start = false;
				}
				if(x_head == x_food && y_head == y_food&&game_start == true)
				{
					count++;
					food = false;
					snake[snake_top].dir = snake[snake_top - 1].dir; 
					if(snake[snake_top - 1].dir == 0)
					{
						snake[snake_top].x = snake[snake_top - 1].x;
						snake[snake_top].y = snake[snake_top - 1].y + 10;
					}
					if(snake[snake_top - 1].dir == 1)
					{
						snake[snake_top].x = snake[snake_top - 1].x;
						snake[snake_top].y = snake[snake_top - 1].y - 10;
					}
					if(snake[snake_top - 1].dir == 2)
					{
						snake[snake_top].x = snake[snake_top - 1].x + 10;
						snake[snake_top].y = snake[snake_top - 1].y;
					}
					if(snake[snake_top - 1].dir == 3)
					{
						snake[snake_top].x = snake[snake_top - 1].x - 10;
						snake[snake_top].y = snake[snake_top - 1].y;
					}
					snake_top++;
					int temp = count;
					top_num = 0;
					
					while(temp > 0)
					{
						num[top_num++] = temp%10 + '0';
						temp/=10;
					}
					
					int i,k = 0;
					char str[10];
					for(i = top_num-1;i >= 0;i--)
					{
						str[k++] = num[i];
					}
					strcpy(num,str);
				}
			}
			if((game_start == false)&&(x_head > X2-10|| x_head < X1||y_head > Y2-10|| y_head < Y1||flag == 1)&&all_dir == -1)
			{
				SetTextColor(hdc,RGB(255,255,0));
				TextOut(hdc,480,260,"Game Over!",10);
				TextOut(hdc,480,280,"点击左键开始",12);
				SetTextColor(hdc,RGB(0,0,0));
			}
			EndPaint(hwnd,&ps);
			Sleep(speed);
			InvalidateRect(hwnd,NULL,1);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
		default:
			return DefWindowProc(hwnd,msg,wParam,lParam);
	}
	return 0;
}
