# SECTIONS DESCRIPTION
There are some sections listed by 'readelf' whose importance may vary from person to person. So, I'll start with the sections which I feel are more important regarding the corresponding probleum domain. 



## FOR PROGRAMMERS

### .TEXT SECTION
This section stores all the executable instructions (i.e. your program code disassembled into assembly instructions plus the C Runtime support) of the program, this section goes into the `code` segment while loading into memory. The section header entry corresponding to the .text section is shown bellow. This section is the most interesting part to Reverse Engineers as it gives idea about the overall design of the software program.

```shell
Section Headers:
[Nr] Name              Type             Address           Offset
     Size              EntSize          Flags  Link  Info  Align
...
[13] .text             PROGBITS         00000000000004f0  000004f0
     00000000000001b2  0000000000000000  AX       0     0     16
...
```
This clearly shows that memory is allocated for this section at runtime i.e. the section gets loaded into the memory (indicated by `A` flag) and is Executable section (identified by the `X` flag). 

Lets See the content of this section. For this we'll use another program called `objdump` which is a tool to display information (mostly disassembly) of the file. We will disassemble and see the content of the file [sd] compiled from the source [sd.c]. See the disassembly of .text section by command `objdump -d .text sd`, you'll see various `symbols` (explained later) with their disassembly, scroll down and look for \<main>  symbol, this is the main() described in the file [sd.c] and look at the source code [sd.c] and then the disassembly of \<main> carefully.

```shell
critical@d3ad:~/BINARY_DISECTION_COURSE/ELF/SECTION_HEADER_TABLE/SECTION_DESCRIPTION$ objdump -M intel -d sd | grep -A14 "main>:"
00000000000005fa <main>:
 5fa:	55                   	push   rbp
 5fb:	48 89 e5             	mov    rbp,rsp
 5fe:	48 c7 c0 04 00 00 00 	mov    rax,0x4
 605:	48 c7 c7 05 00 00 00 	mov    rdi,0x5
 60c:	48 c7 c6 06 00 00 00 	mov    rsi,0x6
 613:	48 c7 c2 07 00 00 00 	mov    rdx,0x7
 61a:	b8 00 00 00 00       	mov    eax,0x0
 61f:	5d                   	pop    rbp
 620:	c3                   	ret    
 621:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
 628:	00 00 00 
 62b:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]

0000000000000630 <__libc_csu_init>:
```
You'll notice that the statements at 5fe, 605, 60c, 613 are the same instructions which we wrote in our source file [sd.c]. I hope this gives a brief idea and understanding towards the .text section.

 
### .RODATA SECTION
This section stores all the string literals defined in the program. Look at the source [sd.c], The string used in the print() is a string literal. To analyse a section for strings we use `-p` option with 'readelf' which dumps the output content as strings.

```shell
critical@d3ad:~/BINARY_DISECTION_COURSE/ELF/SECTION_HEADER_TABLE/SECTIONS_DESCRIPTION$ readelf -p .rodata sd

String dump of section '.rodata':
  [     4]  Hello, critical ^_^

```


### .DATA SECTION
This section holds the initialized data of the program. You will find the initialized global/static variables inside this section. Let's look for .data section is file [sd] by objdump

```shell
critical@d3ad:~/BINARY_DISECTION_COURSE/ELF/SECTION_HEADER_TABLE/SECTIONS_DESCRIPTION$ objdump -M intel -D sd | grep -A20 " .data"
Disassembly of section .data:

0000000000201000 <__data_start>:
	...

0000000000201008 <__dso_handle>:
  201008:	08 10                	or     BYTE PTR [rax],dl
  20100a:	20 00                	and    BYTE PTR [rax],al
  20100c:	00 00                	add    BYTE PTR [rax],al
	...

0000000000201010 <global_var>:
  201010:	08 00                	or     BYTE PTR [rax],al
	...

Disassembly of section .bss:

0000000000201014 <__bss_start>:
  201014:	00 00                	add    BYTE PTR [rax],al
	...

```
Look at the address of the symbol `0000000000201010 <global_var>`, it is the name of the global variable which have the value '8' defined in the source file [sd.c]. Objdump shows the disassembly `or BYTE PTR [rax],dl` here since it is confused between the code and data. Objdump does not differ between the code and data, it disassembles everthing that it is asked to do. In this way, the section and segments helps the processor understand the organisation of code and data.


