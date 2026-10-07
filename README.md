# Ochre
Ochre (pronounced Oak-er) is a simple semi-low level stack programming language with a simple principle: all manipulation STAYS ON THE STACK.
It draws inspiration from features in Assembly, almost 'modernizing' them for use in a pure stack-based environment.

### Special Features (if you weren't interested already):
- Tighter control flow using labels and conditional jump statements
- Manipulation directly on the stack; nothing is abstracted away with syntactical sugar
- Variables to control more important instances while still keeping EVERYTHING on the stack.
- Complex data types
- Heap allocated complex struct instances which can get passed around on the stack.
- Lots of freedom for naming stuff (my favorite)
- Strict type-checking

## Installation

### Prerequisites
- Linux!
- A C++ compiler (currently configured for Clang++)
- [GNU Make](https://www.gnu.org/software/make/)
- The [Flat Assembler 1](https://flatassembler.net/)

### Quick Start (One-Command Setup)
After installing dependancies, one command will get the compiler on your PATH.

```bash
sudo make install
```

You should be able to run `ochre` anywhere now!

### Testing
To make sure `ochre` is working normally, you can also run the test runner suite.
```bash
make test
```
