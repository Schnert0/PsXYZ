src=$(wildcard core/*.cpp)\
$(wildcard core/cpu/*.cpp)\
$(wildcard core/dma/*.cpp)\
$(wildcard core/gpu/*.cpp)\
$(wildcard core/bus/*.cpp)\
$(wildcard core/system/*.cpp)\
$(wildcard renderer/*.cpp)

obj=$(src:.cpp=.o)

LDFLAGS= -lstdc++ -lSDL2 -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -Wno-deprecated-declarations -I.

PsXYZ: $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) PsXYZ
