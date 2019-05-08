#--------------------
# Variable section
#--------------------

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

# Option specification in section 4.2 of assignment
COMP_OPT = -O3

# file will be called pp
TARGET = parkpark
TARGET_FILE = "input/unweighted_50.txt"

# settings
# CFLAGS = -c -Wall

.phony: all

#--------------------
# Command section
#--------------------

%.o: %.cpp
	@g++ $(COMP_OPT) -c $<

$(TARGET): $(OBJ)
	@g++ $(COMP_OPT) -o $@ $(OBJ)


# Compile all files
all: $(TARGET)

# main.o: main.cpp
# 	g++ $(CFLAGS) main.cpp

clean:
	@rm -rf $(TARGET)
	@rm -rf $(OBJ)

run:
	@./$(TARGET)

# Test program
test1:
	cp -f $(TARGET_FILE) ./maxcut.in $<
	@./$(TARGET) $(TARGET_FILE)

test2:
	$(eval TARGET_FILE = input/unweighted_100.txt)
	cp -f $(TARGET_FILE) ./maxcut.in $<
	@./$(TARGET) $(TARGET_FILE)

test3:
	$(eval TARGET_FILE = input/unweighted_500.txt)
	cp -f $(TARGET_FILE) ./maxcut.in $<
	@./$(TARGET) $(TARGET_FILE)

test4:
	$(eval TARGET_FILE = input/weighted_500.txt)
	cp -f $(TARGET_FILE) ./maxcut.in $<
	@./$(TARGET) $(TARGET_FILE)

test5:
	$(eval TARGET_FILE = input/weighted_chimera_297.txt)
	cp -f $(TARGET_FILE) ./maxcut.in $<
	@./$(TARGET) $(TARGET_FILE)

# Report data generation
genreport1:
	@./$(TARGET) input/unweighted_50.txt unweighted_50.csv

genreport2:
	@./$(TARGET) input/unweighted_100.txt unweighted_100.csv

genreport3:
	@./$(TARGET) input/unweighted_500.txt unweighted_500.csv

genreport4:
	@./$(TARGET) input/weighted_500.txt weighted_500.csv

genreport5:
	@./$(TARGET) input/weighted_chimera_297.txt weighted_chimera_297.csv

# Test on all dataset.
testall:
	@./$(TARGET) input/unweighted_50.txt
	@./$(TARGET) input/unweighted_100.txt
	@./$(TARGET) input/unweighted_500.txt
	@./$(TARGET) input/weighted_500.txt
	@./$(TARGET) input/weighted_chimera_297.txt
