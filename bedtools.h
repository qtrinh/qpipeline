//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef BEDTOOLS_H
#define BEDTOOLS_H 

#include "input_data.h"
#include "output_data.h"

#define MODE_PARSE_ALL_FOR_1X_2X 1001


void bedtools_MODE_PARSE_ALL_FOR_1X_2X(struct input_data *id, struct output_data *od, int minReadDepth) ;

#endif 
