#include <wchar.h>

#include "ctype.h"
#include "s21_string.h"

int s21_sprintf(char *str, const char *format, ...) {
  va_list input = {0};
  va_start(input, format);
  s21_FORMAT parameters = {0};
  s21_size_t i = 0;
  s21_size_t lol = 0;
  s21_size_t len = 0;
  str = s21_strcpy(str, "");
  while (*format) {
    parameters.precision = -1;
    parameters.flag_plus = parameters.flag_minus = parameters.flag_space =
        parameters.flag_zero = parameters.flag_sharp = parameters.width =
            parameters.point = parameters.specifier = parameters.length = 0;
    switch (*format) {
      case '%':
        format++;
        if (is_flag(*format)) {
          while (is_flag(*format)) {
            s21_check_flag(&parameters, format++);
          }
        }
        if (is_digit(*format) || *format == '*') {
          format = s21_str_to_int(&parameters.width, format++, &input);
          if (parameters.width < 0) {
            parameters.flag_minus = 1;
            parameters.width *= -1;
          }
        }
        if (*format == '.') {
          parameters.point = 1;
          parameters.precision = 0;
          format++;
          if (is_digit(*format) || *format == '*') {
            format = s21_str_to_int(&parameters.precision, format++, &input);
          } else {
            parameters.precision = -1;
          }
        }
        if (is_length(*format)) {
          while (is_length(*format)) {
            parameters.length = *(format++);
          }
        }
        if (is_specifier(*format)) {
          parameters.specifier = *(format++);
        }
        s21_default_precision(&parameters);
        str = s21_check_specifier(str, &i, &input, &parameters);
        if (parameters.stop == 1) {
          lol = i;
          parameters.stop = 0;
          parameters.returnN = 1;
        }

        break;
      default:
        str[i++] = *(format++);
        str[i] = '\0';
        break;
    }
  }
  va_end(input);
  if (parameters.returnN == 1) {
    len = (int)s21_strlen(str) + parameters.nulls;
    str[lol] = '\0';
  } else
    len = (int)s21_strlen(str);
  // if (parameters.stop == 1) return (int)s21_strlen(str) + 1;
  return len;
  // return (int)s21_strlen(str) + parameters.nulls;
}

void s21_default_precision(s21_FORMAT *parameters) {
  if (parameters->point && parameters->precision <= 0) {
    parameters->precision = 0;
  }
  if (!parameters->point && s21_strchr("eEfgG", parameters->specifier)) {
    parameters->precision = 6;
  }
}
void s21_check_flag(s21_FORMAT *parameters, const char *format) {
  switch (*format) {
    case '-':
      parameters->flag_minus = 1;
      break;
    case '+':
      parameters->flag_plus = 1;
      break;
    case ' ':
      parameters->flag_space = 1;
      break;
    case '#':
      parameters->flag_sharp = 1;
      break;
    case '0':
      parameters->flag_zero = 1;
      break;
  }
}

char *s21_check_specifier(char *str, s21_size_t *str_len, va_list *input,
                          s21_FORMAT *parameters) {
  char *tmp_str = s21_NULL;
  tmp_str = calloc(1024, sizeof(char));
  if (tmp_str != s21_NULL) {
    switch (parameters->specifier) {
      case 'c':
        s21_spec_c(tmp_str, input, parameters);
        break;
      case 'd':
      case 'i':
        s21_spec_di(tmp_str, input, parameters);
        break;
      case 'e':
      case 'E':
        flt_to_str(input, tmp_str, parameters);
        break;
      case 'f':
        flt_to_str(input, tmp_str, parameters);
        break;
      case 'g':
      case 'G':
        flt_to_str(input, tmp_str, parameters);
        break;
      case 'n':
        s21_spec_n(str_len, input);
        break;
      case 'o':
        s21_spec_o(tmp_str, input, parameters);
        break;
      case 'p':
        s21_spec_p(tmp_str, input, parameters);
        break;
      case 's':
        // s21_spec_s(tmp_str, input, parameters);
        s22str_to_str(input, tmp_str, parameters);
        break;
      case 'u':
        s21_spec_u(tmp_str, input, parameters);
        break;
      case 'x':
      case 'X':
        s21_spec_xX(tmp_str, input, parameters);
        break;
      case '%':
        s21_spec_percentage(tmp_str, parameters);
        break;
    }
    s21_strcat(str, tmp_str);
    free(tmp_str);
    *str_len = s21_strlen(str);
  }
  return str;
}

