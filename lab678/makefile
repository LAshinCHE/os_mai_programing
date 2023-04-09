.PHONY: comp
comp: 
		gcc -std=c99 -Wall RC.c computingNode.c -o compNode -L/usr/local/lib -lzmq -lm
		gcc -std=c99 -Wall RC.c nodeArray.c controlNode.c -o main -L/usr/local/lib -lzmq -lm


.PHONY: test
test: 
		gcc -std=c99 -Wall test.c RC.c -o test -lm
