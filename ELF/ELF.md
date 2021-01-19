# ELF (Executable and Linkable Format)
**Ever thought why even a the smallest possible piece of code you compile results in some thousands of bytes in size?** <br>

Any file is made up of at least 2 components -
* **Data** : It is the actual content (main composition) of a file. <br>
* **Metadata** : It is the information describing the actual data.

ELF is the content stored at very begining of an executable and is responsible to direct the OS on how to load the executable into memory for execution. ELF's are the replacement to older object file formats used like **COFF** (**C**ommon **O**bject **F**ile **F**ormat) and **a.out** (**A**ssembler **Out**put) formats.

There are 4 type of ELF binaries on *NIX type systems.  
* **Executable** - It is a form of ready-to-execute binary program.
* **Shared Object** - This type of binary is ready to be linked within 2 views - *statically* & *dynamically*. This means that it be combined with other relocatable or shared object binaries to create another object file (after being processed by the static linker /usr/bin/ld) or it can be combined with executable binaries to create a process image, i.e. behaving like a **shared library**.
* **Relocatable** - The object file which is not yet processed by a compile-time linker (**/usr/bin/ld**) gets stored in the form of a relocatable binary.
* **Core Dumps** - it is the exact runtime image of a program dumped by the kernel at the time of a program crash. It is primarily used for post-mortem debugging and crash analysis.

An ELF binary is made up of 4 main components -

1.  **ELF HEADER** : It describes attributes of an ELF binary which include information useful to the loaders & linkers. It includes location to other body parts of an ELF binary which is helpful while implementing parsers for the binary.
2.  **PROGRAM HEADER TABLE** : A PHT describes the segments of an ELF binary. It is useful to the loader and the runtime linker (ld-Linux.so).
3.  **SECTION HEADER TABLE** : A SHT describes the sections of an ELF binary. It is useful to the compile-time linker (ld) and its presence is optional for program execution.
4.  **SECTIONS** AND **SEGMENTS** : It is the actual content of the binary. Sections are just blocks of bytes present in linking view (on-disk view) to produce segment (which provide a runtime/in-memory view). Segments are blocks composed of one or more sections and are produced by linker.

**NOTE**: ELF header has a field - *e_type* which specifies type of any ELF binary.

<p align="center">
  <img src="./IMAGES/linking_view_and_execution_view.jpg">
</p>
 

Before the loading and execution stage, the ELF contains code and data arranged in sectons. At the loding and execution stage, those sections are organised and put up into correspoding segments. One or more sections may end up getting fitted into a segment in memory depending on the instructions passed to compile-time linker.
Linker scripts can be made which assists linker on how to combine sections into segments. Later on, I plan to cover writing linker scripts in some depth.

Have a look at the overall detailed structure of the elf - [IMAGE]. 
Next, we move on towards the actual dissection part :)

<br>

**[PREV - INTRODUCTION]** <br>
**[NEXT - ELF HEADER]**

[IMAGE]: ./IMAGES/ELF_DISECTION.png
[PREV - INTRODUCTION]: ./../Introduction/Introduction.md
[NEXT - ELF HEADER]: ./ELF_HEADER/ELF_HEADER.md