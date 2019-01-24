#include <stdio.h>



// Function with destructor attribute
__attribute__((destructor)) void dest()
{
	printf("%s: Executing after completion of main()\n", __FUNCTION__);
}


// Function with constructor attribute
__attribute__((constructor)) void cons()
{
	printf("%s: Now main() will start executing\n", __FUNCTION__);
}


__attribute__((section(".abhinav"))) void abhi()
{
	printf("%s: Function resides in .abhinav section\n", __FUNCTION__);
}


int main(int argc, char *argv[])
{
	
	printf("%s: main() starts executing..\n", __FUNCTION__);
	printf("%s: Hell0 Critical\n", __FUNCTION__);
	abhi();

return 0;
}
