CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
LDFLAGS = 
TARGET = main
SRC = main.cpp TableroSudoku.cpp greedy.cpp tabusearch.cpp
OBJ = $(SRC:.cpp=.o)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: TableroSudoku.h greedy.h tabusearch.h
TableroSudoku.o: TableroSudoku.h
greedy.o: greedy.h TableroSudoku.h
tabusearch.o: tabusearch.h TableroSudoku.h

.PHONY: clean
clean:
	rm -f $(OBJ) $(TARGET)

