/*
 * symtab.c
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 *
 * Implements the symbol table
 * A flat symbol table is the root of all evil.
 */

#include "symtab.h"

/*
 * Creates a new scope and adds it to the tail of the list.
 */
struct scope_t *new_scope() {
	struct scope_t *temp_scope;
	temp_scope = (struct scope_t *) malloc(sizeof(struct scope_t));
	CHECK_MEM_ERROR(temp_scope);
	
	temp_scope->attrId = -1;
	temp_scope->parent = NULL;
	temp_scope->program = NULL;
	temp_scope->class_scopes = NULL;
	temp_scope->cl = NULL;
	temp_scope->func_scopes = NULL;
	temp_scope->fd = NULL;
	temp_scope->next = NULL;
	temp_scope->nextSibling = NULL;
	
	// Add it to the master list
	if(allScopes == NULL) {
		allScopes = temp_scope;
	} else {
		// Find the end of the list
		struct scope_t *end = allScopes;
		while(end->next != NULL)
			end = end->next;
		
		// Add temp_scope to the end of the master list
		end->next = temp_scope;
	}
	
	return temp_scope;
}

/*
 * Initializes the symtab by creating the root node
 */
void symtab_init(struct program_t *program) {
	allScopes = NULL;
	rootScope = new_scope();
	rootScope->attrId = SYM_ATTR_PROGRAM;
	rootScope->program = program;
}

/*
 * Frees the symbol table (scope nodes) from memory contained in the master list (allScopes)
 */
void symtab_destroy() {
	struct scope_t *it = allScopes;
	struct scope_t *curr = allScopes;
	while(it != NULL) {
		curr = it;
		it = curr->next;
		free(curr);
	}
	allScopes = NULL;
	rootScope = NULL;
}

/*
 * Creates a function scope using the function_declaration_t
 */
struct scope_t *symtab_create_function_scope(struct function_declaration_t *fd) {
	struct scope_t *temp_scope = new_scope();
	
	// Sets its function properties
	temp_scope->attrId = SYM_ATTR_FUNC;
	temp_scope->fd = fd;

	return temp_scope;
}

/*
 * Creates a symtab scope entry for the class_list_t.
 * Checks for function scopes that have already been added to the symtab
 * and links them to the scope.  Also adds the class to the root(program)
 * scope's class_scopes list.
 */
struct scope_t *symtab_create_class_scope(struct class_list_t *cl) {
	struct scope_t *classScope = new_scope();
	
	// Sets its class properties
	classScope->attrId = SYM_ATTR_CLASS;
	classScope->cl = cl;
	classScope->parent = rootScope; // Parent scope is the program by default;
	
	// Find function scopes for each function in the class
	struct func_declaration_list_t *class_fdl = cl->cb->fdl;
	while(class_fdl != NULL) {
		struct scope_t *foundScope = symtab_lookup_function_scope(class_fdl->fd);
		if(foundScope == NULL) {
			// The function wasn't added to the symtab properly
			//printf("Could not find function scope for function %s for class %s\n", class_fdl->fd->fh->id, cl->ci->id);
		} else {
			// Set class scope as a sibling to other functions in the class
			// First function
			if(classScope->func_scopes == NULL) {
				classScope->func_scopes = foundScope;
			} else {
				// Add to end of sibling list
				struct scope_t *temp_scope = classScope->func_scopes;
				while(temp_scope->nextSibling != NULL) {
					temp_scope = temp_scope->nextSibling;
				}
				temp_scope->nextSibling = foundScope;
			}
			// Update the parent pointers
			foundScope->parent = classScope;
		}
		class_fdl = class_fdl->next;
	}
	
	// Update set class scope as a sibling to other classes in the program
	// First class scope
	if(rootScope->class_scopes == NULL) {
		rootScope->class_scopes = classScope;
	} else {
		// Add at end of sibling list
		struct scope_t *temp_scope = rootScope->class_scopes;
		while(temp_scope->nextSibling != NULL) {
			temp_scope = temp_scope->nextSibling;
		}
		temp_scope->nextSibling = classScope;
	}
	
	return classScope;
}

/*
 * Returns the scope created for a function_declaration_t
 */
struct scope_t *symtab_lookup_function_scope(struct function_declaration_t *fd) {
	struct scope_t *temp_scope = rootScope;
	while(temp_scope != NULL) {
		if(temp_scope->attrId == SYM_ATTR_FUNC) {
			if(temp_scope->fd == fd)
				return temp_scope;
		}
		
		temp_scope = temp_scope->next;
	}
	
	// Not found
	return NULL;
}



/* ------------------------------------------------------------
 * Prints the contents of the symbol table
 * ------------------------------------------------------------
 */
