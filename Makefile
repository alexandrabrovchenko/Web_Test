all: web_alex_static web_alex_dynamic

web_alex_static:  test.o 
	g++ -std=c++11 test.o cards.a -o web_alex_static -s -pthread -lpthread

web_alex_dynamic:  test.o 
	g++ -std=c++11 test.o cards.dll -o web_alex_dynamic -s -pthread -lpthread

test.o: test.cpp
	g++ -std=c++11 -c test.cpp -pthread -lpthread

clean:
	rm -rf *.o web_alex_static.exe web_alex_dynamic.exe

install:
	cp hello_alex.exe C:\Apache24\cgi-bin\