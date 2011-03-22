/*
 * tns.c
 *
 * Copyright 2011, Pablo Alejandro Costesich
 *
 * Licensed under Three-Clause BSD
 *
 */

#include <stdlib.h>
#include "tns.h"

#define GUARD(got, error) ( if ((got) == (error)) return NULL)

struct CTNetStr{
    tns_type   type;
    void *      payload;
};

static int reader_str(char *, char *, unsigned, void *);
static int get_msg_size(void *, tns_reader *, void *)


/* Public functions */

tnetstr * tns_parse(char * input){
    int trailing;
    int offset;
    char * payload;
    tns_type type = tns_Unknown;

    GUARD(get_msg_size(input, &trailing, &offset), -1);
    payload = input + offset;
    type = get_msg_type(input + offset + trailing);

    switch(type){

        case tns_String:
            break;

        default:
            /* free stuff up and throw an error */
    }
}


tns_type tns_get_type(tnetstr * tns){
    return tns->type;
}

/* Helpers and other private functions */

static int get_msg_size(char * input, int * trailing, int * offset){
    int n;
    int x = 0;
    int acc = 1;

    while ((n = *input++) && n != ':'){
        acc++;
        if (isdigit(n))
            x = 10 * x + n - '0';
        else
            return -1;
    }
    *offset = acc;
    *trailing = x;

    return 0;
}

static tns_type get_msg_type(char t){
    switch(t){
        case '"':
        case ',': /* Degenerate case for std. netstrings */
            return tns_String;
        case ']':
            return tns_List;
        case '#':
            return tns_Number;
        case '}':
            return tns_HT;
        case '~':
            return tns_None;
        case '!':
            return tns_Boolean;
        default:
            return tns_Unknown;
    }
}

#undef GUARD
