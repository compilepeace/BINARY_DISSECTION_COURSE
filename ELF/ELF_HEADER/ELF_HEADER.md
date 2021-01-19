#	ELF HEADER
The elf file header is always present at the offset 0 of a binary program (remember FILE OFFSETS from the [Introduction]?) and works as a roadmap to parse ELF binary. Let's have a look at it. 


## ANALYSIS
The files `hello_32` and `hello_64` are the 32-bit and 64-bit compiled versions of the file [hello.c].<br>
Lets have a look at the ELF header of the 32-bit compiled binary. I have cross compiled the source code for a 32 bit platform. Try viewing the file with cat.

```shell
ELF��4,4 	(44�4�  TT�T�����

                                                         �
                                                          �
                                                           ����hh�h�DDP�td�����DDQ�tdR�td

        �
         ���/lib/ld-linux.so.2GNUGNU�z�8��u�6�0P#g�G�  �K��6 
                                                                               ��libc.so.6_IO_stdi,��__libc_start_mainGLIBC_2.0__gmon_start__ii
      �S�����k��������t�&�[��5����%
                                      �h������%��f�1�^�����PTR�#����p���P�����PQV����P�������$�f�f�f�f����f�f�f�f�f�f�f��$�f�f�f�f�f�f���=�t$���tU����h��Ѓ��É���'�Í�&��'��-����������t ���tU����Ph��҃��Ít&�Í��=�uU����l������Ív�Í�&��'U��]��U���
...
problably some more crap    
```
We see here, no meaningful data can be scraped out from this output. Therefore, we ought to use a tool to analyse the information provided by the binary, here comes readelf in action.<br>

### READELF
This tool scraps the ELF binary to extract and display meaningful information from it. The '-h' flag is used to display ELF header of file `hello_64`.

```shell
critical@d3ad:~/COURSE_DISECTING_BINARIES/ELF/ELF_HEADER$ readelf -h hello_64       
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x4003b0
  Start of program headers:          64 (bytes into file)
  Start of section headers:          6256 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         9
  Size of section headers:           64 (bytes)
  Number of section headers:         27
  Section header string table index: 26

```

ELF header is actually a structure - `Elf64_Ehdr` defined in file - `/usr/include/elf.h`.

```shell
critical@d3ad:~$ cat /usr/include/elf.h | grep -B16 " Elf64_Ehdr;"
typedef struct
{
  unsigned char	e_ident[EI_NIDENT];	/* Magic number and other info */
  Elf64_Half	e_type;			/* Object file type */
  Elf64_Half	e_machine;		/* Architecture */
  Elf64_Word	e_version;		/* Object file version */
  Elf64_Addr	e_entry;		/* Entry point virtual address */
  Elf64_Off     e_phoff;		/* Program header table file offset */
  Elf64_Off     e_shoff;		/* Section header table file offset */
  Elf64_Word	e_flags;		/* Processor-specific flags */
  Elf64_Half	e_ehsize;		/* ELF header size in bytes */
  Elf64_Half	e_phentsize;		/* Program header table entry size */
  Elf64_Half	e_phnum;		/* Program header table entry count */
  Elf64_Half	e_shentsize;		/* Section header table entry size */
  Elf64_Half	e_shnum;		/* Section header table entry count */
  Elf64_Half	e_shstrndx;		/* Section header string table index */
} Elf64_Ehdr;
```



### ANALYSING EACH FIELD OF ELF HEADER

####  Magic (e_ident[EI_NIDNT])
 e_ident is an array *identifying* the binary in a **processor-independent** fashion, meaning this array is probably the only part which doesn't use encoding of the target processor. It basically works as an identifier for ELF specifying how to decode and interpret the remaining binary. The output above displays 16 hexadecimal bytes in a row. Here the first 4 bytes (7f 45 4c 46) are called magic numbers. These bytes are used to identify an ELF format. Here first byte is fixed (0x7f). Have a look at the ASCII representation of the hex values (type `man ASCII` to open up ASCII table) and see the values of bytes after 0x7f, i.e. 0x45 0x4c 0x46 are 'E', 'L', 'F' respectively. 


