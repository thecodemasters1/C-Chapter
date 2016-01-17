#if !defined(_DICTOINARY_H_)
#define _DICTOINARY_H_

#include <stdio.h>
#include "list.h"

#define DICTIONARY_MAX_LINE (256)
#define DICTIONARY_SEPERATOR (" \n")

list_node_t * create_dictionary(FILE *fp_dictionary);

char * find_synonym(char *word, list_node_t *dictionary);

#endif