# Sokol C Boilerplate

A minimal, lightning-fast cross-platform boilerplate for C projects using the **Sokol** header-only libraries.

## How to Build and Run

### 1. Download Sokol Dependencies
Fetch the latest Sokol header files and the shader compiler tools:
```bash
# Download latest sokol headers (sokol_app.h, sokol_gfx.h, sokol_glue.h)
make sokol

# Download the sokol-shdc shader compiler
make sokol_tools
```

### 2. Makefile Targets

The `Makefile` supports automatic platform detection and optimized compile times using a precompiled `deps.o` for the Sokol implementation.

| Command | Description |
| :--- | :--- |
| `make` | Compile the main application (default target) |
| `make run` | Compile and launch the application |
| `make shader` | Compile the shader for all targets (`glsl`, `hlsl`, `metal`, `wgsl`) |
| `make clean` | Clean up all generated binary and object files |

### 3. Advanced Build Configurations

You can pass environment variables to `make` to compile for different build styles and operating systems:

* **Build Type (`debug` / `release`)**:
  ```bash
  # Compile release build with -O3 -ffast-math optimizations
  make build=release
  ```
* **Target Platform (`macos` / `windows` / `linux` / `web`)**:
  ```bash
  # Compile for Windows (using MinGW/GCC)
  make platform=windows
  ```