### .BSS SECTION
BSS here stands for **B**lock **S**tarted by **S**ymbol. This section stores the uninitialized data of the program. Any variable in this section is cleared with 0.


## OTHER DOMAINS

### .SHSTRTAB 
Stores a list of **section names** (in the form of NULL terminated strings). Use the following options with readelf -
* `-x` : To dump content of section in hexadecimal format.
* `-p` : To dump content of section in the form of strings.<br>
 
Usage : `readelf -p <section_name> <elf_binary>`<br>
Let's analyse the `.shstrtab` section of the binary [sd].

```shell
critical@d3ad:~/BINARY_DISECTION_COURSE/ELF/SECTION_HEADER_TABLE/SECTIONS_DESCRIPTION$ readelf -p .shstrtab sd

String dump of section '.shstrtab':
  [     1]  .symtab
  [     9]  .strtab
  [    11]  .shstrtab
  [    1b]  .interp
  [    23]  .note.ABI-tag
  [    31]  .note.gnu.build-id
  [    44]  .gnu.hash
  [    4e]  .dynsym
  [    56]  .dynstr
  [    5e]  .gnu.version
  [    6b]  .gnu.version_r
  [    7a]  .rela.dyn
  [    84]  .rela.plt
  [    8e]  .init
  [    94]  .plt.got
  [    9d]  .text
  [    a3]  .fini
  [    a9]  .rodata
  [    b1]  .eh_frame_hdr
  [    bf]  .eh_frame
  [    c9]  .init_array
  [    d5]  .fini_array
  [    e1]  .dynamic
  [    ea]  .data
  [    f0]  .bss
  [    f5]  .comment
```


### .STRTAB 
This section stores list of **symbol names** (in the form of NULL terminated strings) stored in **\.symtab section** (explained bellow). For eg: variable names, function names etc. everything is considered as a symbol. Consider the output bellow.

```shell
critical@d3ad:~/BINARY_DISECTION_COURSE/ELF/SECTION_HEADER_TABLE/SECTIONS_DESCRIPTION$ readelf -p .strtab sd

String dump of section '.strtab':
  [     1]  crtstuff.c
  [     c]  deregister_tm_clones
  [    21]  __do_global_dtors_aux
  [    37]  completed.7696
  [    46]  __do_global_dtors_aux_fini_array_entry
  [    6d]  frame_dummy
  [    79]  __frame_dummy_init_array_entry
  [    98]  sd.c
  [    9d]  __FRAME_END__
  [    ab]  __init_array_end
  [    bc]  _DYNAMIC
  [    c5]  __init_array_start
  [    d8]  __GNU_EH_FRAME_HDR
  [    eb]  _GLOBAL_OFFSET_TABLE_
  [   101]  __libc_csu_fini
  [   111]  _ITM_deregisterTMCloneTable
  [   12d]  puts@@GLIBC_2.2.5
  [   13f]  _edata
  [   146]  global_var
  [   151]  __libc_start_main@@GLIBC_2.2.5
  [   170]  __data_start
  [   17d]  __gmon_start__
  [   18c]  __dso_handle
  [   199]  _IO_stdin_used
  [   1a8]  __libc_csu_init
  [   1b8]  __bss_start
  [   1c4]  main
  [   1c9]  global_var_in_bss
  [   1db]  __TMC_END__
  [   1e7]  _ITM_registerTMCloneTable
  [   201]  __cxa_finalize@@GLIBC_2.2.5
```
You'll find the symbols defined in source file [sd.c] such as the file name `sd.c`, the function `main` and the global variable defined `global_var_in_bss` and `global_var`.


### .DYNSTR 
This section stores the list of **dynamic symbol names**(in the form of NULL terminated strings) stored in **\.dynsym** section (explained next).

```shell
critical@d3ad:~/BINARY_DISECTION_COURSE/ELF/SECTION_HEADER_TABLE/SECTIONS_DESCRIPTION$ readelf -p .dynstr sd

String dump of section '.dynstr':
  [     1]  libc.so.6
  [     b]  puts
  [    10]  __cxa_finalize
  [    1f]  __libc_start_main
  [    31]  GLIBC_2.2.5
  [    3d]  _ITM_deregisterTMCloneTable
  [    59]  __gmon_start__
  [    68]  _ITM_registerTMCloneTable
```


