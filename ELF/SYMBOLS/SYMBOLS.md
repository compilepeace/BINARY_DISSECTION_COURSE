# SYMBOLS AND DEFINITIONS
Remember `.symtab` and `.dynsym` sections from [SECTION DESCRIPTIONS] ?<br>
 Right! these are the tables storing static and dynamic symbols from the binary. In high level programming languages like C/C++, a programmer uses function names that are used as **symbolic name** to represent a particular location (in file or memory). Similarly, all global/static/external variable names are also symbolic representations to some location. Together, we have what we call **function** and **data symbols**, i.e. symbols that represent *subroutines* and *variables* respectively. These symbols need to be fixed-up  with appropriate addresses (both by compile-time and by runtime linker) for the program to run correctly since machines are used to dealing with addresses not labels/symbols/strings. The process is termed as **symbol resolution** which further leads to **stitching**/**binding**/**linking** of many object files together into a single binary file.<br> 
From my understanding, there are 3 stages at which *symbol resolution* occurs- 
* **Compile-time fixup**: relocatable binaries (object files) have both function and data symbols which are resolved by the compile-time linker ld (the one which stiches up one or more object files into final executable binary on disk). After this fixup, the final executable preferably contains an address marking those symbol locations.
* **runtime fixup**: there are certain symbolic names which are external to the binary we are executing and whose location cannot be known until the runtime. One such example is the symbol resolution of **printf()** which is not defined inside our binary and rather inside `libc.so` (which is the Standard C library - remember one of the type of binaries on Linux is the shared object). To replace the call to printf() with an address, we need to know at what base address is `libc.so` mapped into memory which cannot be determined until the program execution (runtime). Therefore, dynamic symbols are resolved at runtime by the dynamic linker (`ld-linux.so`).
* **lazy binding**: A shared library being used by any program contains many functions whose addresses needs to be resolved by the dynamic linker before they can be used. To save a significant amount of load and linking time, **external function symbol resolution** (like printf() from `libc.so`) is postponed to the **first time that function is called by the program**. This is known as **lazy binding** or **delay loading** or **lazy loading** (term varies from one OS platfrom to the other). Ask yourself, why would someone care to fixup a library function address which is never getting called by the program ? This indeed saves a significant amount of program startup time! We'll discuss its mechanism probably later in the course.

**NOTE**: Don't confuse ld (compile-time linker a.k.a static linker) with `ld.so` (dynamic linker). The **compile-time linker (ld)** is a part of the **compiler toolchain** and is responsible for generating the final executable binary from source code while **dynamic linker** (`ld-linux.so` or simply `ld.so`) links the binary program with shared libraries (at runtime, i.e. in memory). The dynamic linker also goes by the name of **runtime linker** and **program interpreter**. I personally have been mindfucked for months while understanding machine level concepts remaining confused in linkers and loaders (it was when I felt lack of beginner friendly resources in this area).

## ANALYSING SYMBOLS
Let's use `-s` flag of readelf to display the symbol tables (both static and dynamic).

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
Each symbol table entry is of type `struct ElfN_Sym` as defined by `/usr/include/elf.h`- 
```
critical@d3ad:~$ cat /usr/include/elf.h | grep -B8 Elf64_Sym\;
typedef struct
{
  Elf64_Word	st_name;		/* Symbol name (string tbl index) */
  unsigned char	st_info;		/* Symbol type and binding */
  unsigned char st_other;		/* Symbol visibility */
  Elf64_Section	st_shndx;		/* Section index */
  Elf64_Addr	st_value;		/* Symbol value */
  Elf64_Xword	st_size;		/* Symbol size */
} Elf64_Sym;
```
The fields in readelf output correlate to the fields in `struct Elf64_Sym` as described below - 
* **`Num`** field indexes the symbol table.<br>

* **`Ndx (st_shndx)`** field contains the `section table index`, i.e. an index into SHT. It indicates the section in which the symbol is placed. Below are some special values for this field.<br> 

| TYPE | DESCRIPTION |
| :--: | :---------- |
| SHN_ABS  | symbol has an absolute value that remains same even afer symbol relocation done by compile-time linker. |
| SHN_COMMON | The symbol labels a `common block` that has not yet been allocated. This symbol's value (st_value) gives alignment constraint to the linker and st_size gives the number of bytes to allocate. Eg: uninitialized variables that goes into .bss section  |
| SHN_UNDEF  | This section index value means the symbol is **undefined**. This Symbols from the shared library, available at runtime are represented as UND. |
---

* **`value (st_value)`**  field has a value whose interpretation depends on the context in which the symbol is being used, i.e. whether the symbol is used during **static linking** (while in relocatable binaries) or **dynamic linking** (while in shared object/executable binaries). Its value interpretation is explained below - <br>

