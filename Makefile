CXX = g++ -Wall -g -std=c++11
LIB = -lpthread

TARGET = universe
BUILD = build
SOURCE = main.cpp time.cpp define.cpp space.cpp world.cpp object.cpp planet.cpp 
OBJS = $(patsubst %.cpp, $(BUILD)/%.o, $(notdir $(SOURCE)))

TARGET: $(BUILD) $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIB)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/main.o: main.cpp *.h
	$(CXX) -o $@ -c $<

$(BUILD)/world.o: world.cpp world.h space.h
	$(CXX) -o $@ -c $<

$(BUILD)/planet.o: planet.cpp planet.h object.h
	$(CXX) -o $@ -c $<

$(BUILD)/%.o: %.cpp %.h
	$(CXX) -o $@ -c $<

.PHONY: clean 

clean:
	rm -f $(TARGET) $(OBJS)
