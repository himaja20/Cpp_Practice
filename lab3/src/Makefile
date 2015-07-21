CC=g++
OBJS=main.o

mmu: $(OBJS)
	$(CC) $(OBJS) -o mmu

main.o: main.cpp
	$(CC) -c -Wall main.cpp

clean: 
	rm -rf ./*.o
