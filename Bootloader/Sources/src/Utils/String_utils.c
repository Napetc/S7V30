﻿// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 2023-02-02
// 15:08:29 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include   "App.h"

const char *monts[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
const char pass_symbols[] = "@*#$&0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

/*-----------------------------------------------------------------------------------------------------


  \param password
  \param pass_len

  \return uint32_t
-----------------------------------------------------------------------------------------------------*/
uint32_t Validate_password_symbols(char *password, uint32_t pass_len)
{

  for (uint32_t i = 0; i < pass_len; i++)
  {
    if (strchr(pass_symbols, password[i]) == NULL) return RES_ERROR;
  }
  return RES_OK;
}

/*-----------------------------------------------------------------------------------------------------


  \param val

  \return uint8_t
-----------------------------------------------------------------------------------------------------*/
uint8_t BCD2ToBYTE(uint8_t val)
{
  uint32_t tmp;
  tmp = ((val & 0xF0) >> 4) * 10;
  return(uint8_t)(tmp + (val & 0x0F));
}

/*-----------------------------------------------------------------------------------------------------


  \param val

  \return uint8_t
-----------------------------------------------------------------------------------------------------*/
uint8_t BYTEToBCD2(uint8_t val)
{
  uint8_t b1 = val / 10;
  uint8_t b2 = val % 10;

  return (b1 << 4) + b2;
}


/*-----------------------------------------------------------------------------------------------------


  \param c

  \return uint8_t
-----------------------------------------------------------------------------------------------------*/
uint8_t ascii_to_hex(uint8_t c)
{
  if (c >= '0' && c <= '9')      return (c - '0') & 0x0f;
  else if (c >= 'a' && c <= 'f') return (c - 'a' + 10) & 0x0f;
  else                           return (c - 'A' + 10) & 0x0f;
}

/*-----------------------------------------------------------------------------------------------------


  \param c

  \return uint8_t
-----------------------------------------------------------------------------------------------------*/
uint8_t hex_to_ascii(uint8_t c)
{
  c = c & 0xf;
  if (c <= 9) return (c + 0x30);
  return (c + 'A' - 10);
}

/*-------------------------------------------------------------------------------------------------------------
   Отсечение пробелов спереди и сзади и снятие кавычек если есть
-------------------------------------------------------------------------------------------------------------*/
char* Trim_and_dequote_str(char *str)
{
  int i;

  while (*str == ' ')
  {
    str++;
    if (*str == 0) return str;
  }

  for (i = (strlen(str) - 1); i > 0; i--)
  {
    if (str[i] != ' ') break;
    str[i] = 0;
  }

  if ((str[0] == '"') && (str[strlen(str) - 1] == '"'))
  {
    str[strlen(str) - 1] = 0;
    str++;
  }
  return str;
}

/*-------------------------------------------------------------------------------------------------------------

  Читать в строку str данные из буфера buf до появления нуля или комбинацией "\r\n" или "\n\r", но не длинее len символов.
  Произвести смещение указателя на буфер buf
  Символы \r и \n из строки удаляються

  Возвращает -1 если в начале файла обнаружен байт  0
-------------------------------------------------------------------------------------------------------------*/
int Read_cstring_from_buf(char **buf, char *str, uint32_t len)
{
  int32_t  pos;
  char    c;
  char    *ptr;

  ptr = *buf;
  pos = 0;
  do
  {
    c = *ptr;      // Читаем символ из текущей позиции
    if (c == 0)
    {
      if (pos == 0)
      {
        *buf = ptr;
        return (-1);  // Возвращаем -1 если первый символ в первой прозиции = 0
      }
      // Удаляем из предыдущей позиции строки символы '\r' и '\n' чтобы их не осталось в строке
      if ((str[pos - 1] == '\r') || (str[pos - 1] == '\n'))
      {
        str[pos - 1] = 0;
      }
      *buf = ptr; // Возвращаем указатель на начале следующей подстроки
      return (len);
    }
    str[pos++] = c; // Записываем символ в строку
    ptr++;          // Смещаем текущую позицию в строке
    str[pos] = 0;   // Добавляем 0 чтобы строка всегда оканчивалась нулем
    if (pos > 1)    // Проверяем на наличие шаблоново окончания строки
    {
      if (strcmp(&str[pos - 2], "\r\n") == 0)
      {
        str[pos - 2] = 0;
        *buf = ptr;
        return (pos - 2);
      }
      if (strcmp(&str[pos - 2], "\n\r") == 0)
      {
        str[pos - 2] = 0;
        ptr--;
        *buf = ptr;
        return (pos - 2);
      }
    }
    if (pos >= len)
    {
      *buf = ptr;
      return (len);
    }
  }while (1);
}

/*-----------------------------------------------------------------------------------------------------
 Поиск строки в буфере заканчивающейся 0 или одним из символов "\r" и "\n"
 Символы "\r" и "\n" в буфере заменяются на 0

 \param **buf      Указатель на буфер. После вызова функции сдвигается к началу следующей строки
 \param *buf_len   Указатель на длину буфера. Если найдена строка указатель возвращает количество оставшихся необработанных данных

 \return char*     Указатель на начало строки оканчивающейся нулем

 Пример:
 Обработка такого буфера -  {0, '1' , '2' , '3' , '\r', '\n' , '4' , '5' , '6', '\r', '7' , '8' , '\n', 0 , 'q', 'w' , 0 , 0 , 0 , 'e', 'r'};
 Приведет к выводу следующих строк: "123" "456" "78" "qw" "e"
-----------------------------------------------------------------------------------------------------*/
uint8_t* Isolate_string_in_buf(uint8_t **buf, uint32_t *buf_len)
{
  uint8_t    *ptr;
  int32_t     pos;
  uint8_t     ch;
  int         cnt;
  int         len;

  len = *buf_len;
  if (len < 2) return 0;
  cnt = 0;
  ptr = *buf;
  pos = 0;
  do
  {
    ch = ptr[pos];      // Читаем символ из текущей позиции
    if (ch == 0)
    {
      if (cnt == 0)
      {
        // Первые нули пропускаем идя до конца буфера
        if (pos < (len - 2))
        {
          pos++;
        }
        else
        {
          return 0;
        }
      }
      else
      {
        *buf = &ptr[pos];
        *buf_len = len - pos;
        return &ptr[pos - cnt];
      }
    }
    else if ((ch == '\r') || (ch == '\n'))
    {
      ptr[pos] = 0;
    }
    else
    {
      if (pos < (len - 1))
      {
        cnt++;
        pos++;
      }
      else
      {
        ptr[pos] = 0;
        *buf = &ptr[pos];
        *buf_len = len - pos;
        return &ptr[pos - cnt];
      }
    }
  }while (1);
}


/*-----------------------------------------------------------------------------------------------------


  \param date_str
  \param pmonts
  \param pday
  \param pyear

  \return uint32_t
-----------------------------------------------------------------------------------------------------*/
void Get_nums_from_date(const char *date_str, uint32_t *pmonts, uint32_t *pday, uint32_t *pyear)
{
  char  month_str[8];

  sscanf(date_str, "%s %d %d", month_str, pday, pyear);
  for (uint32_t i = 0; i < 12; i++)
  {
    if (strncmp(monts[i],month_str,3) == 0)
    {
      *pmonts = i + 1;
      break;
    }
  }
}

/*-----------------------------------------------------------------------------------------------------


  \param time_str
  \param hours
  \param mins
  \param secs
-----------------------------------------------------------------------------------------------------*/
void Get_nums_from_time(const char *time_str, uint32_t *hours, uint32_t *mins, uint32_t *secs)
{
  sscanf(time_str, "%d:%d:%d", hours, mins, secs);
}

/*-----------------------------------------------------------------------------------------------------


  \param ver_str
-----------------------------------------------------------------------------------------------------*/
void Get_build_date_time(char *ver_str)
{
  uint32_t monts;
  uint32_t day;
  uint32_t year;
  uint32_t hours;
  uint32_t mins;
  uint32_t secs;

  Get_nums_from_date(Get_build_date(),&monts,&day,&year);
  Get_nums_from_time(Get_build_time(),&hours,&mins,&secs);
  sprintf(ver_str, "VER: %04d_%02d_%02d_%02d%02d", year, monts, day, hours, mins);
}


/*-----------------------------------------------------------------------------------------------------
  Поиск числа в потоке HTML текста заключенного между маркерами, заданными в управляющей структуре


  \param block      - входной блок данных
  \param block_size - размер фходного блока данных в байтах
  \param fcbl       - управляющая структура автомата

  \return uint32_t
-----------------------------------------------------------------------------------------------------*/
uint32_t Find_marked_number(uint8_t *block, uint32_t block_size, T_marked_str_finder *fnd)
{
  uint8_t b;

  if (fnd->step == 0)
  {
    fnd->cnt         = 0;
    fnd->fragment_sz = 0;
    fnd->step        = 1;
  }
  else if (fnd->step < 0)
  {
    return 0;
  }
  else if (fnd->step > 2)
  {
    return 1;
  }


  while (block_size > 0)
  {
    b = *block;
    switch (fnd->step)
    {
    case 1:
      // Ищем совпадение с левым текстовым маркером
      if (b == fnd->left_mark[fnd->cnt])
      {
        fnd->cnt++;
        if (fnd->cnt == fnd->left_mark_sz)
        {
          fnd->step++;
          fnd->cnt = 0;
        }
      }
      else
      {
        fnd->cnt = 0;
      }
      break;

    case 2:
      // Ищем совпадение с правым текстовым маркером
      if (b == fnd->right_mark[fnd->cnt])
      {
        fnd->cnt++;
        if (fnd->cnt == fnd->right_mark_sz)
        {
          fnd->step = 0;
          if ((fnd->fragment_sz - fnd->cnt) > 0)
          {
            fnd->fragment_sz = fnd->fragment_sz - fnd->cnt + 1;
            fnd->fragment[fnd->fragment_sz] = 0;
            fnd->cnt  = 0;
            fnd->step = 3;
            fnd->next_sym_ptr = block + 1;
            return 1;
          }
          fnd->cnt  = 0;
        }
      }
      else
      {
        fnd->cnt = 0;
      }
      // Накапливаем строку в которой может содержаться искомый фрагмент
      fnd->fragment[fnd->fragment_sz] = b;
      fnd->fragment_sz++;
      if ((fnd->fragment_sz - fnd->cnt) >= MAX_MARKED_STR_SIZE)
      {
        // Если строка достигла предельного размера, то отменяем ее и начинаем весь поиск заново.
        fnd->step = 0;
      }
      break;
    }
    block++;
    block_size--;
  }
  return 0;
}


/*-----------------------------------------------------------------------------------------------------


  \param str
  \param b
-----------------------------------------------------------------------------------------------------*/
void Get_8bit_str(char *str,  uint8_t b)
{
  uint8_t i;
  uint8_t mask = 0x80;
  uint8_t n = 0;
  for (i = 0; i < 4; i++)
  {
    if (b & mask)
    {
      str[n++] = '1';
    }
    else
    {
      str[n++] = '0';
    }
    mask >>= 1;
  }
  str[n++] = ' ';
  for (i = 0; i < 4; i++)
  {
    if (b & mask)
    {
      str[n++] = '1';
    }
    else
    {
      str[n++] = '0';
    }
    mask >>= 1;
  }
  str[n] = 0;
}

/*-----------------------------------------------------------------------------------------------------


  \param str
  \param b
-----------------------------------------------------------------------------------------------------*/
void Get_16bit_str(char *str,  uint16_t b)
{
  uint16_t mask = 0x8000;
  uint8_t  n = 0;
  for (uint8_t j = 0; j < 4; j++)
  {
    for (uint8_t i = 0; i < 4; i++)
    {
      if (b & mask)
      {
        str[n++] = '1';
      }
      else
      {
        str[n++] = '0';
      }
      mask >>= 1;
    }
    str[n++] = ' ';
  }
  str[n] = 0;
}

/*-----------------------------------------------------------------------------------------------------


  \param str
  \param arr

  \return char*
-----------------------------------------------------------------------------------------------------*/
char* Buf_to_hex_str(char *str, uint32_t len, uint8_t *arr)
{
  char *res = str;
  for (uint32_t i = 0; i < len; i++)
  {
    *str++ = hex_to_ascii(arr[i] >> 4);
    *str++ = hex_to_ascii(arr[i]);
    *str++ = ' ';
  }
  *str = 0;
  return res;
}

/*-----------------------------------------------------------------------------------------------------


  \param arr
  \param size

  \return uint8_t Возвращает 0 если в массиве все нули
-----------------------------------------------------------------------------------------------------*/
uint8_t Is_All_Zeros(uint8_t *arr, uint32_t size)
{
  for (uint32_t i = 0; i < size; i++)
  {
    if (arr[i] != 0)
    {
      return 1;
    }
  }
  return 0;
}


/*-----------------------------------------------------------------------------------------------------
  Функци либо печатает в буфер если указатель на буфер не нулевой,
  либо не печатает, но в параметре *offset передает количество печатаемых байт


  \param buffer  - указатель на буфер, куда производится печать
  \param offset  - укзатель на величину принимающую количество напечатанных байт
  \param fmt_ptr - строка форматирования

  \return uint32_t
-----------------------------------------------------------------------------------------------------*/
uint32_t Print_to(char *buffer, uint32_t offset, const char *fmt_ptr, ...)
{
  uint32_t n;
  va_list          ap;
  va_start(ap, fmt_ptr);
  if (buffer != NULL)
  {
    n = vsprintf(buffer + offset, (const char *)fmt_ptr, ap);
  }
  else
  {
    n = vsnprintf(NULL, 0, (const char *)fmt_ptr, ap);
  }
  va_end(ap);
  return n;
}


/*-----------------------------------------------------------------------------------------------------
  Функция удалющая начальные и конечные пробелы из строки
  Содержимое переностится из src в dst

  \param src
  \param dst
  \param dst_size
-----------------------------------------------------------------------------------------------------*/
void Trim_str(const char *src, char *dst, uint32_t dst_size)
{
  if (src == NULL || dst == NULL) return;

  // Пропускаем пробелы в начале строки
  while (isspace((unsigned char)*src)) src++;

  // Копируем строку после удаления начальных пробелов
  // Оставляем место для нуль-терминатора
  size_t to_copy = strlen(src);
  if (to_copy >= dst_size) to_copy = dst_size - 1;

  strncpy(dst, src, to_copy);
  dst[to_copy] = '\0';  // Убедимся, что строка надежно завершена

  // Удаляем пробелы в конце строки
  char *end = dst + strlen(dst) - 1;
  while (end > dst && isspace((unsigned char)*end))
  {
    *end = '\0';
    end--;
  }
}

