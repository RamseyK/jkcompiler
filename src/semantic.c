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
    struct class_list_t *temp_cl;

    temp_cl = p->cl;
    while (temp_cl != NULL) {
        // If temp_cl->extend != null, check if extend is in p->cl

        // Process the variable_declaration_list
        /*
        // for each variable
        	//variable;//FINISH
        	// if a variable->vd->tden->type == TYPE_DENOTER_T_CLASS_TYPE && variable->vd->tden->data->cl == NULL
        		struct type_denoter_t *varTypeDenoter = variable->vd->tden;
        		if(varTypeDenoter->type == TYPE_DENOTER_T_CLASS_TYPE && varTypeDenoter->data->cl == NULL){
        		// look for variable->vd->tden->name in symbol table
        			struct scope_t *foundScope = symtab_lookup_class(varTypeDenoter->name);
        			// if found, update variable->vd->tden->data->cl
        			if(foundScope != NULL) {
        				varTypeDenoter->data->cl = (class_list_t *)foundScope->ptr;
        			}
        			// else error_class_not_found?
        			else {
        				error_type_not_defined(variable->vd->line_number, varTypeDenoter->name);
        			}
        		}
		*/
		
        // Process the func_declaration_list

        temp_cl = temp_cl->next;
    }
}
