EXE=main child

.PHONY: all

all: $(EXE)

%: %.c
	gcc -Wall -ggdb -o $@ $<

.PHONY: clean
clean:
	rm -rfv $(EXE)
