CC=gcc

snake: snake.o
	$(CC) -o snake snake.o -lncurses

.PHONY: clean
clean:
	rm *.o snake
