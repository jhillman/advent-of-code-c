#include <stdbool.h>

int password(char *input, bool includeClicks) {
    FILE *inputFile = fopen(input, "r");
    int zeros = 0;

    if (inputFile) {
	    int dial = 50;
	    char direction;
	    int rotation;
	    char end;
	    bool dialAtZero = false;

	    while (fscanf(inputFile, "%c%d%c", &direction, &rotation, &end) >= 2) {
	    	if (includeClicks) {
	    		zeros += rotation / 100;
	    	}

	    	rotation %= 100;

	        if (direction == 'R') {
	            dial += rotation;

	            if (includeClicks && dial > 100) {
	                ++zeros;
	            }

	            dial %= 100;
	        } else {
	            dial -= rotation;

	            if (dial < 0) {
	                dial += 100;

	                if (includeClicks && !dialAtZero) {
	                   ++zeros;
	                }
	            }
	        }

	        dialAtZero = dial == 0;

	        if (dialAtZero) {
	        	++zeros;
	        }
	    }

	    fclose(inputFile);
	}

    return zeros;
}