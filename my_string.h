//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "input_data.h"


char ** my_string_arrayOfChars2arrayOfStrings(struct input_data *id, char inputStr[], char delimeter, int *length) ;
void    my_string_printArray(char **a, int length) ;
void    my_string_printArrayHorizontal(char **a, int length) ;

void my_string_toupper(char *ptr) ;

