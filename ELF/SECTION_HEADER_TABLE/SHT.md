#	SHT (SECTION HEADER TABLE)
A binary as we know comprises of code and data. The content of binary file is organised into many blocks (made up of meaningful bytes) and each such block is termed as a `Section`. Each section in a binary has a `header` associated with it which describes that section. In other words each section is determined or known by its section header. Section Headers in a file does not overlap meaning that no byte can be present in more than one section.

##  SECTION HEADERS
Now, let's have a look at the sections of the elf binary [main]. We'll use readelf tool with -S option to look at the 'Section Headers' of the executable.

```shell
critical@d3ad:~/BINARY_DISECTION_COURSE/ELF/SECTION_HEADER_TABLE$ readelf -S --wide main
There are 29 section headers, starting at offset 0x1900:

Section Headers:
  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            0000000000000000 000000 000000 00      0   0  0
  [ 1] .interp           PROGBITS        0000000000400238 000238 00001c 00   A  0   0  1
  [ 2] .note.ABI-tag     NOTE            0000000000400254 000254 000020 00   A  0   0  4
  [ 3] .note.gnu.build-id NOTE            0000000000400274 000274 000024 00   A  0   0  4
  [ 4] .gnu.hash         GNU_HASH        0000000000400298 000298 00001c 00   A  5   0  8
  [ 5] .dynsym           DYNSYM          00000000004002b8 0002b8 000060 18   A  6   1  8
  [ 6] .dynstr           STRTAB          0000000000400318 000318 00003d 00   A  0   0  1
  [ 7] .gnu.version      VERSYM          0000000000400356 000356 000008 02   A  5   0  2
  [ 8] .gnu.version_r    VERNEED         0000000000400360 000360 000020 00   A  6   1  8
  [ 9] .rela.dyn         RELA            0000000000400380 000380 000030 18   A  5   0  8
  [10] .rela.plt         RELA            00000000004003b0 0003b0 000018 18  AI  5  22  8
  [11] .init             PROGBITS        00000000004003c8 0003c8 000017 00  AX  0   0  4
  [12] .plt              PROGBITS        00000000004003e0 0003e0 000020 10  AX  0   0 16
  [13] .text             PROGBITS        0000000000400400 000400 000172 00  AX  0   0 16
  [14] .fini             PROGBITS        0000000000400574 000574 000009 00  AX  0   0  4
  [15] .rodata           PROGBITS        0000000000400580 000580 000013 00   A  0   0  4
  [16] .eh_frame_hdr     PROGBITS        0000000000400594 000594 00003c 00   A  0   0  4
  [17] .eh_frame         PROGBITS        00000000004005d0 0005d0 000100 00   A  0   0  8
  [18] .init_array       INIT_ARRAY      0000000000600e10 000e10 000008 08  WA  0   0  8
  [19] .fini_array       FINI_ARRAY      0000000000600e18 000e18 000008 08  WA  0   0  8
  [20] .dynamic          DYNAMIC         0000000000600e20 000e20 0001d0 10  WA  6   0  8
  [21] .got              PROGBITS        0000000000600ff0 000ff0 000010 08  WA  0   0  8
  [22] .got.plt          PROGBITS        0000000000601000 001000 000020 08  WA  0   0  8
  [23] .data             PROGBITS        0000000000601020 001020 000014 00  WA  0   0  8
  [24] .bss              NOBITS          0000000000601034 001034 000004 00  WA  0   0  1
  [25] .comment          PROGBITS        0000000000000000 001034 000024 01  MS  0   0  1
  [26] .symtab           SYMTAB          0000000000000000 001058 0005d0 18     27  43  8
  [27] .strtab           STRTAB          0000000000000000 001628 0001d3 00      0   0  1
  [28] .shstrtab         STRTAB          0000000000000000 0017fb 000103 00      0   0  1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  l (large), p (processor specific)
```

Don't get stunned by this evil looking output. Now we're going to understand the output piece-by-piece.


## HOW TO READ THE OUTPUT
This is probably the most important part for beginners. Consider the snippet taken from above.

### THE FIRST LINE
```shell
There are 29 section headers, starting at offset 0x1900:
```
It briefs about the number of section headers the file has (remember the e_shnum field of ELF header) and the offset to the section header Table (remember the e_shoff field).

### FORMAT OF SECTION HEADER TABLE
It describes the format of section header table.
```shell
Section Headers:
  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al
```

