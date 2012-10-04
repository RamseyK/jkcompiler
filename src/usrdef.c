/*
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 */

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
	struct type_denoter_t *integer_td = new_type_denoter();
	integer_td->type = TYPE_DENOTER_T_IDENTIFIER;
	integer_td->name = PRIMITIVE_TYPE_NAME_INTEGER;
	integer_td->data.id = PRIMITIVE_TYPE_NAME_INTEGER;
	usrdef_insert(integer_td);
	// The second type is real
	struct type_denoter_t *real_td = new_type_denoter();
	real_td->type = TYPE_DENOTER_T_IDENTIFIER;
	real_td->name = PRIMITIVE_TYPE_NAME_REAL;
	real_td->data.id = PRIMITIVE_TYPE_NAME_REAL;
	usrdef_insert(real_td);
	// Third type is boolean
	struct type_denoter_t *boolean_td = new_type_denoter();
	boolean_td->type = TYPE_DENOTER_T_IDENTIFIER;
	boolean_td->name = PRIMITIVE_TYPE_NAME_BOOLEAN;
	boolean_td->data.id = PRIMITIVE_TYPE_NAME_BOOLEAN;
	usrdef_insert(boolean_td);
	// Fourth type is pointer
	struct type_denoter_t *pointer_td = new_type_denoter();
	pointer_td->type = TYPE_DENOTER_T_IDENTIFIER;
	pointer_td->name = PRIMITIVE_TYPE_NAME_POINTER;
	pointer_td->data.id = PRIMITIVE_TYPE_NAME_BOOLEAN;
	usrdef_insert(pointer_td);
}



/* ------------------------------------------------------------
 * Prints the user defined data types table
 * ------------------------------------------------------------
 */
void usrdef_print() {
	struct type_denoter_list_t *temp_il = usrdef_types;
	while(temp_il != NULL) {
		printf("\t%i:%s\n", temp_il->tden->type, temp_il->tden->name);
		temp_il = temp_il->next;
	}
}

/*
 * Inserts the type denoter into the list
 */
struct type_denoter_t *usrdef_insert(struct type_denoter_t *td) {
	// Create head if it does not exist
	if(usrdef_types == NULL) {
		usrdef_types = (struct type_denoter_list_t *) malloc(sizeof(struct type_denoter_list_t));
		usrdef_types->tden = td;
		usrdef_types->next = NULL;
		return usrdef_types->tden;
	} else {
		// Go to the end of the list
		struct type_denoter_list_t *temp_il = usrdef_types;
		GOTO_END_OF_LIST(temp_il);
		// Insert new node at end of list
		temp_il->next = (struct type_denoter_list_t *) malloc(sizeof(struct type_denoter_list_t));
		temp_il->next->tden = td;
		temp_il->next->next = NULL;
		return temp_il->next->tden;
	}
}

/*
 * Lookup by type denoter
 */
struct type_denoter_t *usrdef_lookup_td(struct type_denoter_t *td) {
	// For array types check the inner type since you can have arrays of arrays
	if(td->type == TYPE_DENOTER_T_ARRAY_TYPE) {
		return usrdef_lookup_td(td->data.at->td);
	}
	// Else check through the type denoter list
	struct type_denoter_list_t *temp_il = usrdef_types;
	while(temp_il != NULL) {
		if(temp_il->tden == td)
			return td;
		temp_il = temp_il->next;
	}
	return NULL;
}

/*
 * Lookup by type name
 */
struct type_denoter_t *usrdef_lookup_name(char *usrdef_name) {
	struct type_denoter_list_t *temp_il = usrdef_types;
	while(temp_il != NULL) {
		if(strcmp(temp_il->tden->name,usrdef_name) == 0)
			return temp_il->tden;
		temp_il = temp_il->next;
	}
	// Not found
	return NULL;
}
