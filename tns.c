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

struct CTNetStr{
    tns_type   type;
    void *      payload;
};

static typedef struct{
    unsigned size;
    unsigned used;
    char * str;
} strbuffer;


static int _read(char **, FILE *);
static char * _pop_buffer(strbuffer *);
static strbuffer * _push_buffer(strbuffer *, char);
static strbuffer * _init_buffer(strbuffer *);

tnetstr * tns_parse(char *, FILE *){

}

static char * _pop_buffer(strbuffer * buf){
    char * s = realloc(buf->str, buf->used + 1);
    s[buf->used + 1] = 0;
    if (_init_buffer(buf) == NULL){
         buf->str = s;
         buf->size = buf->used + 1;
         return NULL;
    }
    return s;
}

static strbuffer * _init_buffer(strbuffer * buf){
    char * s = malloc(TNS_BUFF_SIZE);

    if (s == NULL)
        return NULL;

    buf->size = TNS_BUFF_SIZE;
    buf->used = 0;
    buf->str = s;

    return buf;
}

static strbuffer * _push_buffer(strbuffer * buf, char c){
    char * aux;

    if (buf->size < buf->used + 1){
        aux = realloc(buf->str, buf->size * 2);
        if (aux == NULL)
            return NULL;
        buf->str = aux;
    }

    buf->str[buf->used++] = c;
    return buf;
}

static int _read(char **s, FILE *f){
    int c;

    if (s != NULL){
        c = **s;
        *s++;
    } else if (f != NULL){
        c = fgetc(f);
    } else {
        c = EOF;
    }

    return c;
}


tns_type tns_get_type(tnetstr * tns){
    return tns->type;
}
