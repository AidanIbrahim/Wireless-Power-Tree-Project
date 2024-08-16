CXXFLAGS = -Wall -g -fexec-charset=utf-8
CXX = g++

proj2: mytest.o wpower.o
	$(CXX) $(CXXFLAGS) -o proj2 wpower.o mytest.o

driver.o:
	$(CXX) $(CXXFLAGS) -c driver.cpp

mytest.o: mytest.cpp
	$(CXX) $(CXXFLAGS) -c mytest.cpp

wpower.o: wpower.cpp wpower.h
	$(CXX) $(CXXFLAGS) -c wpower.cpp

clean: 
	rm *.o proj2 *~


