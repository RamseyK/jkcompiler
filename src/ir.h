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

char ir_vnt_out_buffer[2048];
 
/* ----------------------------------------------------------------
 * Intermediate Representation functions
 * ----------------------------------------------------------------
 */
 
void ir_init();
void ir_destroy();

// Value Numbering
void ir_value_numbering();
void ir_evn(struct basic_block_t *block, int block_level);
void ir_print_vnt();

void ir_optimize();

void ir_resolve_label_aliases();

#endif
