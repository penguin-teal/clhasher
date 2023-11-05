#include <check.h>
#include <stdlib.h>
#include "escape.h"

START_TEST(ESCAPE)
{
    char *s;

    const char copyright[] = "\251COPYRIGHT";
    const char symbols[] = "\xA9\tAA\nBC\r\\";

    s = mallocEscStr("\\251COPYRIGHT");
    ck_assert_mem_eq(s, copyright, sizeof copyright);
    free(s);

    s = mallocEscStr("\\xA9\\tAA\\nBC\\r\\\\");
    ck_assert_mem_eq(s, symbols, sizeof symbols);
    free(s);

    s = mallocEscStr("HI\\LOW");
    ck_assert_ptr_eq(s, NULL);

    s = mallocEscStr("\\");
    ck_assert_ptr_eq(s, NULL);

}
END_TEST
