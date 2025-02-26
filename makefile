COMPILER := gcc

COMPILE_FLAGS := -Werror -g -Og -std=gnu99 -oFast
LINKER_FLAGS  := -lm `sdl2-config --libs --cflags` -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

program: program.c gui.h
	$(COMPILER) program.c $(COMPILE_FLAGS) $(LINKER_FLAGS) -o $@

clean:
	-rm 2>/dev/null program