void symtab_print(int numOfTabs) {
	symtab_print_recursive(rootScope, 0);
}

/*
 * Recursive print for symtab.  Prints program->classes->functions, indenting
 * with each layer in.  
 */
void symtab_print_recursive(struct scope_t* start, int numTabs) {
	if(start == NULL) {
		return;
	}

	print_tabs(numTabs);
	printf("<Enter Scope>\n");

	struct scope_t* node = start;

	while(node != NULL) {
		// Find node information based on the attribute type
		char *name = NULL;
		if(node->attrId == SYM_ATTR_FUNC) {
			struct function_declaration_t* fd = node->fd;
			name = fd->fh->id;
			if(name == NULL)
				printf("id is null!\n");
			print_tabs(numTabs);
			printf("%p: %s(FUNC)\n", node, name);
			
			// Show method variables
			/*struct variable_declaration_list_t *vdl = fd->fb->vdl;
			while(vdl != NULL) {
				if(vdl->vd != NULL) {
					struct identifier_list_t *il = vdl->vd->il;
					while(il != NULL) {
						print_tabs(numTabs);
						printf("%p: %s(VAR)\n", node, il->id);
						il = il->next;
					}
				}
				vdl = vdl->next;
			}*/

			// Print vars with offsets
			print_tabs(numTabs+1);
			printf("VARS (offset)\n");
			symtab_print_var_offsets(node->offset_list, numTabs+1);
		} else if(node->attrId == SYM_ATTR_CLASS) {
			//printf("attrId = CLASS");
			struct class_list_t* cl = node->cl;
			name = cl->ci->id;
			if(name == NULL)
				printf("id is null!\n");
			print_tabs(numTabs);
			// Show if the class has a base class
			if(cl->ci->extend != NULL)
				printf("%p: %s(CLASS) extends %s\n", node, name, cl->ci->extend);
			else
				printf("%p: %s(CLASS)\n", node, name);
			
			// Show class variables
			/*struct variable_declaration_list_t *vdl = cl->cb->vdl;
			while(vdl != NULL) {
				if(vdl->vd != NULL) {
					struct identifier_list_t *il = vdl->vd->il;
					while(il != NULL) {
						print_tabs(numTabs);
						printf("%p: %s(VAR)\n", node, il->id);
						il = il->next;
					}
				}
				vdl = vdl->next;
			}*/
			
			// Print vars with offsets
			print_tabs(numTabs+1);
			printf("VARS (offset)\n");
			symtab_print_var_offsets(node->offset_list, numTabs+1);

			// Descend into function nodes to print their parse trees
			symtab_print_recursive(node->func_scopes, numTabs+1);
		} else if(node->attrId == SYM_ATTR_PROGRAM) {
			struct program_t* p = node->program;
			name = p->ph->id;
			if(name == NULL)
				printf("id is null!\n");
			print_tabs(numTabs);
			printf("%p: %s(PROGRAM)\n", node, name);

			// Descend into class nodes to print their parse trees
			symtab_print_recursive(node->class_scopes, numTabs+1);
		} else {
			print_tabs(numTabs);
			printf("??\n");
		}
		
		// Move to the adjacent node
		node = node->nextSibling;
	}

	print_tabs(numTabs);	
	printf("<End Scope>\n");
}

/*
 * Prints an offset list by printing the variable name and its offset, and the specified number of tabs
 */
void symtab_print_var_offsets(struct offset_list_t *offsets, int numTabs) {

	struct offset_list_t *it = offsets;
	while(it != NULL) {
		print_tabs(numTabs);

		printf("%s (%i)\n", it->id, it->offset);

		it = it->next;
	}

}

/*
 * Prints the addresses of every node in the symtab in order
 */
void symtab_print_list_addr() {
	struct scope_t *temp = rootScope;
	while(temp != NULL) {
		printf("\t%p", temp);
		temp = temp->next;
	}
	printf("\n");
}

/*
 * Returns the scope of a class based on its name
 */
struct scope_t *symtab_lookup_class(char *name) {
	// Loop through all of the class scopes from the root
	struct scope_t *classScope = rootScope->class_scopes;
	while(classScope != NULL) {
		if(classScope->cl == NULL) {
			printf("class_list_t not set for scope!");
		} else {
			if(strcmp(classScope->cl->ci->id, name) == 0) {
				return classScope;
			}
		}
		classScope = classScope->nextSibling;
	}
	return NULL;
}

/*
 * Returns the scope of a function inside of a class scope using its name.
 * Looks up in parent scopes for classes that extend
 */

