/*
 * tns.h
 *
 * Copyright 2011, Pablo Alejandro Costesich
 *
 * Licensed under Three-Clause BSD
 *
 */

#ifndef _CNETSTR
#define _CNETSTR "0.1-alpha"
/* I should probably add parsing-event hooks to avoid the need for my
 * libs. That also means you could choose. */
#include "ll.h"
#include "ht.h"
#include <stdio.h>
#include <stddef.h>

typedef struct CTNetStr tnetstr;

typedef enum CTNS_Types {
    tns_String,     /* , */
    tns_List,       /* ] */
    tns_HT,         /* } */
    tns_Integer,     /* # */
    tns_None,       /* ~ */
    tns_Boolean,    /* ! (true!, false!, yes!, no!, True!, False!) */
    tns_Unknown     /* Unknown data type */
} tns_type;


tnetstr * tns_fileparse(FILE * file);
tnetstr * tns_parse(char * input);
tnetstr * tns_parser(char * payload, size_t size, tns_type type);
void tns_free(tnetstr * netstr);
tns_type   tns_get_type(tnetstr *);
#endif
