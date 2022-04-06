* Name: Cristian-Tanase Paris 331CAb

# Homework <NR> 2 (Stdio shared library)

* This README is OS independent and should provide proper details for both Linux
and Windows.
* For those assigned with grading the project: this means I won't go into any
specific details about how the OS API related functions work or should be called
as both implementations are more than 95% similar in terms of structure and data flow. The
only differences are, as I said earlier, the API specific functions.

General Structure
-

* For opening and closing the files I've relied upon the close and open system calls, while checking
the return values to make sure that no error was encountered.
* Reading and writing to the file makes use of an internal associated buffer. While reading, the function
tries each time to fill up the buffer to its max capacity (chunks of 4096 bytes). If there is still data
in the buffer, the function automatically retrieves the first character the buffer pointer points to. Writing
goes pretty much the same way, excepting the fact that we write to the main file when: the buffer is full,
flush is called or the file is closed.
* Flushing and closing make the program automatically write the entire content of the internal buffer to the file,
if the last operation was, of course, to write.
* If you open a process using the popen function, the interface for communicating with it is the SO_FILE provided
  (it provides a level of abstraction over the conventional pipe).  That means that if you want to issue an argument
or command to that process, you need to write to the resulting file. Reading the output from that process is a
similar story. This is the main way the parent and child process communicate.
* We always make sure that we close the proper pipe ends in the processes, by checking the type parameter.
* Closing the process automatically frees up all the allocated resources.

* I found this homework extremely useful as I've better understood the core concepts of low-level
I/O functions.
* From my point of view, the current implementation is as simple and efficient as it could possibly be,
while not being naive. It is written in such way that anyone reading the source files for the first time
could understand it.


Implementation
-
* The entire functionality of the homework has been implemented.
* Technical difficulties:
   * The checker is rather slow.
   * No timeout functionality added to a test file that might result in `Segmentations faults`. Also,
    unless the program crashes or does not manifest the correct behaviour, no output
    from the checker is provided, as info messages are not flushed to `stdout`.
   * Checker should take care of calling the Makefile and cleaning the resulting files.
* Interesting facts:
   * If you are not checking the return values for nearly every system call, you are going to
   have a bad time sooner or later.
   * Buffering saves you a great deal of time.
   
How should I compile and run this library?
-
After you've downloaded or cloned the repository, just do `make build` or
`make all` in the folder containing the source files, and you should see 
the `libso_stdio.so` file. Now, there are two main ways of using the
shared library:
1. If you are using it in conjuncture with this Homework's checker, do 
`make -f Makefile.checker` and everything should be taken care of.
2. If you've chosen to use the library in your own projects, then these are 
the steps:
    * Run the provided Makefile and get the shared library file object.
    * Include the `so_stdio.h` header file in your project: `#include "stdio.h"`
    * Compile your program making sure you've added the linkage argument:
      `gcc [gcc arguments] -lso_stdio -L<path>`
    * If you get the `error while loading shared libraries: libso_stdio.so:
      cannot open shared object  file: No such file or directory`, type the 
      following command in your console: `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.` 

Note: If you are as lazy as I am, you might as well just include the header file 
`#include "so_stdio.h"`, make sure the source files are in the same folder as your
project and skip the compilation and linkage part. 

Resources
-
* Lab 1: [Makefile structure](https://ocw.cs.pub.ro/courses/so/laboratoare/laborator-01)
* Lab 2: [xwrite function](https://ocw.cs.pub.ro/courses/so/laboratoare/laborator-02)
* Lab 3: [General process checking procedure](https://ocw.cs.pub.ro/courses/so/laboratoare/laborator-03)
* Operating Systems official GitHub: [so_stdio.h header file](https://github.com/systems-cs-pub-ro/so/tree/master/assignments/2-stdio)
* The Linux Programming Interface: [Book](https://man7.org/tlpi/)

Git
-
* Link: [Stdio-lib](https://github.com/Cristi29P/stdio-lib.git) - repository should be public after the 7th of April. Otherwise, please contact me on Microsoft Teams or e-mail.