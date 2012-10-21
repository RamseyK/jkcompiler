/*
 * usrdef.h
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 *
 * Holds function definitions of a userdefined datatypes linked list
 */

#ifndef USRDEF_H
#define USRDEF_H

#include "rulefuncs.h"
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

