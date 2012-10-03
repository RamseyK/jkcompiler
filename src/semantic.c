/*
 * semantic.c
 *
 * Implements all functions that participate in semantic analysis.
 */


#include "shared.h"
#include "semantic.h"
#include "rulefuncs.h"
#include "usrdef.h"
#include "symtab.h"

/* -----------------------------------------------------------------------
 * Carries out semantic analysis on a program
 * -----------------------------------------------------------------------
 */
void semantic_analysis(struct program_t *p) {
	bool foundProgramClass = false; // Determined if the main Program class is present in the class list
    struct class_list_t *temp_cl;
    temp_cl = p->cl;
    if(p == NULL)
    	printf("Program null\n");
    
    // Initial fix up all base & subclass class structures
    while (temp_cl != NULL) {
		// Found the main program class
		if(strcmp(temp_cl->ci->id, p->ph->id) == 0)
			foundProgramClass = true;

    	// If temp_cl->extend != null, check if extend is in p->cl
		// Check if the class has a base class
		if(temp_cl->ci->extend != NULL) {
			struct scope_t *classScope = symtab_lookup_class(temp_cl->ci->id);
			if(classScope == NULL) {
				printf("Could not find class %s in symtab\n", temp_cl->ci->id);
			} else {
				struct scope_t *extendScope = symtab_lookup_class(temp_cl->ci->extend);
				if(extendScope == NULL) {
					error_extending_missing_class(temp_cl->ci->line_number, temp_cl->ci->id, temp_cl->ci->extend);
				} else {
					// Fix the scope pointers
					classScope->parent = extendScope;
				}
			}
		}

		//printf("Class: %s\n", temp_cl->ci->id);
		temp_cl = temp_cl->next;
    }

	// Main program class wasn't found in the initial class fixup, error
	if(!foundProgramClass)
		error_missing_program_class();
    
	//symtab_print(0);

	
	// Fix up type_denoters and set the appropriate type (determine if class or identifier)
	//fix_type_denoters();
	//usrdef_print();
	//symtab_print(0);
    // Now that the class list is fixed, check deeper into the program
    temp_cl = p->cl;
    while (temp_cl != NULL) {
    	//printf("Class: %s\n", temp_cl->ci->id);
    	// Process the variable declaration list
    	process_variable_declaration_list(temp_cl->cb->vdl);
    	// Check if variable was declared in parent classes
		check_variable_declared_in_parent(temp_cl);

        // Process the func_declaration_list
    	struct func_declaration_list_t *temp_fdl = temp_cl->cb->fdl;
    	while(temp_fdl != NULL) {
    		// Process the variable declaration list in fdl
    		//if(temp_fdl->fd == NULL) printf("Function declaration null\n");
    		//if(temp_fdl->fd->fb == NULL) printf("Function block null\n");
    		//if(temp_fdl->fd->fb->vdl == NULL) printf("Function vdl null\n");
			
			// Check for semantic errors in all statements within the function block
			//verify_statements_in_sequence(temp_fdl->fd->fb->ss);
    		process_variable_declaration_list(temp_fdl->fd->fb->vdl);

    		temp_fdl = temp_fdl->next;
    	}
        // Advance to the next class
        temp_cl = temp_cl->next;
    }
}

/*
 * Checks for compatible types.
 */
bool compatible_types(struct type_denoter_t *t1, struct type_denoter_t *t2){
	// Handle primitive types
	if(t1->type == TYPE_DENOTER_T_IDENTIFIER && t2->type == TYPE_DENOTER_T_IDENTIFIER) {
		if(strcmp(t1->data.id,t2->data.id) == 0) {
			return true;
		}
	}
	// Array types
	if(t1->type == TYPE_DENOTER_T_ARRAY_TYPE && t2->type == TYPE_DENOTER_T_ARRAY_TYPE) {
		return compatible_arrays(t1->data.at, t2->data.at);
	}
	// Class types
	if(t1->type == TYPE_DENOTER_T_CLASS_TYPE && t2->type == TYPE_DENOTER_T_CLASS_TYPE) {
		return compatible_classes(t1->data.cl, t2->data.cl);
	}
	return false;
}

