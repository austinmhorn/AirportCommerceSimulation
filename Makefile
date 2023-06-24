# Usage:
# make			# compile all binary
# make run			# run executable
# make clean		# remove ALL binaries, objects, and executable

# compiler
CC := g++ -std=c++11

# compiler flags
CFLAGS := -Wall -c

# build target
TARGET := AirportCommerceSimulation

$(TARGET): main.o Airport.o Sector.o
	@echo "Creating target..."
	@$(CC) main.o Airport.o Sector.o -o $(TARGET)
	@echo "Target created successfully: " $(TARGET)

main.o: main.cpp main.hpp Sector.hpp Airport.hpp
	@echo "Building main.o..."
	@$(CC) $(CFLAGS) main.cpp
	@echo "Done."

Sector.o: Sector.cpp Sector.hpp
	@echo "Building Sector.o..."
	@$(CC) $(CFLAGS) Sector.cpp
	@echo "Done."

Airport.o: Airport.cpp Airport.hpp
	@echo "Building Airport.o..."
	@$(CC) $(CFLAGS) Airport.cpp
	@echo "Done."

clean:
	@echo "Cleaning..."
	rm core $(TARGET) *.o

run:
	./$(TARGET)
