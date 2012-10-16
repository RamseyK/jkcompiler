/*
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 */

/* set.h
 *
 * Contains function definitions for set operations
 */

#include "set.h"

// Allocates memory for a new set_t on the heap
struct set_t *new_set() {
	struct set_t *temp_set;
	temp_set = (struct set_t *) malloc(sizeof(struct set_t));
	if(temp_set == NULL) {
		printf("Memory allocation error\n");
		exit(1);
	}

	temp_set->value = NULL;
	temp_set->next = NULL;
	return temp_set;
}

// Checks if a value is a member of a set
bool set_contains(struct set_t *set, char *val) {
	struct set_t *set_it = set;
	while(set_it != NULL) {
		if(strcmp(set_it->value,val) == 0)
			return true;
		set_it = set_it->next;
	}
	return false;
}

// Performs a union of two sets, does not allow duplicate values
struct set_t *set_union(struct set_t *set1, struct set_t *set2) {

	struct set_t *temp_set_head = NULL;
	struct set_t *temp_set_it = NULL;
	struct set_t *set_it = set1;

	// Add each element of set1
	while(set_it != NULL) {
		if(!set_contains(temp_set_head,set_it->value)) {
			//printf("Adding element %s\n",set_it->value);
			if(temp_set_head == NULL) {
				temp_set_head = new_set();
				temp_set_head->value = set_it->value;
				temp_set_it = temp_set_head;
			} else {
				temp_set_it->next = new_set();
				temp_set_it->next->value = set_it->value;
				temp_set_it = temp_set_it->next;
			}
		}
		set_it = set_it->next;
	}

	// Add each element of set2
	set_it = set2;
	while(set_it != NULL) {
		if(!set_contains(temp_set_head,set_it->value)) {
			//printf("Adding element %s\n",set_it->value);
			if(temp_set_head == NULL) {
				temp_set_head = new_set();
				temp_set_head->value = set_it->value;
				temp_set_it = temp_set_head;
			} else {
				temp_set_it->next = new_set();
				temp_set_it->next->value = set_it->value;
				temp_set_it = temp_set_it->next;
			}
		}
		set_it = set_it->next;
	}

	return temp_set_head;
}

// Performs an intersection of two sets
struct set_t *set_intersection(struct set_t *set1, struct set_t *set2) {

	if(set1 == EMPTY || set2 == EMPTY)
		return EMPTY;

	struct set_t *temp_set_head = NULL;
	struct set_t *temp_set_it = NULL;
	struct set_t *set_it = set1;

	// Go through every element of set 1
	set_it = set1;
	while(set_it != NULL) {
		// See if the elements value is in set2, but not already in the new set
		if(set_contains(set2, set_it->value) && !set_contains(temp_set_head, set_it->value)) {
			// First element being added to result
			if(temp_set_head == NULL) {
				temp_set_head = new_set();
				temp_set_head->value = set_it->value;
				temp_set_it = temp_set_head;
			} else { // Add to set
				temp_set_it->next = new_set();
				temp_set_it->next->value = set_it->value;
				temp_set_it = temp_set_it->next;
			}
		}
		set_it = set_it->next;
	}
	return temp_set_head;
}

void set_print(struct set_t *set) {
	if(set == EMPTY) {
		printf("(empty)\n");
		return;
	}
	struct set_t *set_it = set;
	while(set_it != NULL) {
		printf("%s ",set_it->value);
		set_it = set_it->next;
	}
	printf("\n");
}
