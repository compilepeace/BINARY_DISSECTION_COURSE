# DISSECTING BINARIES
Hello people, this course aims at giving an insight regarding the inner details of a binary. Currently, this course focuses only on ELF binaries. It may prove useful to people interested in domains like:
- Malware Analysis
- Reverse Engineering
- Writing low level softwares (bootloaders or Operating systems)
- Binary Instrumentation
- Backdooring binaries
- Binary Exploitation



## PRE-REQUISITES
A familiarity with the Linux working environment and experince with some programming languages (not absolutely necessary though) would be appreciated. No issues, if you don't know much, even I am a noob :) I am working on UBUNTU 18.04 (x86_64 bit) as my host machine (We'll be doing cross-compilation for x86_32 bit architecture whenever required). 


## TABLE OF CONTENT
1. [Introduction to world of Binaries] 
2. [Executable and Linkable Format (ELF)] 
   * [ELF HEADER] 
   * [SECTION HEADER TABLE]
   * [SECTIONS DESCRIPTION] 
   * [PROGRAM HEADER TABLE]
  

## USAGE
All the files used in this course are linked and can be downloaded to reproduce the results on your system. First clone the repository on your local system. To do this type the bellow given commands.

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
Please let me know if something needs to be corrected or if you're having any doubts regarding some topic or if some topic is not clear. Feel free to open issues regarding the same and I'll be happy to help :)

<br>

You can also email me regarding any queries,<br>
**NAME** : **ABHINAV THAKUR**<br>
**EMAIL**: **compilepeace@gmail.com**

[Introduction to world of Binaries]: ./Introduction/Introduction.md
[Executable and Linkable Format (ELF)]: ./ELF/ELF.md
[ELF HEADER]: ./ELF/ELF_HEADER/ELF_HEADER.md
[SECTION HEADER TABLE]: ./ELF/SECTION_HEADER_TABLE/SHT.md
[SECTIONS DESCRIPTION]: ./ELF/SECTION_HEADER_TABLE/SECTIONS_DESCRIPTION/SECTIONS_DESCRIPTION.md
[PROGRAM HEADER TABLE]: ./ELF/PROGRAM_HEADER_TABLE/PHT.md
[RELOCATIONS]: ./ELF/RELOCATIONS/RELOCATIONS.md 
[SECTIONS]: ./ELF/SECTIONS/SECTIONS.md
[SEGMENTS]: ./ELF/SEGMENTS/SEGMENTS.md
