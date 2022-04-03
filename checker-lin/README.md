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

Implementation
-


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