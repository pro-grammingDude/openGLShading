CC = g++
CFLAGS = -std=c++11
LIBS = -lglfw3 -lglew32 -lopengl32 -lgdi32

SRC = EBO.cpp
TARGET = program

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

.PHONY: clean

clean:
	rm -f $(TARGET)
