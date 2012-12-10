/*
 * shared.c
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 *
 * Implements all shared subroutines
 */

#include "shared.h"

/* -----------------------------------------------------------------------
 * Returns a string representation of the long value
 * -----------------------------------------------------------------------
 */
char *op_str(int i_op) {
	// Figure out which OP should be printed
	char *temp = NULL;
	char op [8];
	int cw = 0; // characters written
	switch(i_op) {
	case OP_PLUS:
		cw = sprintf(op, "+");
		break;
	case OP_MINUS:
		cw = sprintf(op, "-");
		break;
	case OP_OR:
		cw = sprintf(op, "|");
		break;
	case OP_STAR:
		cw = sprintf(op, "*");
		break;
	case OP_SLASH:
		cw = sprintf(op, "/");
		break;
	case OP_MOD:
		cw = sprintf(op, "%%");
		break;
	case OP_AND:
		cw = sprintf(op, "&");
		break;
	case OP_EQUAL:
		cw = sprintf(op, "=");
		break;
	case OP_NOTEQUAL:
		cw = sprintf(op, "!=");
		break;
	case OP_LT:
		cw = sprintf(op, "<");
		break;
	case OP_GT:
		cw = sprintf(op, ">");
		break;
	case OP_LE:
		cw = sprintf(op, "<=");
		break;
	case OP_GE:
		cw = sprintf(op, ">=");
		break;
	case OP_ASSIGN:
		cw = 0;
		op[0] = '\0';
		break;
	case OP_BRANCH:
		cw = sprintf(op, "goto");
		break;
	case OP_GOTO:
		cw = sprintf(op, "goto");
		break;
	case OP_CALLER_ASSIGN:
		cw = sprintf(op, "C=");
		break;
	case OP_PARAM_ASSIGN:
		cw = sprintf(op, "P=");
		break;
	case OP_FUNC_CALL:
		cw = sprintf(op, "CALL");
		break;
	case OP_FUNC_RETURN:
		cw = sprintf(op, "RETURN");
		break;
	case OP_MEM_ACCESS:
		cw = sprintf(op, "ACCESS");
		break;
	case OP_NEW_OBJ:
		cw = sprintf(op, "NEW");
		break;
	case OP_PRINT:
		cw = sprintf(op, "PRINT");
		break;
	default:
		cw = sprintf(op, "u-h-o-h");
		break;
	}
	
	temp = (char*)malloc(cw + 1);
	strncpy(temp, op, cw+1);
	
	return temp;
}


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
