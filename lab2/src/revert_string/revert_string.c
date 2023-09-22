#include "revert_string.h"
#include <string.h>


void RevertString(char *str)
{
	int length = strlen(str);
    int start = 0;
    int end = length - 1;

    while (start < end) {
        // Swap the characters at the start and end positions
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;

        // Move the start and end pointers towards each other
        start++;
        end--;
    }
}

