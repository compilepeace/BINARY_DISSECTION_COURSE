

# BINARY DISSECTION PRIMER
Hello reader, this primer aims at giving an insight into the inner details of a binary program. Currently, its only focus is on the native file format for Linux binaries, i.e. **E**xecutable and **L**inkable **F**ormat (shortly used as **ELF**). It may prove useful to security researchers or curiosity-driven individuals interested in below mentioned or domains similar to:
- Malware analysis 
- Reverse engineering
- Binary Instrumentation
- Binary Exploitation
- Linkers & loaders
- Writing low level softwares (binary protectors/packers, bootloaders, operating systems etc.)
- Virus research x_x



## PRE-REQUISITES
A familiarity with the Linux environment and experince with some programming language(s) (not absolutely necessary though) would significantly help in having a greater understanding. There are parts of the course which may not make sense at first or may need cross referencing with [official ELF specifications], so don't hesitate to open it up whenever the need calls. I am working on UBUNTU 18.04 (x86_64 bit) as my host machine (We'll be doing cross-compilation for x86_32 bit architecture whenever required). 


## TABLE OF CONTENT
1. [Introduction to world of Binaries] 
2. [Executable and Linkable Format (ELF)] 
   * [ELF HEADER] 
   * [SECTION HEADER TABLE]
   * [SECTIONS DESCRIPTION] 
   * [PROGRAM HEADER TABLE]
   * [SYMBOLS AND DEFINITIONS]
  

## USAGE
All the files used in this primer are linked and can be downloaded to reproduce the results on your system. To follow up with the content you may need compiled binaries for which I suggest you to clone the repository on your local system by typing the bellow given commands.

```shell
critical@d3ad:~$ git clone https://github.com/compilepeace/BINARY_DISSECTION_COURSE
Cloning into 'BINARY_DISSECTION_COURSE'...
remote: Counting objects: 94, done.
remote: Compressing objects: 100% (56/56), done.
remote: Total 94 (delta 22), reused 82 (delta 13), pack-reused 0
Unpacking objects: 100% (94/94), done.
```

If you want to build the binaries from the source code on your system then move to the directory whose binaries you want build for your system and in that directory (for eg: I am in `BINARY_DISSECTION_COURSE/ELF/ELF_HEADER` directory) , type the bellow given commands.

```shell
critical@d3ad:~/BINARY_DISSECTION_COURSE/ELF/ELF_HEADER$ make clean
rm hello_32 hello_64 
critical@d3ad:~/Desktop/BINARY_DISSECTION_COURSE/ELF/ELF_HEADER$ make
gcc -m32 -no-pie hello.c -o hello_32 
gcc -no-pie hello.c -o hello_64 
```

## CONTRIBUTIONS AND ISSUES
I am open to suggestions and corrections. Also, you may like to watch the repository for updates before forking. Please feel free to open up any issues or reach me out via email :)

<br>

**NAME** : **ABHINAV THAKUR**<br>
**EMAIL**: **compilepeace@gmail.com**

[official ELF specifications]: https://refspecs.linuxfoundation.org/elf/elf.pdf
[Introduction to world of Binaries]: ./Introduction/Introduction.md
[Executable and Linkable Format (ELF)]: ./ELF/ELF.md
[ELF HEADER]: ./ELF/ELF_HEADER/ELF_HEADER.md
[SECTION HEADER TABLE]: ./ELF/SECTION_HEADER_TABLE/SHT.md
[SECTIONS DESCRIPTION]: ./ELF/SECTION_HEADER_TABLE/SECTIONS_DESCRIPTION/SECTIONS_DESCRIPTION.md
[PROGRAM HEADER TABLE]: ./ELF/PROGRAM_HEADER_TABLE/PHT.md
[SYMBOLS AND DEFINITIONS]: ./ELF/SYMBOLS/SYMBOLS.md 


