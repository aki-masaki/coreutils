CXX = g++

CXXFLAGS = -Wall -std=c++17

SRC_DIR = src
OUT_DIR = out

SRCS = $(wildcard $(SRC_DIR)/*.cpp)

OUTS = $(patsubst $(SRC_DIR)/%.cpp,$(OUT_DIR)/%,$(SRCS))

all: $(OUTS)

$(OUT_DIR)/%: $(SRC_DIR)/%.cpp
	@mkdir -p $(OUT_DIR) # Ensure output directory exists
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(OUTS)

