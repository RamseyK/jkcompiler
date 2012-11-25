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

struct block_list_t *workList; // Used in CFG processing

/* ----------------------------------------------------------------
 * Output Buffers for debugging
 * ----------------------------------------------------------------
 */

char ir_vnt_out_buffer[2048];
char ir_opt_const_out_buffer[2048];
 
/* ----------------------------------------------------------------
 * Intermediate Representation functions
 * ----------------------------------------------------------------
 */
 
void ir_init();
void ir_destroy();

// IR Processing
void ir_process_cfg(struct block_t *entryBlock);
void ir_block_pass(struct block_t *block, int block_level, struct set_t **cfg_vars);
void ir_calc_flow_vars(struct block_t *block);

// Value Numbering
void ir_value_number_tac(struct three_address_t *tac, int block_level);

// Optimization
void ir_optimize();
void ir_opt_dead_code_elim();
// Constant Folding & Propogation
void ir_opt_const_propagation(struct three_address_t *tac);
void ir_opt_const_folding(struct three_address_t *tac);

// Fixups
void ir_resolve_label_aliases();

#endif
