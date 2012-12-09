/*
 * semantic.c
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 *
 * Implements all functions that participate in semantic analysis.
 */

#include "semantic.h"

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
				SLOG(("Could not find class %s in symtab\n", temp_cl->ci->id));
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
	//usrdef_print();
	
	// Initialize semantic analysis state
	struct semantic_state_t *sem_state = (struct semantic_state_t*)malloc(sizeof(struct semantic_state_t));
	CHECK_MEM_ERROR(sem_state);

    // Now that the class list is fixed, check deeper into the program
    temp_cl = p->cl;
    while (temp_cl != NULL) {
    	//printf("Class: %s\n", temp_cl->ci->id);
    	
    	struct scope_t *classScope = symtab_lookup_class(temp_cl->ci->id);
    	
    	// Process the variable declaration list
    	process_variable_declaration_list(classScope,temp_cl->cb->vdl);

        // Process the func_declaration_list
    	struct func_declaration_list_t *temp_fdl = temp_cl->cb->fdl;
    	while(temp_fdl != NULL) {
    		SLOG(("Looking at function %s\n",temp_fdl->fd->fh->id));
    		// Process the variable declaration list in fdl
    		//if(temp_fdl->fd == NULL) printf("Function declaration null\n");
    		//if(temp_fdl->fd->fb == NULL) printf("Function block null\n");
    		//if(temp_fdl->fd->fb->vdl == NULL) printf("Function vdl null\n");
			
			// Check for semantic errors in all statements within the function block
			struct scope_t *funcScope = symtab_lookup_function(classScope, temp_fdl->fd->fh->id);
    		process_variable_declaration_list(funcScope,temp_fdl->fd->fb->vdl);

    		// Verify each statement in the statement sequence
    		sem_state->scope = funcScope;
    		struct statement_sequence_t *ss = temp_fdl->fd->fb->ss;
    		while(ss != NULL) {
    			verify_statements_in_sequence(sem_state, ss->s);
    			ss = ss->next;
    		}
    		// Next function
    		temp_fdl = temp_fdl->next;
    	}
        // Advance to the next class
        temp_cl = temp_cl->next;
    }
    
    free(sem_state);
}

/*
 * Checks for compatible types.
 */
