CFLAGS +=-Ilib -Isrc/shaders
LIBS +=-framework Cocoa -framework QuartzCore -framework Metal -framework MetalKit
CFLAGS +=-ObjC 
DEFS +=-DSOKOL_METAL

CFLAGS += $(DEFS)

.PHONY: all shader run clean

all: main

DEPS = lib/sokol_app.h \
			 lib/sokol_gfx.h \
			 lib/sokol_glue.h 

lib/deps.o: lib/deps.c $(DEPS)
	clang -c $(CFLAGS) -o lib/deps.o lib/deps.c

main: src/main.c src/shaders/shader_glsl.h lib/deps.o
	clang -o main $(CFLAGS) $(LIBS) lib/deps.o src/main.c

run: main
	@./main

clean:
	rm -f main lib/deps.o

compile_flags.txt: FORCE
	@echo "Generating compile_flags.txt for IDE support"
	@echo $(CFLAGS) | tr ' ' '\n' > $@

FORCE:

shader: src/shaders/shader_glsl.h

src/shaders/shader_glsl.h: src/shaders/shader.glsl
	./sokol-shdc --input src/shaders/shader.glsl --output src/shaders/shader_glsl.h --slang glsl430:hlsl5:metal_macos

sokol:
	wget -O lib/sokol_app.h https://raw.githubusercontent.com/floooh/sokol/refs/heads/master/sokol_app.h
	wget -O lib/sokol_gfx.h https://raw.githubusercontent.com/floooh/sokol/refs/heads/master/sokol_gfx.h
	wget -O lib/sokol_glue.h https://raw.githubusercontent.com/floooh/sokol/refs/heads/master/sokol_glue.h

sokol_tools:
	wget -O sokol_shdc https://github.com/floooh/sokol-tools-bin/raw/refs/heads/master/bin/osx_arm64/sokol-shdc

