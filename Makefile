MAIN = main.cpp olcPixelGameEngine.cpp
OUT = loopover

CC = clang++
EMCC = em++

CFLAGS = -O2 -std=c++20
EMFLAGS = -std=c++20 -O2 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 -s USE_SDL_MIXER=2
LINKS = -lX11 -lGL -lpthread -lpng -std=c++17 -lpulse -lpulse-simple
WLINKS = -luser32 -lgdi32 -lopengl32 -lgdiplus -lshlwapi -ldwmapi -lstdc++fs -lwinmm


default: main.o olcPixelGameEngine.o
	$(CC) $(CFLAGS) -o $(OUT) main.o olcPixelGameEngine.o $(LINKS)

web:
	$(EMCC) $(EMFLAGS) -o $(OUT).html $(MAIN)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OUT) *.o $(OUT).html $(OUT).js $(OUT).wasm

scan-build: clean
	scan-build make