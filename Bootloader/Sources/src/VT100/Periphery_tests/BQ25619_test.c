﻿// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 2023-01-27
// 10:36:28 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include   "App.h"


/*-----------------------------------------------------------------------------------------------------


  \param void
-----------------------------------------------------------------------------------------------------*/
static void _Print_BQ25619_test_header(void)
{
  GET_MCBL;
  MPRINTF(VT100_CLEAR_AND_HOME);
  MPRINTF(" ===  BQ25619 test ===\n\r");
  MPRINTF("ESC - exit, [E] - edit register (format: <HEX_ADDR> <HEX_VAL>),  [P] - power off\n\r");
  MPRINTF("----------------------------------------------------------------------\n\r");
}

/*-----------------------------------------------------------------------------------------------------


  \param void
-----------------------------------------------------------------------------------------------------*/
static void _Print_BQ25619_registers(void)
{
  GET_MCBL;
  uint32_t row = 4;
  uint32_t col = 1;
  T_bq25619_reg_descr const *rds;
  uint8_t  b;
  char     str[16];

  MPRINTF(VT100_CURSOR_SET,row,col);

  for (uint32_t i = 0; i < BQ25619_reg_count(); i++)
  {
    rds = BQ25619_get_reg_descr(i);
    BQ25619_read_reg(rds->reg_addr,rds->reg_val);

    b = *rds->reg_val;
    Get_8bit_str(str, b);
    MPRINTF(VT100_CLR_LINE"[0x%02X] %s = 0x%02X ( %s )",rds->reg_addr, rds->reg_name, b, str);

    switch (rds->reg_addr)
    {
    case BQ25619_INPUT_CURRENT_LIMIT:
      {
        MPRINTF(" Input I Lim = %0.0f mA |", (double)Get_BQ25619_ilim_val(b));
        MPRINTF(" %s|",Get_BQ25619_NTC_monitoring_str(b));
        MPRINTF(" %s ",Get_BQ25619_HiZ_state_str(b));
      }
      break;
    case BQ25619_CHARGER_CONTROL_0:
      {
        MPRINTF(" %s |", Get_BQ25619_PFM_state_str(b));
        MPRINTF(" %s |", Get_BQ25619_WDT_state_str(b));
        MPRINTF(" %s |", Get_BQ25619_boost_state_str(b));
        MPRINTF(" %s |", Get_BQ25619_charge_state_str(b));
        MPRINTF(" sytem min.voltage = %0.1f V", (double)Get_BQ25619_sysv_val(b));
      }
      break;
    case BQ25619_CHARGE_CURRENT_LIMIT:
      {
        MPRINTF(" Charge current limit = %0.0f mA |", (double)Get_BQ25619_charge_curr_lim_val(b));
        MPRINTF(" %s ",Get_BQ25619_Q1_FULLON_str(b));
      }
      break;
    case BQ25619_PRE_CHARGE_TERMINATION_CURRENT_LIMIT:
      {
        MPRINTF(" Precharge current limit = %0.0f mA |", (double)Get_BQ25619_precharge_curr_lim_val(b));
        MPRINTF(" Charge termination current limit = %0.0f mA", (double)Get_BQ25619_charge_term_curr_lim_val(b));
      }
      break;
    case BQ25619_BATTERY_VOLTAGE_LIMIT:
      {
        MPRINTF(" Battery voltage limit = %0.3f V |", (double)Get_BQ25619_batt_volt_lim_val(b));
        MPRINTF(" %s |", Get_BQ25619_TOPOFF_TIMER_str(b));
        MPRINTF(" %s ", Get_BQ25619_VRECHG_str(b));
      }
      break;
    case BQ25619_CHARGER_CONTROL_1:
      {
        MPRINTF(" %s |", Get_BQ25619_EN_TERM_str(b));
        MPRINTF(" %s |", Get_BQ25619_WATCHDOG_str(b));
        MPRINTF(" %s |", Get_BQ25619_EN_TIMER_str(b));
        MPRINTF(" %s |", Get_BQ25619_CHG_TIMER_str(b));
        MPRINTF(" %s |", Get_BQ25619_TREG_str(b));
        MPRINTF(" %s ", Get_BQ25619_JEITA_VSET_str(b));
      }
      break;
    case BQ25619_CHARGER_CONTROL_2:
      {
        MPRINTF(" %s |", Get_BQ25619_OVP_str(b));
        MPRINTF(" %s |", Get_BQ25619_BOOSTV_str(b));
        MPRINTF(" Input voltage threshold = %0.1f V", (double)Get_BQ25619_VINDPM_val(b));
      }
      break;
    case BQ25619_CHARGER_CONTROL_3:
      {
        MPRINTF(" %s |", Get_BQ25619_IINDET_EN_str(b));
        MPRINTF(" %s |", Get_BQ25619_TMR2X_EN_str(b));
        MPRINTF(" %s |", Get_BQ25619_BATFET_DIS_str(b));
        MPRINTF(" %s |", Get_BQ25619_BATFET_RST_WVBUS_str(b));
        MPRINTF(" %s |", Get_BQ25619_BATFET_DLY_str(b));
        MPRINTF(" %s |", Get_BQ25619_BATFET_RST_EN_str(b));
        MPRINTF(" %s  ", Get_BQ25619_VINDPM_BAT_TRACK_str(b));
      }
      break;
    case BQ25619_SYSTEM_STATUS0:
      {
        MPRINTF(" %s |", Get_BQ25619_VBUS_status_str(b));
        MPRINTF(" %s |", Get_BQ25619_charging_status_str(b));
        MPRINTF(" %s |", Get_BQ25619_pgood_state_str(b));
        MPRINTF(" %s |", Get_BQ25619_therm_state_str(b));
        MPRINTF(" %s ", Get_BQ25619_vsys_state_str(b));
      }
      break;
    case BQ25619_SYSTEM_STATUS1:
      {
        MPRINTF(" %s |", Get_BQ25619_wdt_fault_str(b));
        MPRINTF(" %s |", Get_BQ25619_boost_fault_str(b));
        MPRINTF(" %s |", Get_BQ25619_charge_fault_str(b));
        MPRINTF(" %s |", Get_BQ25619_batt_fault_str(b));
        MPRINTF(" %s ", Get_BQ25619_NTC_fault_str(b));
      }
      break;
    case BQ25619_SYSTEM_STATUS2:
      {
        MPRINTF(" %s |", Get_BQ25619_VBUS_GD_str(b));
        MPRINTF(" %s |", Get_BQ25619_VINDPM_STAT_str(b));
        MPRINTF(" %s |", Get_BQ25619_IINDPM_STAT_str(b));
        MPRINTF(" %s |", Get_BQ25619_BATSNS_STAT_str(b));
        MPRINTF(" %s |", Get_BQ25619_TOPOFF_ACTIVE_str(b));
        MPRINTF(" %s |", Get_BQ25619_ACOV_STAT_str(b));
        MPRINTF(" %s |", Get_BQ25619_VINDPM_INT_MASK_str(b));
        MPRINTF(" %s ", Get_BQ25619_IINDPM_INT_MASK_str(b));
      }
      break;
    }

    MPRINTF("\n\r");
  }



}

