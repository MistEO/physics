SOURCE = main.cpp object.cpp wholetimer.cpp space.cpp

TARGET = demo

CXX = g++ -Wall -g -std=c++11 -lpthread
LIB = -lpthread

TARGET: $(SOURCE)
	$(CXX) -o $(TARGET) $(SOURCE) $(LIB)

clean:
	rm -f *.a *.o $(TARGET) core *~ *.so *.lo *.swp
