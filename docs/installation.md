# Installation

Installing a library on Linux can seem a bit technical at first, but eikon keeps things relatively straightforward. Here's how to get it up and running on your system.

### TL;DR

eikon comes with a bash script that automates the installation process. To install everything correctly, just run:
```bash
sudo ./build.sh -i
```
This will compile the library and move all necessary files to the appropriate system directories. Note that `sudo` is required because you’re modifying protected system folders.

### Understanding the Basics

Linux systems follow a well-established convention for organizing code dependencies:
- Header files (`.h` or `.hpp`) contain declarations—function signatures, class definitions, constants, etc. These are needed at compile time so the compiler knows how to interpret the code you're writing.
- Compiled libraries (`.so` files, short for shared object) contain the actual implementation—the compiled machine code that performs the logic. These are needed at link time and runtime.

By default:
- Headers go in `/usr/include`
- Shared libraries go in `/usr/lib`

This separation allows developers to compile their own code while relying on precompiled implementations of external libraries.

A header-only library is one where all the code is written in the headers. You include the `.hpp` file, and the compiler compiles everything inline with your project. This is simple, but it has drawbacks:
- Compilation can be slower, especially for large projects
- You lose modularity—every translation unit compiles the same code repeatedly
- It can lead to code bloat and longer build times

eikon avoids this by separating its interface (headers) from its implementation (source files). This means:
- You include only the headers in your project
- The actual logic lives in a compiled `.so` file
- Your project links to the `.so` file at build time

This approach is more scalable and efficient, especially for larger systems or when multiple projects depend on the same library.
Because eikon’s implementation is not embedded in the headers, you can’t just include a .hpp file and expect everything to work. You need to:
- Compile the implementation into a shared library (`libeikon.so`)
- Install the headers into a known location (e.g. `/usr/include/eikon`)
- Link the library when compiling your own project (`-leikon`)

If you’re working on a system where multiple developers or tools rely on eikon, this structure also makes it easier to maintain and update the library without touching every dependent project. 

### Step-by-Step Installation

Start by compiling the implementation file into a shared object (`.so`) using the `-shared` flag:
```bash
g++ -shared src/eikon.cpp -o libeikon.so
```

This creates a shared library named `libeikon.so`.

It’s good practice to name your library using the `lib*.so` format. When you compile your own project later, you’ll link to this library using the `-l` flag followed by the name (without the lib prefix or .so extension). For example, `libeikon.so` becomes `-l eikon` (you can get rid of the space and write `-leikon`).

Once compiled, move the .so file to the system library directory:
```bash
sudo mv libeikon.so /usr/lib
```

This makes it available for linking when you compile other projects.

eikon includes multiple header files, and some of them may have generic names like `matrix.hpp` and `utils.hpp`. To avoid conflicts with other libraries, it’s best to place all eikon headers inside a dedicated folder:

```bash
sudo mkdir /usr/include/eikon
sudo mv include/*.hpp /usr/include/eikon
```
This keeps your system headers organized and avoids naming collisions.

### Using eikon in Your Project

Once installed, you can start using eikon in your code by including its main header:
```cpp
#include <eikon/eikon.hpp>
```

And when compiling your project, link to the library like this:
```bash
g++ main.cpp -leikon
```

That’s all you need to get started. 