| FILE TYPE  | DESCRIPTION |
| :----: | :---------- |
| Relocatable binaries | For symbols to be able to get fixed up by compile-time linker, this field holds a `section offset`, i.e. it holds an offset value from the **beginnning of the section** identified by **st_shndx** field. <br> For symbols whose **st_shndx** is **SHN_COMMON**, st_value stores **alignment constraints** as described above (useful to the compile-time linker).|
| Shared objects and<br> executable binaries | st_value here stores a virtual address (memory interpretation is obviously more useful to a dynamic/runtime linker) rather than an offset from some section. |
---


* **`Size (st_size) `** field indicates the size in bytes occupied by the symbol. Verify the size of 'global_var_in_bss', i.e. of datatype 'uint32_t' (32-bits), whereas for function symbol *main*, it indicates the size of main(), i.e. 51 bytes.<br>

* **`type (st_info)`** st_info indicates the **type** and **binding** of the symbol.<br>

| TYPE  | DESCRIPTION |
| :----: | :---------- |
| STT_NOTYPE | Type not defined |
| STT_SECTION | Symbol is associated with a section |
| STT_FILE  | Associated with name of a source file |
| STT_FUNC  | Associated with a function |
| STT_OBJECT | Associated with data objects in executable (eg: variable names) |
---


* **`bind (st_info)`** indicates the 'scope' of a symbol. A function can be made of scope 'WEAK' by assigning attribute to it, i.e. by placing *\_\_attribute\_\_((weak))* before any function declaration (its the GCC magic). 

| SCOPE | DESCRIPTION |
| :---: | :---------- |
| STB_LOCAL | Symbols that are not visible outside of the object file.<br>For Eg: symbols associated with static variables/functions.
| STB_GLOBAL| Symbols visible to all object files being combined. <br>For eg: 'global_var_in_bss' defined in [sd.c] |
| STB_WEAK  | Symbols that are globally visible with less preference given . |
---

* **`Vis (st_other)`** indicates the **visibility** of the symbol. Mainly of 2 types.<br>

| VISIBILITY | DESCRIPTION |
| :--------: | :---------- |
| HIDDEN     | The name of the symbol is not visible outside of the running program. Eg: LOCAL symbols|
| DEFAULT    | Visibility depends on the how binding of symbol is done, i.e. GLOABAL and WEAK symbols are visible as DEFAULT. |
---

* **`name (st_name)`** This field stores an index into the symbol string table. readelf parses the string table for us to display symbol name in output above.

<br>

## sh_link and sh_info in SHT
Let's revisit and analyse SHT entries again and to focus only on section headers whose **sh_link** and **sh_info** field links to an index in SHT.

```shell
critical@d3ad:~/BINARY_DISECTION_COURSE/ELF/SECTION_HEADER_TABLE$ readelf -S --wide main
There are 29 section headers, starting at offset 0x1900:

Section Headers:
  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            0000000000000000 000000 000000 00      0   0  0
  [ 1] .interp           PROGBITS        0000000000400238 000238 00001c 00   A  0   0  1
...
  [ 4] .gnu.hash         GNU_HASH        0000000000400298 000298 00001c 00   A  5   0  8
  [ 5] .dynsym           DYNSYM          00000000004002b8 0002b8 000060 18   A  6   1  8
  [ 6] .dynstr           STRTAB          0000000000400318 000318 00003d 00   A  0   0  1
  [ 7] .gnu.version      VERSYM          0000000000400356 000356 000008 02   A  5   0  2
  [ 8] .gnu.version_r    VERNEED         0000000000400360 000360 000020 00   A  6   1  8
  [ 9] .rela.dyn         RELA            0000000000400380 000380 000030 18   A  5   0  8
  [10] .rela.plt         RELA            00000000004003b0 0003b0 000018 18  AI  5  22  8
...
  [20] .dynamic          DYNAMIC         0000000000600e20 000e20 0001d0 10  WA  6   0  8
  [21] .got              PROGBITS        0000000000600ff0 000ff0 000010 08  WA  0   0  8
  [22] .got.plt          PROGBITS        0000000000601000 001000 000020 08  WA  0   0  8
  [23] .data             PROGBITS        0000000000601020 001020 000014 00  WA  0   0  8
  [24] .bss              NOBITS          0000000000601034 001034 000004 00  WA  0   0  1
  [25] .comment          PROGBITS        0000000000000000 001034 000024 01  MS  0   0  1
  [26] .symtab           SYMTAB          0000000000000000 001058 0005d0 18     27  43  8
  [27] .strtab           STRTAB          0000000000000000 001628 0001d3 00      0   0  1
  [28] .shstrtab         STRTAB          0000000000000000 0017fb 000103 00      0   0  1
```

