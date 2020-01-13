CXX = g++ -Wall -g -std=c++11
LIB = -lpthread

TARGET = universe
BUILD = build
SOURCE = main.cpp object.cpp space.cpp time.cpp define.cpp
OBJS = $(patsubst %.cpp, $(BUILD)/%.o, $(notdir $(SOURCE)))

TARGET: $(BUILD) $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIB)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/main.o: main.cpp *.h
	$(CXX) -o $@ -c $<

$(BUILD)/%.o: %.cpp %.h
	$(CXX) -o $@ -c $<

.PHONY: clean 

clean:
	rm -f $(TARGET) $(OBJS)
