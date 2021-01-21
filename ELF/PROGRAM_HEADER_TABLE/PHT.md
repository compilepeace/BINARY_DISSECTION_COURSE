#	PHT (PROGRAM HEADER TABLE)
The `Program Header Table` is an array of structures, each structure descrbing a segment and providing information the system needs to prepare the program for execution (reference : man elf), i.e. it describes the execution view of any ELF binary where each segment hold one or more sections. PHT can be parsed to display a **Sections to Segment** mapping, i.e. which segment in memory will store what sections of binary.

The compile-time linker uses a **linker script** to map sections within segments. We can indeed prepare our own linker script and tell the linker (/usr/bin/ld) explicitly to use our linker script while preparing an executable (unlike the default one it uses - [default_linker_script] which can be seen with the command `ld --verbose`). Writing linker scripts is a topic which is out of scope for this course.<br>

**NOTE**: The program header table is actually meaningful only to binaries that are likely to build up into their process images (executable & shared objects). Relocatable binaries doesn't require a PHT as they are to be processed by the static linker before being able to execute in memory.

Lets have a look at the program header table of the file [pht] compiled from the source [pht.c]. We'll use `-l` option of readelf, which displays the *Program Headers/Segments* for the executable.

```shell
critical@d3ad:~/BINARY_DISECTION_COURSE/ELF/PROGRAM_HEADER_TABLE$ readelf -l pht

Elf file type is EXEC (Executable file)
Entry point 0x400400
There are 9 program headers, starting at offset 64

Program Headers:
  Type           Offset             VirtAddr           PhysAddr
                 FileSiz            MemSiz              Flags  Align
  PHDR           0x0000000000000040 0x0000000000400040 0x0000000000400040
                 0x00000000000001f8 0x00000000000001f8  R      0x8
  INTERP         0x0000000000000238 0x0000000000400238 0x0000000000400238
                 0x000000000000001c 0x000000000000001c  R      0x1
      [Requesting program interpreter: /lib64/ld-linux-x86-64.so.2]
  LOAD           0x0000000000000000 0x0000000000400000 0x0000000000400000
                 0x00000000000008b0 0x00000000000008b0  R E    0x200000
  LOAD           0x0000000000000e00 0x0000000000600e00 0x0000000000600e00
                 0x0000000000000230 0x0000000000000238  RW     0x200000
  DYNAMIC        0x0000000000000e20 0x0000000000600e20 0x0000000000600e20
                 0x00000000000001d0 0x00000000000001d0  RW     0x8
  NOTE           0x0000000000000254 0x0000000000400254 0x0000000000400254
                 0x0000000000000044 0x0000000000000044  R      0x4
  GNU_EH_FRAME   0x00000000000006f8 0x00000000004006f8 0x00000000004006f8
                 0x0000000000000054 0x0000000000000054  R      0x4
  GNU_STACK      0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x0000000000000000 0x0000000000000000  RW     0x10
  GNU_RELRO      0x0000000000000e00 0x0000000000600e00 0x0000000000600e00
                 0x0000000000000200 0x0000000000000200  R      0x1

 Section to Segment mapping:
  Segment Sections...
   00     
   01     .interp 
   02     .interp .note.ABI-tag .note.gnu.build-id .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rela.dyn .rela.plt .init .plt .text .abhinav .fini .rodata .eh_frame_hdr .eh_frame 
   03     .init_array .fini_array .dynamic .got .got.plt .data .bss 
   04     .dynamic 
   05     .note.ABI-tag .note.gnu.build-id 
   06     .eh_frame_hdr 
   07     
   08     .init_array .fini_array .dynamic .got 
```

##  ANALYSING THE OUTPUT 
Let's analyse the output piece by piece. The first 3 lines, I guess are pretty much self explanatory, explaining the file type, the entry point of the program and the number of program headers along with offset to the beginning of PHT.

```shell
Elf file type is EXEC (Executable file)
Entry point 0x400400
There are 9 program headers, starting at offset 64
```

The next block of output,

```shell
Program Headers:
  Type           Offset             VirtAddr           PhysAddr
                 FileSiz            MemSiz              Flags  Align
  PHDR           0x0000000000000040 0x0000000000400040 0x0000000000400040
                 0x00000000000001f8 0x00000000000001f8  R      0x8
  INTERP         0x0000000000000238 0x0000000000400238 0x0000000000400238
                 0x000000000000001c 0x000000000000001c  R      0x1
      [Requesting program interpreter: /lib64/ld-linux-x86-64.so.2]
  LOAD           0x0000000000000000 0x0000000000400000 0x0000000000400000
                 0x00000000000008b0 0x00000000000008b0  R E    0x200000
  LOAD           0x0000000000000e00 0x0000000000600e00 0x0000000000600e00
                 0x0000000000000230 0x0000000000000238  RW     0x200000
  DYNAMIC        0x0000000000000e20 0x0000000000600e20 0x0000000000600e20
                 0x00000000000001d0 0x00000000000001d0  RW     0x8
  NOTE           0x0000000000000254 0x0000000000400254 0x0000000000400254
                 0x0000000000000044 0x0000000000000044  R      0x4
  GNU_EH_FRAME   0x00000000000006f8 0x00000000004006f8 0x00000000004006f8
                 0x0000000000000054 0x0000000000000054  R      0x4
  GNU_STACK      0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x0000000000000000 0x0000000000000000  RW     0x10
  GNU_RELRO      0x0000000000000e00 0x0000000000600e00 0x0000000000600e00
                 0x0000000000000200 0x0000000000000200  R      0x1
```