### .SYMTAB AND .DYNSYM 
The section **\.symtab** contains the static symbols used in the program whereas the section **\.dynsym** contains the Runtime/Dynamic symbols used throughout the program. Static symbols are removed if the binary is stripped. The exported symbols (by Dynamic Link libraries) are also stored in .dynsym section. Let's look at the symbols with `-s` options in readelf which displays the symbol tables (both static and dynamic).

```shell
critical@d3ad:~/BINARY_DISECTION_COURSE/ELF/SECTION_HEADER_TABLE/SECTIONS_DESCRIPTION$ readelf -s sd

Symbol table '.dynsym' contains 7 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _ITM_deregisterTMCloneTab
     2: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND puts@GLIBC_2.2.5 (2)
     3: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND __libc_start_main@GLIBC_2.2.5 (2)
     4: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __gmon_start__
     5: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _ITM_registerTMCloneTable
     6: 0000000000000000     0 FUNC    WEAK   DEFAULT  UND __cxa_finalize@GLIBC_2.2.5 (2)

Symbol table '.symtab' contains 65 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000238     0 SECTION LOCAL  DEFAULT    1 
     2: 0000000000000254     0 SECTION LOCAL  DEFAULT    2 
     3: 0000000000000274     0 SECTION LOCAL  DEFAULT    3 
     4: 0000000000000298     0 SECTION LOCAL  DEFAULT    4 
     5: 00000000000002b8     0 SECTION LOCAL  DEFAULT    5 
     6: 0000000000000360     0 SECTION LOCAL  DEFAULT    6 
     7: 00000000000003e2     0 SECTION LOCAL  DEFAULT    7 
     8: 00000000000003f0     0 SECTION LOCAL  DEFAULT    8 
     9: 0000000000000410     0 SECTION LOCAL  DEFAULT    9 
    10: 00000000000004d0     0 SECTION LOCAL  DEFAULT   10 
    11: 00000000000004e8     0 SECTION LOCAL  DEFAULT   11 
    12: 0000000000000500     0 SECTION LOCAL  DEFAULT   12 
    13: 0000000000000520     0 SECTION LOCAL  DEFAULT   13 
    14: 0000000000000530     0 SECTION LOCAL  DEFAULT   14 
    15: 00000000000006e4     0 SECTION LOCAL  DEFAULT   15 
    16: 00000000000006f0     0 SECTION LOCAL  DEFAULT   16 
    17: 0000000000000708     0 SECTION LOCAL  DEFAULT   17 
    18: 0000000000000748     0 SECTION LOCAL  DEFAULT   18 
    19: 0000000000200db8     0 SECTION LOCAL  DEFAULT   19 
    20: 0000000000200dc0     0 SECTION LOCAL  DEFAULT   20 
    21: 0000000000200dc8     0 SECTION LOCAL  DEFAULT   21 
    22: 0000000000200fb8     0 SECTION LOCAL  DEFAULT   22 
    23: 0000000000201000     0 SECTION LOCAL  DEFAULT   23 
    24: 0000000000201014     0 SECTION LOCAL  DEFAULT   24 
    25: 0000000000000000     0 SECTION LOCAL  DEFAULT   25 
    26: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS crtstuff.c
    27: 0000000000000560     0 FUNC    LOCAL  DEFAULT   14 deregister_tm_clones
    28: 00000000000005a0     0 FUNC    LOCAL  DEFAULT   14 register_tm_clones
    29: 00000000000005f0     0 FUNC    LOCAL  DEFAULT   14 __do_global_dtors_aux
    30: 0000000000201014     1 OBJECT  LOCAL  DEFAULT   24 completed.7696
    31: 0000000000200dc0     0 OBJECT  LOCAL  DEFAULT   20 __do_global_dtors_aux_fin
    32: 0000000000000630     0 FUNC    LOCAL  DEFAULT   14 frame_dummy
    33: 0000000000200db8     0 OBJECT  LOCAL  DEFAULT   19 __frame_dummy_init_array_
    34: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS sd.c
    35: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS crtstuff.c
    36: 000000000000084c     0 OBJECT  LOCAL  DEFAULT   18 __FRAME_END__
    37: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS 
    38: 0000000000200dc0     0 NOTYPE  LOCAL  DEFAULT   19 __init_array_end
    39: 0000000000200dc8     0 OBJECT  LOCAL  DEFAULT   21 _DYNAMIC
    40: 0000000000200db8     0 NOTYPE  LOCAL  DEFAULT   19 __init_array_start
    41: 0000000000000708     0 NOTYPE  LOCAL  DEFAULT   17 __GNU_EH_FRAME_HDR
    42: 0000000000200fb8     0 OBJECT  LOCAL  DEFAULT   22 _GLOBAL_OFFSET_TABLE_
    43: 00000000000006e0     2 FUNC    GLOBAL DEFAULT   14 __libc_csu_fini
    44: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _ITM_deregisterTMCloneTab
    45: 0000000000201000     0 NOTYPE  WEAK   DEFAULT   23 data_start
    46: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND puts@@GLIBC_2.2.5
    47: 0000000000201014     0 NOTYPE  GLOBAL DEFAULT   23 _edata
    48: 00000000000006e4     0 FUNC    GLOBAL DEFAULT   15 _fini
    49: 0000000000201010     4 OBJECT  GLOBAL DEFAULT   23 global_var
    50: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND __libc_start_main@@GLIBC_
    51: 0000000000201000     0 NOTYPE  GLOBAL DEFAULT   23 __data_start
    52: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __gmon_start__
    53: 0000000000201008     0 OBJECT  GLOBAL HIDDEN    23 __dso_handle
    54: 00000000000006f0     4 OBJECT  GLOBAL DEFAULT   16 _IO_stdin_used
    55: 0000000000000670   101 FUNC    GLOBAL DEFAULT   14 __libc_csu_init
    56: 0000000000201020     0 NOTYPE  GLOBAL DEFAULT   24 _end
    57: 0000000000000530    43 FUNC    GLOBAL DEFAULT   14 _start
    58: 0000000000201014     0 NOTYPE  GLOBAL DEFAULT   24 __bss_start
    59: 000000000000063a    51 FUNC    GLOBAL DEFAULT   14 main
    60: 0000000000201018     4 OBJECT  GLOBAL DEFAULT   24 global_var_in_bss
    61: 0000000000201018     0 OBJECT  GLOBAL HIDDEN    23 __TMC_END__
    62: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _ITM_registerTMCloneTable
    63: 0000000000000000     0 FUNC    WEAK   DEFAULT  UND __cxa_finalize@@GLIBC_2.2
    64: 00000000000004e8     0 FUNC    GLOBAL DEFAULT   11 _init

```