| FIELD | Description |
| :---: | :--------- |
| [Nr]  | Identifies the index of section |
| Name  | Identifies the name of the section | 
| Type  | Identifies the section type. Type describes what kind of data section stores. <br>See [SECTION TYPES]. It is defined by `sh_type` member of Elf64_Shdr  structure<br> defined in `/usr/include/elf.h`. |
| Address | Identifies the `virtual address`(`logical addresss`) at which the section<br> starts. (Each process has its own Virtual Address provided by the OS kernel).<br> For beginners, it can be thought of as the memory address in RAM where the<br> section starts. |
| Offset | It is the distance (in bytes) from the begining of the file to the section. Don't<br> confuse youself between offset and Virtual Address, Virtual addresss is a<br> runtime concept whereas offset tells tells about physical location in the file. |
| Size | Identifies the size of each section .| 
| EntSize | Contains a 0 value if the section does not hold any table of fixed size entries. |
| Flags | Identifies the attributes/properties of a section. A lot can be judged from the<br> Flags of the Executable. Most common ones are `R`(Readable), `W`(Writable),<br> `X`(Executable), `A`(Allocate memory), `T`(Thread Local Storage).|
| Link | Stores the link information. The interpretation of this field depends on the<br> SECTION TYPE. |
| Info | Stores the auxiliary information. The interpretation of this field depends on<br> the SECTION TYPE. | 
| Align | Allignment constraint. This value ensures that the offset of the section should<br> be divisible by this value. Value 0 means that there is no alignment constraint. |

The structure named `Elf64_Shdr` (`Elf32_Shdr` on a 32-bit platform) defined in `/usr/include/elf.h` describes a section header of an ELF.
 
```shell
critical@d3ad:~$ cat /usr/include/elf.h | grep -B12 "} Elf64_Shdr"
typedef struct
{
  Elf64_Word	sh_name;		/* Section name (string table index) */
  Elf64_Word	sh_type;		/* Section type */
  Elf64_Xword	sh_flags;		/* Section flags */
  Elf64_Addr	sh_addr;		/* Section virtual addr at execution */
  Elf64_Off	sh_offset;		/* Section file offset */
  Elf64_Xword	sh_size;		/* Section size in bytes */
  Elf64_Word	sh_link;		/* Link to another section */
  Elf64_Word	sh_info;		/* Additional section information */
  Elf64_Xword	sh_addralign;		/* Section alignment */
  Elf64_Xword	sh_entsize;		/* Entry size if section holds table */
} Elf64_Shdr;
```

Let's see what all we understood by talking about some interesting entries from above.

```shell
Section Headers:
  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            0000000000000000 000000 000000 00      0   0  0
```
The first entry is always a NULL entry. It marks the section as inactive and is always present at the begining of Section Header Table (I mark it useless in my subconcious mind though). This section header is perhaps this way due to historical reasons when first entry was zeroed out to identify NULL references in a program (where all NULL references were made to jump to NULL entries) which certainly doesn't make sense to put in SHT as it doesn't contribute to program runtime (afterall, it was just a wild guess from my end!).

```shell
Section Headers:
  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al
...
  [13] .text             PROGBITS        0000000000400400 000400 000172 00  AX  0   0 16

...
```
Here, <br>
* **[Nr]** is index of the entry in SHT which is 13. In the world of computers, indexing usually starts from 0 rather than 1. <br>
* **Name (sh_name)**: it is the index into the *section header string table* (.shstrtab section). Above, readelf gets to .shstrtab and gets ".text" to display us.<br>
* **Type (sh_type)** of this section is PROGBITS (meaning that it stores programmer defined data). Almost every important section have a unique value for this field which is discussed next in section descriptions page. <br>
* **Address (sh_addr)** is `0x0000000000400400` meaning that this section loads at `0x0000000000400400` virtual address in memory. For correct information, virtual address must be consulted from PHT rather than SHT (which is ignored during program runtime). <br>
* **Offset (sh_offset)** tells that .text section is `0x00000400` bytes from the beginning of file.<br>
* **Size (sh_size)** of the section is `0x0000000000000172` bytes (representing on-disk size).<br>
* **ES (sh_entsize)** holds the single entry size for the sections that hold a table of fixed-sized entries.<br>
* **Flags (sh_flags)** set are `AX`, i.e. for 'Allocate' and 'Execute', meaning that memory should be allocated for this section at the runtime and that this section is supposed to be executable. The `A` flag set on a section means that the section will be present and loaded into the memory at the runtime unlike the sections without 'A' flag which will not be loaded into memory at the time of execution of program.<br>
* **Link (sh_link)** and **Info (sh_info)** are 0 meaning that this section does not link to any section or entry in the table.<br>
* **Align (sh_addralign)** is 16 which means that the offset `0x00000400`, should be divisible by 16(0x10), i.e. 0x00000400/0x10 = 0x0000040 (divisible by 16)
<br>



Go through each section names and analyse the Section Header Table (at least to be familiarised with the output). Now, I guess we understand how to read the output of `readelf`. Next, we'll dig a little deeper  to gain an understanding on the purpose of various sections listed in section header table.

<br>

[PREV - ELF HEADER] <br>
[NEXT - SECTIONS DESCRIPTION]



[main]: ./main
[SECTION TYPES]: ./../IMAGES/SECTION_TYPES.png
[PREV - ELF HEADER]: ./../ELF_HEADER/ELF_HEADER.md
[NEXT - SECTIONS DESCRIPTION]: ./SECTIONS_DESCRIPTION/SECTIONS_DESCRIPTION.md
