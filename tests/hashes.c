#include <check.h>
#include "fnv.h"

START_TEST(FNV1A)
{
    ck_assert_int_eq(fnv1a_32((uint8_t*)"TESTSTRING",   sizeof("TESTSTRING") - 1),   0xB90B6318);
    ck_assert_int_eq(fnv1a_32((uint8_t*)"string no. 2", sizeof("string no. 2") - 1), 0xDBB79CB9);

    ck_assert_int_eq(fnv1a_64((uint8_t*)"rando# str;",  sizeof("rando# str;") - 1),  0xEC9733A1F3FC9CA4);
    ck_assert_int_eq(fnv1a_64((uint8_t*)"$thisSTR[]",   sizeof("$thisSTR[]") - 1),   0x7EDD671CD3D0AFA2);
}
END_TEST

START_TEST(FNV1)
{
    ck_assert_int_eq(fnv1_32((uint8_t*)"myD4TAPACK3t'@@", sizeof("myD4TAPACK3t'@@") - 1), 0xEA5C61F7);
    ck_assert_int_eq(fnv1_32((uint8_t*)"herebeSTRING=$",  sizeof("herebeSTRING=$") - 1),  0x77DBA174);

    ck_assert_int_eq(fnv1_64((uint8_t*)"rando# str;",     sizeof("rando# str;") - 1),     0x54EEED3F32FAF7BA);
    ck_assert_int_eq(fnv1_64((uint8_t*)"$thisSTR[]",      sizeof("$thisSTR[]") - 1),      0xF19D02D15737439C);
}
END_TEST

START_TEST(FNV0)
{
    ck_assert_int_eq(fnv0_32((uint8_t*)"myD4TAPACK3t@",   sizeof("myD4TAPACK3t@") - 1), 0xCC3A1038);

    ck_assert_int_eq(fnv0_64((uint8_t*)"myD4TAPACK3t@",   sizeof("myD4TAPACK3t@") - 1), 0x8EA573C766F7B7F8);
}
END_TEST

int main(void)
{
    Suite *s1 = suite_create("Core");
    TCase *tc1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1);

    tcase_add_test(tc1, FNV1A);
    tcase_add_test(tc1, FNV1);
    tcase_add_test(tc1, FNV0);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf ? 1 : 0;
}