# Detect operating system
ifndef platform
  ifeq ($(OS),Windows_NT)
    platform = windows
  else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
      platform = linux
    endif
    ifeq ($(UNAME_S),Darwin)
      platform = macos
    endif
  endif
endif

# Define build type (debug / release)
ifndef build
  build = debug
endif

# Common compiler and paths settings
CC = clang
CFLAGS = -std=c99 -Wall -Wextra -Ilib -Isrc/shaders
LIBS = 
DEFS = 
OUTEXT = 

# Debug vs Release flags
ifeq ($(build), debug)
  CFLAGS += -g -O0
  DEFS += -DSOKOL_DEBUG
else
  CFLAGS += -O3 -ffast-math
  DEFS += -DNDEBUG
endif

# Platform-specific configurations
ifeq ($(platform), macos)
  DEFS += -DSOKOL_METAL
  LIBS += -framework Cocoa -framework QuartzCore -framework Metal -framework MetalKit
  # On macOS, Sokol implementations must be compiled as Objective-C
  SOKOL_CFLAGS = $(CFLAGS) -ObjC
else ifeq ($(platform), windows)
  CC = gcc
  DEFS += -DSOKOL_D3D11
  LIBS += -lkernel32 -luser32 -lshell32 -lgdi32 -ld3d11 -ldxgi
  OUTEXT = .exe
  SOKOL_CFLAGS = $(CFLAGS)
else ifeq ($(platform), linux)
  DEFS += -DSOKOL_GLCORE
  LIBS += -lX11 -lXi -lXcursor -lGL -ldl -lm -pthread
  SOKOL_CFLAGS = $(CFLAGS)
else ifeq ($(platform), web)
  CC = emcc
  DEFS += -DSOKOL_GLES3
  LIBS += -sFULL_ES3
  OUTEXT = .html
  SOKOL_CFLAGS = $(CFLAGS)
endif

CFLAGS += $(DEFS)

.PHONY: all shader run clean

all: main

DEPS = lib/sokol_app.h \
			 lib/sokol_gfx.h \
			 lib/sokol_glue.h \
			 lib/sokol_debugtext.h \
			 lib/HandmadeMath.h \
			 lib/cgltf.h

# Compile Sokol implementation once (with SOKOL_CFLAGS to apply -ObjC only on macOS)
lib/deps.o: lib/deps.c $(DEPS)
	$(CC) -c $(SOKOL_CFLAGS) -o lib/deps.o lib/deps.c

# Compile and link main executable
main: src/main.c src/shaders/shader_glsl.h lib/deps.o
	$(CC) -o main$(OUTEXT) $(CFLAGS) $(LIBS) lib/deps.o src/main.c

run: main
	@./main$(OUTEXT)

clean:
	rm -f main main.exe main.html main.js main.wasm lib/deps.o src/shaders/shader_glsl.h

compile_flags.txt: FORCE
	@echo "Generating compile_flags.txt for IDE support"
	@echo $(CFLAGS) | tr ' ' '\n' > $@

FORCE:

shader: src/shaders/shader_glsl.h

# Compile shaders for multiple backends
src/shaders/shader_glsl.h: src/shaders/shader.glsl
	./sokol-shdc --input src/shaders/shader.glsl --output src/shaders/shader_glsl.h --slang glsl430:glsl300es:hlsl5:metal_macos:metal_ios:wgsl

sokol:
	wget -O lib/sokol_app.h https://raw.githubusercontent.com/floooh/sokol/refs/heads/master/sokol_app.h
	wget -O lib/sokol_gfx.h https://raw.githubusercontent.com/floooh/sokol/refs/heads/master/sokol_gfx.h
	wget -O lib/sokol_glue.h https://raw.githubusercontent.com/floooh/sokol/refs/heads/master/sokol_glue.h
	wget -O lib/HandmadeMath.h https://raw.githubusercontent.com/HandmadeMath/HandmadeMath/refs/heads/master/HandmadeMath.h
	wget -O lib/cgltf.h https://raw.githubusercontent.com/jkuhlmann/cgltf/refs/heads/master/cgltf.h
	wget -O lib/sokol_debugtext.h https://raw.githubusercontent.com/floooh/sokol/refs/heads/master/util/sokol_debugtext.h

sokol_tools:
	wget -O sokol_shdc https://github.com/floooh/sokol-tools-bin/raw/refs/heads/master/bin/osx_arm64/sokol-shdc