Each program header or PHT entry is actually a C structure (shown bellow) named `Elf64_Phdr` (`Elf32_Phdr` on a 32-bit platform) defined in `/usr/include/elf.h`.
```shell
critical@d3ad:~$ cat /usr/include/elf.h | grep -B9 " Elf64_Phdr"
{ 
  Elf64_Word	p_type;			/* Segment type */
  Elf64_Word	p_flags;		/* Segment flags */
  Elf64_Off	    p_offset;		/* Segment file offset */
  Elf64_Addr	p_vaddr;		/* Segment virtual address */
  Elf64_Addr	p_paddr;		/* Segment physical address */
  Elf64_Xword	p_filesz;		/* Segment size in file */
  Elf64_Xword	p_memsz;		/* Segment size in memory */
  Elf64_Xword	p_align;		/* Segment alignment */
} Elf64_Phdr;
```

### ANALYSING EACH FIELD (IN FIRST BLOCK OF OUTPUT)


**Type / p_type** : Similar to a section, each program header has a **Type** 

| TYPE | DESCRIPTION |
| :--: | :---------- |
| PHDR / PT_PHDR| It is the first entry specifying the **PHT** itself. (We can ofcourse write a linker script to add new segments and place any section into our desired segment). We can even omit this segment but if this segment is present, it should be placed above any loadable segment entry. |
| INTERP / PT_INTERP | This segment stores a NULL terminated string indicating the location of the interpreter program. It should be placed above any loadable segment entry. |
| LOAD / PT_LOAD | This type of segment hold the sections which will get loaded into the process address space when the program executes. Loadable segment entries in PHT appear in ascending order sorted by 'p_vaddr' member. |
| DYNAMIC / PT_DYNAMIC| specifies that this segment will store the dynamic linking information (i.e. the `.dynamic` section) | 
| NOTE / PT_NOTE | Specifies the location of auxiliary information (notes). |
| GNU_EH_FRAME | Exception Handling frame. |
| GNU_STACK / PT_GNU_STACK | This is a GNU extension used by the kernel to control the stack functionality by the segment flags set in *p_flags* member of struct 'Elf65_Phdr' |  

**Offset / p_offset** : This member holds the offset from beginning to the first byte of the corresponding segment.<br>
**VirtAddr / p_vaddr** : This member stores the virtual address, i.e. address where the segment resides will reside in memory.<br>
**PhysAddr / p_paddr** : Memory segment's physical address. This is mostly important to embedded systems where `virtual memory model` is not applied. <br>
**FileSiz / p_filesz** : Stores the size of segment in the file image (i.e. cummulative sum of on-disk size for all the sections in current segment) <br>
**Memsiz / p_memsz** : Stores the size of segment when the binary is loaded into memory.<br>
**Flags / p_flags** : This member/field stores the permissions (bit mask) for the segment. The 3 flag types are 

| FLAGS | DESCRIPTION |
| :---: | :---------- |
| PF_R | Marks the segment as **Readable** |
| PF_W | Marks the segment as **Writable** |
| PF_X | Marks the segment as **Executable** |

**Align / p_align** : This member holds the alignment constraint. The values of 0 and 1 mean that no alignment is required, otherwise, the condition `VirtAddr = Offset % Align` should be satisfied. All loadable segments are PAGE aligned.


Analysing second block of output, which indicates which section resides in which segment.

```shell
 Section to Segment mapping:
  Segment Sections...
   00     
   01     .interp 
   02     .interp .note.ABI-tag .note.gnu.build-id .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rela.dyn .rela.plt .init .plt .text .abhinav .fini .rodata .eh_frame_hdr .eh_frame 
   03     .init_array .fini_array .dynamic .got .got.plt .data .bss 
   04     .dynamic 
   05     .note.ABI-tag .note.gnu.build-id 
   06     .eh_frame_hdr 
   07     
   08     .init_array .fini_array .dynamic .got
```

Here, the left row of numbers (00, 01, 02, 03, ..), each correspond to the respected program header types described in first block (above), i.e. we have to link these numbers (indexes) to the program header types. Here for example, the number