char *strchrncat(char *str, char ch, int n, int f) {
  if (n <= 0) return str;
  s21_size_t m = s21_strlen(str);
  if (f) {
    s21_memset(str + m, ch, n);
    str[m + n] = '\0';
  } else {
    s21_memmove(str + n, str, m + 1);
    s21_memset(str, ch, n);
  }
  return str;
}
char *s21_itoa(long long value, char *str, int radix) {
  char *buffer = str;
  value = llabs(value);
  do {
    *buffer = value % radix + (value % radix < 10 ? 48 : 87);
    ++buffer;
    value /= radix;
  } while (value != 0);
  *buffer = '\0';
  --buffer;
  // перевернуть строку
  for (int i = 0; (str + i) < (buffer - i); i++) {
    char temp = *(str + i);
    *(str + i) = *(buffer - i);
    *(buffer - i) = temp;
  }
  return str;
}

char *s21_ftoa(long double value, char *str, s21_size_t accuracy, int f) {
  char *p;  // указатель на начало дробной части
  char *digit;
  s21_size_t n = (f ? 0 : accuracy);
  value = fabsl(value);
  do {
    long double a, b;
    b = modfl(roundl(value * pow(10, n)) / pow(10, n), &a);
    s21_itoa(llroundl(a), str, 10);
    p = str + s21_strlen(str);
    // преобразовать в строку дробную часть
    if (n > 0) s21_itoa(llroundl(b * pow(10, n)), p, 10);
    // добавить в дробную часть недостающие нули
    strchrncat(p, '0', n++ - s21_strlen(p), 0);
    if (f && value == 0 && n == accuracy) break;
    digit = s21_strpbrk(str, "123456789");
  } while (f && (!digit || s21_strlen(digit) < accuracy));
  // вставить точку перед дробной частью
  strchrncat(p, '.', (--n > 0), 0);
  return str;
}  // конец s21_ftoa

