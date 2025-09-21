#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "counter.h"  // Inclui a biblioteca a ser testada

// Teste a função reset()
void test_reset() {
    reset();
    CU_ASSERT(get() == 0);
}

// Teste a função incr()
void test_incr() {
    reset();
    incr();
    incr();
    CU_ASSERT(get() == 2);
}

// Configuração dos testes
int main() {
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("Counter Test Suite", NULL, NULL);
    if (!suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (!CU_add_test(suite, "Test reset()", test_reset) ||
        !CU_add_test(suite, "Test incr()", test_incr)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}
