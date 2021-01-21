#include <stdio.h>

extern int external_variable;
static int static_var;
char global_array[100];


void call_me () {
	fprintf (stderr, "calling %s\n", __FUNCTION__);
}

int main () {

	char main_array[100];
	int  local_variable;

	//fprintf (stderr, "%d\n", external_variable);

	call_me();
}