char *flt_to_str(va_list *args, char *str, s21_FORMAT *parameters) {
  long double arg = 0;  // значение вещественного аргумента
  int ll = 0;
  char *nifn = s21_NULL;

  if (parameters->length == 0) arg = (double)va_arg(*args, double);
  if (parameters->length == 'h') arg = (float)va_arg(*args, double);
  if (parameters->length == 'l') arg = (double)va_arg(*args, double);
  if (parameters->length == 'L') arg = (long double)va_arg(*args, long double);
  long double mantissa = arg;
  int n = 0;  // порядок степени аргумента в экспоненциальной нотации
  if ((isinf(arg) || isnan(arg) || arg == 0) &&
      (parameters->specifier == 'g' || parameters->specifier == 'G')) {
    nifn = arg == 0 ? "0" : (arg == INFINITY ? "inf" : "nan");
    s21_strcat(str, nifn);
  } else {
    long double left = 0;    // long double
    modfl(mantissa, &left);  // modfl
    char text[999] = {0};
    s21_itoa(left, text, 10);
    if (((text[0] - '0') * pow(10, 0) == 9) && (text[1] - '0') * pow(10, 0) > 4)
      ll = 1;

    if (s21_strchr("eEgG", parameters->specifier)) {
      while (fabsl(mantissa) >= (ll ? 1 : 10)) {
        mantissa /= 10;
        n++;
      }
      while (0 < fabsl(mantissa) && fabsl(mantissa) < (ll ? 0 : 1)) {
        mantissa *= 10;
        n--;
      }
    }

    int f_exp = (n < -4 || n >= parameters->precision);
    if (0 == arg || (1 <= arg && arg <= 9)) f_exp = 0;
    if (s21_strchr("f", parameters->specifier))
      s21_ftoa(arg, str, parameters->precision, 0);
    if (s21_strchr("eE", parameters->specifier))
      s21_ftoa(mantissa, str, parameters->precision, 0);
    if (s21_strchr("gG", parameters->specifier))
      s21_ftoa(f_exp ? mantissa : arg, str, parameters->precision, 1);
    // подавить вывод нулей и точки в конце дробной части для "gG"
    if (s21_strchr("gG", parameters->specifier) && !parameters->flag_sharp) {
      char *dot = s21_strchr(str, '.');
      for (char *p;
           dot && (p = s21_strrchr(dot, '0')) && !s21_strpbrk(p, "123456789");)
        *p = '\0';
      if (dot && !s21_strpbrk(dot, "0123456789")) *dot = '\0';
    }
    int k = 0;  // количество знаков в итоговом префиксе +/-, ' '
    if (arg < 0) strchrncat(str, '-', (++k, 1), 0);
    if (parameters->flag_plus && arg >= 0) strchrncat(str, '+', (++k, 1), 0);
    if (parameters->flag_space && !parameters->flag_plus && arg >= 0)
      strchrncat(str, ' ', (++k, 1), 0);
    // добавить "." при нулевой точности
    if (parameters->flag_sharp && parameters->precision == 0)
      s21_strcat(str, ".");
    // добавить суффикс эксп. нотации
    if (s21_strchr("eE", parameters->specifier) ||
        (s21_strchr("gG", parameters->specifier) && f_exp)) {
      strchrncat(str, (isupper(parameters->specifier) ? 'E' : 'e'), 1, 1);
      strchrncat(str, (n >= 0 ? '+' : '-'), 1, 1);
      strchrncat(str, '0', abs(n) <= 9, 1);
      s21_itoa(n, str + s21_strlen(str), 10);
    }

    char ch = (parameters->flag_zero && !parameters->flag_minus ? '0' : ' ');
    strchrncat(str + (ch == '0' ? k : 0), ch,
               parameters->width - s21_strlen(str), parameters->flag_minus);
  }
  return str;
}

char *s21_str_to_int(int *number, const char *format, va_list *input) {
  char wth[128] = {0};
  char *num = NULL;
  int count = 0;
  while (is_digit(*format)) {
    wth[count++] = *(format)++;
  }
  num = wth;
  while (count > 0) {
    *number += ((*(num)++) - '0') * pow(10, --count);
  }
  if (*format == '*') {
    *number = va_arg(*input, int);
    format++;
  }
  return (char *)format;
}

void s21_int_to_str(s21_FORMAT *parameters, char *str, long double number) {
  char *point = s21_NULL;
  int flag = 1;
  parameters->sign = number < 0 ? -1 : 1;
  number *= parameters->sign;
  point = str;
  if (parameters->specifier != 'n') {
    while (flag) {
      int digit = 0;
      if (number >= 10) {
        digit = (int)fmod(number, 10);
        number = (number - digit) / 10;
      } else {
        digit = roundl(number);  //(int)number   fff
        // digit = (int)number;  //(int)number
        flag = 0;
      }
      *(point++) = (char)(digit + '0');
    }
    if (s21_strchr("dioxX", parameters->specifier) &&
        parameters->precision == 0 && number == 0 && parameters->point)
      str[0] = '\0';
    *point = '\0';
    s21_reverse_writing(str);
  }
}

