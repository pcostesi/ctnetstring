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

typedef struct CTNetStr tnetstr;
typedef enum CTNS_Types {
    tns_String,
    tns_List,
    tns_HT,
    tns_Number,
    tns_None,
    tns_Boolean
} tns_type;

const char tns_type_repr[] = {
    "\"",
    "]",
    "}",
    "#",
    "~",
    "!"
};

#define tns_parse_stream(F) tns_parse(NULL, (F))
#define tns_parse_string(S) tns_parse((S), NULL)

tnetstr *   tns_parse(char *, FILE *);
tnetstr *   tns_parse_string(char *);
tnetstr *   tns_parse_stream(FILE *);
tns_type   tns_get_type(tnetstr *);
#endif
