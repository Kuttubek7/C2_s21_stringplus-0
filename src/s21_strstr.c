#include "s21_string.h"
// Находит первое вхождение всей строки needle (не включая завершающий нулевой
// символ), которая появляется в строке haystack.
char* s21_strstr(const char* haystack, const char* needle) {
  s21_size_t needle_len = s21_strlen(needle);
  int flag = 1;

  if (needle_len) {
    flag = 0;
    s21_size_t haystack_len = s21_strlen(haystack);
    while (haystack_len-- >= needle_len) {
      if (!s21_memcmp(haystack, needle, needle_len)) {
        flag = 1;
        break;
      }
      haystack++;
    }
  }

  return flag ? (char*)haystack : s21_NULL;
}
