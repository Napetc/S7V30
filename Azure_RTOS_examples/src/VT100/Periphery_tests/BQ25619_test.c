﻿// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 2023-01-27
// 10:36:28 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include   "S7V30.h"


/*-----------------------------------------------------------------------------------------------------


  \param void
-----------------------------------------------------------------------------------------------------*/
static void _Print_BQ25619_test_header(void)
{
  GET_MCBL;
  MPRINTF(VT100_CLEAR_AND_HOME);
  MPRINTF(" ===  BQ25619 test ===\n\r");
  MPRINTF("ESC - exit, [S] - run test\n\r");
  MPRINTF("----------------------------------------------------------------------\n\r");

}

/*-----------------------------------------------------------------------------------------------------


  \param src

  \return char const*
-----------------------------------------------------------------------------------------------------*/
static char const* _Conv_pwr_src_to_name(uint8_t src)
{
  src = src & 0x07;
  switch (src)
  {
  case BQ25619_PWR_SRC_ACCUM    :
    return "ACCUM   ";
  case BQ25619_PWR_SRC_USB_HOST :
    return "USB HOST";
  case BQ25619_PWR_SRC_ADAPTER  :
    return "ADAPTER ";
  case BQ25619_PWR_SRC_BOOST      :
    return "BOOST   ";
  }
  return "UNDEF   ";
}

/*-----------------------------------------------------------------------------------------------------


  \param void
-----------------------------------------------------------------------------------------------------*/
static uint32_t _BQ25619_Test(void)
{
  uint8_t   power_source = 0;
  uint8_t   b;
  ssp_err_t vres;
  GET_MCBL;
  if (BQ25619_open() != SSP_SUCCESS)
  {
    MPRINTF("Connection to BQ25619 error.\n\r");
    return RES_ERROR;
  }

  vres = BQ25619_read_reg(BQ25619_PART_INFORMATION,&b);
  if (vres != SSP_SUCCESS)
  {
    MPRINTF("Read BQ25619_PART_INFORMATION error.\n\r");
    goto EXIT_ON_ERROR;
  }

  if ((b & 0x7F) == 0x2C)
  {
    MPRINTF("Charger chip BQ25619 is identified.\n\r");
  }
  else
  {
    MPRINTF("Charger chip BQ25619 is not  identified %02X.\n\r", b);
    goto EXIT_ON_ERROR;
  }


  vres = BQ25619_read_reg(BQ25619_SYSTEM_STATUS0,&b);
  if (vres != SSP_SUCCESS)
  {
    MPRINTF("Read BQ25619_SYSTEM_STATUS0 error.\n\r");
    goto EXIT_ON_ERROR;
  }


  power_source =(b >> 5) & 0x07;
  MPRINTF("Power source identified as  %s.\n\r", _Conv_pwr_src_to_name(power_source));

    // Проверяем бит Power Good.
  if (b & BQ25619_PG_STAT)
  {
      // Питание от внешнего источника питания
    MPRINTF("The module is powered from an external source.\n\r");
  }
  else
  {
      // Питание от аккумулятора
    MPRINTF("The module is powered from an accumulator.\n\r");

  }



  BQ25619_close();
  return RES_OK;

EXIT_ON_ERROR:

  BQ25619_close();
  return RES_ERROR;
}

/*-----------------------------------------------------------------------------------------------------


  \param keycode
-----------------------------------------------------------------------------------------------------*/
void Do_BQ25619_test(uint8_t keycode)
{
  uint8_t             b;
  GET_MCBL;

  _Print_BQ25619_test_header();

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
          _BQ25619_Test();
          MPRINTF("\r\nPress any key to continue.\r\n");
          WAIT_CHAR(&b,  ms_to_ticks(100000));
          _Print_BQ25619_test_header();
          break;

        case VT100_ESC:
          return;

        default:
          _Print_BQ25619_test_header();
          break;
        }
      }
    }
  } while (1);


}


