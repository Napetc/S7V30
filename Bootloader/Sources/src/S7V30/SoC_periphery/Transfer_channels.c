﻿// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 2023-01-27
// 18:08:36 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include   "App.h"


//........................................................................................
//  Организация канала передачи для SD карты
//  Используется модуль DMAC канал 2
//........................................................................................
SSP_VECTOR_DEFINE_CHAN(dmac_int_isr,        DMAC, INT, 2);
SSP_VECTOR_DEFINE_CHAN(sdhimmc_accs_isr,    SDHIMMC, ACCS,    0);
//SSP_VECTOR_DEFINE_CHAN(sdhimmc_card_isr,    SDHIMMC, CARD,    0);
//SSP_VECTOR_DEFINE_CHAN(sdhimmc_dma_req_isr, SDHIMMC, DMA_REQ, 0);

dmac_instance_ctrl_t DMA_CH2_control_block;

transfer_info_t DMA_CH2_transfer_info =
{
  .dest_addr_mode      = TRANSFER_ADDR_MODE_FIXED,
  .repeat_area         = TRANSFER_REPEAT_AREA_SOURCE,
  .irq                 = TRANSFER_IRQ_EACH,
  .chain_mode          = TRANSFER_CHAIN_MODE_DISABLED,
  .src_addr_mode       = TRANSFER_ADDR_MODE_INCREMENTED,
  .size                = TRANSFER_SIZE_1_BYTE,
  .mode                = TRANSFER_MODE_NORMAL,
  .p_dest              = (void *) NULL,
  .p_src               = (void const *) NULL,
  .num_blocks          = 0,
  .length              = 0,
};

const transfer_on_dmac_cfg_t DMA_CH2_extended_configuration =
{
  .channel             = 2,
  .offset_byte         = 0,
};

const transfer_cfg_t DMA_CH2_configuration =
{
  .p_info              =&DMA_CH2_transfer_info,
  .activation_source   = ELC_EVENT_SDHIMMC0_DMA_REQ,
  .auto_enable         = false,
  .p_callback          = NULL,
  .p_context           =&DMA_CH2_transfer_instance,
  .irq_ipl             =(1),
  .p_extend            =&DMA_CH2_extended_configuration,
};

const transfer_instance_t DMA_CH2_transfer_instance =
{
  .p_ctrl        =&DMA_CH2_control_block,
  .p_cfg         =&DMA_CH2_configuration,
  .p_api         =&g_transfer_on_dmac
};

//........................................................................................
//  Организация канала приема данных по SPI5
//  Используется модуль DTC
//........................................................................................

dtc_instance_ctrl_t DTC_SPI5_RX_control_block;

transfer_info_t DTC_SPI5_RX_transfer_info =
{
  .dest_addr_mode      = TRANSFER_ADDR_MODE_INCREMENTED,
  .repeat_area         = TRANSFER_REPEAT_AREA_DESTINATION,
  .irq                 = TRANSFER_IRQ_END,
  .chain_mode          = TRANSFER_CHAIN_MODE_DISABLED,
  .src_addr_mode       = TRANSFER_ADDR_MODE_FIXED,
  .size                = TRANSFER_SIZE_1_BYTE,
  .mode                = TRANSFER_MODE_NORMAL,
  .p_dest              = (void *) NULL,
  .p_src               = (void const *) NULL,
  .num_blocks          = 0,
  .length              = 0,
};

const transfer_cfg_t DTC_SPI5_RX_configuration =
{
  .p_info              =&DTC_SPI5_RX_transfer_info,
  .activation_source   = ELC_EVENT_SCI5_RXI,
  .auto_enable         = false,
  .p_callback          = NULL,
  .p_context           =&DTC_SPI5_RX_transfer_instance,
  .irq_ipl             =(BSP_IRQ_DISABLED)
};

const transfer_instance_t DTC_SPI5_RX_transfer_instance =
{
  .p_ctrl        =&DTC_SPI5_RX_control_block,
  .p_cfg         =&DTC_SPI5_RX_configuration,
  .p_api         =&g_transfer_on_dtc
};


//........................................................................................
//  Организация канала передачи данных по SPI5
//  Используется модуль DTC
//........................................................................................

dtc_instance_ctrl_t DTC_SPI5_TX_control_block;

