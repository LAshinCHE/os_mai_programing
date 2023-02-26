EXE=read_file comunicate_process

.PHONY: all

all: $(EXE)

%: %.c
	gcc -Wall -ggdb -o $@ $<

.PHONY: clean
clean:
	rm -rfv $(EXE)
