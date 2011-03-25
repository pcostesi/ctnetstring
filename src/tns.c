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
#include <string.h>
#include "tns.h"
#ifdef _TNS_FDPARSE
#include <unistd.h>
#endif

#define GUARD(got, error) if ((got) == (error)) return NULL
#define JUMP(got, error, jump) do{if ((got) == (error)) {goto jump;}}while(0)

struct CTNetStr{
    tns_type    type;
    union {
        void *  ptr;
        int     integer;
        char *  str;
        unsigned short   bool;
    } payload;
};

tnetstr None = {tns_None, NULL};
tnetstr Yes = {.type = tns_Boolean, .payload.bool = 1};
tnetstr No = {.type = tns_Boolean, .payload.bool = 0};

typedef struct {
    char * text;
    short  meaning;
} tns_truth_tuple;

static const tns_truth_tuple tns_truth_table[] = {
    {"true",  1}, {"True", 1},
    {"false", 0}, {"False", 0},
    {"yes", 1}, {"no", 0},
    NULL
};

static int next_fragment(char * in, char ** start, size_t * size, tns_type * );
static tnetstr * parse_Boolean(char * input, size_t len);
static tnetstr * parse_Integer(char * input, size_t len);
static tnetstr * parse_HT(char * input, size_t len);
static tnetstr * parse_List(char * input, size_t len);
static tnetstr * parse_String(char * input, size_t len);
static tnetstr * new_tnetstr(tns_type type);
static tns_type get_msg_type(char t);
static int get_msg_size(char * input, size_t * trailing, size_t * offset);
static int free_tns_ht(size_t s, const char * k, const void * v, void * d);

/* Public functions */

#ifdef _TNS_FDPARSE


/* TODO: make a nonblocking-enabled version of this function */
tnetstr * tns_fdparse(int fd){
    int n = 0;
    char c;
    char * payload;
    tns_type type;

    while (read(fd, &c, 1) != -1 && c != ':'){
        acc++;
        if (isdigit(c))
            n = 10 * n + c - '0';
        else
            return NULL;
    }

    payload = malloc(n);
    read(fd, payload, n);
    read(fd, &c, 1);
    type = get_msg_type(c);

    return tns_parser(payload, n, type);
}

#endif

tnetstr * tns_fileparse(FILE * file){
    char * payload;
    tns_type type;
    int size, total;

    GUARD(fscanf(file, "%d:", &size), 0);
    payload = malloc(size);
    GUARD(payload, NULL);

    total = fread(payload, (size_t) size, 1, file);
    if (total < size && ferror(file))
        return NULL;

    return tns_parser(payload, size, type);
}

tnetstr * tns_parse(char * input){
    size_t trailing, offset;
    int total;
    char * payload;
    tns_type type = tns_Unknown;

    total = get_msg_size(input, &trailing, &offset);
    GUARD(total, -1);
    payload = input + offset;
    type = get_msg_type((size_t) input[total]);

    return tns_parser(payload, trailing, type);
}


tnetstr * tns_parser(char * payload, size_t size, tns_type type){
    tnetstr * ret = NULL;

    switch(type){

        case tns_String:
            ret = parse_String(payload, size);
            break;

        case tns_None:
            ret = new_tnetstr(tns_None);
            break;

        case tns_List:
            ret = parse_List(payload, size);
            break;

        case tns_Boolean:
            ret = parse_Boolean(payload, size);
            break;

        case tns_Integer:
            ret = parse_Integer(payload, size);
            break;

        case tns_HT:
            ret = parse_HT(payload, size);
            break;

        default:
            return NULL;
            /* free stuff up and throw an error */
    }
}