|  BYTES     |    Description  |
|:----------:|:---------------|
|  7f        |  It is a Fixed byte     |
|  45 4c 46  |  ELF signature. Look at the ASCII table (with command <br> `man ASCII`), the values corresponding to them are<br> 'E', 'L', 'F'. This indicates that it is an ELF executable.
|  02        |  It is the class described bellow. (ELF64) |
|  01        |  It is the Data field described bellow (2's complement,<br>little endian) 
|  01        | Version field bellow (Version Number 1)|
|  00        | OS/ABI field bellow  (UNIX - System V) | 
|  00 00 00 00 00 00 00 00 | Padded bytes for allignment, currently unused, kept<br> for future use.

The file `/usr/include/elf.h` includes the MACROS which define values for each byte.
```shell
critical@d3ad:~$ cat /usr/include/elf.h | grep -n -A2 "EI_MAG"
 
107:#define EI_MAG0		0		/* File identification byte 0 index */
108-#define ELFMAG0		0x7f		/* Magic number byte 0 */
109-
110:#define EI_MAG1		1		/* File identification byte 1 index */
111-#define ELFMAG1		'E'		/* Magic number byte 1 */
112-
113:#define EI_MAG2		2		/* File identification byte 2 index */
114-#define ELFMAG2		'L'		/* Magic number byte 2 */
115-
116:#define EI_MAG3		3		/* File identification byte 3 index */
117-#define ELFMAG3		'F'		/* Magic number byte 3 */
118-
```

Verify the magic field by hd (hexdump) tool.
```shell
critical@d3ad:~/COURSE_DISECTING_BINARIES/ELF/ELF_HEADER$ hd -n 16 hello_64 
00000000  7f 45 4c 46 02 01 01 00  00 00 00 00 00 00 00 00  |.ELF............|
00000010
```


####  Class 
 It is the 5th byte in ELF header with value of 0x01, specifying a 64-bit binary, i.e. the entire binary is made up of 64-bit objects.

| Value | Description |
| :---: | :---------: |
|  1    |  32-bit objects|
|  2    |  64-bit ojects |

```shell
critical@d3ad:~$ cat /usr/include/elf.h | grep -n -A5 "EI_CLASS"
123:#define EI_CLASS	4		/* File class byte index */
124-#define ELFCLASSNONE 0		/* Invalid class */
125-#define ELFCLASS32	1		/* 32-bit objects */
126-#define ELFCLASS64	2		/* 64-bit objects */
127-#define ELFCLASSNUM	3
128-
```


#### Data 
This field describes the data encoding (little-endian in this case). You can have a look at the `/usr/include/elf.h` to know what each value means.

| Value  | Description |
| :---:  | :---------: |
|  1     | 2's complement, little endian |
|  2     | 2's complement, big endian    |

```shell
critical@d3ad:~$ cat /usr/include/elf.h | grep -n -A5 "EI_DATA"
129:#define EI_DATA	5		/* Data encoding byte index */
130-#define ELFDATANONE	0		/* Invalid data encoding */
131-#define ELFDATA2LSB	1		/* 2's complement, little endian */
132-#define ELFDATA2MSB	2		/* 2's complement, big endian */
133-#define ELFDATANUM	3
134-	
```

#### Version
ELF header version number.


```shell
critical@d3ad:~$ cat /usr/include/elf.h | grep -n -A2 "EI_VERSION"
135:#define EI_VERSION	6		/* File version byte index */
136-					/* Value must be EV_CURRENT */
137-
```

#### OS/ABI
These are the values for target OS ABI (**A**pplication **B**inary **I**nterface).
```shell
critical@d3ad:~$ cat /usr/include/elf.h | grep -n -A19 "EI_OSABI"
138:#define EI_OSABI	7		/* OS ABI identification */
139-#define ELFOSABI_NONE		0	/* UNIX System V ABI */
140-#define ELFOSABI_SYSV		0	/* Alias.  */
141-#define ELFOSABI_HPUX		1	/* HP-UX */
142-#define ELFOSABI_NETBSD		2	/* NetBSD.  */
143-#define ELFOSABI_GNU		3	/* Object uses GNU ELF extensions.  */
144-#define ELFOSABI_LINUX         ELFOSABI_GNU /* Compatibility alias.  */
145-#define ELFOSABI_SOLARIS            6	/* Sun Solaris.  */
146-#define ELFOSABI_AIX		7	/* IBM AIX.  */
147-#define ELFOSABI_IRIX		8	/* SGI Irix.  */
148-#define ELFOSABI_FREEBSD	        9	/* FreeBSD.  */
149-#define ELFOSABI_TRU64		10	/* Compaq TRU64 UNIX.  */
150-#define ELFOSABI_MODESTO	        11	/* Novell Modesto.  */
151-#define ELFOSABI_OPENBSD	        12	/* OpenBSD.  */
152-#define ELFOSABI_ARM_AEABI	        64	/* ARM EABI */
153-#define ELFOSABI_ARM		97	/* ARM */
154-#define ELFOSABI_STANDALONE	        255	/* Standalone (embedded) application */
155-
156-#define EI_ABIVERSION	        8		/* ABI version */
157-
```

#### TYPE (e_type)
MACROs identifying the type of ELF binary (executable, shared object, relocatable and CORE dumps).

```shell
critical@d3ad:~/COURSE_DISECTING_BINARIES/Introduction$ cat /usr/include/elf.h | grep -n -A9 "ET_NONE"
162:#define ET_NONE		0		/* No file type */
163-#define ET_REL		1		/* Relocatable file */
164-#define ET_EXEC		2		/* Executable file */
165-#define ET_DYN		3		/* Shared object file */
166-#define ET_CORE		4		/* Core file */
167-#define	ET_NUM		5		/* Number of defined types */
168-#define ET_LOOS		0xfe00		/* OS-specific range start */
169-#define ET_HIOS		0xfeff		/* OS-specific range end */
170-#define ET_LOPROC	0xff00		/* Processor-specific range start */
171-#define ET_HIPROC	0xffff		/* Processor-specific range end */
```

#### MACHINE (e_machine)
It is the machine specification required to properly run this ELF.
```shell
critical@d3ad:~$ cat /usr/include/elf.h | grep -n -A190 "EM_NONE"
175:#define EM_NONE		 0	/* No machine */
176-#define EM_M32		 1	/* AT&T WE 32100 */
177-#define EM_SPARC	 2	/* SUN SPARC */
178-#define EM_386		 3	/* Intel 80386 */
179-#define EM_68K		 4	/* Motorola m68k family */
180-#define EM_88K		 5	/* Motorola m88k family */
181-#define EM_IAMCU	 6	/* Intel MCU */
182-#define EM_860		 7	/* Intel 80860 */
183-#define EM_MIPS		 8	/* MIPS R3000 big-endian */
184-#define EM_S370		 9	/* IBM System/370 */
185-#define EM_MIPS_RS3_LE	10	/* MIPS R3000 little-endian */
....
And tons of other machine types (nearly 200)
```

#### VERSION (e_version)
Specifies the version of the file. (not the ELF header's)

```shell
critical@d3ad:~$ cat /usr/include/elf.h | grep -n -A4 "EV_NONE"
378:#define EV_NONE		0		/* Invalid ELF version */
379-#define EV_CURRENT	        1		/* Current version */
380-#define EV_NUM		2
381-
```

#### ENTRY POINT ADDRESS (e_entry)
This specifies the address where the execution of the program starts. If you've ever programmed in C/C++ languages or read books regarding the same, you may have read - **execution of the program always begins from main()**. While this introductory statement might be helpful to beginners, it is **not** an absolulte truth. Let's examine what location does this entry point address point to.
```
critical@d3ad:~$ readelf -h hello_32 | grep Entry
  Entry point address:               0x80482e0

critical@d3ad:~$ objdump -d ./hello_32 | grep -A18 80482e0
080482e0 <_start>:
 80482e0:	31 ed                	xor    %ebp,%ebp
 80482e2:	5e                   	pop    %esi
 80482e3:	89 e1                	mov    %esp,%ecx
 80482e5:	83 e4 f0             	and    $0xfffffff0,%esp
 80482e8:	50                   	push   %eax
 80482e9:	54                   	push   %esp
 80482ea:	52                   	push   %edx
 80482eb:	e8 23 00 00 00       	call   8048313 <_start+0x33>
 80482f0:	81 c3 10 1d 00 00    	add    $0x1d10,%ebx
 80482f6:	8d 83 70 e4 ff ff    	lea    -0x1b90(%ebx),%eax
 80482fc:	50                   	push   %eax
 80482fd:	8d 83 10 e4 ff ff    	lea    -0x1bf0(%ebx),%eax
 8048303:	50                   	push   %eax
 8048304:	51                   	push   %ecx
 8048305:	56                   	push   %esi
 8048306:	c7 c0 f6 83 04 08    	mov    $0x80483f6,%eax
 804830c:	50                   	push   %eax
 804830d:	e8 ae ff ff ff       	call   80482c0 <__libc_start_main@plt>
 8048312:	f4                   	hlt 
```
As you can see, entry point address (0x80482e0) points to a label named `_start`. If you've ever programmed in assembly you might already be familiar of `_start`, which marks the beginning of programmer specified instructions. In C/C++ compiled binaries, this routine is embedded by the compiler which acts as initialization code to setup environment for the execution of `main()` as specified by the programmer. <br>
So, where is the **main()** and how is it called ? <br>
**main()** is called by **__libc_start_main()**. Analysing this function call, we can know what parameters it takes by looking at what all is pushed onto the stack. Search out the function prototype for **__libc_start_main()** and also read **x86 calling conventions**. Can you figure out the address of main() by yourself ?

#### START OF PROGRAM HEADERS (e_phoff)
It specifies the offset to the **P**rogram **H**eader **T**able (**PHT**) (explained later in the course), from the very first byte in the file.

#### START OF SECTION HEADERS (e_shoff)
It specifies the offset to the **S**ection **H**eader **T**able (**SHT**) (explained later in the course), from the very first byte in the file.

#### FLAGS (e_flags)
The `EFLAGS` register is set to this value when the program is loaded into memory.

#### SIZE OF THIS HEADER (e_ehsize)
Identifies the size of this 'ELF Header'

#### SIZE OF PROGRAM HEADERS (e_phentsize)
Identifies the size of each entry in PHT.

#### NUMBER OF PROGRAM HEADERS (e_phnum)
Displays the number of Program Headers (Segments) in the PHT.

#### SIZES OF SECTION HEADERS (e_shentsize)
Displays size of each section header.

#### NUMBER OF SECTION HEADERS (e_shnum)
Specifies the number of section headers (starting from the NULL entry) in SHT.

#### SECTION HEADER STRING TABLE INDEX (e_shstrndx)
The value is 26. It means the section at index number 26 (in Section Header Table), stores all the section names (which are NULL terminated ASCII strings). Let's have a look at this section.


```shell
critical@d3ad:~/COURSE_DISECTING_BINARIES/ELF/ELF_HEADER$ readelf -p 26 hello_64

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
  [    84]  .init
  [    8a]  .text
  [    90]  .fini
  [    96]  .rodata
  [    9e]  .eh_frame_hdr
  [    ac]  .eh_frame
  [    b6]  .init_array
  [    c2]  .fini_array
  [    ce]  .dynamic
  [    d7]  .got
  [    dc]  .got.plt
  [    e5]  .data
  [    eb]  .bss
  [    f0]  .comment

```


## NOTE
  You can't mug up these things. All this will start making sense after getting some hands on disecting and analysing ELF binaries. We'll now start with SHT and PHT. This was designed to give you an entry point into binary dissection :)


<br>

**[PREV - ELF]** <br>
**[NEXT - SECTION HEADER TABLE]**

[Introduction]: ./../../Introduction/Introduction.md
[hello.c]: ./hello.c
[PREV - ELF]: ./../ELF.md
[NEXT - SECTION HEADER TABLE]: ./../SECTION_HEADER_TABLE/SHT.md