transfer_info_t DTC_SPI5_TX_transfer_info =
{
  .dest_addr_mode      = TRANSFER_ADDR_MODE_FIXED,
  .repeat_area         = TRANSFER_REPEAT_AREA_SOURCE,
  .irq                 = TRANSFER_IRQ_END,
  .chain_mode          = TRANSFER_CHAIN_MODE_DISABLED,
  .src_addr_mode       = TRANSFER_ADDR_MODE_INCREMENTED,
  .size                = TRANSFER_SIZE_1_BYTE,
  .mode                = TRANSFER_MODE_NORMAL,
  .p_dest              = (void *) NULL,
  .p_src               = (void const *) NULL,
  .num_blocks          = 0,
  .length              = 0,
};

const transfer_cfg_t DTC_SPI5_TX_configuration =
{
  .p_info              =&DTC_SPI5_TX_transfer_info,
  .activation_source   = ELC_EVENT_SCI5_TXI,
  .auto_enable         = false,
  .p_callback          = NULL,
  .p_context           =&DTC_SPI5_TX_transfer_instance,
  .irq_ipl             =(BSP_IRQ_DISABLED)
};


const transfer_instance_t DTC_SPI5_TX_transfer_instance =
{
  .p_ctrl        =&DTC_SPI5_TX_control_block,
  .p_cfg         =&DTC_SPI5_TX_configuration,
  .p_api         =&g_transfer_on_dtc
};


//........................................................................................
//  Организация канала передачи на шине SPI5
//........................................................................................

SSP_VECTOR_DEFINE_CHAN(sci_spi_rxi_isr, SCI, RXI, 5);
SSP_VECTOR_DEFINE_CHAN(sci_spi_txi_isr, SCI, TXI, 5);
SSP_VECTOR_DEFINE_CHAN(sci_spi_tei_isr, SCI, TEI, 5);
SSP_VECTOR_DEFINE_CHAN(sci_spi_eri_isr, SCI, ERI, 5);


sci_spi_instance_ctrl_t SPI5_control_block;

const sci_spi_extended_cfg SPI5_extended_configuration =
{
  .bitrate_modulation  = true
};

const spi_cfg_t SPI5_configuration =
{
  .channel             = 5,
  .operating_mode      = SPI_MODE_MASTER,
  .clk_phase           = SPI_CLK_PHASE_EDGE_ODD,
  .clk_polarity        = SPI_CLK_POLARITY_LOW,
  .mode_fault          = SPI_MODE_FAULT_ERROR_DISABLE,
  .bit_order           = SPI_BIT_ORDER_MSB_FIRST,
  .bitrate             = 1000000,
  .p_transfer_tx       =&DTC_SPI5_TX_transfer_instance,
  .p_transfer_rx       =&DTC_SPI5_RX_transfer_instance,
  .p_callback          = NULL,
  .p_context           = &SPI5_interface_instance,
  .rxi_ipl             = 12,   // Receive interrupt priority
  .txi_ipl             = 12,   // Transmit interrupt priority
  .tei_ipl             = 12,   // Transmit end interrupt priority
  .eri_ipl             = 12,   // Error interrupt priority
  .p_extend            =&SPI5_extended_configuration,
};

const spi_instance_t SPI5_interface_instance =
{
  .p_ctrl        =&SPI5_control_block,
  .p_cfg         =&SPI5_configuration,
  .p_api         =&g_spi_on_sci
};

//........................................................................................
//  Организация шины для коммуникации со многими дивайсами на шине SPI5
//........................................................................................

static TX_MUTEX             SPI5_bus_mutex;
static TX_EVENT_FLAGS_GROUP SPI5_bus_events;
static sf_spi_ctrl_t       *current_device_on_SPI5_bus_pointer;

sf_spi_bus_t SPI5_bus =
{
  .p_bus_name       = (uint8_t *)"SPI5_bus",
  .channel          = 5,
  .freq_hz_min      = 0,
  .p_lock_mutex     = &SPI5_bus_mutex,
  .p_sync_eventflag = &SPI5_bus_events,
  .pp_curr_ctrl     = (sf_spi_ctrl_t **)&current_device_on_SPI5_bus_pointer,
  .p_lower_lvl_api  = (spi_api_t *)&g_spi_on_sci,
  .device_count     = 0,
};

//........................................................................................
//  Организация канала приема данных по I2C2
//  Используется модуль DTC
//........................................................................................
dtc_instance_ctrl_t DTC_I2C2_RX_control_block;