void s21_ul_to_str(char *str, unsigned long long number,
                   s21_FORMAT *parameters) {
  char *point = s21_NULL;
  int flag = 1;

  point = str;

  while (flag) {
    int digit = 0;
    if (number >= 10) {
      digit = (int)fmod(number, 10);
      number = (number - digit) / 10;
    } else {
      digit = (int)number;
      flag = 0;
    }
    *(point++) = (char)(digit + '0');
  }
  if (parameters->precision == 0 && number == 0 && parameters->point)
    str[0] = '\0';
  *point = '\0';
  s21_reverse_writing(str);
}

void s21_ol_to_str(char *str, unsigned long long number, int prefix) {
  char *point = s21_NULL;
  int flag = 1;
  int nulevoi = 0;

  point = str;

  while (flag) {
    unsigned int digit;
    if (number > 7) {
      digit = number % 8;
      number = (number - digit) / 8;
    } else {
      digit = number;
      flag = 0;
    }
    *(point++) = (char)(digit + '0');
  }
  if (str[0] == '0' && str[1] == '\0') {
    nulevoi = 1;
  }

  if (prefix && !nulevoi) *(point++) = '0';  // prefix == 1 && nulevoy != 1
  *point = '\0';
  s21_reverse_writing(str);
}

void s21_hl_to_str(char *str, unsigned long long number,
                   s21_FORMAT *parameters) {
  char *point = s21_NULL;
  int flag = 1, shift = 0;
  point = str;
  shift = parameters->specifier == 'x' ? 32 : 0;

  while (flag) {
    unsigned int digit;
    if (number > 15) {
      digit = number % 16;
      number = (number - digit) / 16;
    } else {
      digit = number;
      flag = 0;
    }
    if (digit < 10)
      *(point++) = (char)(digit + '0');
    else
      *(point++) = (char)(digit + 'A' + shift - 10);
  }
  *point = '\0';
  if (number == 0) parameters->flag_sharp = 0;
  s21_reverse_writing(str);
}

void s21_wch_to_str(char *str, wchar_t *wstr, s21_size_t length) {
  s21_size_t str_len = s21_strlen(str);
  int cnt = length;
  char *p = str + str_len;

  while (*wstr != '\0' && (cnt-- > 0 || length == 0)) {
    *(p++) = (char)*(wstr++);
  }
  *p = '\0';
}

/* Функция сдвига строки на 1 позицию вправо */
void s21_move_string(char *str) {
  int pos = (int)s21_strlen(str);
  str[pos + 1] = '\0';
  while (pos > 0) {
    str[pos] = str[pos - 1];
    pos--;
  }
}

void s21_move_string_left(char *str) {
  int pos = (int)s21_strlen(str);
  int i = 0;
  str[pos + 1] = '\0';
  while (pos > 0) {
    str[i] = str[i + 1];
    i++;
    pos--;
  }
}

void s21_make_string_flags(s21_FORMAT *parameters, char *str) {
  int pos = 0;
  if (!s21_strchr("cuo\%", parameters->specifier)) {
    if (parameters->specifier != 'x' && parameters->specifier != 'X') {
      if (parameters->sign < 0) {
        s21_move_string(str);
        str[pos++] = '-';
        parameters->sign = 0;
      }
      if (parameters->flag_plus && parameters->sign != 0) {
        s21_move_string(str);
        str[pos] = (parameters->sign > 0) ? '+' : '-';
        parameters->sign = 0;
      }
      if (parameters->flag_space && parameters->sign != 0) {
        s21_move_string(str);
        str[pos] = (parameters->sign > 0) ? ' ' : '-';
        parameters->sign = 0;
      }
    }
    if (parameters->flag_sharp) {
      s21_sharp_flag(parameters, str);
    }
  }
  if (parameters->flag_minus) {
    pos = (int)s21_strlen(str);
    while (pos < parameters->width) {
      str[pos++] = ' ';
    }
  }
}

