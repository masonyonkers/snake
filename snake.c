#include<ncurses.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<time.h>

typedef struct snake_segment
{
    int row;
    int col;
    struct snake_segment *next;
}	snake_segment;

void* snake(void*);
void* poll_input(void* args);
void* random_x(void* args);

snake_segment* snake_create(int, int);
void snake_add(snake_segment **, int, int);
void snake_free(snake_segment *);

pthread_mutex_t lock;

int main(void)
{
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

void* random_x(void* args)
{
	volatile char* ch = args;
	srand(time(NULL));

	int row;
	int col;

	while (*ch!='q') {
		usleep(1000000);
		row = rand() % 50 + 1;
		col = rand() % 50 + 1;
		mvaddch(row,col,'x');
	}
}

void* poll_input(void* args)
{
	volatile char* ch = args;

	char temp;
	while(*ch!='q') {
		temp = getch();
		pthread_mutex_lock(&lock);
		*ch = temp;
		pthread_mutex_unlock(&lock);
	}
	
}

void* snake(void* args)
{
	(void) args;
	volatile char* ch = args;
	int initial_size = 2;
	int row = 5;
	int col = 5;
	snake_segment *head = NULL;
	int done = 0;

	noecho();
	curs_set(0);

	// for (int i = 0; i < initial_size; i++) {
		// snake_add(&head, row, col - i);
	// }

	while (!done) {
		usleep(100000);

		pthread_mutex_lock(&lock);
		char input = *ch;
		pthread_mutex_unlock(&lock);

		switch(input) {
			case 'w':
				row--;
				if (mvinch(row - 1, col) == '.') {
					done = 1;
					continue;
				} else if (mvinch(row - 1, col) != 'x') {
					snake_segment *tail = head;

					while (tail->next->next != NULL)
						tail = tail-> next;

					mvaddch(tail->next->row, tail->next->col, ' ');
					free(tail->next);
					tail->next = NULL;
				}
				break;
			case 'a':
				col--;
				if (mvinch(row, col - 1) == '.') {
					done = 1;
					continue;
				} else if (mvinch(row, col - 1) != 'x') {
					snake_segment *tail = head;

					while (tail->next->next != NULL)
						tail = tail-> next;

					mvaddch(tail->next->row, tail->next->col, ' ');
					free(tail->next);
					tail->next = NULL;
				}
				break;
			case 's':
				row++;
				if (mvinch(row + 1, col) == '.') {
					done = 1;
					continue;
				} else if (mvinch(row + 1, col) != 'x') {
					snake_segment *tail = head;

					while (tail->next->next != NULL)
						tail = tail-> next;

					mvaddch(tail->next->row, tail->next->col, ' ');
					free(tail->next);
					tail->next = NULL;
				}
				break;
			case 'd':
				col++;
				if (mvinch(row, col + 1) == '.') {
					done = 1;
					continue;
				} else if (mvinch(row, col + 1) != 'x') {
					snake_segment *tail = head;

					while (tail->next->next != NULL)
						tail = tail-> next;

					mvaddch(tail->next->row, tail->next->col, ' ');
					free(tail->next);
					tail->next = NULL;
				}
				break;
			case 'q':
				done = 1;
				continue;
			default:
				break;
		}

		snake_add(&head, row, col);

		// if (mvinch(row, col) != 'x') {
			// snake_segment *tail = head;
// 
			// while (tail->next->next != NULL)
				// tail = tail-> next;
// 
			// mvaddch(tail->next->row, tail->next->col, ' ');
			// free(tail->next);
			// tail->next = NULL;
		// }

		refresh();
	}

	snake_free(head);
	return args;
}

snake_segment* snake_create(int row, int col)
{
	snake_segment *new = malloc(sizeof(snake_segment));

	// if (new == NULL)
		// return NULL;

	new->row  = row;
	new->col  = col;
	new->next = NULL;

	return new;
}

void snake_add(snake_segment **snake, int row, int col)
{
	snake_segment *new = snake_create(row, col);

	if (new == NULL)
		return;

	mvaddch(row, col, '.');


	if (*snake == NULL) {
		*snake = new;
	} else {
		new->next = *snake;
		*snake = new;
	}
}

void snake_free(snake_segment *snake)
{
	while (snake != NULL) {
		snake_segment *temp = snake;
		snake = snake->next;
		free(temp);
	}
}
