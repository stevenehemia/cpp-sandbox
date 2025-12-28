printers: main.o network.o computer.o printer.o
	g++ -Wall -g main.o network.o computer.o printer.o -o printers

main.o: main.cpp network.h computer.h printer.h
	g++ -Wall -g -c main.cpp

network.o: network.cpp network.h computer.h printer.h
	g++ -Wall -g -c network.cpp

printer.o: printer.cpp printer.h network.h
	g++ -Wall -g -c printer.cpp

computer.o: computer.cpp computer.h network.h
	g++ -Wall -g -c computer.cpp

clean:
	rm -f *.o printers