**`Sym Num`** field indexes the symbol table.<br>
**`value`** field indicates the location of the symbol in virtual address space.<br>
**`Size`** field indicates the size in bytes of the symbol. Verify the size of 'global_var_in_bss', i.e. of datatype 'uint32_t' (32-bits).<br>
**`type`** indicates the type of symbol.<br>


| TYPE  | DESCRIPTION |
| :----: | :---------- |
| NOTYPE | Type not defined |
| SECTION | Symbol is associated with a section |
| FILE  | Associated with name of a source file |
| FUNC  | Associated with a function |
| OBJECT | Associated with data objects in executable (eg: variable names) |

**`bind`** indicates the 'scope' of a symbol. A function can be made of scope 'WEAK' by assigning attribute to it, i.e. by placing *\_\_attribute\_\_((weak))* before any function declaration (in C programming language). 

| SCOPE | DESCRIPTION |
| :---: | :---------- |
| LOCAL | These are those symbols which are locally available to the object file.
| GLOBAL| These are the symbols available to other object files at the time of<br> linking. For eg: 'global_var_in_bss' defined in [sd.c] |
| WEAK  | The definition of these symbols can be redefined. |

**`Vis`** indicates the **visibility** of the symbol. Mainly of 2 types.<br>

| VISIBILITY | DESCRIPTION |
| :--------: | :---------- |
| HIDDEN     | The name of the symbol is not visible outside of the running program.<br> Eg: LOCAL symbols|
| DEFAULT    | Visibility depends on the how binding of symbol is done, i.e. GLOABAL<br> AND WEAK symbols are visible as DEFAULT. |

