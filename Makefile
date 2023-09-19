# Variables
CC = g++
CPPFLAGS = -std=c++17 -pthread
SRC = main.cpp node/Node.cpp worker/Workers.cpp topography/Topography.cpp
OUT = Mesh

# Rules
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CPPFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)
