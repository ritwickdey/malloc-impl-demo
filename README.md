Minimal implementation of malloc and free using `sbrk()` and `brk()` system calls. 

### Context:
How process allocate/deallocate memory block in runtime?

Whenever we create an object or list, we allocate memory in heap. Unlike `stack`, memory allocation in `heap` happens in runtime. No matter what programming language you choose, the fundamentals remain the same. 

In Clang, there is `malloc()` to create memory in heap and `free()` to release the memory. If you don't know `Clang`, you can relate `malloc()` call as your creating object from Class.
But there are no `free()` equivalent functions available in other high level-programming languages as they have automatic garbage collection. Probably garbage collector has some function like `free()`.

But `malloc()` and `free()` is library (`glibc`) functions, which means anyone can implement those function or tweak those function on their way. Then the question is how `malloc()` or `free()` allocate or deallocate memory. 

There is a few `system call` available to do that. `brk`, `sbrk` and `mmap` and `munmap`. Please check the man page to know more about the functions.

Here I've tried to implement `malloc` and `free` using `brk`, `sbrk`.

-------------
### Local setup
```
> make build
> ./bin/mem_test
```
-------------


### Gist
Try to read **only** the comments.

`my_malloc()` implementation using `brk` and `sbrk`.
![malloc](./img/malloc.png)


`my_free()` implementation using `brk` and `sbrk`.
![free](./img/free.png)



---------