/* Функция обработки флага '#' */
void s21_sharp_flag(s21_FORMAT *parameters, char *str) {
  int pos = 0;
  if (s21_strchr("oxX", parameters->specifier)) {
    if (parameters->specifier == 'x' || parameters->specifier == 'X') {
      s21_move_string(str);
    }
    parameters->specifier == 'o' ? 1 : s21_move_string(str);
    str[pos++] = '0';
    if (parameters->specifier != 'o')
      str[pos] = parameters->specifier == 'x' ? 'x' : 'X';
  }
  if (s21_strchr("eEf", parameters->specifier)) {
    if (!s21_strchr(str, '.')) {
      str[s21_strlen(str)] = '.';
    }
  }
}

/* Функция обработки ширины форматной строки */
void s21_make_string_width(s21_FORMAT *parameters, char *str) {
  int pos_str = (int)s21_strlen(str);
  int minus = 0;
  int nulevoi = 0;
  if (str[0] == '0' && str[1] == '\0') {
    nulevoi = 1;
  }

  char znak;
  int local_width = 0;
  local_width = parameters->width;

  if (parameters->width > pos_str) {
    if ((((str[0] == '-') || (str[0] == '+') || str[0] == ' ') &&
         (parameters->flag_zero) && ((parameters->specifier != 'c'))) ||
        ((parameters->flag_zero) && (parameters->flag_plus) &&
         ((parameters->specifier != 'c')))) {
      // znak = str[0] == '-' ? '-' : '+';
      switch (str[0]) {
        case '-':
          znak = '-';
          minus = 1;
          break;
        case '+':
          znak = '+';
          minus = 1;
          break;
        case ' ':
          znak = ' ';
          minus = 1;
          break;
      }
      // minus = 1;
      s21_move_string_left(str);
      parameters->width += 1;
    }

    str[parameters->width + 1] = '\0';
    while (pos_str >= 0) {
      str[parameters->width--] = str[pos_str--];
    }
    for (pos_str = 0; pos_str <= parameters->width; pos_str++) {
      if (s21_strchr("diopuxX", parameters->specifier)) {
        str[pos_str] =
            (parameters->flag_zero && !parameters->point) ? '0' : ' ';
      }
      if (s21_strchr("dfeEgGcs", parameters->specifier) && minus == 1) {
        str[pos_str] = (parameters->flag_zero) ? '0' : ' ';
        if (pos_str == parameters->width) {
          str[0] = znak;
        }
      }

      if (s21_strchr("feEgGcs", parameters->specifier)) {
        str[pos_str] = (parameters->flag_zero) ? '0' : ' ';
      }
    }  //%.X
  }
  if (parameters->point && parameters->precision == 0 && local_width >= 0 &&
      nulevoi == 1) {
    if (local_width == 0) {
      str[0] = '\0';
    } else
      s21_memset(str, ' ', local_width);
  }
}

/* Функция обработки точности форматной строки */
void s21_make_string_precision(s21_FORMAT *parameters, char *str) {
  char *string_with_precision = s21_NULL;
  string_with_precision = calloc(1024, sizeof(char));
  int pos = 0;
  int point = 0;
  // int pop = 1;
  if ((parameters->specifier == 'p' && !parameters->flag_minus &&
       ((parameters->width >= 0 && parameters->precision >= 9) ||
        (parameters->width >= 0 && parameters->precision >= 0 &&
         (s21_strcmp(str, "0x0") == 0))))) {
    ;
    /* code */

    point = 2;
    s21_move_string_left(str);
    s21_move_string_left(str);
  }
  if (string_with_precision != s21_NULL) {
    if (s21_strchr("pdiouxX", parameters->specifier)) {
      while (pos < parameters->precision - (int)s21_strlen(str) + point) {
        string_with_precision[pos++] = '0';
      }
      for (int i = 0; str[i] != '\0'; i++) {
        string_with_precision[pos++] = str[i];
      }
      s21_strcpy(str, string_with_precision);
      if (point > 0) {
        str[0] = '0';
        str[1] = 'x';
      }
    }
    free(string_with_precision);
  }
}
// s21 === *=   0000000000000x1056bb6d9=* = 25=
//  man === *=0x00000000000000100cbb6d9=*

