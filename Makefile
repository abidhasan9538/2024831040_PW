CC = g++
CFLAGS = -std=c++17 $(shell sdl2-config --cflags)
# you can adjust CFLAGS or LDFLAGS if your SDL2 is installed in a custom location
LDFLAGS = $(shell sdl2-config --libs)
TARGET = task101

all: $(TARGET)

$(TARGET): task101.cpp
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
