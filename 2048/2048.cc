#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <curses.h>

void initIalize() {
	//init
	initscr();
	//不需要回车直接交互
	cbreak();
	//按键不显示
	noecho();
	//隐藏光标
	curs_set(0);
	//随机数
	srand(time(NULL));
}

void shutDown() {
	endwin();
}

int main()
{
	initIalize();

	char ch = 'n';
	do{
		move(5, 5);
		addch(ch);
		mvprintw(2, 2, "http://www.zyoneg.cn");
		ch = getch();
	} while(ch != 'Q' && ch != 'q');

	shutDown();
	return 0;
}