/*-----------------------------------------------------------------------------------------------------


  \param keycode
-----------------------------------------------------------------------------------------------------*/
void Do_BQ25619_test(uint8_t keycode)
{
  uint8_t             b;
  char                str[16];
  GET_MCBL;

  if (Is_BQ25619_opened() == 0)
  {
    MPRINTF(VT100_CLEAR_AND_HOME);
    MPRINTF("BQ25619 is not opened. \n\r");
    Wait_ms(1000);
    return;
  }

  // Останавливаем работу задачи зарядника
  Charger_task_suspend();
  Wait_ms(100);

  _Print_BQ25619_test_header();

  do
  {
    if (WAIT_CHAR(&b,  ms_to_ticks(100)) == RES_OK)
    {
      if (b > 0)
      {
        switch (b)
        {
        case 'P':
        case 'p':
          BQ25619_switch_off_vbus();
          _Print_BQ25619_test_header();
          break;

        case 'E':
        case 'e':
          if (VT100_edit_string_in_pos(str, sizeof(str), 18, 0)==RES_OK)
          {
            uint32_t reg_addr;
            uint32_t reg_val;
            if (sscanf(str, "%02X %02X",&reg_addr,&reg_val) == 2)
            {
              if (BQ25619_write_reg(reg_addr,reg_val)!=SSP_SUCCESS)
              {
                MPRINTF("\n\r Write error!");
                Wait_ms(1000);
              }
            }
            else
            {
              MPRINTF("\n\r String error!");
              Wait_ms(1000);
            }
          }
          _Print_BQ25619_test_header();
          break;

        case VT100_ESC:
          Charger_task_resume();
          return;

        default:
          _Print_BQ25619_test_header();
          break;
        }
      }
    }
    else
    {
      _Print_BQ25619_registers();
    }
  } while (1);


}


