﻿// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 2023-01-28
// 13:19:14 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include   "App.h"


/*-----------------------------------------------------------------------------------------------------


  \param void
-----------------------------------------------------------------------------------------------------*/
static void   _Print_MAX17262_test_header(void)
{
  GET_MCBL;
  MPRINTF(VT100_CLEAR_AND_HOME);
  MPRINTF(" ===  MAX17262 test ===\n\r");
  MPRINTF("ESC - exit\n\r");
  MPRINTF("----------------------------------------------------------------------\n\r");

}

/*-----------------------------------------------------------------------------------------------------
  
  
  \param void  
-----------------------------------------------------------------------------------------------------*/
static void _Print_MAX17262_registers(void)
{
  GET_MCBL;
  uint32_t row;
  uint32_t col;
  T_MAX17262_reg_descr const *rds;
  uint16_t b;
  char     str[64];

  row = 4;
  col = 1;

  for (uint32_t i = 0; i < MAX17262_reg_count(); i++)
  {
    rds = MAX17262_get_reg_descr(i);
    MAX17262_read_reg(rds->reg_addr,rds->reg_val);

    b =*rds->reg_val;
    Get_16bit_str(str, b);

    if (((i % 31) == 0) && (i != 0))
    {
      row = 4;
      col += 60;
    }
    MPRINTF(VT100_CURSOR_SET,row,col);
    MPRINTF("[0x%02X] %s = 0x%04X ( %s )",rds->reg_addr, rds->reg_name, b, str);
    MPRINTF("\n\r");
    row++;
  }
}

/*-----------------------------------------------------------------------------------------------------


  \param void
-----------------------------------------------------------------------------------------------------*/
void Do_MAX17262_test(uint8_t keycode)
{
  uint8_t             b;
  GET_MCBL;

  if (Is_MAX17262_opened() == 0)
  {
    MPRINTF(VT100_CLEAR_AND_HOME);
    MPRINTF("MAX17262 is not opened. \n\r");
    Wait_ms(1000);
    return;
  }

  _Print_MAX17262_test_header();

  do
  {
    if (WAIT_CHAR(&b,  ms_to_ticks(100)) == RES_OK)
    {
      if (b > 0)
      {
        switch (b)
        {
        case VT100_ESC:
          return;

        default:
          _Print_MAX17262_test_header();
          break;
        }
      }
    }
    else
    {
      _Print_MAX17262_registers();
    }
  } while (1);
}


