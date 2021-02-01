INC := include
SRC := src
LIBS := -lpthread

CXX := g++
OPT := -O2
CXXFLAGS :=
AR := ar
ARFLAGS := -rcs

SRC_FILES := $(shell ls $(SRC)/*.cpp)
SAMPLE_FILE := sample/sample.cpp

BUILD_DIR := $(shell mkdir -p build)build
OBJS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(notdir $(SRC_FILES)))
SHARED_LIB := $(BUILD_DIR)/libmeophys.so
STATIC_LIB := $(BUILD_DIR)/libmeophys.a
SAMPLE_OBJ := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(notdir $(SAMPLE_FILE)))
SAMPLE_OUT := $(patsubst %.o, $(BUILD_DIR)/%.out, $(notdir $(SAMPLE_OBJ)))

static: $(STATIC_LIB)
	$(CXX) -Wall $(OPT) -std=c++17 $(CXXFLAGS) -o $(SAMPLE_OUT) $(SAMPLE_FILE) $(STATIC_LIB) -I$(INC) $(LIBS)

$(STATIC_LIB): $(OBJS)
	$(AR) $(ARFLAGS) $(STATIC_LIB) $(OBJS)

shared: $(SHARED_LIB)
	$(CXX) -Wall $(OPT) -std=c++17 $(CXXFLAGS) -o $(SAMPLE_OUT) $(SAMPLE_FILE) -I$(INC) $(LIBS) -L$(BUILD_DIR) -libmeophys

$(SHARED_LIB):
	$(CXX) -Wall $(OPT) -std=c++17 $(CXXFLAGS) -o $(SHARED_LIB) $(SRC_FILES) -I$(INC) $(LIBS) -fPIC -shared

$(BUILD_DIR)/%.o: $(SRC)/%.cpp $(INC)/*.h
	$(CXX) -Wall $(OPT) -std=c++17 $(CXXFLAGS) -o $@ -c $< -I$(INC)

.PHONY: clean

clean:
	rm -f $(OBJS) $(SHARED_LIB) $(STATIC_LIB) $(SAMPLE_OBJ) $(SAMPLE_OUT) $(BUILD_DIR)/*.o