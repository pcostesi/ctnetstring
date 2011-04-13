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

#include "CUnit/Basic.h"

int init_tns_suite(void);

void test_tns(void);

int clean_tns_suite(void);


int main(int argc, char **argv)
{
    /* Initialize the CUnit test registry */
    if ( CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    CU_pSuite tnsSuite = CU_add_suite("Tagged NetStrings Suite", init_tns_suite, clean_tns_suite);
    

    if ( NULL == tnsSuite ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /*add the test cases to the suites */
    if ((NULL == CU_add_test(tnsSuite, "test_tns", test_tns))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
