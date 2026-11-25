# CHANGELOG
- Fix bug where consecutive move instructions was being collapsed at the wrong places, resulting in code crashing or unexpected output. 
- Make the number of optimization passes non-fixed, based on whether modifications in the previous pass were made, stopping only when no changes are made in a pass.
- Examples: add Brainf*ck interpretor to showcase larger project potential
- Add 'not' keyword to flip 'boolean' values (non-zero integers to zero).
- Change primitive types to be more strict, with Char (characters) needing to be cast to Int (integer) type for arithmetic and logical code.

### v0.2.1 Release Notes (2026-10-13)
- Optimize chained 'mov' instructions to reduce them down to a single instruction
- Fix bug where conditional 'mov' instructions weren't tracking saved registers, resulting in optimizations breaking code
- Update forgotten version number to latest version

### v0.2.0 Release Notes (2026-10-12)
- Add syntax for characters (e.g., 'A', '\n') to be pushed onto the stack.
- Add ability to read arguments through 'argv' keyword which pushes the number of arguments then the argument array.
- Add early return keyword ('return'), which programs can use to prematurely exit functions as long as the stack matches the return stack (obviously).
- Update compatible escape sequences for strings and characters.

### v0.1.0 Release Notes (2026-10-10)
- Added fixed-length array allocation in the form of heap pointers.
- Added type casting for both primitive and struct pointers.
- Updated naming convention allowance to exclude colons (':') from all names in a program
- Allowed the inclusion of periods ('.') in function names.
- For Emacs: added simple syntax highlighting
