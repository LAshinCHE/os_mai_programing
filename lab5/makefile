EXE=prog1 prog
COMP=lib1.o prog1.o lib2.o
LIB=libd1.so libd2.so
PROG=main2.out main1.out
.PHONY: comp
comp: 
	gcc -c -Wall -o lib1.o lib1.c
	gcc -c -Wall -o prog1.o prog1.c


PHONY: link
link: 	
	gcc -g -Wall -o prog1 lib1.o  prog1.o


.PHONY: Lib
Lib:
	gcc -fPIC -c lib1.c -o lib1.o 
	gcc -fPIC -c lib2.c -o lib2.o

	gcc -shared -o libd1.so lib1.o
	gcc -shared -o libd2.so lib2.o 	

	gcc prog2.c -L. -o main2.out -Wl,-rpath -Wl,.


.PHONY: cleanLib
cleanLib:
	rm -rfv $(LIB)


.PHONY: cleanComp
cleanComp:
	rm -rfv $(COMP)


.PHONY: cleanExe
cleanExe:
	rm -rfv $(EXE)


.PHONY: cleanProg
cleanProg:
	rm -rfv $(PROG)


.PHONY: clean
clean:
	rm -rfv $(PROG) $(EXE) $(COMP) $(LIB)