transfer_info_t DTC_I2C2_RX_transfer_info =
{
  .dest_addr_mode      = TRANSFER_ADDR_MODE_INCREMENTED,
  .repeat_area         = TRANSFER_REPEAT_AREA_DESTINATION,
  .irq                 = TRANSFER_IRQ_END,
  .chain_mode          = TRANSFER_CHAIN_MODE_DISABLED,
  .src_addr_mode       = TRANSFER_ADDR_MODE_FIXED,
  .size                = TRANSFER_SIZE_1_BYTE,
  .mode                = TRANSFER_MODE_NORMAL,
  .p_dest              = (void *) NULL,
  .p_src               = (void const *) NULL,
  .num_blocks          = 0,
  .length              = 0,
};

const transfer_cfg_t DTC_I2C2_RX_configuration =
{
  .p_info              =&DTC_I2C2_RX_transfer_info,
  .activation_source   = ELC_EVENT_IIC2_RXI,
  .auto_enable         = false,
  .p_callback          = NULL,
  .p_context           =&DTC_I2C2_RX_transfer_instance,
  .irq_ipl             =(BSP_IRQ_DISABLED)  // Interrupt priority level
};

const transfer_instance_t DTC_I2C2_RX_transfer_instance =
{
  .p_ctrl        =&DTC_I2C2_RX_control_block,
  .p_cfg         =&DTC_I2C2_RX_configuration,
  .p_api         =&g_transfer_on_dtc
};



//........................................................................................
//  Организация канала передачи данных по I2C2
//  Используется модуль DTC
//........................................................................................

dtc_instance_ctrl_t DTC_I2C2_TX_control_block;

transfer_info_t DTC_I2C2_TX_transfer_info =
{
  .dest_addr_mode      = TRANSFER_ADDR_MODE_FIXED,
  .repeat_area         = TRANSFER_REPEAT_AREA_SOURCE,
  .irq                 = TRANSFER_IRQ_END,
  .chain_mode          = TRANSFER_CHAIN_MODE_DISABLED,
  .src_addr_mode       = TRANSFER_ADDR_MODE_INCREMENTED,
  .size                = TRANSFER_SIZE_1_BYTE,
  .mode                = TRANSFER_MODE_NORMAL,
  .p_dest              = (void *) NULL,
  .p_src               = (void const *) NULL,
  .num_blocks          = 0,
  .length              = 0,
};

const transfer_cfg_t DTC_I2C2_TX_configuration =
{
  .p_info              =&DTC_I2C2_TX_transfer_info,
  .activation_source   = ELC_EVENT_IIC2_TXI,
  .auto_enable         = false,
  .p_callback          = NULL,
  .p_context           =&DTC_I2C2_TX_transfer_instance,
  .irq_ipl             =(BSP_IRQ_DISABLED)
};

const transfer_instance_t DTC_I2C2_TX_transfer_instance =
{
  .p_ctrl        =&DTC_I2C2_TX_control_block,
  .p_cfg         =&DTC_I2C2_TX_configuration,
  .p_api         =&g_transfer_on_dtc
};

//........................................................................................
//  Организация приема-передачи по I2C2
//........................................................................................

SSP_VECTOR_DEFINE_CHAN(iic_rxi_isr, IIC, RXI, 2);
SSP_VECTOR_DEFINE_CHAN(iic_txi_isr, IIC, TXI, 2);
SSP_VECTOR_DEFINE_CHAN(iic_tei_isr, IIC, TEI, 2);
SSP_VECTOR_DEFINE_CHAN(iic_eri_isr, IIC, ERI, 2);

riic_instance_ctrl_t    I2C2_control_block;

const riic_extended_cfg I2C2_extended_configuration =
{
  .timeout_mode = RIIC_TIMEOUT_MODE_SHORT,
};

const i2c_cfg_t I2C2_configuration =
{
  .channel             = 2,
  .rate                = I2C_RATE_STANDARD,
  .slave               = 0,
  .addr_mode           = I2C_ADDR_MODE_7BIT,
  .sda_delay           = 300,
  .p_transfer_tx       =&DTC_I2C2_TX_transfer_instance,
  .p_transfer_rx       =&DTC_I2C2_RX_transfer_instance,
  .p_callback          = NULL,
  .p_context           = (void *)&I2C2_interface_instance,
  .rxi_ipl             =(12),  // Receive interrupt priority
  .txi_ipl             =(12),  // Transmit interrupt priority
  .tei_ipl             =(12),  // Transmit end interrupt priority
  .eri_ipl             =(12),  // Error interrupt priority
  .p_extend            =&I2C2_extended_configuration,
};

