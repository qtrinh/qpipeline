//
// written by Quang Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "input_data.h"
#include "output_data.h"
#include "acgtn.h"
#include "fastq.h"

void utils_addTextHeader(char header[], char newColumnHeader[]) ;
void utils_getKeyNValue(char tuple[], char key[], char value[]) ;
