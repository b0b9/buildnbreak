bnbcode:bnbcode.cpp logo.res
	g++ -Wall -o $@ $< $(shell pkg-config --cflags --libs sdl2 glew sdl2_mixer sdl2_image) -lopengl32 logo.res
