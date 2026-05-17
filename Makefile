CFLAGS +=-Ilib

LIBS +=-framework Cocoa -framework QuartzCore -framework Metal -framework MetalKit
CFLAGS +=-ObjC 
DEFS +=-DSOKOL_METAL

CFLAGS += $(DEFS)

main:	 src/main.c
	 clang -o main $(CFLAGS) $(LIBS) src/main.c
run: main
	@./main

compile_flags.txt: FORCE
	@echo "Generating compile_flags.txt for IDE support"
	@echo $(CFLAGS) | tr ' ' '\n' > $@

FORCE:
