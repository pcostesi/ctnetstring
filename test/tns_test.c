/*
 *      tns_test.c
 *
 *      Copyright 2010:
 *          Pablo Alejandro Costesich <pcostesi@alu.itba.edu.ar>
 *
 *      Redistribution and use in source and binary forms, with or without
 *      modification, are permitted provided that the following conditions are
 *      met:
 *
 *      * Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following disclaimer
 *        in the documentation and/or other materials provided with the
 *        distribution.
 *      * Neither the name of the Owner nor the names of its
 *        contributors may be used to endorse or promote products derived from
 *        this software without specific prior written permission.
 *
 *      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *      "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *      LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *      A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *      OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *      SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *      LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *      DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *      THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *      (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *      OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
