#include<ncurses.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<time.h>

void* snake(void*);
void* poll_input(void* args);
void* random_x(void* args);

typedef struct {
	int row, col;
} point_t;

point_t *grow(point_t old [], int size);

pthread_mutex_t lock;

int main(void) {
	char ch = 0;
	initscr();

	addstr("Use WASD to move.\n");
	refresh();

	pthread_mutex_init(&lock, NULL);

	pthread_t thread1;
	pthread_create(&thread1, NULL, &snake, &ch);

	pthread_t thread2;
	pthread_create(&thread2, NULL, &poll_input, &ch);

	pthread_t thread3;
	pthread_create(&thread3, NULL, &random_x, &ch);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);

	refresh();
	
	endwin();

	return EXIT_SUCCESS;
}

void* random_x(void* args) {
	volatile char* ch = args;
	srand(time(NULL));

	int row;
	int col;

	while (*ch!='q') {
		usleep(1000000);
		row = rand() % 100 + 1;
		col = rand() % 100 + 1;
		mvaddch(row,col,'x');
	}
}

void* poll_input(void* args) {
	volatile char* ch = args;

	char temp;
	while(*ch!='q') {
		temp = getch();
		pthread_mutex_lock(&lock);
		*ch = temp;
		pthread_mutex_unlock(&lock);
	}
	
}

void* snake(void* args) {
	(void) args;
	volatile char* ch = args;
	int row;
	int col;
	int size = 1;
	point_t snake[size];
	int tail = 0;
	int done = 0;

	for (int i = 0; i < size; i++) {
		snake[i].row = 1;
		snake[i].col = i;
		mvaddch(1, i, '.');
	}

	row = snake[size-1].row;
	col = snake[size-1].col;

	noecho();
	curs_set(0);
	
	while(!done) {

		usleep(100000);

		pthread_mutex_lock(&lock);
		char input = *ch;
		pthread_mutex_unlock(&lock);

		mvaddch(snake[tail].row, snake[tail].col, ' ');
		
		switch(input) {
			case 'w':
				if (mvinch(row-1,col) == '.') {
					done = 1;
					refresh();
					mvprintw(row/2,(col-strlen("Game Over :("))/2,"%s","Game Over :(");
					refresh();
				} else if (mvinch(row-1,col) == 'x') {
					mvaddch(--row,col,'.');
					*snake = *grow(snake, ++size);
					refresh();
				} else {
					mvaddch(--row,col,'.');
					refresh();
				}
				break;
			case 's':
				if (mvinch(row+1,col) == '.') {
					done = 1;
					refresh();
					mvprintw(row/2,(col-strlen("Game Over :("))/2,"%s","Game Over :(");
					refresh();
				} else if (mvinch(row+1,col) == 'x') {
					mvaddch(++row,col,'.');
					*snake = *grow(snake, ++size);
					refresh();
				} else {
					mvaddch(++row,col,'.');
					refresh();
				}
				break;
			case 'a':
				if (mvinch(row,col-1) == '.') {
					done = 1;
					refresh();
					mvprintw(row/2,(col-strlen("Game Over :("))/2,"%s","Game Over :(");
					refresh();
				} else if (mvinch(row,col-1) == 'x') {
					mvaddch(row,--col,'.');
					*snake = *grow(snake, ++size);
					refresh();
				} else {
					mvaddch(row,--col,'.');
					refresh();
				}
				break;
			case 'd':
				if (mvinch(row,col+1) == '.') {
					done = 1;
					refresh();
					mvprintw(row/2,(col-strlen("Game Over :("))/2,"%s","Game Over :(");
					refresh();
				} else if (mvinch(row,col+1) == 'x') {
					mvaddch(row,++col,'.');
					*snake = *grow(snake, ++size);
					refresh();
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

		if (!done) {
		snake[tail].row = row;
		snake[tail].col = col;
		tail = (tail + 1) % size;
		refresh();
		}
	}

	return args;

}

point_t *grow(point_t old [], int size)
{
	point_t new[size];
	point_t *ptr = new;

	for (int i = 0; i < size; i++)
	{
		new[i] = old[i];
	}

	return ptr;
}