void tns_free(tnetstr * netstr){
    llist * list;
    tnetstr * tmp;

    switch(netstr->type){
        case tns_String:
            free(netstr->payload.str);
            break;
        case tns_HT:
            ht_each((ht *) netstr->payload.ptr, free_tns_ht, NULL);
            ht_free((ht *) netstr->payload.ptr);
            break;
        case tns_List:
            list = (llist *) netstr->payload.ptr;
            for (; list != NULL; list = ll_next(list)){
                ll_get(list, &tmp, sizeof(tnetstr *));
                tns_free(tmp);
            }
            ll_free((llist *) netstr->payload.ptr);
        case tns_Boolean:
        case tns_None:
            return;
    }
    free(netstr);
}

tnetstr * tns_get_dict(tnetstr * tns, char * key){
    tnetstr * ret;
    size_t s;

    if (tns->type != tns_HT)
        return NULL;
    s = ht_get(tns->payload.ptr, key, &ret, sizeof(tnetstr *));
    return s ? ret : NULL;
}

tnetstr * tns_set_dict(tnetstr * tns, char * key, tnetstr * val){
    ht * t;

    if (tns->type != tns_HT)
        return NULL;
    t = ht_set(tns->payload.ptr, key, &val, sizeof(tnetstr *));
    return t == NULL ? NULL : tns;
}

tnetstr * tns_del_dict(tnetstr * tns, char * key){
    ht * t;

    if (tns->type != tns_HT)
        return NULL;
    t = ht_del(tns->payload.ptr, key);
    return t == NULL ? NULL : tns;
}

tnetstr * tns_new_str(char * str, size_t s){
    tnetstr * ret;

    s = s ? s : strlen(str);
    ret = new_tnetstr(tns_String);
    GUARD(ret, NULL);
    ret->payload.str = malloc(s + 1);
    if(ret->payload.str){
        free(ret);
        return NULL;
    }
    strncpy(ret->payload.str, str, s);
    return ret;
}

tnetstr * tns_new_int(int i){
    tnetstr * ret;

    ret = new_tnetstr(tns_Integer);
    GUARD(ret, NULL);
    ret->payload.integer = i;
    return ret;
}

tnetstr * tns_new_bool(int b){
    tnetstr * ret;

    ret = new_tnetstr(tns_Boolean);
    GUARD(ret, NULL);
    ret->payload.bool = (unsigned short) b;
    return ret;
}

tnetstr * tns_new_none(void){
    return new_tnetstr(tns_None);
}

tnetstr * tns_new_ht(void){
    tnetstr * ret;
    ht * t;

    ret = new_tnetstr(tns_HT);
    GUARD(ret, NULL);
    t = ht_new(NULL);
    if (t == NULL){
        free(ret);
        return NULL;
    }
    ret->payload.ptr = t;
    return ret;
}

tns_type tns_get_type(tnetstr * tns){
    return tns->type;
}

int tns_int(tnetstr * tns){
    return tns->payload.integer;
}

size_t tns_str(tnetstr * tns, char * buff, size_t s){
    size_t l;

    if (tns->type != tns_String)
        return 0;
    l = strlen(tns->payload.str);
    l = s < l ? s : l;
    strncpy(buff, tns->payload.str, l);
    return l;
}

int tns_bool(tnetstr * tns){
    return tns->payload.bool;
}

size_t tns_strlen(tnetstr * tns){
    if (tns->type != tns_String)
        return 0;
    return strlen(tns->payload.str);
}

/* Helpers and other private functions */

static int free_tns_ht(size_t s, const char * k, const void * v, void * d){
    tns_free((tnetstr *) v);
    return 0;
}

/** returns consumed bytes */
static int get_msg_size(char * input, size_t * trailing, size_t * offset){
    int n;
    size_t x = 0;
    size_t acc = 1;

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
        case ',':
            return tns_String;
        case ']':
            return tns_List;
        case '#':
            return tns_Integer;
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
    tnetstr * ret;
    switch(type){
        case tns_None:
            ret = &None;
        default:
        ret = malloc(sizeof(tnetstr));
        if (ret)
            ret->type = type;
    }
    return ret;
}

