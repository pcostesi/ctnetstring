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
