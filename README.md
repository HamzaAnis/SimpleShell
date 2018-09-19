# Simple Shell

 A simple shell that implements several linux builtin commands and `cd`,`history` and `exit`
 and capable of executing a sequence of programs using `pipes` and `dup2`.
## Repo overview
```    
|   main.c              //Starting location of shell
|   Makefile            //Makefile to run program
|   README.md           //Documenation
|   shell.c             //Shell class implementations
|   shell.h             //Shell class declaration
|   written.txt         
|
+---run
|       demo_run.txt    //Sample run of the program
|
\---test                //Test cases
```

### How to Compile
```
$ make
$ ./w4118_sh
```

#### Requirements
- C

[SampleRun](demo_run.txt)