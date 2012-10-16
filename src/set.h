/*
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 */

/* set.h
 *
 * Contains struct and function declarations for set operations
 */

#ifndef SET_H_
#define SET_H_


#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define EMPTY NULL

struct set_t {
	char *value;
	struct set_t *next;
};

// Set functions
struct set_t *new_set();
bool set_contains(struct set_t *set, char *val);
struct set_t *set_union(struct set_t *set1, struct set_t *set2);
struct set_t *set_intersection(struct set_t *set1, struct set_t *set2);
void set_print(struct set_t *set);

#endif /* SET_H_ */