bool compatible_types(struct type_denoter_t *t1, struct type_denoter_t *t2){
	SLOG(("entered compatible types\n"));
	if(t1 == NULL || t2 == NULL)
		return false;
	SLOG(("not null\n"));
	// Handle primitive types
	if(t1->type == TYPE_DENOTER_T_IDENTIFIER && t2->type == TYPE_DENOTER_T_IDENTIFIER) {
		if(strcmp(t1->data.id,t2->data.id) == 0) {
			return true;
		}
	}
	// Array types
	if(t1->type == TYPE_DENOTER_T_ARRAY_TYPE && t2->type == TYPE_DENOTER_T_ARRAY_TYPE) {
		SLOG(("checking compatible arrays\n"));
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
	if(a1 == NULL || a2 == NULL)
		return false;
	int size1 = a1->r->max - a1->r->min;
	int size2 = a2->r->max - a2->r->min;
	
	SLOG(("sz1: %i sz2: %i\n", size1, size2));
		
	return (size1 == size2) && compatible_types(a1->td, a2->td);
}

/*
 * Check class compatibility.  Classes are compatible if all of their fields, in order,
 * are compatible.
 */

bool compatible_classes(struct class_list_t *c1, struct class_list_t *c2) {
	if(c1 == NULL || c2 == NULL)
		return false;
	// Pointer to first variable declaration in c1 and c2
	struct variable_declaration_list_t *temp_vdl1 = c1->cb->vdl;
	struct variable_declaration_list_t *temp_vdl2 = c2->cb->vdl;

	// Iterate through c1's variable declarations
	while(temp_vdl1 != NULL) {
		// Make sure the lists are the same size
		bool same_size = (identifier_list_size(temp_vdl1->vd->il) == identifier_list_size(temp_vdl2->vd->il));
		// Make sure the types are compatible
		bool types_compat = compatible_types(temp_vdl1->vd->tden, temp_vdl2->vd->tden);
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
	if(lhs == NULL || rhs == NULL)
		return false;
	// Look up the classes in the symbol table
	struct scope_t *left = symtab_lookup_class(lhs->ci->id);
	struct scope_t *right = symtab_lookup_class(rhs->ci->id);
	// If the classes are both in the symbol table and are the same they are compatible
	if(right == left) {
		return true;
	}
	// If the right scope is an ancestor of the left, the assignment is valid
	while(left != NULL) {
		if(right == left)
			return true;
		left = left->parent;
	}
	// If the classes are compatible the assignment is valid
	if(compatible_classes(lhs,rhs))
		return true;
	// Not compatible
	return false;
}

void verify_statements_in_sequence(struct semantic_state_t *sem_state, struct statement_t *s) {
	if(s == NULL)
		return;
		
	sem_state->line_number = s->line_number;
	
	// Check the statement type and handle accordingly
	if(s->type == STATEMENT_T_ASSIGNMENT) {
		// Check the id (LHS) of the assignment
		if(s->data.as == NULL) {
			SLOG(("s->data.as is null\n"));
		}
		SLOG(("About to enter verify_variable_access\n"));
		sem_state->left = true;
		sem_state->allowThis = true;
		char *left_type = verify_variable_access(sem_state, s->data.as->va);
		
		// Check the RHS of the assignment
		sem_state->left = false;
		char *right_type = verify_expression(sem_state, s->data.as->e);

		// Check the object instantiation identifier to ensure NEW _classname_ is a real class
		if(s->data.as->oe != NULL) {
			char *clName = s->data.as->oe->id;
			struct type_denoter_t *clTd = usrdef_lookup_name(clName);
			if(clTd == NULL)
				error_type_not_defined(sem_state->line_number, clName);
		} else {
			// See if the assignment is valid
			// Make sure neither type was unknown.  If they were unknown then the mismatch got caught
			// in simple expression
			// Look up both types
			if(strcmp(left_type,PRIMITIVE_TYPE_NAME_UNKNOWN) != 0 && strcmp(right_type,PRIMITIVE_TYPE_NAME_UNKNOWN) != 0){
				struct type_denoter_t *left_td = usrdef_lookup_name(left_type);
				struct type_denoter_t *right_td = usrdef_lookup_name(right_type);
				//printf("Checking if %s and %s are compatible\n",lhs,rhs);
				if(left_td->type == TYPE_DENOTER_T_CLASS_TYPE && right_td->type == TYPE_DENOTER_T_CLASS_TYPE) {
					 if(!compatible_class_assignment(left_td->data.cl,right_td->data.cl)) {
						 error_class_not_base_class(sem_state->line_number, left_type, right_type);
					 }
				} else {
					SLOG(("Checking compatible types\n"));
					if(!compatible_types(left_td,right_td)) {
						error_type_mismatch(sem_state->line_number, left_type, right_type);
					}
				}
			}

		}
	} else if(s->type == STATEMENT_T_SEQUENCE) {
		struct statement_sequence_t *ss = s->data.ss;
		while(ss != NULL) {
			verify_statements_in_sequence(sem_state, s->data.ss->s);
			ss = ss->next;
		}
	} else if(s->type == STATEMENT_T_IF) {
		verify_statements_in_sequence(sem_state, s->data.is->s1);
		verify_statements_in_sequence(sem_state, s->data.is->s2);
	} else if(s->type == STATEMENT_T_WHILE) {
		verify_statements_in_sequence(sem_state, s->data.ws->s);
	} else if(s->type == STATEMENT_T_PRINT) {
		sem_state->allowThis = true;
		verify_variable_access(sem_state, s->data.ps->va);
	} else {
	}
}

// Returns the identifier of the type that this variable access represents
char *verify_variable_access(struct semantic_state_t *sem_state, struct variable_access_t *va) {
	if(va == NULL)
		return PRIMITIVE_TYPE_NAME_UNKNOWN;

	// Check the variable access type and find the id accordingly
	if(va->type == VARIABLE_ACCESS_T_IDENTIFIER) {
		SLOG(("%i: VA identifier %s\n", sem_state->line_number,va->data.id));
		
		// Check for "this" reference
		if(sem_state->allowThis) {
			if(strcmp(va->data.id,"this") == 0) {
				// Return the type of the parent class for this scope
				SLOG(("\"this\" reference found.  Class: %s\n", sem_state->scope->parent->cl->ci->id));
				return sem_state->scope->parent->cl->ci->id;
			}
		}

		// Check for the values "true" and "false" not case sensitive and return
		// this as a bool if found.  Also update the tacData to reflect a bool value
		char *bool_lower = stringtolower(va->data.id);
		if(strcmp(bool_lower,BOOLEAN_VALUE_TRUE) == 0 || strcmp(bool_lower,BOOLEAN_VALUE_FALSE) == 0) {
			// Update the tacdata
			va->expr->tacData->type = TAC_DATA_TYPE_BOOL;
			if(strcmp(bool_lower,BOOLEAN_VALUE_TRUE) == 0) {
				va->expr->tacData->d.b = true;
			} else {
				va->expr->tacData->d.b = false;
			}
			free(bool_lower);
			return PRIMITIVE_TYPE_NAME_BOOLEAN;
		}
		free(bool_lower);

		// Check for function name to indicate return value being set
		if(sem_state->scope->attrId == SYM_ATTR_FUNC) {
			if(strcmp(va->data.id, sem_state->scope->fd->fh->id) == 0) {
				//If it is the left hand side, return value is allowed
				if(sem_state->left) {
					return sem_state->scope->fd->fh->res;
				} else {
					error_cannot_set_return_value_on_right_hand_side(sem_state->line_number, va->data.id);
					return PRIMITIVE_TYPE_NAME_UNKNOWN;
				}
			}
		}
		SLOG(("This far\n"));
		// Check the function parameters and variables for the identifier
		struct variable_declaration_t *vd = symtab_lookup_variable(sem_state->scope, va->data.id);
		if(vd == NULL) {
			struct formal_parameter_section_t *fps = symtab_lookup_function_param(sem_state->scope, va->data.id);
			if(fps == NULL) {
				// If the name doesn't match the function name, it's undeclared or a function call
				if(strcmp(sem_state->scope->fd->fh->id, va->data.id) != 0) {
					SLOG(("Could not find the field.  Checking if it is a function: %s\n", va->data.id));

					// Check to see if the identifier is actually a function (in this class) with no parameters
					struct scope_t *funcScope = symtab_lookup_function(sem_state->scope->parent, va->data.id);
					if(funcScope == NULL) {
						// not declared
						error_variable_not_declared(sem_state->line_number, va->data.id);
						return PRIMITIVE_TYPE_NAME_UNKNOWN;
					} else {
						// Change the tac node to be a function call
						va->expr->tac->op = OP_FUNC_CALL;
						va->expr->tac->op2 = va->expr->tac->op1;
						va->expr->tac->op1 = NULL;

						return funcScope->fd->fh->res;
					}
				}


			}
			// Return the type name of the variable in the parameter section
			//printf("(In function params)Identifier %s has type %s\n",va->data.id,fps->id);
			return fps->id;
		}
		// Return the type of the found variable declaration
		SLOG(("%i: Identifier %s has type %s\n", sem_state->line_number, va->data.id, vd->tden->name));
		return vd->tden->name;
	} else if(va->type == VARIABLE_ACCESS_T_INDEXED_VARIABLE) {
		SLOG(("Accessing indexed variable\n"));
		// Accessing an indexed variable (array)
	
		// Recursively check the inner variable access for the array var identifier
		sem_state->allowThis = false;
		char *inner_va = verify_variable_access(sem_state, va->data.iv->va);
		SLOG(("inner_va = %s\n",inner_va));
		// Check if the array index is out of bounds
		struct variable_declaration_t *vd = symtab_lookup_variable(sem_state->scope, va->data.iv->va->data.id);
		if(vd != NULL) {
			// Ensure the indexed variable is actually an array
			if(vd->tden->type == TYPE_DENOTER_T_ARRAY_TYPE) {
				// Loop through inner ranges and check each index in a comma separated expression list
				// The expression MUST be evaluated to an integer. Otherwise variables are involved in the index
				struct array_type_t *arr = vd->tden->data.at;
				struct index_expression_list_t *iel = va->data.iv->iel;
				
				while(iel != NULL && arr != NULL) {
					//printf("Inner: [%i,%i]\n", arr->r->min->ui, arr->r->max->ui);
					if(strcmp(iel->e->expr->type, PRIMITIVE_TYPE_NAME_INTEGER) == 0) {
						int idx = (int)iel->e->expr->val;
						struct range_t *range = arr->r;
						if(idx > range->max->ui || idx < range->min->ui)
							error_array_index_out_of_bounds(sem_state->line_number, idx, range->min->ui, range->max->ui);		
					}
						
					// If the inner type is an array (another dimension) move to it
					if(arr->td->type == TYPE_DENOTER_T_ARRAY_TYPE)
						arr = arr->td->data.at;
					else
						arr = NULL;
						
					iel = iel->next;
				}
				
				// Check type of index
				if(strcmp(vd->tden->data.at->inner_type_name, PRIMITIVE_TYPE_NAME_INTEGER) != 0) {
					error_array_index_is_not_integer(sem_state->line_number, va->data.iv->va->data.id);
				}
			} else {
				error_indexed_variable_not_an_array(sem_state->line_number, va->data.iv->va->data.id);
			}
		}
		//SLOG(("Returning array inner type %s",vd->tden->data.at->inner_type_name));
		return inner_va;
		
	} else if(va->type == VARIABLE_ACCESS_T_ATTRIBUTE_DESIGNATOR) {
		// Check if the data.ad->id is a field of the data.ad->va->data.id's class
		// Resolve the variable access part
		// Determine if "this" should be allowed.  "this" should only be allowed on the innermost
		// variable_access.  If the inner va type is identifier, then "this" is allowed.
		SLOG(("Attribute Designator\n"));
		if(va->data.ad->va->type == VARIABLE_ACCESS_T_IDENTIFIER) {
			sem_state->allowThis = true;
		} else {
			sem_state->allowThis = false;
		}
		char *va_class = verify_variable_access(sem_state, va->data.ad->va);
		SLOG(("va_class = %s\n", va_class));
		if(va_class != NULL) {
			// Look up the class
			struct scope_t *va_classScope = symtab_lookup_class(va_class);
			if(va_classScope == NULL) {
				//printf("Class scope for %s was null\n",va_class);
			}
			//printf("Class scope found: %s\n",va_classScope->cl->ci->id);
			// Look up the field in the class
			SLOG(("Looking for field %s in class\n",va->data.ad->id));
			struct variable_declaration_t *var = symtab_lookup_variable(va_classScope, va->data.ad->id);
			if(var == NULL) {
				SLOG(("Could not find the field.  Checking if it is a function: %s\n", va->data.ad->id));

				// Check to see if the identifier is actually a function with no parameters
				struct scope_t *funcScope = symtab_lookup_function(va_classScope, va->data.ad->id);
				if(funcScope == NULL) {
					// not declared
					error_variable_not_declared(sem_state->line_number, va->data.ad->id);
					return PRIMITIVE_TYPE_NAME_UNKNOWN;
				} else {
					// Fix the tac node to be a function call
					va->expr->tac->op = OP_FUNC_CALL;

					return funcScope->fd->fh->res;
				}
			}
			// Variable was found in the class, return the class name..... its type
			SLOG(("Variable found in class, returning class name %s\n", var->tden->name));
			return var->tden->name;
		}
		// Class was not found for va
		return PRIMITIVE_TYPE_NAME_UNKNOWN;
		//printf("VA attribute designator %s\n", va->data.ad->id);
	} else if(va->type == VARIABLE_ACCESS_T_METHOD_DESIGNATOR) {
		//return verify_variable_access(scope, va->data.md->va, line_number, allowThis);
		// Should work similar to attribute designator but instead the return type
		// of the method should be returned.  Check if va->data.md->fd->id
		// is a function of the class from va->data.md->va;
		// Allow this if the va is an identifier (left most)
		SLOG(("Function/Method Designator\n"));
		if(va->data.md == NULL) {
			SLOG(("va->data.md is null\n"));
		}
		if(va->data.md->va == NULL) {
			SLOG(("va->data.md->va is null\n"));
		}
		if(va->data.md->va->type == VARIABLE_ACCESS_T_IDENTIFIER) {
			sem_state->allowThis = true;
		} else {
			sem_state->allowThis = false;
		}
		// Verify the left side (recursive)
		char *va_class = verify_variable_access(sem_state, va->data.md->va);
		if(va_class != NULL) {
			// Look up the class
			struct scope_t *va_classScope = symtab_lookup_class(va_class);
			if(va_classScope == NULL) {
				SLOG(("Class scope for %s was null\n",va_class));
			}
			SLOG(("Class scope found: %s\n",va_classScope->cl->ci->id));
			// Look up the function in the class
			SLOG(("%i: Looking for method %s in class\n", sem_state->line_number, va->data.md->fd->id));
			struct scope_t *func = symtab_lookup_function(va_classScope, va->data.md->fd->id);
			if(func == NULL) {
				SLOG(("%i: Could not find the method\n", sem_state->line_number));
				// not declared
				error_function_not_declared(sem_state->line_number, va->data.md->fd->id);
				return PRIMITIVE_TYPE_NAME_UNKNOWN;
			}
			// function was found in the class, return the return type of the function
			SLOG(("%i: Method found in class, returning return type %s\n", sem_state->line_number, func->fd->fh->res));
			return func->fd->fh->res;
		}

		// Class was not found for va
		return PRIMITIVE_TYPE_NAME_UNKNOWN;

		//printf("VA method designator %s\n", va->data.md->fd->id);
	} else {
		return PRIMITIVE_TYPE_NAME_UNKNOWN;
	}
}

char *verify_simple_expression(struct semantic_state_t *sem_state, struct simple_expression_t *se) {
	SLOG(("verify_simple_expression\n"));
	if(se == NULL)
		return NULL;

	char *se_type = NULL;
	char *term_type = NULL;

	// Loop through all terms
	struct term_t *term = se->t;
	while(term != NULL) {
		term_type = verify_term(sem_state, term);

		// Check if this term_type is compatible with the se_type thus far
		if(se_type == NULL) { // First iteration
			se_type = term_type;
		} else {
			// Look up each type by name
			struct type_denoter_t *se_td = usrdef_lookup_name(se_type);
			struct type_denoter_t *term_td = usrdef_lookup_name(term_type);
			if(compatible_types(se_td,term_td)) {
				// Do nothing
			} else {
				// Type mismatch?
				se_type = PRIMITIVE_TYPE_NAME_UNKNOWN;
			}
		}
		SLOG(("Term type: %s  se_type: %s\n",term_type,se_type));
		term = term->next;
	}
	
	// Check if the next simple expression type matches
	if(se->next != NULL) {
		char *next_type = verify_simple_expression(sem_state, se->next);
		// Look up each type by name
		struct type_denoter_t *se_td = usrdef_lookup_name(se_type);
		struct type_denoter_t *next_td = usrdef_lookup_name(next_type);
		if(compatible_types(se_td,next_td)) {
			return se_type;
		} else {
			// Type mismatch?
			//error_type_mismatch(sem_state->line_number,se_type,term_type);
			se_type = PRIMITIVE_TYPE_NAME_UNKNOWN;
		}
	}
	SLOG(("Simple expression type: %s\n",se_type));
	//if(strcmp(se_type,PRIMITIVE_TYPE_NAME_UNKNOWN) == 0)
		//error_type_mismatch(sem_state->line_number,se_type,term_type);
	return se_type;
}

/*
 * Verify term
 */
char *verify_term(struct semantic_state_t *sem_state, struct term_t *t) {
	SLOG(("verify_term\n"));
	struct factor_t *factor = t->f;
	// Check for too many signs
	if(factor->type == FACTOR_T_SIGNFACTOR){
		if(factor->data.f.next->type == FACTOR_T_SIGNFACTOR)
			error_too_many_signs(sem_state->line_number);
		SLOG(("Factor expr type: %s\n", factor->expr->type));
		return factor->expr->type;
	} else {
		char *pri_type = verify_primary(sem_state, factor->data.p);
		SLOG(("Factory primary type: %s\n",pri_type));
		return pri_type;
	}
}
/*
 * Verify primary
 */
char *verify_primary(struct semantic_state_t *sem_state, struct primary_t *p) {
	SLOG(("verify_primary\n"));
	if(p == NULL)
		return PRIMITIVE_TYPE_NAME_UNKNOWN;
	if(p->type == PRIMARY_T_VARIABLE_ACCESS) {
		SLOG(("VA\n"));
		sem_state->allowThis = true;
		return verify_variable_access(sem_state, p->data.va);
	} else if(p->type == PRIMARY_T_UNSIGNED_CONSTANT) {
		SLOG(("Unsigned Constant type: %s\n",p->data.un->expr->type));
		return p->data.un->expr->type;
		//printf("unsigned constant: %i\n", p->data.un->ui);
	} else if(p->type == PRIMARY_T_FUNCTION_DESIGNATOR) {
		SLOG(("FD\n"));
		// Function calls
		//printf("func desig: %s\n", p->data.fd->id);

		// Lookup the function in the parent of the current scope (the class scope)
		struct scope_t* funcScope = symtab_lookup_function(sem_state->scope->parent, p->data.fd->id);
		if(funcScope == NULL) {
			// Attempting to call a function that doesn't exist
			error_function_not_declared(sem_state->line_number, p->data.fd->id);
			return PRIMITIVE_TYPE_NAME_UNKNOWN;
		}

		int numParams = 0, actualParams = 0;
		struct actual_parameter_list_t *apl = p->data.fd->apl;
		while(apl != NULL) {
			//verify_expression(sem_state, apl->ap->e1);
			//verify_expression(sem_state, apl->ap->e2);
			//verify_expression(sem_state, apl->ap->e3);
			if(apl->ap->e1 != NULL) numParams++;
			if(apl->ap->e2 != NULL) numParams++;
			if(apl->ap->e3 != NULL) numParams++;
			apl = apl->next;
		}

		// Count the number of parameters expected for a successful function call
		if(funcScope->fd->fh->fpsl != NULL) {
			struct formal_parameter_section_list_t *fpsl = funcScope->fd->fh->fpsl;
			while(fpsl != NULL) {
				struct identifier_list_t *il = fpsl->fps->il;
				while(il != NULL) {
					actualParams++;
					il = il->next;
				}
				fpsl = fpsl->next;
			}
		}

		// If the number of parameters in the call doesn't meet the required amount, error
		if(numParams != actualParams)
			error_function_not_declared(sem_state->line_number, funcScope->fd->fh->id);

		// Return the return type of the function
		return funcScope->fd->fh->res;
	} else if(p->type == PRIMARY_T_EXPRESSION) {
		SLOG(("EXPRESSION\n"));
		return verify_expression(sem_state, p->data.e);
	} else if(p->type == PRIMARY_T_PRIMARY) {
		SLOG(("PRIMARY\n"));
		return verify_primary(sem_state, p->data.p.next);
	} else {
		SLOG(("UNKNOWN\n"));
		return PRIMITIVE_TYPE_NAME_UNKNOWN;
	}
}

/*
 * Returns the id of the type that the expression evaluates to
 */
char *verify_expression(struct semantic_state_t *sem_state, struct expression_t *e) {
	if(e == NULL)
		return PRIMITIVE_TYPE_NAME_UNKNOWN;

	// Always evaluate the first simple expression
	char *type1 = verify_simple_expression(sem_state, e->se1);
	if(type1 == NULL)
		return PRIMITIVE_TYPE_NAME_UNKNOWN;

	// Two expressions are present
	if(e->se2 != NULL) {
		char *type2 = verify_simple_expression(sem_state, e->se2);
	
		// Check for type mismatch
		//if(strcmp(e->se1->expr->type, e->se2->expr->type) != 0) {
		//	error_type_mismatch(line_number, e->se1->expr->type, e->se2->expr->type);
		//}
		// Get the type denoters for the two types
		struct type_denoter_t *td1 = usrdef_lookup_name(type1);
		struct type_denoter_t *td2 = usrdef_lookup_name(type2);
		if(!compatible_types(td1,td2)) {
			//printf("%i: Detected incompatible type %s and %s",line_number,td1->name,td2->name);
			error_type_mismatch(sem_state->line_number, td1->name, td2->name);
			return PRIMITIVE_TYPE_NAME_UNKNOWN;
		} else {
			//return td1->name;
			return e->expr->type;
		}
	}

	return type1;
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
		free(id_lower);
		return false;
	}
	//printf("Variable id: %s foudn valid\n",id);
	free(id_lower);
	return true;
}

/*
 * Checks that all types are defined for a variable_declaration_list_t.
 * Also checks that variable names are valid. Prints appropriate error messages
 */
void process_variable_declaration_list(struct scope_t *scope, struct variable_declaration_list_t *vdl) {
	// Process the variable_declaration_list
	struct variable_declaration_list_t *temp_vdl = vdl;

	// Master set of identifiers to check for already declared
	struct set_t *id_set = NULL;

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
			error_type_not_defined(temp_vdl->vd->line_number, temp_vtden->data.at->inner_type_name);
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
			
			// Reset the id_set
			free_set(id_set);
			id_set = NULL;
			
			while(temp_il != NULL) {
				// Check if it is a valid variable name
				if(!check_variable_valid_name(temp_il->id)) {
					error_variable_name_invalid(temp_vdl->vd->line_number, temp_il->id);
				}
				// Checks if the variable was declared in the same line, adds it to master list if not
				if(!set_contains(id_set,temp_il->id)) {
					if(id_set == NULL)
						id_set = new_set(temp_il->id);
					else
						set_add(id_set, temp_il->id);
				} else {
					error_variable_already_declared(temp_vd->line_number,temp_il->id,temp_vd->line_number);
				}

				// Check if the variable name is the same as the function name
				if(scope->attrId == SYM_ATTR_FUNC) {
					if(strcmp(temp_il->id,scope->fd->fh->id) == 0) {
						error_variable_name_reserved_for_function_return_value(temp_vd->line_number,temp_il->id);
					}
				}

				// If the scope is a class see if the variable was declared in a parent
				if(scope->attrId == SYM_ATTR_CLASS) {
					if(scope->parent->attrId == SYM_ATTR_CLASS) {
						struct variable_declaration_t *foundVd = symtab_lookup_variable(scope->parent,temp_il->id);
						if(foundVd != NULL) {
							error_variable_already_declared(temp_vdl->vd->line_number,temp_il->id,foundVd->line_number);
						}
					}
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
		
		free_set(id_set);
		id_set = NULL;
		
		temp_vdl = temp_vdl->next;
		//printf("loop iter\n");
	}
}