/* Функция обратной записи в строку */
void s21_reverse_writing(char *str) {
  char c = 0;
  s21_size_t length = s21_strlen(str);
  for (s21_size_t i = 0; i < length / 2; i++) {
    c = str[i];
    str[i] = str[length - 1 - i];
    str[length - 1 - i] = c;
  }
}

/* Символ */
void s21_spec_c(char *str, va_list *input, s21_FORMAT *parameters) {
  wchar_t wchar = 0;
  wchar_t wchar_s[10] = {0};
  switch (parameters->length) {
    case 'l':
      wchar = va_arg(*input, wchar_t);
      wchar_s[0] = wchar;
      s21_wch_to_str(str, wchar_s, 0);
      break;
    default:
      str[0] = va_arg(*input, int);
      if (str[0] == '\0') {
        parameters->nulls += 1;
        if (parameters->returnN != 1) {
          parameters->stop = 1;
        }
      }

      str[1] = '\0';
      break;
  }
  s21_make_string_flags(parameters, str);
  s21_make_string_width(parameters, str);
}

/* Знаковое десятичное целое число */
void s21_spec_di(char *str, va_list *input, s21_FORMAT *parameters) {
  long int ld_number = 0;
  short int sd_number = 0;
  int number = 0;

  switch (parameters->length) {
    case 'h':
      sd_number = (short int)va_arg(*input, int);
      s21_int_to_str(parameters, str, sd_number);
      break;
    case 'l':
      ld_number = (long int)va_arg(*input, long int);
      s21_int_to_str(parameters, str, ld_number);
      break;
    default:
      number = va_arg(*input, int);
      s21_int_to_str(parameters, str, number);
      break;
  }
  s21_make_string_precision(parameters, str);
  s21_make_string_flags(parameters, str);
  s21_make_string_width(parameters, str);
}

/* Беззнаковое восьмеричное число */
void s21_spec_o(char *str, va_list *input, s21_FORMAT *parameters) {
  unsigned long int ld_number = 0;
  unsigned short int sd_number = 0;
  unsigned int number = 0;

  switch (parameters->length) {
    case 'h':
      sd_number = (unsigned short int)va_arg(*input, unsigned int);
      s21_ol_to_str(str, sd_number, parameters->flag_sharp);
      break;
    case 'l':
      ld_number = (unsigned long int)va_arg(*input, unsigned long int);
      s21_ol_to_str(str, ld_number, parameters->flag_sharp);
      break;
    default:
      number = va_arg(*input, unsigned int);
      s21_ol_to_str(str, number, parameters->flag_sharp);
      break;
  }
  s21_make_string_precision(parameters, str);
  s21_make_string_flags(parameters, str);
  s21_make_string_width(parameters, str);
}

char *s22str_to_str(va_list *args, char *str, s21_FORMAT *cs) {
  // строковое значение аргумента ограничить точностью
  if (cs->length == 0) {
    char *tmpstr = va_arg(*args, char *);
    s21_strncat(str, (tmpstr ? tmpstr : "(null)"), cs->precision);
  }
  // широкую строку аргумента преобразовать в многобайтовую строку
  // и ограничить ее точностью
  if (cs->length == 'l') {
    wchar_t *pwcs = va_arg(*args, wchar_t *);
    int n = wcstombs(str, pwcs, wcslen(pwcs) * MB_CUR_MAX);
    if (cs->precision < n) n = cs->precision;
    str[n] = '\0';
  }
  // дополнить пробелами до заданной ширины
  // и выровнять вывод по левому или правому краю
  strchrncat(str, cs->flag_zero ? '0' : ' ', cs->width - s21_strlen(str),
             cs->flag_minus);
  return str;
}  // конец str_to_str

