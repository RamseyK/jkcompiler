/*
 * rulefuncs.c
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 *
 * Implements all functions that assist in developing the yacc rules
 */

#include "rulefuncs.h"

/* -----------------------------------------------------------------------
 * Returns a pointer to a new identifier_list
 * -----------------------------------------------------------------------
 */
struct identifier_list_t *new_identifier_list() {
    struct identifier_list_t *il;

    il = (struct identifier_list_t *) malloc(sizeof(struct identifier_list_t));
    CHECK_MEM_ERROR(il)
    il->id = NULL;
    il->next = NULL;

    return il;
}

/* -----------------------------------------------------------------------
 * Adds an identifier to the end of the identifier_list
 * -----------------------------------------------------------------------
 */
void add_to_identifier_list(struct identifier_list_t **il, char *id) {
    if (*il == NULL) {
        *il = new_identifier_list();
        (*il)->id = id;
    } else {
        struct identifier_list_t *temp;

        temp = *il;

        GOTO_END_OF_LIST(temp)
        temp->next = new_identifier_list();
        temp->next->id = id;
    }
}

/* -----------------------------------------------------------------------
 * Returns the number of identifiers in an identifier list
 * -----------------------------------------------------------------------
 */
int identifier_list_size(struct identifier_list_t *il) {
    struct identifier_list_t *temp_il;
    int count = 0;

    temp_il = il;
    while (temp_il != NULL) {
        count++;
        temp_il = temp_il->next;
    }

    return count;
}

/* -----------------------------------------------------------------------
 * Returns a reference to the identifier_list node whose id matches the id parameter
 * -----------------------------------------------------------------------
 */
struct identifier_list_t *find_identifier_list(struct identifier_list_t *il, char *id) {
    struct identifier_list_t *temp_il = il;
    //printf("Looking for identifier: %s\n", id);
    while(temp_il != NULL) {
    	//printf("Checking against: %s\n", temp_il->id);
        if(strcmp(temp_il->id, id) == 0)
            return temp_il;
        temp_il = temp_il->next;
    }
    return NULL;
}

/* -----------------------------------------------------------------------
 * Frees all nodes in an identifier list and its components from memory
 * -----------------------------------------------------------------------
 */
