/**************************************************************************************
 * Author: Abhinav Thakur                                                             *
 * email : compilepeace@gmail.com                                                     *
 *                                                                                    *
 * Description: Parser for Program Header Table as a part of Binary Dissection Course *
 *                                                                                    *
 **************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>

/*
            typedef struct {
               uint32_t   p_type;
               uint32_t   p_flags;
               Elf64_Off  p_offset;
               Elf64_Addr p_vaddr;
               Elf64_Addr p_paddr;
               uint64_t   p_filesz;
               uint64_t   p_memsz;
               uint64_t   p_align;
           } Elf64_Phdr;
*/

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


int main (int argc, char **argv) {

	int fd;
	struct stat sb;
	uint8_t *map;

	if (argc != 2) {
		fprintf (stderr, "Usage: %s <target_binary_path>\n", argv[0]);
		exit (-1);
	}

	fd = open (argv[1], O_RDONLY);
	if (fd == -1) {
		perror ("while opening target binary");
		exit (-1);
	}

	if (fstat (fd, &sb) == -1) {
		perror ("while fstat'ing target binary");
		exit (-1);
	}

	map = (uint8_t *) mmap ( NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (map == MAP_FAILED) {
		perror ("while mmap'ing target binary");
		exit(-1);
	}

	parsePht (map);
}
