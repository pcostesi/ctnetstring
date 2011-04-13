/*
//      main.c
//
//      Copyright 2010 Pablo Alejandro Costesich <pcostesi@alu.itba.edu.ar>
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.
*/

#include <stdio.h>
#include <string.h>
#include "tns.h"
#include "ht.h"
#include "CUnit/Basic.h"

int init_tns_suite(void) {
    return 0;
}

int clean_tns_suite(void) {
    return 0;
}

void test_tns(void){
    tnetstr * dict, * str, * str2, * number;
    char c[300];
    size_t l;

	/* Test str */
	str2 = NULL;
    
    str2 = tns_parse("2:hi,");
    CU_ASSERT( NULL != str2 );
    
    l = tns_str(str2, c, 300);
    CU_ASSERT( 0 == strncmp("hi", c, l) );
    
    tns_free(str2);
    str2 = NULL;

	/* Test dict */
    dict = tns_parse("18:2:hi,6:hello!,1:1#}");
    CU_ASSERT( NULL != dict );
    
    str = tns_get_dict(dict, "hi");
    CU_ASSERT( NULL != str );
    
    l = tns_str(str, c, 300);
    CU_ASSERT( 0 == strncmp("hello!", c, l) );
    
    tns_free(dict); /* str goes away with dict */

}
