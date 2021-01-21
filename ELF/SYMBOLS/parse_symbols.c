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
#include <string.h>

/*
	>> static symbols in .symtab
        	typedef struct {
               uint32_t      st_name;
               unsigned char st_info;
               unsigned char st_other;
               uint16_t      st_shndx;
               Elf64_Addr    st_value;
               uint64_t      st_size;
        	} Elf64_Sym;

	>> dynamic symbols in .dynsym
			typedef struct {
               Elf64_Sxword    d_tag;
               union {
                   Elf64_Xword d_val;
                   Elf64_Addr  d_ptr;
               } d_un;
        	} Elf64_Dyn;
*/

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

	parseSymbols (map);
}
