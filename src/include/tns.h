/*
 * tns.h
 *
 * Copyright 2011, Pablo Alejandro Costesich
 *
 * Licensed under Three-Clause BSD
 *
 */

#ifndef _CNETSTR
#define _CNETSTR "0.2.1-alpha"
/* I should probably add parsing-event hooks to avoid the need for my
 * libs. That also means you could choose. */
#include "ll.h"
#include "ht.h"
#include <stdio.h>
#include <stddef.h>

typedef struct CTNetStr tnetstr;

typedef enum CTNS_Types {
	tns_Unknown	= 0  ,	/* Unknown data type */
    tns_String	= ',',	/* , */
    tns_List	= ']',	/* ] */
    tns_HT		= '}',	/* } */
    tns_Integer	= '#',	/* # */
    tns_None	= '~',	/* ~ */
    tns_Boolean	= '!' 	/* ! (true!, false!, yes!, no!, True!, False!) */
} tns_type;

typedef int (*tns_eachf)(int item, tnetstr * elem, void * d);

/* parsing and parsing primitives */
#ifdef _TNS_FDPARSE
tnetstr * tns_fdparse(int fd);
#endif
tnetstr * tns_fileparse(FILE * file);
tnetstr * tns_parse(char * input);
tnetstr * tns_parser(char * payload, size_t size, tns_type type);

/* type macros */
#define TNS_is_None(T) 		(tns_get_type(T) == tns_None)
#define TNS_is_HT(T) 		(tns_get_type(T) == tns_HT)
#define TNS_is_List(T) 		(tns_get_type(T) == tns_List)
#define TNS_is_String(T) 	(tns_get_type(T) == tns_String)
#define TNS_is_Integer(T) 	(tns_get_type(T) == tns_Integer)
#define TNS_is_Boolean(T) 	(tns_get_type(T) == tns_Boolean)

/* type getters */
int     tns_int(tnetstr *);
size_t  tns_str(tnetstr *, char * buff, size_t s);
int     tns_bool(tnetstr *);

/* type modifiers */
tnetstr * tns_dict_get(tnetstr * tns, char * key);
tnetstr * tns_dict_set(tnetstr * tns, char * key, tnetstr * val);
tnetstr * tns_dict_del(tnetstr * tns, char * key);
/* list modifiers *./
tnetstr * tns_list_add(tnetstr * list, tnetstr * elem);
tnetstr * tns_list_del(tnetstr * list_elem);
tnetstr * tns_list_swp(tnetstr * list, tnetstr * new);
int		  tns_list_foreach(tns_eachf * f, void * d);
 */

/* type constructors */
tnetstr * tns_new_str(char * str, size_t s);
tnetstr * tns_new_int(int i);
tnetstr * tns_new_bool(int b);
tnetstr * tns_new_none(void);
tnetstr * tns_new_ht(void);
tnetstr * tns_new_list(tnetstr * arr, size_t size); 

/* misc (free, type) */
void 		tns_free(tnetstr * netstr);
tns_type	tns_get_type(tnetstr *);
size_t		tns_strlen(tnetstr * tns);


#endif
