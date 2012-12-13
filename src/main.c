/*
 * main.c
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 *
 * Implements an object oriented pascal compiler
 */

#include "mc.h"
#include "ir.h"
#include "semantic.h"
#include "symtab.h"
#include "rulefuncs.h"
#include "shared.h"

/* Flags if any errors occured */
int error_flag = 0;

/* Holds the command-line arguments */
struct args_t cmdArgs;
char *outputFile = NULL; // The file name used for output

/* This is the data structure we are left with (the parse tree) after
   yyparse() is done . */
extern struct program_t *program;

extern void yyparse();


/* -----------------------------------------------------------------------
 * Prints a usage banner
 * -----------------------------------------------------------------------
 */
void print_usage() {
    printf("%s: Usage: %s [-v] [-o <file>] [-exitsem] [-O] [-ir] [-free] [-fcf] [-fvn] [-fgre] [--help]\n",
           PROGRAMNAME, PROGRAMNAME);
}


/* -----------------------------------------------------------------------
 * parses the command-line arguments to main
 * -----------------------------------------------------------------------
 */
void parse_command_line_arguments(int argc, char **argv, struct args_t *args) {
    int i = 1;

    if (argc < 1) {
        print_usage();
        exit(1);
    }

    memset(args, 0, sizeof(struct args_t));

    while (i < argc) {
        if (strcmp(argv[i], "-v") == 0 ) {
            args->verbose = 1;
        } else if (strcmp(argv[i], "-o") == 0) {
        	// Make sure the following argument is a file name
        	if((i+1) < argc && argv[i+1][0] != '-') {
        		outputFile = argv[i+1];
        		i++;
        	} else {
        		fprintf(stderr, "Invalid argument '%s'\n", argv[i]);
        	}
        } else if (strcmp(argv[i], "--help") == 0 ||
                   strcmp(argv[i], "-h") == 0 ) {
            print_usage();
            exit(3);
        } else if (strcmp(argv[i], "-exitsem") == 0) {
            args->exit_after_sem = 1;
        } else if (strcmp(argv[i], "-ir") == 0) {
            args->printIr = 1;
        } else if (strcmp(argv[i], "-lir") == 0) {
            args->printLir = 1;
        } else if (strcmp(argv[i], "-fcf") == 0) {
            args->optimize_cf = 1;
        } else if (strcmp(argv[i], "-free") == 0) {
            args->optimize_ree = 1;
        } else if (strcmp(argv[i], "-fvn") == 0) {
            args->optimize_vn = 1;
        } else if (strcmp(argv[i], "-fgre") == 0) {
            args->optimize_gre = 1;
        } else if (strcmp(argv[i], "-O") == 0) {
            args->optimize_cf = 1;
            args->optimize_ree = 1;
            args->optimize_vn = 1;
            args->optimize_gre = 1;
        } else {
            fprintf(stderr, "Invalid argument '%s'\n", argv[i]);
            print_usage();
            exit(1);
        }
        i++;
    }

}



/* -----------------------------------------------------------------------
 * main function, where all hell breaks loose
 * -----------------------------------------------------------------------
 */
int main(int argc, char **argv) {
    parse_command_line_arguments(argc, argv, &cmdArgs);

	// Initialize global data structures: managed strs, program, user defined types, symbol table, control flow graph
	program = new_program();
    usrdef_init();
    symtab_init(program);
	cfg_init();

    /* begin parsing */
    yyparse();
    if (error_flag == 1) {
		// Errors during parsing
        printf("Errors detected. Exiting.\n");
        exit(-1);
    }

    // Perform semantic analysis
    semantic_analysis(program);
    if (error_flag == 1) {
		// Errors during semantic analysis
        printf("Errors detected. Exiting.\n");
        exit(-1);
    }

    // If we should only perform semantic analysis, exit
    if (cmdArgs.exit_after_sem == 1) {
        exit(0);
    }
    
    if(cmdArgs.verbose == 1) {
        printf("USER DEFINED DATA TYPES:\n");
        printf("------------------------\n");
        usrdef_print();

        printf("\n\n");
        printf("SYMBOL TABLE:\n");
        printf("-------------\n");
        symtab_print(0);

        printf("\nTACs (before optimization):\n");
        printf("-------\n");
       	cfg_print_tacs();
       	printf("\n");

       	printf("\nPrint Blocks (before optimization):\n");
       	printf("---------------\n");
       	cfg_print_blocks();
       	printf("\n");
    }

    // Perform Intermediate Representation
    ir_init();
    ir_optimize();
    
    // Calculate usrdef sizes
    symtab_calc_sizes();
    // Calc var offsets
    symtab_calc_offsets();

    if (cmdArgs.verbose == 1) {
        printf("\nTACs (after optimization):\n");
        printf("-------\n");
       	cfg_print_tacs();
       	printf("\n");

       	printf("\nPrint Blocks (after optimization):\n");
       	printf("---------------\n");
       	cfg_print_blocks();
       	printf("\n");    

       	printf("\nPrint value numbering:\n");
		printf("%s", ir_vnt_out_buffer);
		printf("\n");

		printf("\nPrint constant expression optimizations:\n");
		printf("%s", ir_opt_const_out_buffer);
		printf("\n");

		printf("\nPrint Blocks (after dead code elim):\n");
		cfg_print_blocks();
		printf("\n");

		printf("\nTAC after extended value numbering\n");
		cfg_print_tacs();
    }
    
    // Generate Code
    mc_init();
    mc_consume_cfg_list();
    mc_add_bootstrap(program->ph->id);
    mc_print_listing();
    if(outputFile != NULL) {
    	mc_write_listing(outputFile);
    }
    mc_destroy();
    
    // Free memory
    ir_destroy();
	cfg_destroy();
	symtab_destroy();
	usrdef_destroy();
    free_program(program);

    return 0;
}
