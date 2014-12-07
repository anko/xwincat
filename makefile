xwincat: xwincat.c
	clang -O3 --std=gnu11 -pedantic -Wall -lxcb -o xwincat xwincat.c
clean:
	rm xwincat
.PHONY: xwincat