struct scope_t *symtab_lookup_function(struct scope_t *classScope, char *name) {
	// Go through all of the func scopes for the class
	struct scope_t *it = classScope->func_scopes;
	while(it != NULL) {
		if(it->attrId == SYM_ATTR_FUNC && it->fd != NULL) {
			if(strcmp(it->fd->fh->id, name) == 0)
				return it;
		} else {
			SLOG(("Scope in func_scope for class %s was not properly created", classScope->cl->ci->id));
		}
		it = it->nextSibling;
	}
	// Look in the parent class if it is not root
	if(classScope->parent != rootScope) {
		return symtab_lookup_function(classScope->parent, name);
	}
	// Not found, return null
	return NULL;
}

/*
 * Looks up a variable using its name starting in the scope.  Looks in the
 * parent scope until rootScope is reached.  Returns the variable declaration
 */
struct variable_declaration_t *symtab_lookup_variable(struct scope_t *scope, char *name) {
	// Reached the root scope and not found
	if(scope == rootScope)
		return NULL;
	
	struct variable_declaration_list_t *vdl;
	// If scope is a class scope
	if(scope->attrId == SYM_ATTR_CLASS) {
		vdl = scope->cl->cb->vdl;
	// The scope is a function scope
	} else if(scope->attrId == SYM_ATTR_FUNC) {
		vdl = scope->fd->fb->vdl;
	// Scope isn't the right type
	} else {
		return NULL;
	}
	// Go through every variable declaration in the list
	while(vdl != NULL) {
		struct identifier_list_t *il = vdl->vd->il;
		// Go through every identifier in the list
		while(il != NULL) {
			//printf("Comparing %s to %s\n",il->id,name);
			if(strcmp(il->id,name) == 0) {
				//printf("Match found! %s to %s",il->id,name);
				return vdl->vd;
			}
			il = il->next;
		}
		vdl = vdl->next;
	}
	
	// Not found, look in parent scope
	//printf("Looking in parent scope\n");
	return symtab_lookup_variable(scope->parent, name);
}

/*
 * Look's up a parameter in the function's parameter list by name and returns the formal_parameter_section node
 * Return's NULL if the variable was not found
 */
struct formal_parameter_section_t *symtab_lookup_function_param(struct scope_t *funcScope, char *name) {
	if(funcScope->attrId != SYM_ATTR_FUNC)
		return NULL;

	if(funcScope->fd->fh->fpsl != NULL) {
		struct formal_parameter_section_list_t *fpsl = funcScope->fd->fh->fpsl;
		while(fpsl != NULL) {
			struct identifier_list_t *il = fpsl->fps->il;
			//char *type = fpsl->fps->id;
			while(il != NULL) {
				if(strcmp(name, il->id) == 0) {
					return fpsl->fps;
				}
				il = il->next;
			}
			fpsl = fpsl->next;
		}
	}
	
	return NULL;
}

/*
 * Calculates the size (in bytes) of each type denoter for a class in the symtab
 */
void symtab_calc_sizes() {

	// Iterate through each usrdef entry
	struct type_denoter_list_t *td_it = usrdef_types;
	while(td_it != NULL) {

		// Only calculate the size if it has not been set
		if(td_it->tden->size == 0) {

			symtab_calc_td_size(td_it->tden);

		}

		td_it = td_it->next;
	}

}

/*
 * Returns the size of the scope in bytes.  Includes the size of any inherited types.
 */
int symtab_calc_scope_size(struct scope_t *scope) {
	int size = 0;

	if(scope->attrId == SYM_ATTR_CLASS) {

		// Increment size by the size of each variable declared
		struct variable_declaration_list_t * vd_it = scope->cl->cb->vdl;
		while(vd_it != NULL) {

			// Get the size for the type being declared
			int vd_size;

			// If the td is a class type then treat the variable as a pointer to an object
			// Which has a size of one word.  Otherwise get the actual size.
			if(vd_it->vd->tden->type == TYPE_DENOTER_T_CLASS_TYPE) {
				vd_size = SIZE_WORD;
			} else {
				vd_size = symtab_calc_td_size(vd_it->vd->tden);
			}

			// Add the size one time each for every identifier in the list
			struct identifier_list_t *id_it = vd_it->vd->il;
			while(id_it != NULL) {
				size += vd_size;
				id_it = id_it->next;
			}

			vd_it = vd_it->next;
		}

		// Increment the size by the size of the parent class, if any
		if(scope->cl->ci->extend != NULL) {

			// Get the scope for the parent
			struct scope_t *parent = symtab_lookup_class(scope->cl->ci->extend);
			size += symtab_calc_scope_size(parent);
		}
	} else if(scope->attrId == SYM_ATTR_FUNC) {
		// TODO: Need to include function parameters
		struct variable_declaration_list_t *vd_it = scope->fd->fb->vdl;
		while(vd_it != NULL) {
			// Get the size for the type being declared
			int vd_size = symtab_calc_td_size(vd_it->vd->tden);

			// Add the size one time each for every identifier in the list
			struct identifier_list_t *id_it = vd_it->vd->il;
			while(id_it != NULL) {
				size += vd_size;
				id_it = id_it->next;
			}

			vd_it = vd_it->next;
		}
	}

	return size;
}

