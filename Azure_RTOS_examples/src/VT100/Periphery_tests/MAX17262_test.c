﻿// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 2023-01-28
// 13:19:14 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include   "S7V30.h"


/*-----------------------------------------------------------------------------------------------------


  \param void
-----------------------------------------------------------------------------------------------------*/
static void   _Print_MAX17262_test_header(void)
{
  GET_MCBL;
  MPRINTF(VT100_CLEAR_AND_HOME);
  MPRINTF(" ===  MAX17262 test ===\n\r");
  MPRINTF("ESC - exit, [S] - run test\n\r");
  MPRINTF("----------------------------------------------------------------------\n\r");

}

/*-----------------------------------------------------------------------------------------------------


  \param void

  \return uint32_t
-----------------------------------------------------------------------------------------------------*/
static uint32_t _MAX17262_Test(void)
{
  uint16_t   w;

  GET_MCBL;


  if (MAX17262_open(0) != SSP_SUCCESS)
  {
    MPRINTF("Connection to MAX17262 error.\n\r");
    return RES_ERROR;
  }

  if (MAX17262_read_reg(MAX17262_DEVNAME,&w) != RES_OK)
  {
    MPRINTF("Read MAX17262_DEVNAME error.\n\r");
    goto EXIT_ON_ERROR;
  }
  MPRINTF("MAX17262_DEVNAME register = %04X. ", w);

  if (w == MAX17262_CHIP_ID)
  {
    MPRINTF(" Correct!\n\r");
  }
  else
  {
    MPRINTF(" Incorrect!\n\r");
  }



  MAX17262_close();
  return RES_OK;

EXIT_ON_ERROR:

  MAX17262_close();
  return RES_ERROR;
}


/*-----------------------------------------------------------------------------------------------------


  \param void
-----------------------------------------------------------------------------------------------------*/
void Do_MAX17262_test(uint8_t keycode)
{
  uint8_t             b;
  GET_MCBL;

  _Print_MAX17262_test_header();

  do
  {
    if (WAIT_CHAR(&b,  ms_to_ticks(100)) == RES_OK)
    {
      if (b > 0)
      {
        switch (b)
        {
        case 'S':
        case 's':
          _MAX17262_Test();
          MPRINTF("\r\nPress any key to continue.\r\n");
          WAIT_CHAR(&b,  ms_to_ticks(100000));
          _Print_MAX17262_test_header();
          break;

        case VT100_ESC:
          return;

        default:
          _Print_MAX17262_test_header();
          break;
        }
      }
    }
  } while (1);
}


