/*
 * ir.h
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 *
 * Contains functions to manipulate the Intermediate Representation
 */

#ifndef IR_H_
#define IR_H_

#include <stdio.h>

#include "shared.h"
#include "cfg.h"

/* ----------------------------------------------------------------
 * IR Structures
 * ----------------------------------------------------------------
 */

struct basic_block_list_t *workList; // Used in value numbering

/* ----------------------------------------------------------------
 * State variables
 * ----------------------------------------------------------------
 */
 

 
/* ----------------------------------------------------------------
 * Intermediate Representation functions
 * ----------------------------------------------------------------
 */

// Value Numbering
void ir_value_numbering();
void ir_evn(struct basic_block_t *block, int block_level);

void ir_optimize();

#endif
