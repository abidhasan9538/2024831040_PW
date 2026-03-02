# task101 - SDL2 Demo

A simple SDL2 initialization program that demonstrates setting up SDL2 and checking for initialization errors.

## Prerequisites

You need to have SDL2 installed on your machine. Follow the instructions below for your operating system.

### macOS
```sh
brew install SDL2
```

### Linux (Ubuntu/Debian)
```sh
sudo apt-get update
sudo apt-get install libsdl2-dev
```

### Linux (Fedora/CentOS)
```sh
sudo dnf install SDL2-devel
```

### Windows
1. Download SDL2 from https://www.libsdl.org/download-2.0.php
2. Extract the archive to a known location
3. Update your build command to include the path to SDL2

## Build

### macOS/Linux
```sh
clang++ -g task101.cpp -o task101 \
  -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2
```

(On Linux, the include path may be `/usr/include` and lib path `/usr/lib` or `/usr/lib/x86_64-linux-gnu`)

### Using VS Code (macOS/Linux)
Press `Ctrl+Shift+B` to run the configured build task.

## Run
```sh
./task101
```

You should see:
```
SDL Initialized successfully!
```

## Troubleshooting

**"SDL2/SDL.h: No such file or directory"**
- Make sure SDL2 is installed: `brew install SDL2` (macOS) or `sudo apt-get install libsdl2-dev` (Linux)
- Verify the include path in your build command matches where SDL2 is installed

**Linker errors about undefined SDL symbols**
- Ensure you're using `clang++` (not `clang`)
- Make sure `-lSDL2` is in your link flags

## License

This is a class assignment.
