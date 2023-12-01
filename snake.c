#include<ncurses.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

void* snake(void*);

typedef struct {
	int row, col;
} point_t;

pthread_mutex_t lock;

int main(void)
{
	char ch = 0;
	initscr();

	addstr("Use WASD to move.\n");
	refresh();

	pthread_mutex_init(&lock, NULL);

	pthread_t threadID;
	pthread_create(&threadID, NULL, snake, &ch);

	char temp;
	while(true) {
		temp = getch();
		pthread_mutex_lock(&lock);
		ch = temp;
		pthread_mutex_unlock(&lock);
	}

	pthread_join(threadID, NULL);

	refresh();
	
	endwin();

	return EXIT_SUCCESS;
}

void* snake(void* args)
{
	(void) args;
	volatile char* ch = args;
	int row;
	int col;
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
		usleep(100000);
		pthread_mutex_lock(&lock);
		char input = *ch;
		pthread_mutex_unlock(&lock);

		mvaddch(snake[tail].row, snake[tail].col, ' ');
		
		switch(input)
		{
			case 'w':
				if (mvinch(row-1,col) == '.') {
					done = 1;
				} else {
					mvaddch(--row,col,'.');
					refresh();
				}
				break;
			case 's':
				if (mvinch(row+1,col) == '.') {
					done = 1;
				} else {
					mvaddch(++row,col,'.');
					refresh();
				}
				break;
			case 'a':
				if (mvinch(row,col-1) == '.') {
					done = 1;
				} else {
					mvaddch(row,--col,'.');
					refresh();
				}
				break;
			case 'd':
				if (mvinch(row,col+1) == '.') {
					done = 1;
				} else {
					mvaddch(row,++col,'.');
					refresh();
				}
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

}
