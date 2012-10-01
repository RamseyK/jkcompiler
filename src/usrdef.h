/* usrdef.h
 *
 * Holds function definitions of a hash table of user defined data types
 *
 */

#ifndef USRDEF_H
#define USRDEF_H

#include "shared.h"

char * usrdef_new_name();
void usrdef_init();
void usrdef_print();
void usrdef_insert(char *usrdef_name);
bool usrdef_lookup(char *usrdef_name);

struct identifier_list_t *usrdef_types;

#endif

