CPPFLAGS += -Iinclude
CXXFLAGS += -std=c++0x -Wall -Wextra
LDLIBS += -lgtest -lpthread

INCLUDE_DIR = include/tcframe
TEST_DIR = test
TEST_SRCS = $(TEST_DIR)/*.cpp $(INCLUDE_DIR)/*.h
TEST_MAIN_SRC = $(TEST_DIR)/main_test.cpp

.PHONY: test clean

test: main_test

main_test: $(TEST_SRCS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(TEST_MAIN_SRC) -o $@ $(LDLIBS) && ./main_test

clean:
	rm -f main_test
