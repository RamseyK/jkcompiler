#include "set.h"

// Allocates memory for a new set_t on the heap
struct set_t *new_set(const char *val) {
	struct set_t *temp_set;
	temp_set = (struct set_t *) malloc(sizeof(struct set_t));
	if(temp_set == NULL) {
		printf("Memory allocation error\n");
		exit(1);
	}

	temp_set->value = NULL;
	temp_set->next = NULL;
	if(val != NULL) {
		temp_set->value = (char*)malloc(strlen(val) + 1);
		strcpy(temp_set->value, val);
	}
	return temp_set;
}

// Traverses a set and frees all set_t structs from memory
void free_set(struct set_t *set) {
	struct set_t *set_it = set;
	struct set_t *current = set;

	while(set_it != NULL) {
		current = set_it;
		set_it = set_it->next;
		if(current->value != NULL) {
			free(current->value);
			current->value = NULL;
		}
		free(current);
	}
}

// Print's the contents of the set
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

// Returns the size / cardinality of the set
int set_size(struct set_t *set) {
	int count = 0;
	struct set_t *set_it = set;

	while(set_it != NULL) {
		count++;
		set_it = set_it->next;
	}

	return count;
}

// Checks whether the two given sets are equal (contain all and only the same elements)
bool set_equals(struct set_t *set1, struct set_t *set2) {
	// Sizes must be equal
	if(set_size(set1) != set_size(set2))
		return false;

	// All elements in set1 must be in set2 (in other words, a strict subset if true)
	return set_subset(set1, set2);
}

// Checks if a value is a member of a set
bool set_contains(struct set_t *set, const char *val) {
	if(val == NULL)
		return false;

	struct set_t *set_it = set;
	while(set_it != NULL) {
		if(strcmp(set_it->value,val) == 0)
			return true;
		set_it = set_it->next;
	}
	return false;
}

// Add an element (val) to a non-empty set if it doesn't already exist in set
// Returns the previously added set_t object
struct set_t *set_add(struct set_t *set, const char *val) {
	if(set == NULL || val == NULL)
		return NULL;

	// Find the end of the list
	struct set_t *set_it = set;
	struct set_t *set_last = set;
	while(set_it != NULL) {
		// Val already exists, return the passed in set_t node
		if(strcmp(set_it->value, val) == 0)
			return NULL;

		set_last = set_it;
		set_it = set_it->next;
	}
	
	// Create a new set_t object. Add to the end of the list and copy the value
	set_last->next = new_set(val);
	return set_last->next;
}

// Remove val from the set if it exists
void set_remove(struct set_t *set, const char *val) {
	if(set == NULL || val == NULL)
		return;

	// Find val in the set and fix up pointers in the set
	struct set_t *set_it = set;
	struct set_t *set_last = set;
	while(set_it != NULL) {
		// Val exists, unlink the current set node
		if(strcmp(set_it->value, val) == 0) {
			set_last->next = set_it->next;
			free_set(set_it);
			return;
		}

		set_last = set_it;
		set_it = set_it->next;
	}
}

// Performs a union of two sets, does not allow duplicate values
struct set_t *set_union(struct set_t *set1, struct set_t *set2) {

	struct set_t *temp_set_head = NULL;
	struct set_t *temp_set_it = NULL;
	struct set_t *set_it = set1;

	// Add each element of set1
	while(set_it != NULL) {
		if(temp_set_head == NULL) {
			temp_set_head = new_set(set_it->value);
			temp_set_it = temp_set_head;
		} else {
			temp_set_it->next = set_add(temp_set_head, set_it->value);
			if(temp_set_it->next != NULL)
				temp_set_it = temp_set_it->next;
		}
		set_it = set_it->next;
	}

	// Add each element of set2
	set_it = set2;
	while(set_it != NULL) {
		if(temp_set_head == NULL) {
			temp_set_head = new_set(set_it->value);
			temp_set_it = temp_set_head;
		} else {
			temp_set_it->next = set_add(temp_set_head, set_it->value);
			if(temp_set_it->next != NULL)
				temp_set_it = temp_set_it->next;
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
		// See if the elements value is also in set2
		if(set_contains(set2, set_it->value)) {
			if(temp_set_head == NULL) {
				temp_set_head = new_set(set_it->value);
				temp_set_it = temp_set_head;
			} else {
				// set_add will resolve any attempts to add duplicate elements
				temp_set_it->next = set_add(temp_set_head, set_it->value);
				if(temp_set_it->next != NULL)
					temp_set_it = temp_set_it->next;
			}
		}
		set_it = set_it->next;
	}
	return temp_set_head;
}

// Relative complement of set1 in set2. Returns a set of elements that exist in set2 but not in set1
struct set_t *set_difference(struct set_t *set1, struct set_t *set2) {
	struct set_t *temp_set_head = NULL;
	struct set_t *temp_set_it = NULL;
	struct set_t *set_it = set2;
	
	// Loop through every element in set2
	while(set_it != NULL) {
	
		// Element must not exist in set1
		if(!set_contains(set1, set_it->value)) {
			if(temp_set_head == NULL) {
				temp_set_head = new_set(set_it->value);
				temp_set_it = temp_set_head;
			} else {
				temp_set_it->next = set_add(temp_set_head, set_it->value);
				if(temp_set_it->next != NULL)
					temp_set_it = temp_set_it->next;
			}
		}
		
		set_it = set_it->next;
	}
	
	return temp_set_head;
}

// Test if every element in set1 is contained within set2
bool set_subset(struct set_t *set1, struct set_t *set2) {
	struct set_t *set_it = set1;
	
	while(set_it != NULL) {
		if(!set_contains(set2, set_it->value))
			return false;
			
		set_it = set_it->next;
	}
	
	return true;
}



