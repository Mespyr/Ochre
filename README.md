# WARNING

I have stopped working on this project and am currently working on something new. Check out [Taro](https://github.com/Mespyr/Taro) and [easygc](https://github.com/Mespyr/easygc), which are the new projects I am working on.

# Ochre 
Ochre is a stack-based programming language with a combination of both high-level and low-level features. It aims to have the simplicity of a Forth-like language, while still possessing the modern features that allow an ease of development.

## TODO??
- [ ] Finish assembly optimizations
- [ ] Cond (if/switch statement)
```
fun fizzbuzz(Int)
  cond
  (dup 15 modulo 0 =) pop "Fizzbuzz\n" print ;
  (dup 3 modulo 0 = ) pop "Fizz\n" print ;
  (dup 5 modulp 0 = ) pop "Buzz\n" print ;
  else dump ;

fun fib(Int) {Int}
  cond (dup 1 >)
    dup 1 - fib 
    swp 2 - fib + ;
```

- [ ] While Loops
```
fun main()
  0 while (dup 100 <)
    dup fizzbuzz 1 + ;
  pop
```

- [ ] Type Cast
```
fun mmap_string(Int) {^Char}
  # mmap syscall
  0 swp # item on stack is size
  3 33 1 neg 0
  9 call6
  cast ^Char # syscalls push integer, so cast to ^Char
```

- [ ] Generics
```
fun alloc[T](Int) {T}
  # mmap syscall
  0 swp
  3 33 1 neg 0
  9 call6 cast T

fun main()
  ^Char str
  20 alloc[^Char] @str
```
