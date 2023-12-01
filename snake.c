#include<ncurses.h>
#include<stdlib.h>

void snake();

int main(void)
{
	//freopen("/dev/tty", "rw", stdin);
	initscr();

	addstr("Use WASD to move.\n");
	refresh();

	snake();

	refresh();

	getch();

	endwin();

	return EXIT_SUCCESS;
}

typedef struct {
	int row, col;
} point_t;

void snake()
{
	int row;
	int col;
	char ch;
	point_t snake[30];
	int tail = 0;
	int done = 0;

	for (int i = 0; i < 30; i++) {
		snake[i].row = 1;
		snake[i].col = i;
		mvaddch(1, i, '.');
	}

	row = snake[29].row;
	col = snake[29].col;

	noecho();								// turns off echoing
	curs_set(0);							// turns off cursor display
	
	while(!done)
	{
		ch = getch();						// get user input as a char	
		mvaddch(snake[tail].row, snake[tail].col, ' ');

		switch(ch)
		{
			case 'w':						// if input is 'w'
				mvaddch(--row,col,'.');		// move the cursor then add '.'
				refresh();					// refresh the scre
				break;
			case 's':
				mvaddch(++row,col,'.');
				refresh();
				break;
			case 'a':
				mvaddch(row,--col,'.');
				refresh();
				break;
			case 'd':
				mvaddch(row,++col,'.');
				refresh();
				break;
			case 'q':
				done = 1;
				break;
			default:
				break;
		}

		snake[tail].row = row;
		snake[tail].col = col;
		tail = (tail + 1) % 30;
	}

	curs_set(1);							// turn on cursor display

	// addch('\n');
}