const i2c_master_instance_t I2C2_interface_instance =
{
  .p_ctrl        =&I2C2_control_block,
  .p_cfg         =&I2C2_configuration,
  .p_api         =&g_i2c_master_on_riic
};

//........................................................................................
//  Организация шины для коммуникации со многими дивайсами на шине I2C2
//........................................................................................

static TX_MUTEX                     i2c2_bus_mutex;
static TX_EVENT_FLAGS_GROUP         i2c2_bus_events;
static sf_i2c_instance_ctrl_t      *current_device_on_I2C2_bus_pointer;
static sf_i2c_instance_ctrl_t      *sf_curr_bus_ctrl_g_sf_i2c_bus2;

sf_i2c_bus_t I2C2_bus =
{
  .p_bus_name       = (uint8_t *)"I2C2_bus",
  .channel          = 2,
  .p_lock_mutex     = &i2c2_bus_mutex,
  .p_sync_eventflag = &i2c2_bus_events,
  .pp_curr_ctrl     = (sf_i2c_ctrl_t **)&current_device_on_I2C2_bus_pointer,
  .p_lower_lvl_api  = (i2c_api_master_t *)&g_i2c_master_on_riic,
  .device_count     = 0,
  .pp_curr_bus_ctrl = (sf_i2c_ctrl_t **)&sf_curr_bus_ctrl_g_sf_i2c_bus2,
};

//........................................................................................
//  Организация коммуникации с чипом bq25619
//........................................................................................
const i2c_cfg_t g_sf_i2c_bq25619_i2c_cfg =
{
  .channel             = 2,
  .rate                = I2C_RATE_STANDARD,
  .slave               = 0x6A,
  .addr_mode           = I2C_ADDR_MODE_7BIT,
  .sda_delay           = (600),
  .p_transfer_tx       = &DTC_I2C2_TX_transfer_instance,
  .p_transfer_rx       = &DTC_I2C2_RX_transfer_instance,
  .p_callback          = (NULL),
  .p_context           = &I2C2_interface_instance,
  .rxi_ipl             = 12,
  .txi_ipl             = 12,
  .tei_ipl             = 12,
  .eri_ipl             = 12,
  .p_extend            = &I2C2_extended_configuration,
};

sf_i2c_instance_ctrl_t g_sf_i2c_bq25619_ctrl =
{
  .p_lower_lvl_ctrl =&I2C2_control_block,
};

const sf_i2c_cfg_t g_sf_i2c_bq25619_cfg =
{
  .p_bus                    = (sf_i2c_bus_t *)&I2C2_bus,
  .p_lower_lvl_cfg          =&g_sf_i2c_bq25619_i2c_cfg,
};


/* Instance structure to use this module. */
const sf_i2c_instance_t g_sf_i2c_bq25619 =
{
  .p_ctrl        =&g_sf_i2c_bq25619_ctrl,
  .p_cfg         =&g_sf_i2c_bq25619_cfg,
  .p_api         =&g_sf_i2c_on_sf_i2c
};

//........................................................................................
//  Организация коммуникации с чипом max17262
//........................................................................................
const i2c_cfg_t g_sf_i2c_max17262_i2c_cfg =
{
  .channel             = 2,
  .rate                = I2C_RATE_STANDARD,
  .slave               = 0x36,
  .addr_mode           = I2C_ADDR_MODE_7BIT,
  .sda_delay           = (600),
  .p_transfer_tx       = &DTC_I2C2_TX_transfer_instance,
  .p_transfer_rx       = &DTC_I2C2_RX_transfer_instance,
  .p_callback          = (NULL),
  .p_context           = &I2C2_interface_instance,
  .rxi_ipl             = 12,
  .txi_ipl             = 12,
  .tei_ipl             = 12,
  .eri_ipl             = 12,
  .p_extend            = &I2C2_extended_configuration,
};

sf_i2c_instance_ctrl_t g_sf_i2c_max17262_ctrl =
{
  .p_lower_lvl_ctrl =&I2C2_control_block,
};
const sf_i2c_cfg_t g_sf_i2c_max17262_cfg =
{
  .p_bus                    = (sf_i2c_bus_t *)&I2C2_bus,
  .p_lower_lvl_cfg          =&g_sf_i2c_max17262_i2c_cfg,
};

const sf_i2c_instance_t g_sf_i2c_max17262 =
{
  .p_ctrl        =&g_sf_i2c_max17262_ctrl,
  .p_cfg         =&g_sf_i2c_max17262_cfg,
  .p_api         =&g_sf_i2c_on_sf_i2c
};