/*
 *  Check array compatibility.  Arrays are compatible if they have the same number of elements
 *  and the elements are of compatible types.
 */
bool compatible_arrays(struct array_type_t *a1, struct array_type_t *a2) {
	int size1 = a1->r->max - a1->r->min;
	int size2 = a2->r->max - a2->r->min;
	return (size1 == size2) && compatible_types(a1->td, a2->td);
}

/*
 * Check class compatibility.  Classes are compatible if all of their fields, in order,
 * are compatible.
 */

bool compatible_classes(struct class_list_t *c1, struct class_list_t *c2) {
	// Pointer to first variable declaration in c1 and c2
	struct variable_declaration_list_t *temp_vdl1 = c1->cb->vdl;
	struct variable_declaration_list_t *temp_vdl2 = c2->cb->vdl;

	// Iterate through c1's variable declarations
	while(temp_vdl1 != NULL) {
		// Make sure the lists are the same size
		int same_size = (identifier_list_size(temp_vdl1->vd->il) == identifier_list_size(temp_vdl2->vd->il));
		// Make sure the types are compatible
		int types_compat = compatible_types(temp_vdl1->vd->tden, temp_vdl2->vd->tden);
		if(!same_size || !types_compat)
			return false;
		temp_vdl1 = temp_vdl1->next;
		temp_vdl2 = temp_vdl2->next;
		// If one list is bigger than the other, not compatible
		if((temp_vdl1 == NULL && temp_vdl2 != NULL) || (temp_vdl1 != NULL && temp_vdl2 == NULL))
			return false;
	}
	return true;
}

/*
 * Checks if an assignment of one class to another is compatible.  An assignment is
 * compatible if the rhs is a descendant of the lhs (lhs is an ancestor)
 */
bool compatible_class_assignment(struct class_list_t *lhs, struct class_list_t *rhs) {
	// Look up the classes in the symbol table
	struct scope_t *left = symtab_lookup_class(lhs->ci->id);
	struct scope_t *right = symtab_lookup_class(rhs->ci->id);
	// If the classes are both in the symbol table and are the same they are compatible
	if(right == left) {
		return true;
	}
	// If the right scope is an ancestor of the left, they are compatible
	while(right != NULL) {
		if(right == left)
			return true;
		right = right->parent;
	}
	// Not compatible
	return false;
}

void verify_statements_in_sequence(struct statement_sequence_t *ss) {
	if(ss == NULL)
		return;
	
	// Check the statement type and handle accordingly
	if(ss->s->type == STATEMENT_T_ASSIGNMENT) {
		// Check the id (LHS) of the assignment
		verify_identifiers_in_variable_access(ss->s->data.as->va);
		
		// Check the object instantiation identifier to ensure NEW _classname_ is a real class
		if(ss->s->data.as->oe != NULL) {
			char *clName = ss->s->data.as->oe->id;
			struct type_denoter_t *clTd = usrdef_lookup_name(clName);
			if(clTd == NULL)
				error_type_not_defined(ss->s->line_number, clName);
		}
		
	} else if(ss->s->type == STATEMENT_T_SEQUENCE) {
		verify_statements_in_sequence(ss->s->data.ss);
	} else if(ss->s->type == STATEMENT_T_IF) {
		// Combine both statements into a single sequence and check the entire statement as a whole
		struct statement_sequence_t *ss1 = new_statement_sequence();
		struct statement_sequence_t *ss2 = new_statement_sequence();
		ss1->s = ss->s->data.is->s1;
		ss2->s = ss->s->data.is->s2;
		ss1->next = ss2;
		verify_statements_in_sequence(ss1);
		free(ss1);
		free(ss2);
	} else if(ss->s->type == STATEMENT_T_WHILE) {
	
		// Check the statement within the while statement
		struct statement_sequence_t *ss1 = new_statement_sequence();
		ss1->s = ss->s->data.ws->s;
		verify_statements_in_sequence(ss1);
		free(ss1);
	} else if(ss->s->type == STATEMENT_T_PRINT) {
		verify_identifiers_in_variable_access(ss->s->data.ps->va);
	} else {
	}
	
	verify_statements_in_sequence(ss->next);
}

