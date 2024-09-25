CXX = g++

CXXFLAGS = -Wall -std=c++17

SRC_DIR = src
OUT_DIR = out
LIB_DIR = lib

SRCS = $(wildcard $(SRC_DIR)/*.cpp)

OUTS = $(patsubst $(SRC_DIR)/%.cpp,$(OUT_DIR)/%,$(SRCS))

LIB_SRCS = $(LIB_DIR)/cmdparse.cpp

LIB_OBJS = $(patsubst $(LIB_DIR)/%.cpp,$(LIB_DIR)/%.o,$(LIB_SRCS))

all: $(LIB_OBJS) $(OUTS)

$(OUT_DIR)/%: $(SRC_DIR)/%.cpp $(LIB_OBJS)
	@mkdir -p $(OUT_DIR) # Ensure output directory exists
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIB_OBJS)

$(LIB_DIR)/%.o: $(LIB_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OUT_DIR)/* $(LIB_DIR)/*.o

