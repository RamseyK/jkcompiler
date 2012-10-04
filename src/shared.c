/*
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 */

/* shared.c
 *
 * Implements all shared subroutines
 */

#include "shared.h"



/* -----------------------------------------------------------------------
 * Returns a hashkey value for the given lexeme
 * -----------------------------------------------------------------------
 */
int makekey(char* lexeme, int max_hashkeys) {
    int len;
    int i;
    long charsum = 0;

    len = strlen(lexeme);
    for (i = 0; i < len; i++) {
        charsum += lexeme[i];
    }

    return charsum % max_hashkeys;
}



/* -----------------------------------------------------------------------
 * Prints the specified amount of tabs
 * -----------------------------------------------------------------------
 */
void print_tabs(int numOfTabs) {
    int i = 0;
    while (i < numOfTabs) {
        printf("\t");
        i++;
    }
}



/* -----------------------------------------------------------------------
 * Returns a string representation of the int value
 * -----------------------------------------------------------------------
 */
char *inttostring(int value) {
    char *s;

    s = (char *) malloc(MAX_NEW_CHAR_SIZE);
    sprintf(s, "%d", value);

    return s;
}



/* -----------------------------------------------------------------------
 * Returns a string representation of the long value
 * -----------------------------------------------------------------------
 */
char *longtostring(long value) {
    char *s;

    s = (char *) malloc(MAX_NEW_CHAR_SIZE);
    sprintf(s, "%ld", value);

    return s;
}

/*
 * Converts a string to all lower case characters
 */
char *stringtolower(char *str) {
	int i = 0;
	char *lower = (char *)malloc((strlen(str)+1) * sizeof(char));
	while(str[i] != '\0') {
		//printf("Converting %c to lower: ",str[i]);
		lower[i] = (char)tolower(str[i]);
		//printf("%c\n",lower[i]);
		i++;
	} 
	lower[i] = '\0';
	return lower;
}
