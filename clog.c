#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv) {
	FILE *infile = stdin;

	char *color = NULL;
	char *white="\033[38;5;231m";
	char *lightblue="\033[38;5;75m";
	char *lightgreen="\033[38;5;40m";
	char *yellow="\033[38;5;166m";
	char *lightred="\033[38;5;196m";
	char *red="\033[31m";
	char *none="\033[0m";

	char buf[4096] = { '\0' };
	if (argc == 2) {
		if((strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "--help") == 0) || (strcmp(argv[1], "--usage") == 0)) {
			printf("Usage: %s logcat.log\n", argv[0]);
			exit(0);
		} else {
			infile = fopen(argv[1], "r");
			if (!infile) {
				fprintf(stderr, "Unable to open file %s: %s\n", argv[1], strerror(errno));
				exit(EXIT_FAILURE);
			}
		}
	}


	while (fgets((char*)&buf, (int)sizeof(buf), infile)) {
		char loglevel = '\0';
		if ((strlen(buf) > 2) && (buf[1] == '/'))
			loglevel = buf[0];
		else if ((strlen(buf) > 32) && (buf[30] == ' ') && (buf[32] == ' '))
			loglevel = buf[31];
		else if ((strlen(buf) > 20) && (buf[18] == ' ') && (buf[20] == '/'))
			loglevel = buf[19];
// 		printf ("20=%c 21=%c 22=%c 23=%c\n", buf[20], buf[21], buf[22], buf[23]);
		switch(loglevel) {
			case 'F': color = red; break;
			case 'E': color = lightred; break;
			case 'W': color = yellow; break;
			case 'I': color = lightgreen; break;
			case 'D': color = lightblue; break;
			case 'V': color = white; break;
			default:  color = NULL; break;
		   
		}
		if (color) {
			buf[strlen(buf)-1] = '\0';
			printf("%s%s%s\n", color, buf, none);
		} else {
			printf("%s", buf);
		}
	}
	(void)fclose(infile);
	return 0;
}
