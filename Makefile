CC=gcc

snake: snake.o
	$(CC) -o snake snake.o -lncurses -pthread

.PHONY: run
run:
	./snake

.PHONY: clean
clean:
	rm *.o snake
