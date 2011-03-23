/*
 * tns.c
 *
 * Copyright 2011, Pablo Alejandro Costesich
 *
 * Licensed under Three-Clause BSD
 *
 */

#include <stdlib.h>
#include <ctype.h>
#include "tns.h"

#define GUARD(got, error) ( if ((got) == (error)) return NULL)

struct CTNetStr{
    tns_type   type;
    union {
        void *  ptr;
        int     number;
        char *  str;
        short   bool;
    } payload;
};

static int reader_str(char *, char *, unsigned, void *);
static int get_msg_size(void *, tns_reader *, void *)


/* Public functions */

#ifdef _TNS_NET_

tnetstr * tns_netparse(int fd){
    return NULL;
}

#endif

tnetstr * tns_parse(char * input){
    int trailing, offset, total;
    char * payload;
    tns_type type = tns_Unknown;

    total = get_msg_size(input, &trailing, &offset);
    GUARD(total, -1);
    payload = input + offset;
    type = get_msg_type(input[total]);

    return tns_parser(payload, trailing, type);
}


tnetstr * tns_parser(char * payload, int size, tns_type type){
    tnetstr * ret = NULL;

    switch(type){

        case tns_String:
            ret = parse_String(payload, size);
            break;

        case tns_None:
            ret = new_tnetstr(tns_None);
            break;

        case tns_List:
            ret = parse_List(input, len);
            break;

        case tns_Boolean:
            ret = parse_Boolean(input, len);
            break;

        case tns_Number:
            ret = parse_Number(input, len);
            break;

        case tns_HT:
            ret = parse_HT(input, len);
            break;

        default:
            /* free stuff up and throw an error */
    }
}


tns_type tns_get_type(tnetstr * tns){
    return tns->type;
}

/* Helpers and other private functions */

/* returns consumed bytes */
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

    return acc + x;
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

static tnetstr * new_tnetstr(tns_type type){
    tnetstr * ret = malloc(sizeof(tnetstr));
    if (ret)
        ret->type = type;
    return ret;
}

static tnetstr * parse_String(char * input, int len){
    char * c;
    tnetstr * ret;

    ret = new_tnetstr(tns_String);
    GUARD(ret, NULL);
    c = malloc(len + 1);
    GUARD(c, NULL);

    strncpy(c, input, len);
    c[len] = 0;
    ret->payload.str = c;

    return ret;
}

static tnetstr * parse_List(char * input, int len){
    tnetstr * ret, * element;
    char * frag;
    int frag_size, consumed, next;
    tns_type frag_type;
    llist * list;

    consumed = 0;
    while (size - consumed > 0){
        next = next_fragment(input + consumed, &frag, &frag_size, &frag_type);
        GUARD(next, -1);
        consumed += next;
        element = tns_parser(frag, frag_size, frag_type);
        list = ll_insert(list, &element, sizeof(tnetstr *));
    }

    ret = new_tnetstr(tns_List);
    ret->payload.ptr = ll_head(list);

}

static tnetstr * parse_HT(char * input, int len){
    tnetstr * ret, * element;
    char * frag, * key;
    int frag_size, consumed, next;
    tns_type frag_type;
    ht * table;

    consumed = 0;
    table = ht_new(NULL);

    while (size - consumed > 0){
        /* parse the string key */
        next = next_fragment(input + consumed, &frag, &frag_size, &frag_type);
        GUARD(next, -1);
        consumed += next;

        /* do the same, but for the matching value */
        next = next_fragment(input + consumed, &frag, &frag_size, &frag_type);
        GUARD(next, -1);
        consumed += next;
        element = tns_parser(frag, frag_size, frag_type);
        ht_set(table, key, &element, sizeof(tnetstr *));
    }

    ret = new_tnetstr(tns_HT);
    ret->payload.ptr = table;

}

static tnetstr * parse_Number(char * input, int len){
    int n;
    tnetstr * ret = new_tnetstr(tns_Number);

    for (n = 0; len > 0 && isdigit(*input); len--, input++){
        n = n * 10 + *input - '0';
    }
    ret->payload.number = n;
}

static tnetstr * parse_Boolean(char * input, int len){
    int idx;
    tnetstr * ret = new_tnetstr(tns_Boolean);
    GUARD(ret, NULL);

    for (idx = 0; tns_truth_table[idx] != NULL; idx++){
        if (!strncmp(input, tns_truth_table[idx]->text, len){
            ret->payload.bool = tns_truth_table[idx]->meaning;
            return ret;
        }
    }
    free(ret);
    return NULL;
}

static int next_fragment(char * in, char * start, int * size, tnetstr * type){
    int consumed;

    consumed = get_msg_size(in, size, start);
    GUARD(consumed, -1);
    *type = get_msg_type(in[consumed]);

    return consumed + 1;
}


#undef GUARD
