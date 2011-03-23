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
/* I should probably add parsing-event hooks to avoid the need for my
 * libs. That also means you could choose. */
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
    tns_Boolean,    /* ! (true!, false!) */
    tns_Unknown     /* Unknown data type */
} tns_type;

typedef struct {
    char * text;
    short  meaning;
} tns_truth_tuple;

const tns_truth_tuple tns_truth_table[] = {
    {"true",  1}, {"True", 1},
    {"false", 0}, {"False", 0},
    {"yes", 1}, {"no", 0},
    NULL
}

tnetstr *   tns_parse(char *);
tns_type   tns_get_type(tnetstr *);
#endif
