/* symtab.c
 *
 * Implements the symbol table
 *
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
	
	// Add it to the master list, udpate tail
	if(tailScope == NULL) {
		tailScope = temp_scope;
	} else {
		tailScope->next = temp_scope;
		tailScope = temp_scope;
	}
	return temp_scope;
}

/*
 * Initializes the symtab by creating the root node
 */
void symtab_init(struct program_t *program) {
	rootScope = new_scope();
	rootScope->attrId = SYM_ATTR_PROGRAM;
	rootScope->program = program;
	currScope = rootScope;
}

/*
 * Creates a function scope using the function_declaration_t
 */
void symtab_create_function_scope(struct function_declaration_t *fd) {
	struct scope_t *temp_scope = new_scope();
	
	// Sets its function properties
	temp_scope->attrId = SYM_ATTR_FUNC;
	temp_scope->fd = fd;
}

/*
 * Creates a symtab scope entry for the class_list_t.
 * Checks for function scopes that have already been added to the symtab
 * and links them to the scope.  Also adds the class to the root(program)
 * scope's class_scopes list.
 */
void symtab_create_class_scope(struct class_list_t *cl) {
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
			struct variable_declaration_list_t *vdl = fd->fb->vdl;
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
			}
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
			struct variable_declaration_list_t *vdl = cl->cb->vdl;
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
			}
			
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
 * Returns the scope of a function inside of a class scope using its name
 */

struct scope_t *symtab_lookup_function(struct scope_t *classScope, char *name) {
	// Go through all of the func scopes for the class
	struct scope_t *it = classScope->func_scopes;
	while(it != NULL) {
		if(it->attrId == SYM_ATTR_FUNC && it->fd != NULL) {
			if(strcmp(it->fd->fh->id, name) == 0)
				return it;
		} else {
			printf("Scope in func_scope for class %s was not properly created", classScope->cl->ci->id);
		}
		it = it->nextSibling;
	}
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
			if(strcmp(il->id,name) == 0)
				return vdl->vd;
			il = il->next;
		}
		vdl = vdl->next;
	}
	
	// Not found, look in parent scope
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