void verify_identifiers_in_variable_access(struct variable_access_t *va) {
	// Check the variable access type and find the id accordingly
	if(va->type == VARIABLE_ACCESS_T_IDENTIFIER) {
		printf("found VA identifier %s\n", va->data.id);
	} else if(va->type == VARIABLE_ACCESS_T_INDEXED_VARIABLE) {
		verify_identifiers_in_variable_access(va->data.iv->va);
	} else if(va->type == VARIABLE_ACCESS_T_ATTRIBUTE_DESIGNATOR) {
		verify_identifiers_in_variable_access(va->data.ad->va);
		printf("found VA attribute designator %s\n", va->data.ad->id);
	} else if(va->type == VARIABLE_ACCESS_T_METHOD_DESIGNATOR) {
		verify_identifiers_in_variable_access(va->data.md->va);
		printf("found VA method designator %s\n", va->data.md->fd->id);
	} else {
	}
}


/*
 * Checks if any variables in a variable declaration list have been
 * declared in parent classes
 */

void check_variable_declared_in_parent(struct class_list_t *cl) {
	if(cl->ci->extend == NULL) {
		return; // No parent to look in
	}
	if(symtab_lookup_class(cl->ci->extend) == NULL) {
		return; // Base class was not found
	}
	// Get the scope for the class
	struct scope_t *classScope = symtab_lookup_class(cl->ci->id);
	// Get the variable declaration list
	struct variable_declaration_list_t *temp_vdl = cl->cb->vdl;
	while(temp_vdl != NULL) {
		// Check each identifier in the identifier list
		struct identifier_list_t *temp_il = temp_vdl->vd->il;
		while(temp_il != NULL) {
			struct variable_declaration_t *foundVd = symtab_lookup_variable(classScope->parent,temp_il->id);
			if(foundVd != NULL) {
				error_variable_already_declared(temp_vdl->vd->line_number,temp_il->id,foundVd->line_number);
			}
			temp_il = temp_il->next;
		}
		temp_vdl = temp_vdl->next;
	}
}

/*
 * Checks if a variable name is the same as one of the reserved words (integer, boolean, True, False...)
 * ignoring case.
 */
bool check_variable_valid_name(char *id) {
	char *id_lower = stringtolower(id);
	if(strcmp(id_lower,PRIMITIVE_TYPE_NAME_INTEGER) == 0 ||
		strcmp(id_lower,PRIMITIVE_TYPE_NAME_REAL) == 0 ||
		strcmp(id_lower,PRIMITIVE_TYPE_NAME_BOOLEAN) == 0 ||
		strcmp(id_lower,PRIMITIVE_TYPE_NAME_POINTER) == 0 ||
		strcmp(id_lower,PRIMITIVE_TYPE_NAME_UNKNOWN) == 0 ||
		strcmp(id_lower,BOOLEAN_VALUE_TRUE) == 0 ||
		strcmp(id_lower,BOOLEAN_VALUE_FALSE) == 0) {
		//printf("Variable id: %s found invalid\n",id);
		return false;
	}
	//printf("Variable id: %s foudn valid\n",id);
	return true;
}

/*
 * Checks that all types are defined for a variable_declaration_list_t.
 * Also checks that variable names are valid. Prints appropriate error messages
 */