static tnetstr * parse_String(char * input, size_t len){
    char * c;
    tnetstr * ret;

    ret = new_tnetstr(tns_String);
    GUARD(ret, NULL);
    c = malloc(len + 1);
    if (c == NULL){
        free(ret);
        return NULL;
    }

    strncpy(c, input, len);
    c[len] = 0;
    ret->payload.str = c;

    return ret;
}

static tnetstr * parse_List(char * input, size_t len){
    tnetstr * ret, * element;
    char * frag;
    size_t frag_size, consumed, next;
    tns_type frag_type;
    llist * list, * aux_list;

    ret = new_tnetstr(tns_List);
    GUARD(ret, NULL);
    consumed = 0;
    while (len - consumed > 0){
        next = next_fragment(input + consumed, &frag, &frag_size, &frag_type);
        JUMP(next, -1, fragment_error);
        consumed += next;
        element = tns_parser(frag, frag_size, frag_type);
        JUMP(element, NULL, element_error);
        aux_list = ll_insert(list, &element, sizeof(tnetstr *));
        JUMP(aux_list, NULL, element_error);
        list = aux_list;
    }

    ret->payload.ptr = ll_head(list);
    return ret;

    element_error:
    fragment_error:
        ret->payload.ptr = ll_head(list);
        tns_free(ret);
        return NULL;
}

static tnetstr * parse_HT(char * input, size_t len){
    tnetstr * ret, * element;
    char * frag, * key = NULL;
    size_t frag_size, consumed;
    int next;
    tns_type frag_type;
    ht * table;

    consumed = 0;
    table = ht_new(NULL);
    GUARD(table, NULL);

    ret = new_tnetstr(tns_HT);
    if (ret == NULL){
        ht_free(table);
        return NULL;
    }
    ret->payload.ptr = table;

    key = malloc(len);
    JUMP(key, NULL, fragment_error);

    while (len - consumed > 0){
        /* parse the string key */
        next = next_fragment(input + consumed, &frag, &frag_size, &frag_type);
        JUMP(next, -1, fragment_error);
        consumed += next;

        strncpy(key, frag, frag_size);
        key[frag_size] = 0;

        /* do the same, but for the matching value */
        next = next_fragment(input + consumed, &frag, &frag_size, &frag_type);
        JUMP(next, -1, fragment_error);
        consumed += next;
        element = tns_parser(frag, frag_size, frag_type);
        JUMP(element, NULL, element_error);
        table = ht_set(table, key, &element, sizeof(tnetstr *));
        JUMP(table, NULL, element_error);
    }

    free(key);
    return ret;

    fragment_error:
    element_error:
        tns_free(ret);
        free(key);
        return NULL;
}

static tnetstr * parse_Integer(char * input, size_t len){
    int n;
    tnetstr * ret = new_tnetstr(tns_Integer);
    GUARD(ret, NULL);

    for (n = 0; len > 0 && isdigit(*input); len--, input++){
        n = n * 10 + *input - '0';
    }
    ret->payload.integer = n;
}

static tnetstr * parse_Boolean(char * input, size_t len){
    int idx;
    /*
    tnetstr * ret = new_tnetstr(tns_Boolean);
    GUARD(ret, NULL);
    */
    for (idx = 0; idx < sizeof(tns_truth_table); idx++){
        if (!strncmp(input, tns_truth_table[idx].text, len)){
            /*
            ret->payload.bool = (unsigned short) tns_truth_table[idx].meaning;
            return ret;
            */
            return tns_truth_table[idx].meaning ? &Yes : &No;
        }
    }
    /*
    free(ret);
    */
    return NULL;
}

static int next_fragment(char * in, char ** start, size_t * size, tns_type * type){
    int consumed;
    size_t offset, trailing;

    consumed = get_msg_size(in, &trailing, &offset);
    if (consumed == -1)
        return 0;
    *type = get_msg_type(in[consumed]);
    *start = in + offset;
    *size = trailing;
    return consumed + 1;
}

#undef GUARD
#undef JUMP