00 - corresponds to segment of type PT_PHDR.<br>
01 - corresponds to segment of type PT_INTERP.<br>
02 - corresponds to segment of type PT_LOAD. (with permissions R(Read), E(Execute))<br>
03 - corresponds to segment of type PT_LOAD. (with permissions R(Read), W(Write))<br>
04 - corresponds to segment of type PT_DYNAMIC.<br>
.<br>
.<br>
and so on...

* Can you find the .text section ? What are the permissions of segment in which it resides ?<br>
Yes, it resides in 02 (LOAD) segment with RE flags (Read, Execute) turned on. This also makes sense since there is no need for the .text section to be writable.<br>

* Can you find the .data/.bss section ? What are the permissions of segment in which both of the resides ?<br>
Yes, they are found in the 03 (LOAD) segment with RW (Read, Write) flags turned on, i.e. this segment is both Readable and Writable. 
Therefore, 
the sections at number - 01 and 02 (i.e. of type PT_LOAD) are interesting. These segments contain all those sections which will be loaded into memory at the runtime including (.text, .data, .bss, .init, .init_array, .fini_array, .dynsym, .dynstr, .rodata). Notice how sections with same attribute (permissions) happens to be present in the same segment. All of the sections lie between start and end of the segment addresses.

**Security concerns** - Notice here that all of the segments will have either of the write / execute flags *mutually exclusively* turned on (W^X). This is a security concern to explicitly differentiate between CODE and DATA. If it is a CODE segment, it must not write its own memory (protection from self-modifying code) and if it is DATA segment it must not be interpreted as instructions (protection against arbitrary code injection attacks where the attacker after hijacking code flow might want to execute his malicious intensions preferably injected in DATA segment). Ofcourse there are ways to bypass these mitigations which mostly depends on the memory state and bug getting exploited during the attack scenario. Discussion in this area is out of scope for this course.


According to default linker scripts, the linker assigns the bellow given addresses as starting addresses to the segments  and its corresponding sections.<br>
`0x80480000` for i386 (linux platform)<br>
`0x00400000` for x86 (windows platform)<br>
`0x00010000` for ARM.<br>
For eg: to find out any Section address, simply<br>
Section Address = `0x80480000` + section_offset (on i386 linux)<br>
Section Address = `0x00400000` + section_offset (on x86 windows)<br>
Section Address = `0x00010000` + section_offset (on ARM)<br>

**NOTE** : A section is of interest to linker (which places the sections in appropriate segments according to what is specified in linker script), whereas a segment is how a program gets its sections loaded in memory space for execution.

Let's parse PHT programatically with the help of low-level structures provided by `/usr/include/elf.h`. Please look [parse_pht.c] for full source.
```
void parsePht (uint8_t *map) {
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *) map;
	Elf64_Phdr *pht  = (Elf64_Phdr *) &map[ehdr->e_phoff];

	for (int i = 0; i < ehdr->e_phnum; ++i) {
		
		switch (pht[i].p_type) {
			case PT_NULL:		fprintf (stderr, "NULL");
					 			break;
			case PT_LOAD:		fprintf (stderr, "LOAD");
					 			break;
			case PT_DYNAMIC:	fprintf (stderr, "DYNAMIC");
					 			break;
			case PT_INTERP:		fprintf (stderr, "INTERP");
					 			break;
			case PT_NOTE:		fprintf (stderr, "NOTE");
					 			break;
			case PT_SHLIB:		fprintf (stderr, "SHLIB");
					 			break;
			case PT_PHDR:		fprintf (stderr, "PHDR");
					 			break;
			case PT_LOPROC:		fprintf (stderr, "LOPROC");
					 			break;
			case PT_HIPROC:		fprintf (stderr, "HIPROC");
					 			break;
			case PT_GNU_STACK:	fprintf (stderr, "GNU_STACK");
					 			break;
			case PT_GNU_RELRO:  fprintf (stderr, "GNU_RELRO");
								break;
			case PT_GNU_EH_FRAME:   fprintf (stderr, "GNU_EH_FRAME");
									break;
			default: 			fprintf (stderr, "Unknown Phdr type");
					 			break;
		}

		fprintf (stderr, "\n\t0x%lx: 0x%lx", pht[i].p_offset, pht[i].p_filesz);
		
		/* Similarly, one can access any field of a program header */

		fprintf (stderr, "\n");
	}
}
```

With this, we conclude our discussion over fundamental components of ELF binary. Let's move on to understanding some internals of where the programmer-defined data resides in an ELF binary and source code to access/modify it.

<br>

[PREV - SECTIONS DESCRIPTION]<br>
[NEXT - SYMBOLS AND DEFINITIONS]



[default_linker_script]: ./default_linker_script
[pht]: ./pht
[pht.c]: ./pht.c
[parse_pht.c]: ./parse_pht.c

[PREV - SECTIONS DESCRIPTION]: ./../SECTION_HEADER_TABLE/SECTIONS_DESCRIPTION/SECTIONS_DESCRIPTION.md
[NEXT - SYMBOLS AND DEFINITIONS]: ./../SYMBOLS/SYMBOLS.md