**`Ndx`** field here contains the index number of the section from SHT. This field indicates the section in which the symbol is placed. Bellow are some special values for 'Ndx' field. 2 common types are explained bellow.<br> 

| TYPE | DESCRIPTION |
| :--: | :---------- |
| ABS  | Index remains the same even afer symbol relocation. |
| UND  | Symbols from the shared library, available at runtime are represented as UND. |

**`name`** Contains the name of the symbol.


### .INTERP 
Stores the location of the runtime linker (`ld.so`) used to link the object files together in a dynamic binary. 

```shell
critical@d3ad:~/BINARY_DISECTION_COURSE/ELF/SECTION_HEADER_TABLE/SECTIONS_DESCRIPTION$ readelf -p .interp sd

String dump of section '.interp':
  [     0]  /lib64/ld-linux-x86-64.so.2

```

### .GNU.HASH 
Stores the hash table for symbols in the binary.

### .DYNAMIC 
This section is present in dynamic binaries. It stores the dynamic linking information used by the linker (i.e. `ld.so`).



### .INIT 
Code executed before the program code executes to initialize the program. Useful in cases where object files without any main() is to be executed, this section provides a proper environment setup before the code in object files runs. Each object file requires an .init section for proper environment initialization.<br>


### .FINI 
This section includes code which is executed after the program executes successfully.

### .INIT_ARRAY 
This is an array of function pointers whose values (functions) and .init is executed first. If we mark any function with the attribute `constructor`, it will be treated as one of the elements of .init_array section, i.e. will be present in .init_array section rather than .text section. Furthur priorities can be assigned and order of execution for these initialization functions can be set (deciding which functions to be executed first among the array of function pointers which store addresses of the functions). The priority values can be set by giving values to `constructor` attribute from `101` onwards (since priority values from `0-100` are reserved for GCC. See for eg: the bellow source code. The usage of attributes is demonstrated bellow

```shell
__attribute__((constructor(102))) void func1() {}
__attribute__((constructor(101))) void func2() {} 
__attribute__((section(".fini"))) void abhi() {}
```
Here, func2 is given higher priority, i.e. 101, than func1(), i.e. 102, therefore will be executed before func1 after the source code is compiled.<br>
The `attribute((section(".fini")))` makes sure that the corresponding function named `abhi` gets placed in `.fini` section rather than .text section and hence will execute when the program ends. This way you can manage in which section does your function code gets stored in the final executable. If a section name (which does not exist) is used, then a new section will be created by that name specified and function will be placed in that newly created section. 

### .FINI_ARRAY 
It is an array of function pointers which execute right after the program terminates and exits main(). Crashes will stop the functions from being executed, i.e. functions in this section are executed at the end of a normally terminated program. To place a function in .fini section, use with attribute `destructor` (usage is demonstrated above).

### .PREINIT_ARRAY
This section stores an array of function pointers which are executed before any of the functions in .init section.

### .RELA.DYN 
Stores the runtime/dynamic relocation table. Stores the information regarding relocation of **variables** at the time of loading . More in this later in the course.

### .RELA.PLT 
Stores the runtime/dynamic relocation table. Stores the information regarding relocation of **functions** (unlike rela.dyn for varibles) at the time of loading.




**NOTE** : Don't get stressed out, it's OK to be not getting everything mentioned in here. Although not everything is mentioned here (sincere apologies), ofcourse there are many more sections left to be explained (that I'll include later after I am done making the course), but most important one's (which I personally feel) are mentioned right now to keep you going. This could be more useful if used as a review material at the time of analysis or understanding particular sections. Cool :)

<br>

[PREV - SECTION HEADER TABLE]<br>
[NEXT - PROGRAM HEADER TABLE]

 

[sd]: ./sd
[sd.c]: ./sd.c
[PREV - SECTION HEADER TABLE]: ./../SHT.md
[NEXT - PROGRAM HEADER TABLE]: ./../../PROGRAM_HEADER_TABLE/PHT.md