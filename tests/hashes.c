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
    ck_assert_int_eq(fnv0_32((uint8_t*)"myD4TAPACK3t@",   sizeof("myD4TAPACK3t@") - 1), 0x2237C53B);

    ck_assert_int_eq(fnv0_64((uint8_t*)"myD4TAPACK3t@",   sizeof("myD4TAPACK3t@") - 1), 0x82C5DD97E679F8FB);
}
END_TEST