void process_variable_declaration_list(struct variable_declaration_list_t *vdl) {
	// Process the variable_declaration_list
	struct variable_declaration_list_t *temp_vdl = vdl;

	// Master list of identifiers to check for already declared
	struct identifier_list_t *id_list = NULL;

	// for each variable_declaration_list
	while(temp_vdl != NULL) {
		//printf("Variable: %s\n", temp_vdl->vd->il->id);

		// Check nulls for testing
		//if(temp_vdl->vd == NULL) printf("Variable decl null\n");
		//if(temp_vdl->vd->tden == NULL) printf("Variable decl tden null\n");

		// Check if the type is defined
		struct type_denoter_t *temp_vtden = temp_vdl->vd->tden;
		// Look up the name for class
		if(temp_vtden->type == TYPE_DENOTER_T_CLASS_TYPE && usrdef_lookup_td(temp_vtden) == NULL)
			error_type_not_defined(temp_vdl->vd->line_number,temp_vtden->name);
		// Look up the array type's type denoter for arrays
		if(temp_vtden->type == TYPE_DENOTER_T_ARRAY_TYPE && usrdef_lookup_td(temp_vtden->data.at->td) == NULL)
			error_type_not_defined(temp_vdl->vd->line_number, temp_vtden->data.at->td->name);
		// Look up for identifier types
		if(temp_vtden->type == TYPE_DENOTER_T_IDENTIFIER) {
			// Make sure the name is not integer, boolean, pointer
			if((strcmp(temp_vtden->name,PRIMITIVE_TYPE_NAME_INTEGER) != 0) &&
				(strcmp(temp_vtden->name,PRIMITIVE_TYPE_NAME_BOOLEAN) != 0) &&
				(strcmp(temp_vtden->name,PRIMITIVE_TYPE_NAME_POINTER) != 0)) {
				error_type_not_defined(temp_vdl->vd->line_number,temp_vtden->name);
			}
		}

		// Check if variable name is valid
		struct variable_declaration_t *temp_vd = temp_vdl->vd;
		if(temp_vd != NULL) {
			struct identifier_list_t *temp_il = temp_vd->il;
			// Reset the id_list
			id_list = NULL;
			while(temp_il != NULL) {
				// Check if it is a valid variable name
				if(!check_variable_valid_name(temp_il->id)) {
					error_variable_name_invalid(temp_vdl->vd->line_number, temp_il->id);
				}
				// Checks if the variable was declared in the same line, adds it to master list if not
				//if(full_id_list == NULL) printf("full_id_list null\n");
				//if(temp_il->id == NULL) printf("temp_il->id null\n");
				if(find_identifier_list(id_list,temp_il->id) == NULL) {
					add_to_identifier_list(&id_list,temp_il->id);
				} else {
					error_variable_already_declared(temp_vd->line_number,temp_il->id,temp_vd->line_number);
				}

				// Check the vdl up to this element to see if the variable was declared in a previous vdl
				struct variable_declaration_list_t *it_vdl = vdl;
				while(it_vdl != temp_vdl) {
					struct variable_declaration_t *it_vd = it_vdl->vd;
					if(it_vd != NULL) {
						struct identifier_list_t *it_il = it_vd->il;
						while(it_il != NULL) {
							if(find_identifier_list(it_il,temp_il->id) != NULL) {
								error_variable_already_declared(temp_vd->line_number,temp_il->id,it_vd->line_number);
							}
							it_il = it_il->next;
						}
					}
					it_vdl = it_vdl->next;
				}

				temp_il = temp_il->next;
			}
		}
		temp_vdl = temp_vdl->next;
		//printf("loop iter\n");
	}
}

/*
 * Returns the number of elements in an identifier list
 */
int identifier_list_size(struct identifier_list_t *il) {
	int count = 0;
	struct identifier_list_t *temp_il = il;
	while(temp_il != NULL) {
		count++;
		temp_il = temp_il->next;
	}
	return count;
}
