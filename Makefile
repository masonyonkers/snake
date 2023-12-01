CC=gcc

snake: snake.o
	$(CC) -o snake snake.o -lncurses -pthread

.PHONY: clean
clean:
	rm *.o snake