Analysing the output, we can see how `sh_link` and `sh_info` are used to connect various sections together - 
* `.gnu.hash` links to `.dynsym` section which makes sense as it is used by dynamic linker to quickly compute what symbols are not present in .dynsym section (remember bloom filters).
* `.dynsym` links to `.dynstr` section which stores the ASCII strings for dynamic symbols. Its info field points to `.interp` section which perhaps indicate the program interpreter used to process dynamic symbols.
* `.gnu.version` and `.gnu.version_r` sections are responsible to handle `symbol versioning` (therefore, linked to `.dynsym` and `.dynstr` section).
* `.rela.dyn` and `.rela.plt` sections contain **relocation records** (for data and function symbols respectively) that are consulted by dynamic linker while fixing up addresses for dynamic symbols. Therefore are linked to `.dynsym` section. Notice the *sh_info* field of `.rela.plt` section points to .got.plt section. We'll discuss relocations in more depth later in the primer.
* `.dynamic` links to `.dynstr` section used as string table.
* `.symtab` (storing static symbols) links to its corresponding string table - `.strtab` section.

## Parsing Static and Dynamic Symbols
Below is how you can find symbols in a binary - 
* Parse SHT.
* Find .dynsym & .symtab section headers as well as .dynstr and .strtab section for respective symbol names. <br>

You can find the entire source [here]. I've just printed symbol `st_name` and `st_value` of static & dynamic symbols to STDERR while you can get any attribute of symbols you like.

```
void parseSymbols (uint8_t *map) {
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *) map;
	Elf64_Shdr *sht  = (Elf64_Shdr *) &map[ehdr->e_shoff];
	char *shstrtab   = (char *) &map[sht[ehdr->e_shstrndx].sh_offset];
	Elf64_Sym *symtab = NULL, *dynsym = NULL;
	uint64_t dynsym_size = 0, symtab_size = 0;
	char *dynstr = NULL, *strtab = NULL;


	for (int i = 0; i < ehdr->e_shnum; ++i) {
		char *section_name = &shstrtab[sht[i].sh_name];
		
		/* dynamic symbols found */
		if (strncmp (".dynsym", section_name, strlen(".dynsym")) == 0 &&
			sht[i].sh_type == SHT_DYNSYM) {
			fprintf (stderr, "%s shdr @ 0x%lx\n", section_name, sht[i].sh_offset);
			dynsym = (Elf64_Sym *) &map[sht[i].sh_offset];
			dynsym_size = sht[i].sh_size;
		}

		/* .dynstr section found */
		if (strncmp (".dynstr", section_name, strlen(".dynstr")) == 0 &&
			sht[i].sh_type == SHT_STRTAB) {
			fprintf (stderr, "%s shdr @ 0x%lx\n", section_name, sht[i].sh_offset);
			dynstr = (char *) &map[sht[i].sh_offset];
		}
		

		/* static symbols found */
		if (strncmp (".symtab", section_name, strlen (".symtab")) == 0 &&
			sht[i].sh_type == SHT_SYMTAB) {
			fprintf (stderr, "%s shdr @ 0x%lx\n", section_name, sht[i].sh_offset);
			symtab = (Elf64_Sym *) &map[sht[i].sh_offset];
			symtab_size = sht[i].sh_size;
		}

		/* .strtab section found */
		if (strncmp (".strtab", section_name, strlen (".strtab")) == 0 &&
			sht[i].sh_type == SHT_STRTAB) {
			fprintf (stderr, "%s shdr @ 0x%lx\n", section_name, sht[i].sh_offset);
			strtab = (char *) &map[sht[i].sh_offset];
		} 
	}

	/* parsing dynamic symbol table */
	fprintf (stderr, "\n\n# .dynsym entries: \n");
	for (uint64_t i = 0; i < dynsym_size / sizeof (Elf64_Sym); ++i) {
		char *symname = &dynstr[dynsym[i].st_name];

		fprintf (stderr, "\t0x%06lx: ", dynsym[i].st_value);
		if (*symname == '\x00') 
			fprintf (stderr, "NULL\n");
		else 
			fprintf (stderr, "%s\n", symname);
	}

	/* parsing static symbol table */
	fprintf (stderr, "\n# .symtab entries: \n");
	for (uint64_t i = 0; i < symtab_size / sizeof (Elf64_Sym); ++i) {
		char *symname = &strtab[symtab[i].st_name];
		fprintf (stderr, "\t0x%06lx: ", symtab[i].st_value);
		if (*symname == '\x00')
			fprintf (stderr, "NULL\n");
		else
			fprintf (stderr, "%s\n", symname);
	}
}
```

Now, that we've discussed what symbols are and the process of accessing them from a binary, next, we will look into how are these symbolic references stitched/linked/connected to their symbolic definitions (a process known as **relocation**). 


[PREV - PROGRAM HEADER TABLE]<br>
[NEXT - RELOCATIONS]

[SECTION DESCRIPTIONS]: ./../SECTION_HEADER_TABLE/SECTIONS_DESCRIPTION/SECTIONS_DESCRIPTION.md
[sd.c]: ./../SECTION_HEADER_TABLE/SECTIONS_DESCRIPTION/sd.c
[here]: ./parse_symbols.c

[PREV - PROGRAM HEADER TABLE]: ./../PROGRAM_HEADER_TABLE/PHT.md
[NEXT - RELOCATIONS]: ./../RELOCATIONS/RELOCATIONS.md
