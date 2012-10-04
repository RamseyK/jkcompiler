/*
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 */

/* usrdef.h
 *
 * Holds function definitions of a linked list of user defined data types
 *
 */

#ifndef USRDEF_H
#define USRDEF_H

#include "shared.h"

char * usrdef_new_name();
void usrdef_init();
void usrdef_print();
struct type_denoter_t *usrdef_insert(struct type_denoter_t *td);
struct type_denoter_t *usrdef_lookup_td(struct type_denoter_t *td);
struct type_denoter_t *usrdef_lookup_name(char *usrdef_name);

struct type_denoter_list_t *usrdef_types;

struct type_denoter_list_t {
	struct type_denoter_t *tden;
	struct type_denoter_list_t *next;
};

#endif

