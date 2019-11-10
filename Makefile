CXX     	:= g++
CFLAGS  	:= -Wall -Wextra -pedantic-errors -fsanitize=address -fsanitize=undefined -g -D_GLIBCXX_DEBUG
LDFLAGS 	:= -fsanitize=address -fsanitize=undefined
SOURCES 	:= $(wildcard *.cpp)
OBJECTS 	:= $(SOURCES:.cpp=.o)
EXECUTABLE 	:= Little

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) -c -MMD -MP $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE) *.d

-include $(SOURCES:.cpp=.d)