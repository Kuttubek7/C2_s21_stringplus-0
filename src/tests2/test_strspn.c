#include "test_me.h"

START_TEST(strspn_1) {
  char s1[] = "123456789";
  char s2[] = "123";
  ck_assert_int_eq(strspn(s1, s2), s21_strspn(s1, s2));
}
END_TEST

START_TEST(strspn_2) {
  char s1[] = "123456789";
  char s2[] = "456";
  ck_assert_int_eq(strspn(s1, s2), s21_strspn(s1, s2));
}
END_TEST

START_TEST(strspn_3) {
  char s1[] = "v";
  char s2[] = "vendetta";
  ck_assert_int_eq(strspn(s1, s2), s21_strspn(s1, s2));
}
END_TEST

START_TEST(strspn_4) {
  char s1[] = "";
  char s2[] = "Hello, world!";
  ck_assert_int_eq(strspn(s1, s2), s21_strspn(s1, s2));
}
END_TEST

START_TEST(strspn_5) {
  char s1[] = "Hello, world!";
  char s2[] = "";
  ck_assert_int_eq(strspn(s1, s2), s21_strspn(s1, s2));
}
END_TEST

START_TEST(strspn_6) {
  char s1[] = "6";
  char s2[] = "67";
  ck_assert_int_eq(strspn(s1, s2), s21_strspn(s1, s2));
}
END_TEST

START_TEST(strspn_7) {
  char s1[] = "69917020";
  char s2[] = "69917020";
  ck_assert_int_eq(strspn(s1, s2), s21_strspn(s1, s2));
}
END_TEST

START_TEST(strspn_8) {
  char s1[] = "69917020";
  char s2[] = "69917020";
  ck_assert_int_eq(strspn(s1, s2), s21_strspn(s1, s2));
}
END_TEST

START_TEST(strspn_9) {
  char s1[] = "699";
  char s2[] = "69917020";
  ck_assert_int_eq(strspn(s1, s2), s21_strspn(s1, s2));
}
END_TEST

START_TEST(strspn_10) {
  char s1[] = "69917020";
  char s2[] = "699";
  ck_assert_int_eq(strspn(s1, s2), s21_strspn(s1, s2));
}
END_TEST

START_TEST(strspn_11) {
  char s1[] = "";
  char s2[] = "";
  ck_assert_int_eq(strspn(s1, s2), s21_strspn(s1, s2));
}
END_TEST

START_TEST(strspn_12) {
  char s1[] = "69917020";
  char s2[] = "6991702H";
  ck_assert_int_eq(strspn(s1, s2), s21_strspn(s1, s2));
}
END_TEST

Suite *test_strspn(void) {
  Suite *s = suite_create("\033[45m-=S21_STRSPN=-\033[0m");
  TCase *tc = tcase_create("strspn_tc");

  tcase_add_test(tc, strspn_1);
  tcase_add_test(tc, strspn_2);
  tcase_add_test(tc, strspn_3);
  tcase_add_test(tc, strspn_4);
  tcase_add_test(tc, strspn_5);
  tcase_add_test(tc, strspn_6);
  tcase_add_test(tc, strspn_7);
  tcase_add_test(tc, strspn_8);
  tcase_add_test(tc, strspn_9);
  tcase_add_test(tc, strspn_10);
  tcase_add_test(tc, strspn_11);
  tcase_add_test(tc, strspn_12);

  suite_add_tcase(s, tc);
  return s;
}
