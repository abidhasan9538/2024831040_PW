CC = g++
CFLAGS = -std=c++17 $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs)
TARGET = task101

all: $(TARGET)

$(TARGET): task101.cpp
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
