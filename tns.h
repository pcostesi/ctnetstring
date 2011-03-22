/*
 * tns.h
 *
 * Copyright 2011, Pablo Alejandro Costesich
 *
 * Licensed under Three-Clause BSD
 *
 */

#ifndef _CNETSTR
#define _CNETSTR 0.1
#include "ll.h"
#include "ht.h"
#include <stdio.h>

#define TNS_BUFF_SIZE 16
#define TNS_PARSE_STREAM 1
#define TNS_PARSE_STRING 2
#define TNS_PARSE_FD 3


typedef struct CTNetStr tnetstr;

typedef enum CTNS_Types {
    tns_String,     /* " */
    tns_List,       /* ] */
    tns_HT,         /* } */
    tns_Number,     /* # */
    tns_None,       /* ~ */
    tns_Boolean,    /* ! */
    tns_Unknown     /* Unknown data type */
} tns_type;

tnetstr *   tns_parse(char *);
tns_type   tns_get_type(tnetstr *);
#endif
