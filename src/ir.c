/*
 * ir.h
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 *
 * Implements functions to manipulate Intermediate Representation
 */

#include "ir.h"

void ir_optimize() {

	// Print the CFG blocks
	printf("\nPrint Blocks:\n");
	cfg_print_blocks();
	printf("\n");
	printf("\nPrint variables and TAC:\n");
	cfg_print_vars_tac();
	printf("\n");

}

