#include <check.h>
#include "hashes.c"
#include "escaping.c"

int main(void)
{
    Suite *suiteCore = suite_create("Core");
    TCase *caseHashes = tcase_create("Hashes");
    TCase *caseEscaping = tcase_create("Escaping");
    SRunner *sr = srunner_create(suiteCore);
    int nf;

    suite_add_tcase(suiteCore, caseHashes);
    suite_add_tcase(suiteCore, caseEscaping);

    tcase_add_test(caseHashes, FNV1A);
    tcase_add_test(caseHashes, FNV1);
    tcase_add_test(caseHashes, FNV0);

    tcase_add_test(caseEscaping, ESCAPE);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf ? 1 : 0;
}