/*
 * Returns the size of a td or calculates and sets if if not set
 */
int symtab_calc_td_size(struct type_denoter_t *td) {
	// Size already set, also covers primitives
	if(td->size != 0) return td->size;

	// Array type
	if(td->type == TYPE_DENOTER_T_ARRAY_TYPE) {
		int element_count = td->data.at->r->max->ui - td->data.at->r->min->ui + 1;
		int element_size = symtab_calc_td_size(td->data.at->td);
		td->size = element_count * element_size;
		return td->size;
	}

	// Class type
	if(td->type == TYPE_DENOTER_T_CLASS_TYPE) {
		// Lookup the scope for the class and then calculate
		// Look up the scope for the class using the id
		struct scope_t *foundScope = symtab_lookup_class(td->data.cl->ci->id);

		if(foundScope != NULL) {
			td->size = symtab_calc_scope_size(foundScope);
		} else {
			IRLOG(("A usrdef entry does not have a scope_t: %s", td->data.cl->ci->id));
		}

		return td->size;
	}

	return 0;
}

/*
 * Calculates variable offsets for variables declared in classes or functions
 */
void symtab_calc_offsets() {

	struct scope_t *scope_it = allScopes;

	// Iterate through the scopes
	while(scope_it != NULL) {

		// Only calculate offsets for scopes that have not been done yet.
		if(scope_it->offset_list == NULL) {
			symtab_calc_scope_offsets(scope_it);
		}

		scope_it = scope_it->next;
	}

}

/*
 * Calculates the offsets for vars in a single scope.  Returns the offset
 * of the last declared var + the sizeof that vars type (the next available offset)
 */
int symtab_calc_scope_offsets(struct scope_t *scope) {

	// If the offsets have already been done, just return the last value
	if(scope->offset_list != NULL) {
		struct offset_list_t *end = scope->offset_list;
		GOTO_END_OF_LIST(end);

		// Figure out the size of this variable
		struct variable_declaration_t *varDecl = symtab_lookup_variable(scope, end->id);
		int size = symtab_calc_td_size(varDecl->tden);

		// Return the sum, which would be the next available offset
		return end->offset + size;
	}

	// For classes, have to check for inheritence and continue offsets where we left off in parent
	if(scope->attrId == SYM_ATTR_CLASS || scope->attrId == SYM_ATTR_FUNC) {
		int off = 0;
		struct variable_declaration_list_t *vdl_it = NULL;
		if(scope->attrId == SYM_ATTR_CLASS) {
			// Determine the start by checking for parent classes
			if(scope->cl->ci->extend != NULL) {
				off = symtab_calc_scope_offsets(scope->parent);
			}

			// Iterate through every variable declared
			vdl_it = scope->cl->cb->vdl;
		} else if(scope->attrId == SYM_ATTR_FUNC) {
			vdl_it = scope->fd->fb->vdl;
		}


		while(vdl_it != NULL) {
			// Size of the data structure for the var
			int size = symtab_calc_td_size(vdl_it->vd->tden);
			printf("Size is %i\n", size);

			// Iterate through every identifier in the list
			struct identifier_list_t *il_it = vdl_it->vd->il;
			while(il_it != NULL) {
				// Add to the offset list
				add_to_offset_list(&scope->offset_list, il_it->id, off);
				off += size; // Update the start by the size

				il_it = il_it->next;
			}

			vdl_it = vdl_it->next;
		}

		return off;

	}
	return 0;
}

/*
 * Adds a new offset list entry with the id and offset to an offsetList
 */
struct offset_list_t *add_to_offset_list(struct offset_list_t **offsetList, char *id, int offset) {

	// List was empty so initialize
	if(*offsetList == NULL) {
		*offsetList = new_offset_list();
		(*offsetList)->id = new_identifier(id);
		(*offsetList)->offset = offset;

		return *offsetList;
	} else {
		struct offset_list_t *end = *offsetList;
		GOTO_END_OF_LIST(end);
		end->next = new_offset_list();
		end->next->id = new_identifier(id);
		end->next->offset = offset;

		return end->next;
	}

}

/*
 * Allocates memory for a new offset list
 */
struct offset_list_t *new_offset_list() {

	struct offset_list_t *temp_offset = (struct offset_list_t *) malloc(sizeof(struct offset_list_t));
	CHECK_MEM_ERROR(temp_offset);
	temp_offset->id = NULL;
	temp_offset->next = NULL;
	temp_offset->offset = 0;

	return temp_offset;

}
