# Usage:
# make			# compile all binary
# make run			# run executable
# make clean		# remove ALL binaries, objects, and executable

# compiler
CC := g++ -std=c++11

# compiler flags
CFLAGS := -Wall -c

# build target
TARGET := AirportSimulation

$(TARGET): main.o Airport.o Sector.o
	@echo "Creating target..."
	@$(CC) main.o Airport.o Sector.o -o $(TARGET)
	@echo "Target created successfully."

main.o: main.cpp main.hpp Sector.hpp Airport.hpp
	@echo "Updating main.o..."
	@$(CC) $(CFLAGS) main.cpp
	@echo "Complete."

Sector.o: Sector.cpp Sector.hpp
	@echo "Updating Sector.o..."
	@$(CC) $(CFLAGS) Sector.cpp
	@echo "Complete."

Airport.o: Airport.cpp Airport.hpp
	@echo "Updating Airport.o..."
	@$(CC) $(CFLAGS) Airport.cpp
	@echo "Complete."

clean:
	rm *.o
	rm AirportSimulation

run:
	./AirportSimulation