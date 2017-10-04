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
		if ((strlen(buf) > 2) && (buf[1] == '/')) // logcat -v brief; logcat -v tag
			loglevel = buf[0];
		else if ((strlen(buf) > 32) && (buf[30] == ' ') && (buf[32] == ' ')) // logcat -v threadtime
			loglevel = buf[31];
		else if ((strlen(buf) > 20) && (buf[18] == ' ') && (buf[20] == '/')) // logcat -v time
			loglevel = buf[19];
		else if ((strlen(buf) > 8) && (buf[1] == '(') && (buf[7] == ')') && (buf[8] == ' ')) // logcat -v process
			loglevel = buf[0];
		else if ((strlen(buf) > 15) && (buf[1] == '(') && (buf[7] == ':') && (buf[13] == ')') && (buf[14] == ' ')) // logcat -v thread
			loglevel = buf[0];
		else if ((strlen(buf) > 4) && (buf[0] == '<') && (buf[2] == '>') && (buf[3] == '[') ) { // dmesg, part 1: loglevels 0..7
			switch(buf[1]) { // Map kernel log levels to android log levels: https://elinux.org/Debugging_by_printing
				case '0': loglevel = 'F'; break;
				case '1': loglevel = 'F'; break;
				case '2': loglevel = 'F'; break;
				case '3': loglevel = 'E'; break;
				case '4': loglevel = 'W'; break;
				case '5': loglevel = 'I'; break;
				case '6': loglevel = 'I'; break;
				case '7': loglevel = 'D'; break;
				default : loglevel = 'V'; break;
			}
		}
		else if ((strlen(buf) > 5) && (buf[0] == '<') && (buf[3] == '>') && (buf[4] == '[') ) // dmesg, part2: loglevels 11..99
			loglevel = 'V';

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