/* Беззнаковое десятичное целое число */
void s21_spec_u(char *str, va_list *input, s21_FORMAT *parameters) {
  unsigned short us_number = 0;
  unsigned long ul_number = 0;
  unsigned int u_number = 0;

  switch (parameters->length) {
    case 'h':
      us_number = (unsigned short)va_arg(*input, unsigned int);
      s21_ul_to_str(str, us_number, parameters);
      break;
    case 'l':
      ul_number = (unsigned long)va_arg(*input, unsigned long);
      s21_ul_to_str(str, ul_number, parameters);
      break;
    default:
      u_number = (unsigned int)va_arg(*input, unsigned int);
      s21_ul_to_str(str, u_number, parameters);
      break;
  }

  s21_make_string_precision(parameters, str);
  s21_make_string_flags(parameters, str);
  s21_make_string_width(parameters, str);
}

/* Беззнаковое шестнадцатеричное целое число(X - заглавные буквы) */
void s21_spec_xX(char *str, va_list *input, s21_FORMAT *parameters) {
  unsigned long int ld_number = 0;
  unsigned short int sd_number = 0;
  unsigned int number = 0;

  switch (parameters->length) {
    case 'h':
      sd_number = (unsigned short int)va_arg(*input, unsigned int);
      s21_hl_to_str(str, sd_number, parameters);
      break;
    case 'l':
      ld_number = (unsigned long int)va_arg(*input, unsigned long int);
      s21_hl_to_str(str, ld_number, parameters);
      break;
    default:
      number = (unsigned int)va_arg(*input, unsigned int);
      s21_hl_to_str(str, number, parameters);
      break;
  }
  s21_make_string_precision(parameters, str);
  s21_make_string_flags(parameters, str);
  s21_make_string_width(parameters, str);
}

/* Адрес указателя */
void s21_spec_p(char *str, va_list *input, s21_FORMAT *parameters) {
  int width = parameters->width < 15 ? 15 : parameters->width;
  char *str_start = s21_NULL;
  str_start = str;
  int *p = va_arg(*input, int *);
  char buf[4] = "x0";
  if (p == s21_NULL) {
    s21_strcpy(buf, "0x0");
  }
  for (int *j = p, k = 0; j && k < width;
       j = (void *)(((s21_size_t)j) >> 4), k++) {
    unsigned int dig = ((s21_size_t)j) % 0x10;
    if (dig < 10)
      *(str_start++) = (char)('0' + dig);
    else
      *(str_start++) = (char)('a' + (dig - 10));
  }
  *str_start = '\0';
  s21_strcat(str, buf);
  s21_reverse_writing(str);
  s21_make_string_precision(parameters, str);
  s21_make_string_flags(parameters, str);
  s21_make_string_width(parameters, str);
}

/* Количество символов, напечатанных до появления %n */
void s21_spec_n(s21_size_t const *str_len, va_list *input) {
  int *p = va_arg(*input, int *);
  *p = (int)*str_len;
}

/* Символ % */
void s21_spec_percentage(char *str, s21_FORMAT *parameters) {
  s21_strcat(str, "%");
  s21_make_string_precision(parameters, str);
  s21_make_string_flags(parameters, str);
  s21_make_string_width(parameters, str);
}

// #define test
// #ifdef test
// int main() {
//   int h = 0;
//   int m = 0;

//   char str1[400];
//   char str2[400];
//   char *str3 = "test: %+ 0Lf!\ttest: %060Lf!\ttest: %.15Lf!";
//   long double num = -9325781235683689988.;
//   m = sprintf(str1, str3, num, num, num);
//   h = s21_sprintf(str2, str3, num, num, num);
//   printf("s21 === *%s* = %d=\n", str2, h);
//   printf("man === *%s* = %d=\n", str1, m);

//   return 0;
// }
// #endif
