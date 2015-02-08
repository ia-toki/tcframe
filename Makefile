CPPFLAGS += -Iinclude
CXXFLAGS += -std=c++0x -Wall -Wextra
LDLIBS += -lgtest -lpthread

INCLUDE_DIR = include/tcframe
TEST_DIR = test
TEST_SRCS = $(TEST_DIR)/*.cpp $(INCLUDE_DIR)/*.h
TEST_MAIN_SRC = $(TEST_DIR)/tests.cpp

tests: $(TEST_SRCS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(TEST_MAIN_SRC) -o $@ $(LDLIBS)

clean:
	rm -f tests
