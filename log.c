//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "log.h"

void log_msg(log_index_t logIndex, char *fileName, int lineNumber, char msg[], char data[]) {

	printf ("\n\n");
	printf ("[%s:%d]", fileName, lineNumber);
	if (logIndex == LOG_ERROR)
		printf ("- ERROR: ");
	else if (logIndex == LOG_WARNING)
		printf ("- WARNING: ");
	else if (logIndex == LOG_INFO)
		printf ("- INFO: ");
		
	printf ("%s", msg);
	printf ("\n\n%s", data);
	printf ("\n\n");
}
