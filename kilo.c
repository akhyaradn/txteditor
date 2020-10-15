#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios og_termios;

/*
set terminal config to default at program exit
*/
void disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &og_termios);
}

/*
enable non canonical mode
*/
void enableRawMode() {
	tcgetattr(STDIN_FILENO, &og_termios);
	atexit(disableRawMode);

	struct termios raw = og_termios;
	raw.c_cc[VTIME] = 10; //1 sec to wait input before exit
	raw.c_lflag &= ~(ECHO | ICANON);

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
	enableRawMode();;

	char c;
	while(read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
		if(iscntrl(c)) {
			printf("%d\n", c);
		} else {
			printf("%d ('%c')\n", c, c);
		}
	}
	return 0;
}