void free_identifier_list(struct identifier_list_t *il) {
	struct identifier_list_t *it = il, *current;
	while(it != NULL) {
		current = it;
		free_identifier(current->id);
		it = it->next;
		free(current);
	}
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new identifier
 * -----------------------------------------------------------------------
 */
char *new_identifier(const char *text) {
	if(text == NULL)
		return NULL;
	
    char *id = (char *) malloc(strlen(text) + 1); /* +1 for '\0' */
    CHECK_MEM_ERROR(id)
    strcpy(id, text);
    
    return id;
}

/* -----------------------------------------------------------------------
 * Frees an identifier from memory
 * -----------------------------------------------------------------------
 */
void free_identifier(char *id) {
	if(id == NULL)
		return;
	free(id);
}



/* -----------------------------------------------------------------------
 * Adds a class_identification and block to the end of the class_list
 * -----------------------------------------------------------------------
 */
struct class_list_t *add_to_class_list(struct class_list_t **cl, struct class_identification_t *ci, struct class_block_t *cb) {
    if (*cl == NULL) {
        *cl = new_class_list();
        (*cl)->ci = ci;
        (*cl)->cb = cb;
        return *cl;
    } else {
        struct class_list_t *temp;

        temp = *cl;

        GOTO_END_OF_LIST(temp)
        temp->next = new_class_list();
        temp->next->ci = ci;
        temp->next->cb = cb;
        return temp->next;
    }
}

/* -----------------------------------------------------------------------
 * Returns a reference to the class_list_t node whose class_identifier id matches the id parameter
 * -----------------------------------------------------------------------
 */
struct class_list_t *find_class_list(struct class_list_t *cl, char *id) {
	struct class_list_t *temp_cl = cl;

	while(temp_cl != NULL) {
		if(strcmp(temp_cl->ci->id, id) == 0)
			return temp_cl;
		temp_cl = temp_cl->next;
	}

	return NULL;
}

/* -----------------------------------------------------------------------
 * Returns a pointer to a new class list node
 * -----------------------------------------------------------------------
 */
struct class_list_t *new_class_list() {
    struct class_list_t *cl;

    cl = (struct class_list_t *) malloc(sizeof(struct class_list_t));
    CHECK_MEM_ERROR(cl)
    cl->ci = NULL;
    cl->cb = NULL;
    cl->next = NULL;

    return cl;
}

/* -----------------------------------------------------------------------
 * Frees all nodes in a class list and their components
 * -----------------------------------------------------------------------
 */
void free_class_list(struct class_list_t *cl) {
	struct class_list_t *it = cl, *current;
	while(it != NULL) {
		current = it;
		free_class_identification(current->ci);
		free_class_block(current->cb);
		it = it->next;
		free(current);
	}
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new class_identification
 * -----------------------------------------------------------------------
 */
struct class_identification_t *new_class_identification() {
    struct class_identification_t *ci;

    ci = (struct class_identification_t*) malloc(sizeof(struct class_identification_t));
    CHECK_MEM_ERROR(ci)
    ci->id = NULL;
    ci->extend = NULL;

    return ci;
}

/* -----------------------------------------------------------------------
 * Frees class_identification and components
 * -----------------------------------------------------------------------
 */
void free_class_identification(struct class_identification_t *ci) {
	free_identifier(ci->id);
	free_identifier(ci->extend);
	free(ci);
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new actual_parameter_list
 * -----------------------------------------------------------------------
 */
struct actual_parameter_list_t *new_actual_parameter_list() {
    struct actual_parameter_list_t *apl;

    apl = (struct actual_parameter_list_t *)
          malloc(sizeof(struct actual_parameter_list_t));
    CHECK_MEM_ERROR(apl)
    apl->ap = NULL;
    apl->next = NULL;

    return apl;
}

/* -----------------------------------------------------------------------
 * Adds an actual_parameter to the end of the actual_parameter_list
 * -----------------------------------------------------------------------
 */
void add_to_actual_parameter_list(struct actual_parameter_list_t **apl,
                                  struct actual_parameter_t *ap) {
    if (*apl == NULL) {
        *apl = new_actual_parameter_list();
        (*apl)->ap = ap;
    } else {
        struct actual_parameter_list_t *temp;

        temp = *apl;

        GOTO_END_OF_LIST(temp)
        temp->next = new_actual_parameter_list();
        temp->next->ap = ap;
    }
}

/* -----------------------------------------------------------------------
 * Frees an actual_parameter_list and its components from memory
 * -----------------------------------------------------------------------
 */
void free_actual_parameter_list(struct actual_parameter_list_t *apl) {
	struct actual_parameter_list_t *it = apl, *current;
	while(it != NULL) {
		current = it;
		free_actual_parameter(current->ap);
		it = it->next;
		free(current);
	}
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new type_denoter
 * -----------------------------------------------------------------------
 */
struct type_denoter_t *new_type_denoter() {
    struct type_denoter_t *td;

    td = (struct type_denoter_t *)
         malloc(sizeof(struct type_denoter_t));
    CHECK_MEM_ERROR(td)
    td->type = -1;
    td->name = NULL;
    td->size = 0;

    return td;
}

/* -----------------------------------------------------------------------
 * Frees type denoter and variables local to the struct (not in data union)
 * -----------------------------------------------------------------------
 */
void free_type_denoter(struct type_denoter_t *td) {
	//free_identifier(td->name);
	free(td);
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new variable_declaration_list
 * -----------------------------------------------------------------------
 */
struct variable_declaration_list_t *new_variable_declaration_list() {
    struct variable_declaration_list_t *vdl;

    vdl = (struct variable_declaration_list_t *)
          malloc(sizeof(struct variable_declaration_list_t));
    CHECK_MEM_ERROR(vdl)
    vdl->vd = NULL;
    vdl->next = NULL;

    return vdl;
}

/* -----------------------------------------------------------------------
 * Frees all nodes and components in the variable_declaration_list
 * -----------------------------------------------------------------------
 */
void free_variable_declaration_list(struct variable_declaration_list_t *vdl) {
	struct variable_declaration_list_t *it = vdl, *current;
	while(it != NULL) {
		current = it;
		free_variable_declaration(current->vd);
		it = it->next;
		free(current);
	}
}

/* -----------------------------------------------------------------------
 * Adds a variable_declaration to the end of the variable_declaration_list
 * -----------------------------------------------------------------------
 */
void add_to_variable_declaration_list(struct variable_declaration_list_t **vdl,
                                      struct variable_declaration_t *vd) {
    if (*vdl == NULL) {
        *vdl = new_variable_declaration_list();
        (*vdl)->vd = vd;
    } else {
        struct variable_declaration_list_t *temp;

        temp = *vdl;

        GOTO_END_OF_LIST(temp)

        temp->next = new_variable_declaration_list();
        temp->next->vd = vd;
    }
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new range_t
 * -----------------------------------------------------------------------
 */
struct range_t *new_range() {
    struct range_t *r;

    r = (struct range_t *) malloc(sizeof(struct range_t));
    CHECK_MEM_ERROR(r)
    r->min = NULL;
    r->max = NULL;

    return r;
}

/* -----------------------------------------------------------------------
 * Frees range and components
 * -----------------------------------------------------------------------
 */
void free_range(struct range_t *r) {
	free_unsigned_number(r->min);
	free_unsigned_number(r->max);
	free(r);
}


/* -----------------------------------------------------------------------
 * Returns a pointer to a new function_designator_t
 * -----------------------------------------------------------------------
 */
struct function_designator_t *new_function_designator() {
    struct function_designator_t *fd;

    fd = (struct function_designator_t *) malloc(sizeof(struct function_designator_t));
    CHECK_MEM_ERROR(fd)
    fd->id = NULL;
    fd->apl = NULL;

    return fd;
}

/* -----------------------------------------------------------------------
 * Frees function_designator_t
 * -----------------------------------------------------------------------
 */
void free_function_designator(struct function_designator_t *fd) {
	free_identifier(fd->id);
	free_actual_parameter_list(fd->apl);
	free(fd);
}


/* -----------------------------------------------------------------------
 * Returns a pointer to a new formal_parameter_section_list
 * -----------------------------------------------------------------------
 */
struct formal_parameter_section_list_t *new_formal_parameter_section_list() {
    struct formal_parameter_section_list_t *fpsl;

    fpsl = (struct formal_parameter_section_list_t *)
           malloc(sizeof(struct formal_parameter_section_list_t));
    CHECK_MEM_ERROR(fpsl)
    fpsl->fps = NULL;
    fpsl->next = NULL;

    return fpsl;
}

/* -----------------------------------------------------------------------
 * Frees all nodes in a formal_parameter_section_list
 * -----------------------------------------------------------------------
 */
void free_formal_parameter_section_list(struct formal_parameter_section_list_t *fpsl) {
	struct formal_parameter_section_list_t *it = fpsl, *current;
	while(it != NULL) {
		current = it;
		free_formal_parameter_section(current->fps);
		it = it->next;
		free(current);
	}
}

/* -----------------------------------------------------------------------
 * Adds a formal_parameter_section to the end of the formal_parameter_section_list
 * -----------------------------------------------------------------------
 */
void add_to_formal_parameter_section_list(struct formal_parameter_section_list_t **fpsl,
        struct formal_parameter_section_t *fps) {
    if (*fpsl == NULL) {
        *fpsl = new_formal_parameter_section_list();
        (*fpsl)->fps = fps;
    } else {
        struct formal_parameter_section_list_t *temp;

        temp = *fpsl;

        GOTO_END_OF_LIST(temp)

        temp->next = new_formal_parameter_section_list();
        temp->next->fps = fps;
    }
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new variable_access
 * -----------------------------------------------------------------------
 */
struct variable_access_t *new_variable_access() {
    struct variable_access_t *va;

    va = (struct variable_access_t *) malloc(sizeof(struct variable_access_t));
    CHECK_MEM_ERROR(va)
    va->type = -1;
    va->recordname = NULL;
    va->expr = NULL;

    return va;
}

/* -----------------------------------------------------------------------
 * Frees variable_access and component based on type
 * -----------------------------------------------------------------------
 */
void free_variable_access(struct variable_access_t *va) {
	switch(va->type) {
		case VARIABLE_ACCESS_T_IDENTIFIER:
			free_identifier(va->data.id);
		break;
		
		case VARIABLE_ACCESS_T_INDEXED_VARIABLE:
			free_indexed_variable(va->data.iv);
		break;
		
		case VARIABLE_ACCESS_T_ATTRIBUTE_DESIGNATOR:
			free_attribute_designator(va->data.ad);
		break;
		
		case VARIABLE_ACCESS_T_METHOD_DESIGNATOR:
			free_method_designator(va->data.md);
		break;
		
		default:
		break;
	}
	
	free_identifier(va->recordname);
	free_expression_data(va->expr);
	free(va);
}


/* -----------------------------------------------------------------------
 * Returns a pointer to a new object_instantiation
 * -----------------------------------------------------------------------
 */
struct object_instantiation_t *new_object_instantiation() {
    struct object_instantiation_t *os;

    os = (struct object_instantiation_t *) malloc(sizeof(struct object_instantiation_t));
    CHECK_MEM_ERROR(os)
    os->id = NULL;
    os->apl = NULL;

    return os;
}

/* -----------------------------------------------------------------------
 * Frees object_instantiation
 * -----------------------------------------------------------------------
 */
void free_object_instantiation(struct object_instantiation_t *oe) {
	if(oe == NULL)
		return;
	
	free_identifier(oe->id);
	free_actual_parameter_list(oe->apl);
	free(oe);
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new assignment_statement
 * -----------------------------------------------------------------------
 */
struct assignment_statement_t *new_assignment_statement() {
    struct assignment_statement_t *as;

    as = (struct assignment_statement_t *) malloc(sizeof(struct assignment_statement_t));
    CHECK_MEM_ERROR(as)
    as->va = NULL;
    as->e = NULL;
    as->oe = NULL;
    as->block = NULL;

    return as;
}

/* -----------------------------------------------------------------------
 * Frees assignment_statement
 * -----------------------------------------------------------------------
 */
void free_assignment_statement(struct assignment_statement_t *as) {
	free_variable_access(as->va);
	free_expression(as->e);
	free_object_instantiation(as->oe);
	free(as);
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new print_statement
 * -----------------------------------------------------------------------
 */
struct print_statement_t *new_print_statement() {
    struct print_statement_t *ps;

    ps = (struct print_statement_t *) malloc(sizeof(struct print_statement_t));
    CHECK_MEM_ERROR(ps)
    ps->va = NULL;

    return ps;
}

/* -----------------------------------------------------------------------
 * Free print_statement
 * -----------------------------------------------------------------------
 */
void free_print_statement(struct print_statement_t *ps) {
	free_variable_access(ps->va);
	free(ps);
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new expression
 * -----------------------------------------------------------------------
 */
struct expression_t *new_expression() {
    struct expression_t *e;

    e = (struct expression_t *) malloc(sizeof(struct expression_t));
    CHECK_MEM_ERROR(e)
    e->se1 = NULL;
    e->relop = -1;
    e->se2 = NULL;
    e->expr = NULL;

    return e;
}

/* -----------------------------------------------------------------------
 * Free expression
 * -----------------------------------------------------------------------
 */
void free_expression(struct expression_t* e) {
	if(e == NULL)
		return;

	free_simple_expression(e->se1);
	if(e->se2 != NULL) {
		free_simple_expression(e->se2);
		free_expression_data(e->expr); // expr is only instantiated if theres a second term and propagated all the way from factor
	}
	free(e);
}


/* -----------------------------------------------------------------------
 * Returns a pointer to a new statement
 * -----------------------------------------------------------------------
 */
struct statement_t *new_statement() {
    struct statement_t *s;

    s = (struct statement_t*) malloc(sizeof(struct statement_t));
    CHECK_MEM_ERROR(s)
    s->type = -1;
    s->line_number = -1;
    s->block = NULL;

    return s;
}

/* -----------------------------------------------------------------------
 * Frees statement and correct component depending on type
 * -----------------------------------------------------------------------
 */
void free_statement(struct statement_t *s) {
	if(s == NULL)
		return;

	switch(s->type) {
		case STATEMENT_T_ASSIGNMENT:
			free_assignment_statement(s->data.as);
		break;
		
		case STATEMENT_T_SEQUENCE:
			free_statement_sequence(s->data.ss);
		break;
		
		case STATEMENT_T_IF:
			free_if_statement(s->data.is);
		break;
		
		case STATEMENT_T_WHILE:
			free_while_statement(s->data.ws);
		break;
		
		case STATEMENT_T_PRINT:
			free_print_statement(s->data.ps);
		break;
		
		default:
		break;
	}
	
	free(s);
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new statement_sequence
 * -----------------------------------------------------------------------
 */
struct statement_sequence_t *new_statement_sequence() {
    struct statement_sequence_t *ss;

    ss = (struct statement_sequence_t *)
         malloc(sizeof(struct statement_sequence_t));
    CHECK_MEM_ERROR(ss)
    ss->s = NULL;
    ss->next = NULL;
    ss->block = NULL;

    return ss;
}

/* -----------------------------------------------------------------------
 * Adds a statement to the end of the statement_sequence
 * -----------------------------------------------------------------------
 */
void add_to_statement_sequence(struct statement_sequence_t **ss, struct statement_t *s) {
    if (*ss == NULL) {
        *ss = new_statement_sequence();
        (*ss)->s = s;
    } else {
        struct statement_sequence_t *temp;

        temp = *ss;

        GOTO_END_OF_LIST(temp)

        temp->next = new_statement_sequence();
        temp->next->s = s;
    }
}

/* -----------------------------------------------------------------------
 * Frees all nodes in a statement_sequence
 * -----------------------------------------------------------------------
 */
void free_statement_sequence(struct statement_sequence_t *ss) {
	if(ss == NULL)
		return;

	struct statement_sequence_t *it = ss, *current;
	while(it != NULL) {
		current = it;
		free_statement(current->s);
		it = it->next;
		free(current);
	}
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new function_block_t
 * -----------------------------------------------------------------------
 */
struct function_block_t *new_function_block() {
    struct function_block_t *fb;

    fb = (struct function_block_t *) malloc(sizeof(struct function_block_t));
    CHECK_MEM_ERROR(fb)
    fb->vdl = NULL;
    fb->ss = NULL;

    return fb;
}

/* -----------------------------------------------------------------------
 * Frees function_block_t
 * -----------------------------------------------------------------------
 */
void free_function_block(struct function_block_t *fb) {
	free_variable_declaration_list(fb->vdl);
	free_statement_sequence(fb->ss);
	free(fb);
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new if_statement
 * -----------------------------------------------------------------------
 */
struct if_statement_t *new_if_statement() {
    struct if_statement_t *is;

    is = (struct if_statement_t *) malloc(sizeof(struct if_statement_t));
    CHECK_MEM_ERROR(is)
    is->e = NULL;
    is->s1 = NULL;
    is->s2 = NULL;
    is->block = NULL;

    return is;
}

/* -----------------------------------------------------------------------
 * Frees an if_statement
 * -----------------------------------------------------------------------
 */
void free_if_statement(struct if_statement_t *is) {
	free_expression(is->e);
	free_statement(is->s1);
	free_statement(is->s2);
	free(is);
}


/* -----------------------------------------------------------------------
 * Returns a pointer to a new while_statement
 * -----------------------------------------------------------------------
 */
struct while_statement_t *new_while_statement() {
    struct while_statement_t *ws;

    ws = (struct while_statement_t *) malloc(sizeof(struct while_statement_t));
    CHECK_MEM_ERROR(ws)
    ws->e = NULL;
    ws->s = NULL;
    ws->block = NULL;

    return ws;
}

/* -----------------------------------------------------------------------
 * Free while_statement
 * -----------------------------------------------------------------------
 */
void free_while_statement(struct while_statement_t *ws) {
	free_expression(ws->e);
	free_statement(ws->s);
	free(ws);
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new indexed_variable
 * -----------------------------------------------------------------------
 */
struct indexed_variable_t *new_indexed_variable() {
    struct indexed_variable_t *iv;

    iv = (struct indexed_variable_t *) malloc(sizeof(struct indexed_variable_t));
    CHECK_MEM_ERROR(iv)
    iv->va = NULL;
    iv->iel = NULL;
    iv->expr = NULL;

    return iv;
}

/* -----------------------------------------------------------------------
 * Free indexed_variable
 * -----------------------------------------------------------------------
 */
void free_indexed_variable(struct indexed_variable_t *iv) {
	free_variable_access(iv->va);
	free_index_expression_list(iv->iel);
	free_expression_data(iv->expr);
	free(iv);
}


/* -----------------------------------------------------------------------
 * Returns a pointer to a new attribute_designator
 * -----------------------------------------------------------------------
 */
struct attribute_designator_t *new_attribute_designator() {
    struct attribute_designator_t *fd;

    fd = (struct attribute_designator_t *) malloc(sizeof(struct attribute_designator_t));
    CHECK_MEM_ERROR(fd)
    fd->va = NULL;
    fd->id = NULL;

    return fd;
}

/* -----------------------------------------------------------------------
 * Frees attribute_designator
 * -----------------------------------------------------------------------
 */
void free_attribute_designator(struct attribute_designator_t *ad) {
	free_variable_access(ad->va);
	free_identifier(ad->id);
	free(ad);
}


/* -----------------------------------------------------------------------
 * Returns a pointer to a new method_designator
 * -----------------------------------------------------------------------
 */
struct method_designator_t *new_method_designator() {
    struct method_designator_t *md;

    md = (struct method_designator_t *) malloc(sizeof(struct method_designator_t));
    CHECK_MEM_ERROR(md)
    md->va = NULL;
    md->fd = NULL;

    return md;
}

/* -----------------------------------------------------------------------
 * Free method_designator
 * -----------------------------------------------------------------------
 */
void free_method_designator(struct method_designator_t *md) {
	free_variable_access(md->va);
	free_function_designator(md->fd);
	free(md);
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new index_expression_list
 * -----------------------------------------------------------------------
 */
struct index_expression_list_t *new_index_expression_list() {
    struct index_expression_list_t *iel;

    iel = (struct index_expression_list_t *)
          malloc(sizeof(struct index_expression_list_t));
    CHECK_MEM_ERROR(iel)
    iel->e = NULL;
    iel->next = NULL;
    iel->expr = NULL;

    return iel;
}

/* -----------------------------------------------------------------------
 * Frees all nodes in an index_expression_list
 * -----------------------------------------------------------------------
 */
void free_index_expression_list(struct index_expression_list_t *iel) {
	struct index_expression_list_t *it = iel, *current;
	while(it != NULL) {
		current = it;
		free_expression(current->e);
		it = it->next;
		free(current);
	}
}


/* -----------------------------------------------------------------------
 * Adds a index_expression to the end of the index_expression_list
 * -----------------------------------------------------------------------
 */
void add_to_index_expression_list(struct index_expression_list_t **iel,
                                  struct expression_t *e) {
    if (*iel == NULL) {
        *iel = new_index_expression_list();
        (*iel)->e = e;
    } else {
        struct index_expression_list_t *temp;

        temp = *iel;

        GOTO_END_OF_LIST(temp)

        temp->next = new_index_expression_list();
        temp->next->e = e;
    }
}


/* -----------------------------------------------------------------------
 * Returns the number of index_expression_list items that are nested
 * (pointing to each other)
 * -----------------------------------------------------------------------
 */
long index_expression_list_count(struct index_expression_list_t *iel) {
    struct index_expression_list_t *temp_iel;
    long count = 0;

    temp_iel = iel;
    while (temp_iel != NULL) {
        count++;
        temp_iel = temp_iel->next;
    }

    return count;

}

/* -----------------------------------------------------------------------
 * Returns a pointer to a new actual_parameter
 * -----------------------------------------------------------------------
 */
struct actual_parameter_t *new_actual_parameter() {
    struct actual_parameter_t *ap;

    ap = (struct actual_parameter_t *) malloc(sizeof(struct actual_parameter_t));
    CHECK_MEM_ERROR(ap)
    ap->e1 = NULL;
    ap->e2 = NULL;
    ap->e3 = NULL;

    return ap;
}

/* -----------------------------------------------------------------------
 * Frees an actual_parameter
 * -----------------------------------------------------------------------
 */
void free_actual_parameter(struct actual_parameter_t *ap) {
	free_expression(ap->e1);
	free_expression(ap->e2);
	free_expression(ap->e3);
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new simple_expression
 * -----------------------------------------------------------------------
 */
struct simple_expression_t *new_simple_expression() {
    struct simple_expression_t *se;

    se = (struct simple_expression_t *)
         malloc(sizeof(struct simple_expression_t));
    CHECK_MEM_ERROR(se)
    se->t = NULL;
    se->addop = -1;
    se->next = NULL;
    se->expr = NULL;

    return se;
}

/* -----------------------------------------------------------------------
 * Free all simple_expression nodes in the list
 * -----------------------------------------------------------------------
 */
void free_simple_expression(struct simple_expression_t *se) {
	struct simple_expression_t *it = se, *current;
	while(it != NULL) {
		current = it;
		free_term(current->t);
		it = it->next;
		free(current);
	}
}



/* -----------------------------------------------------------------------
 * Adds a term to the end of the simple_expression
 * -----------------------------------------------------------------------
 */
void add_to_simple_expression(struct simple_expression_t **se,
                              int addop,
                              struct term_t *t) {
    if (*se == NULL) {
        *se = new_simple_expression();
        (*se)->t = t;
    } else {
        struct simple_expression_t *temp;

        temp = *se;

        GOTO_END_OF_LIST(temp)

        temp->next = new_simple_expression();
        temp->next->t = t;
        temp->addop = addop;
    }
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new term
 * -----------------------------------------------------------------------
 */
struct term_t *new_term() {
    struct term_t *t;

    t = (struct term_t *)
        malloc(sizeof(struct term_t));
    CHECK_MEM_ERROR(t)
    t->f = NULL;
    t->mulop = -1;
    t->next = NULL;
    t->expr = NULL;

    return t;
}

/* -----------------------------------------------------------------------
 * Free all nodes in term
 * -----------------------------------------------------------------------
 */
void free_term(struct term_t *t) {
	struct term_t *it = t, *current;
	while(it != NULL) {
		current = it;
		free_factor(current->f);
		it = it->next;
		free(current);
	}
}

/* -----------------------------------------------------------------------
 * Adds a factor to the end of the term
 * -----------------------------------------------------------------------
 */
void add_to_term(struct term_t **t, int mulop, struct factor_t *f) {
    if (*t == NULL) {
        *t = new_term();
        (*t)->f = f;
    } else {
        struct term_t *temp;

        temp = *t;

        GOTO_END_OF_LIST(temp)

        temp->next = new_term();
        temp->next->f = f;
        temp->mulop = mulop;
    }
}



/* -----------------------------------------------------------------------
 * Returns a pointer to the new factor
 * -----------------------------------------------------------------------
 */
struct factor_t *new_factor() {
    struct factor_t *f;

    f = (struct factor_t *)
        malloc(sizeof(struct factor_t));
    CHECK_MEM_ERROR(f)
    f->type = -1;
    f->expr = NULL;

    return f;
}

/* -----------------------------------------------------------------------
 * Free factor
 * -----------------------------------------------------------------------
 */
void free_factor(struct factor_t *f) {
	if(f == NULL)
		return;

	if(f->type == FACTOR_T_SIGNFACTOR) {
		free_sign(f->data.f.sign);
		free_factor(f->data.f.next);
	} else { // FACTOR_T_PRIMARY
		free_primary(f->data.p);
	}
	
	free(f);
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new sign
 * -----------------------------------------------------------------------
 */
int *new_sign() {
    int *s;

    s = (int *) malloc(sizeof(int));
    CHECK_MEM_ERROR(s)

    return s;
}

/* -----------------------------------------------------------------------
 * Free sign
 * -----------------------------------------------------------------------
 */
void free_sign(int *s) {
	free(s);
}



/* -----------------------------------------------------------------------
 * Returns a pointer to the new primary
 * -----------------------------------------------------------------------
 */
struct primary_t *new_primary() {
    struct primary_t *p;

    p = (struct primary_t *) malloc(sizeof(struct primary_t));
    CHECK_MEM_ERROR(p)
    p->type = -1;
    p->expr = NULL;

    return p;
}

/* -----------------------------------------------------------------------
 * Free primary
 * -----------------------------------------------------------------------
 */
 #define PRIMARY_T_VARIABLE_ACCESS 1
#define PRIMARY_T_UNSIGNED_CONSTANT 2
#define PRIMARY_T_FUNCTION_DESIGNATOR 3
#define PRIMARY_T_EXPRESSION 4
#define PRIMARY_T_PRIMARY 5
void free_primary(struct primary_t *p) {
	if(p == NULL)
		return;

	switch(p->type) {
		case PRIMARY_T_VARIABLE_ACCESS:
			free_variable_access(p->data.va);
		break;
		
		case PRIMARY_T_UNSIGNED_CONSTANT:
			free_unsigned_number(p->data.un);
		break;
		
		case PRIMARY_T_FUNCTION_DESIGNATOR:
			free_function_designator(p->data.fd);
			free_expression_data(p->expr);
		break;
		
		case PRIMARY_T_EXPRESSION:
			free_expression(p->data.e);
		break;
		
		case PRIMARY_T_PRIMARY:
			free_primary(p->data.p.next);
		break;
		
		default:
		break;
	}

	free(p);
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new unsigned_number
 * -----------------------------------------------------------------------
 */
struct unsigned_number_t *new_unsigned_number() {
    struct unsigned_number_t *un;

    un = (struct unsigned_number_t *) malloc(sizeof(struct unsigned_number_t));
    CHECK_MEM_ERROR(un)
    un->expr = NULL;

    return un;
}

/* -----------------------------------------------------------------------
 * Free unsigned_number
 * -----------------------------------------------------------------------
 */
void free_unsigned_number(struct unsigned_number_t *un) {
	free_expression_data(un->expr);
	free(un);
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new array_type
 * -----------------------------------------------------------------------
 */
struct array_type_t *new_array_type() {
    struct array_type_t *at;

    at = (struct array_type_t *) malloc(sizeof(struct array_type_t));
    CHECK_MEM_ERROR(at)

    return at;
}

/* -----------------------------------------------------------------------
 * Frees an array_type and components
 * -----------------------------------------------------------------------
 */
void free_array_type(struct array_type_t *at) {
	free_range(at->r);
	free_type_denoter(at->td);
	free_identifier(at->inner_type_name);
	free(at);
}


/* -----------------------------------------------------------------------
 * Returns a pointer to a new class_block
 * -----------------------------------------------------------------------
 */
struct class_block_t *new_class_block() {
    struct class_block_t *cb;

    cb = (struct class_block_t *) malloc(sizeof(struct class_block_t));
    CHECK_MEM_ERROR(cb)
    cb->vdl = NULL;
    cb->fdl = NULL;

    return cb;
}

/* -----------------------------------------------------------------------
 * Frees class_block and components
 * -----------------------------------------------------------------------
 */
void free_class_block(struct class_block_t *cb) {
	free_variable_declaration_list(cb->vdl);
	free_func_declaration_list(cb->fdl);
	free(cb);
}

/* -----------------------------------------------------------------------
 * Returns a pointer to a new func_declaration_list
 * -----------------------------------------------------------------------
 */
struct func_declaration_list_t *new_func_declaration_list() {
    struct func_declaration_list_t *fdl;

    fdl = (struct func_declaration_list_t *)
          malloc(sizeof(struct func_declaration_list_t));
    CHECK_MEM_ERROR(fdl)
    fdl->fd = NULL;
    fdl->next = NULL;

    return fdl;
}

/* -----------------------------------------------------------------------
 * Frees all nodes in a func_declaration_list and their components
 * -----------------------------------------------------------------------
 */
void free_func_declaration_list(struct func_declaration_list_t *fdl) {
	struct func_declaration_list_t *it = fdl, *current;
	while(it != NULL) {
		current = it;
		free_function_declaration(current->fd);
		it = it->next;
		free(current);
	}
}

/* -----------------------------------------------------------------------
 * Adds a function_declaration to the end of the func_declaration_list
 * -----------------------------------------------------------------------
 */
void add_to_func_declaration_list(struct func_declaration_list_t **fdl,
                                  struct function_declaration_t *fd) {
    if (*fdl == NULL) {
        *fdl = new_func_declaration_list();
        (*fdl)->fd = fd;
    } else {
        struct func_declaration_list_t *temp;

        temp = *fdl;

        GOTO_END_OF_LIST(temp)

        temp->next = new_func_declaration_list();
        temp->next->fd = fd;
    }
}


/* -----------------------------------------------------------------------
 * Find's function_declaration_list node based on the id of the function_heading
 * -----------------------------------------------------------------------
 */
struct func_declaration_list_t *find_func_list(struct func_declaration_list_t *fdl, char *headingId) {
	struct func_declaration_list_t *temp_fdl = fdl;

	while(temp_fdl != NULL) {
		if(strcmp(temp_fdl->fd->fh->id, headingId) == 0)
			return temp_fdl;
		temp_fdl = temp_fdl->next;
	}

	return NULL;
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new function_declaration
 * -----------------------------------------------------------------------
 */
struct function_declaration_t *new_function_declaration() {
    struct function_declaration_t *fd;

    fd = (struct function_declaration_t *)
         malloc(sizeof(struct function_declaration_t));
    CHECK_MEM_ERROR(fd)
    fd->fh = NULL;
    fd->fb = NULL;
    fd->line_number = -1;

    return fd;
}

/* -----------------------------------------------------------------------
 * Frees a function_declaration and its components
 * -----------------------------------------------------------------------
 */
void free_function_declaration(struct function_declaration_t *fd) {
	free_function_heading(fd->fh);
	free_function_block(fd->fb);
	free(fd);
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new function_heading
 * -----------------------------------------------------------------------
 */
struct function_heading_t *new_function_heading() {
    struct function_heading_t *fh;

    fh = (struct function_heading_t *)
         malloc(sizeof(struct function_heading_t));
    CHECK_MEM_ERROR(fh)
    fh->id = NULL;
    fh->res = NULL;
    fh->fpsl = NULL;

    return fh;
}

/* -----------------------------------------------------------------------
 * Frees function_heading and its components
 * -----------------------------------------------------------------------
 */
void free_function_heading(struct function_heading_t *fh) {
	free_identifier(fh->id);
	free_identifier(fh->res);
	free_formal_parameter_section_list(fh->fpsl);
	free(fh);
}


/* -----------------------------------------------------------------------
 * Returns a pointer to a new variable_declaration
 * -----------------------------------------------------------------------
 */
struct variable_declaration_t *new_variable_declaration() {
    struct variable_declaration_t *vd;

    vd = (struct variable_declaration_t *)
         malloc(sizeof(struct variable_declaration_t));
    CHECK_MEM_ERROR(vd)
    vd->il = NULL;
    vd->tden = NULL;
    vd->line_number = -1;

    return vd;
}

/* -----------------------------------------------------------------------
 * Frees variable_declaration and its components
 * -----------------------------------------------------------------------
 */
void free_variable_declaration(struct variable_declaration_t *vd) {
	free_identifier_list(vd->il);
	free(vd);
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new formal_parameter_section
 * -----------------------------------------------------------------------
 */
struct formal_parameter_section_t *new_formal_parameter_section() {
    struct formal_parameter_section_t *fps;

    fps = (struct formal_parameter_section_t *)
          malloc(sizeof(struct formal_parameter_section_t));
    CHECK_MEM_ERROR(fps)
    fps->il = NULL;
    fps->id = NULL;
    fps->is_var = -1;

    return fps;
}

/* -----------------------------------------------------------------------
 * Frees a formal_parameter_section and its components
 * -----------------------------------------------------------------------
 */
void free_formal_parameter_section(struct formal_parameter_section_t *fps) {
	free_identifier_list(fps->il);
	free_identifier(fps->id);
	free(fps);
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new program
 * -----------------------------------------------------------------------
 */
struct program_t *new_program() {
    struct program_t *p;

    p = (struct program_t *) malloc(sizeof(struct program_t));
    CHECK_MEM_ERROR(p)
    p->ph = NULL;
    p->cl = NULL;

    return p;
}

/* -----------------------------------------------------------------------
 * Frees program and everything under it
 * -----------------------------------------------------------------------
 */
void free_program(struct program_t *p) {
	free_program_heading(p->ph);
	free_class_list(p->cl);
	free(p);
}


/* -----------------------------------------------------------------------
 * Returns a pointer to a new program_heading
 * -----------------------------------------------------------------------
 */
struct program_heading_t *new_program_heading() {
    struct program_heading_t *ph;

    ph = (struct program_heading_t *) malloc(sizeof(struct program_heading_t));
    CHECK_MEM_ERROR(ph)
    ph->id = NULL;
    ph->il = NULL;

    return ph;
}

/* -----------------------------------------------------------------------
 * Frees program_heading and its components
 * -----------------------------------------------------------------------
 */
void free_program_heading(struct program_heading_t *ph) {
	free_identifier(ph->id);
	free_identifier_list(ph->il);
	free(ph);
}



/* -----------------------------------------------------------------------
 * Returns a pointer to a new expression_data
 * -----------------------------------------------------------------------
 */
struct expression_data_t *new_expression_data() {
    struct expression_data_t *ed;

    ed = (struct expression_data_t *) malloc(sizeof(struct expression_data_t));
    CHECK_MEM_ERROR(ed)
    ed->val = -1;
    ed->type = NULL;

    ed->tacData = (struct tac_data_t *) malloc(sizeof(struct tac_data_t));
    ed->tacData->type = 0;
    ed->tacData->d.id = NULL;

    return ed;
}

/* -----------------------------------------------------------------------
 * Frees expression_data
 * -----------------------------------------------------------------------
 */
void free_expression_data(struct expression_data_t *ed) {
	if(ed == NULL)
		return;
	
	free(ed);
}

