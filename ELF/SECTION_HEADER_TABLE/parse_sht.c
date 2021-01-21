/**************************************************************************************
 * Author: Abhinav Thakur                                                             *
 * email : compilepeace@gmail.com                                                     *
 *                                                                                    *
 * Description: Parser for Section Header Table as a part of Binary Dissection Course *
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
               uint32_t   sh_name;
               uint32_t   sh_type;
               uint64_t   sh_flags;
               Elf64_Addr sh_addr;
               Elf64_Off  sh_offset;
               uint64_t   sh_size;
               uint32_t   sh_link;
               uint32_t   sh_info;
               uint64_t   sh_addralign;
               uint64_t   sh_entsize;
           } Elf64_Shdr;
*/


void parseSht (uint8_t *map) {
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *) map;
	Elf64_Shdr *sht  = (Elf64_Shdr *) &map[ehdr->e_shoff];
	char *shstrtab   = &map[ sht[ehdr->e_shstrndx].sh_offset ]; 

	fprintf (stderr, "shstrtab: first string: %s\n", shstrtab);

	for (int i = 0; i < ehdr->e_shnum; ++i) {

		/* sh_name */
		fprintf (stderr, "[%d] %s\n\t", i, &shstrtab[sht[i].sh_name]);
		
		/* sh_type */
		switch (sht[i].sh_type) {
			case SHT_NULL: 		fprintf (stderr, "NULL");
				       			break;
			case SHT_PROGBITS:	fprintf (stderr, "PROGBITS");
					  			break;
			case SHT_SYMTAB:	fprintf (stderr, "SYMTAB");
								break;
			case SHT_STRTAB: 	fprintf (stderr, "STRTAB");
								break;
			case SHT_RELA:		fprintf (stderr, "RELA");
								break;
			case SHT_GNU_HASH:  fprintf (stderr, "GNU HASH");
								break;
			case SHT_DYNAMIC:   fprintf (stderr, "DYNAMIC");
                                break;
			case SHT_NOTE:      fprintf (stderr, "NOTE");
                                break;
            case SHT_NOBITS:    fprintf (stderr, "NOBITS");
                                break;
			case SHT_REL:       fprintf (stderr, "REL");
                                break;
            case SHT_SHLIB:     fprintf (stderr, "SHLIB");
                                break;
			case SHT_DYNSYM:    fprintf (stderr, "DYNSYM");
                                break;
            case SHT_LOPROC:    fprintf (stderr, "LOPROC");
                                break;
			case SHT_HIPROC:    fprintf (stderr, "HIPROC");
                                break;
            case SHT_LOUSER:    fprintf (stderr, "LOUSER");
                                break;
			case SHT_HIUSER:    fprintf (stderr, "HIUSER");
                                break;
            default:			fprintf (stderr, "Unknown Section");
								break;	
		}		
		fprintf (stderr, "\t");

		/* sh_flags */
		if (sht[i].sh_flags & SHF_WRITE)	fprintf (stderr, "W");
		else								fprintf (stderr, " ");
		if (sht[i].sh_flags & SHF_ALLOC)	fprintf (stderr, "A");
											fprintf (stderr, " ");
		if (sht[i].sh_flags & SHF_EXECINSTR)fprintf (stderr, "X");
											fprintf (stderr, " ");
		if (sht[i].sh_flags & SHF_MASKPROC) fprintf (stderr, "M");
											fprintf (stderr, " ");
		fprintf (stderr, "\t");

		/* Similarly, one can access any field of a section header */

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

	parseSht (map);
}
