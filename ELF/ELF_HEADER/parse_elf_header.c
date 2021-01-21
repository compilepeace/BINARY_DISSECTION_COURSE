/**************************************************************************************
 * Author: Abhinav Thakur                                                             *
 * email : compilepeace@gmail.com                                                     *
 *                                                                                    *
 * Description: Parser for ELF Header as a part of Binary Dissection Course           *
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
           ElfN_Addr       Unsigned program address, uintN_t
           ElfN_Off        Unsigned file offset, uintN_t
           ElfN_Section    Unsigned section index, uint16_t
           ElfN_Versym     Unsigned version symbol information, uint16_t
           Elf_Byte        unsigned char
           ElfN_Half       uint16_t
           ElfN_Sword      int32_t
           ElfN_Word       uint32_t
           ElfN_Sxword     int64_t
           ElfN_Xword      uint64_t

           typedef struct {
               unsigned char e_ident[EI_NIDENT];
               uint16_t      e_type;
               uint16_t      e_machine;
               uint32_t      e_version;
               ElfN_Addr     e_entry;
               ElfN_Off      e_phoff;
               ElfN_Off      e_shoff;
               uint32_t      e_flags;
               uint16_t      e_ehsize;
               uint16_t      e_phentsize;
               uint16_t      e_phnum;
               uint16_t      e_shentsize;
               uint16_t      e_shnum;
               uint16_t      e_shstrndx;
           } ElfN_Ehdr;
*/

void parseElfHeader (uint8_t *map) {
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *) map;

	fprintf (stderr, "Entry point: 0x%lx\n", ehdr->e_entry);

	/* Similarly, one can access any field of ELF header */
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

	parseElfHeader (map);
}
