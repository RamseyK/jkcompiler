/* 
 * set.h
 * From jkcompiler
 * Keilan Jackson, Ramsey Kant
 *
 * A list based implementation of a set
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
struct set_t *new_set(const char *val);
void free_set(struct set_t *set);
void set_print(struct set_t *set);

int set_size(struct set_t *set);
bool set_equals(struct set_t *set1, struct set_t *set2);
bool set_contains(struct set_t *set, const char *val);

struct set_t *set_add(struct set_t *set, const char *val);
void set_remove(struct set_t *set, const char *val);

struct set_t *set_union(struct set_t *set1, struct set_t *set2);
struct set_t *set_intersection(struct set_t *set1, struct set_t *set2);
struct set_t *set_difference(struct set_t *set1, struct set_t *set2);
bool set_subset(struct set_t *set1, struct set_t *set2);

#endif /* SET_H_ */
