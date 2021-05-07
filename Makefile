CC = g++
FLAGS = -std=c++11 -c -g

OBJS = main.o Node.o HashPageTable.o Address.o LRU.o SecondChance.o Simulator.o
Headers = Node.h HashPageTable.h Address.h LRU.h SecondChance.h Simulator.h
EXEC = main


$(EXEC): $(OBJS) $(Headers)
	$(CC)  $(OBJS) -o $(EXEC)

main.o: main.cpp 
	$(CC) $(FLAGS) main.cpp

Node.o: Node.cpp
	$(CC) $(FLAGS) Node.cpp

HashPageTable.o: HashPageTable.cpp
	$(CC) $(FLAGS) HashPageTable.cpp

Address.o: Address.cpp
	$(CC) $(FLAGS) Address.cpp

LRU.o: LRU.cpp
	$(CC) $(FLAGS) LRU.cpp

SecondChance.o: SecondChance.cpp
	$(CC) $(FLAGS) SecondChance.cpp

Simulator.o: Simulator.cpp
	$(CC) $(FLAGS) Simulator.cpp


clean:
	rm -f $(EXEC) $(OBJS)