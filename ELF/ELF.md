# ELF (Executable and Linkable Format)
**Ever thought why even a the smallest possible piece of code you compile results in some thousands of bytes in size?** <br>

A file is made up of 2 things - 
* **Metadata** : It is the information describing the data.
* **Data** : It is the Raw/Actual data. <br>

ELF is the content stored at very begining of an executable and is responsible to direct the OS on how to load the executable into memory for execution. ELF's are the replacement to older object file formats used like **COFF** (**C**ommon **O**bject **F**ile **F**ormat) and **a.out** (**A**ssembler **Out**put) formats. 

An ELF constitutes of the following components -

1.  **ELF HEADER** : It is the first section of the executable. It describes the organisation of the executable. 
2.  **PROGRAM HEADER TABLE** : A PHT describes the segments of the executable used at runtime.
3.  **SECTION HEADER TABLE** : A SHT lists the sections of the executable 
4.  **SECTIONS** AND **SEGMENTS** : It is the actual content of the executable. Sections are just blocks of binary present in linking view to produce segment. Segments are blocks composed of one or more sections and are produced by linker.

<p align="center">
  <img src="./IMAGES/linking_view_and_execution_view.jpg">
</p>
 

Before the loading and execution stage, the ELF contains code and data arranged in sectons. At the loding and execution stage, those sections are organised and put up into correspoding segments.
Linker scripts can be made which assists linker on how to combine sections to create segments. We'll study more on linker scripts maybe later in the course.

Have a look at the overall detailed structure of the elf - [IMAGE]. 
Let's move towards starting disection. Next, we're gonna discuss each of these in detail ahead :)

<br>

**[PREV - INTRODUCTION]** <br>
**[NEXT - ELF HEADER]**

[IMAGE]: ./IMAGES/ELF_DISECTION.png
[PREV - INTRODUCTION]: ./../Introduction/Introduction.md
[NEXT - ELF HEADER]: ./ELF_HEADER/ELF_HEADER.md