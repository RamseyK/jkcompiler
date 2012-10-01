/* usrdef.c
 *
 * Implements the userdefined datatypes hash table.
 *
 */

#include "usrdef.h"
#include "rulefuncs.h"


char * usrdef_new_name() {
    char *n;

    n = (char *) malloc(MAX_NEW_CHAR_SIZE);
    strcpy(n, "__must_create_a_UNIQUE_anonymous_datatype_here_");

    return  n;
}


/* ------------------------------------------------------------
 * Initializes the usrdef
 * Adds into it the primitive data types
 * ------------------------------------------------------------
 */
void usrdef_init() {
	// The first type is integer
	usrdef_insert(PRIMITIVE_TYPE_NAME_INTEGER);
	// Second type is boolean
	usrdef_insert(PRIMITIVE_TYPE_NAME_BOOLEAN);
	// Third type is unknown
	usrdef_insert(PRIMITIVE_TYPE_NAME_POINTER);
}



/* ------------------------------------------------------------
 * Prints the user defined data types table
 * ------------------------------------------------------------
 */
void usrdef_print() {
	struct identifier_list_t *temp_il = usrdef_types;
	while(temp_il != NULL) {
		printf("\t%s\n", temp_il->id);
		temp_il = temp_il->next;
	}
}

void usrdef_insert(char *usrdef_name) {
	// Create head if it does not exist
	if(usrdef_types == NULL) {
		usrdef_types = (struct identifier_list_t *) malloc(sizeof(struct identifier_list_t));
		char *typename = usrdef_new_name();
		typename = usrdef_name;
		usrdef_types->id = typename;
		usrdef_types->next = NULL;
	} else {
		// Go to the end of the list
		struct identifier_list_t *temp_il = usrdef_types;
		GOTO_END_OF_LIST(temp_il);
		// Insert new node at end of list
		temp_il->next = (struct identifier_list_t *) malloc(sizeof(struct identifier_list_t));
		char *typename = usrdef_new_name();
		typename = usrdef_name;
		usrdef_types->id = typename;
		temp_il->next->next = NULL;
	}
}

bool usrdef_lookup(char *usrdef_name) {
	struct identifier_list_t *temp_il = usrdef_types;
	while(temp_il != NULL) {
		if(strcmp(temp_il->id,usrdef_name))
			return true;
		temp_il = temp_il->next;
	}
	// Not found
	return false;
}
