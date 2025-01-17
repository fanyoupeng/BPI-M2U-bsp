/*
 * Copyright (C) 2013 Allwinnertech, kevin.z.m <kevin@allwinnertech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Adjustable factor-based clock implementation
 */
#ifndef __MACH_SUNXI_CLK_SUN8IW17_H
#define __MACH_SUNXI_CLK_SUN8IW17_H

#include <linux/clk-provider.h>
#include <linux/clkdev.h>
#include <linux/io.h>
#include "clk-factors.h"

/* CCMU Register List */
#define PLL_CPU0            0x0000
#define PLL_CPU1            0x0008
#define PLL_DDR0            0x0010
#define PLL_DDR1            0x0018
#define PLL_PERIPH0         0x0020
#define PLL_PERIPH1         0x0028
#define PLL_GPU             0x0030
#define PLL_VIDEO0          0x0040
#define PLL_VIDEO1          0x0048
#define PLL_VE              0x0058
#define PLL_DE              0x0060
#define PLL_ISP             0x0068
#define PLL_HSIC            0x0070
#define PLL_AUDIO           0x0078
#define PLL_MIPI            0x0080
#define PLL_EVE             0x00C0

#define PLL_CPU0PAT         0x0100
#define PLL_CPU1PAT         0x0108
#define PLL_DDR0PAT         0x0110
#define PLL_DDR1PAT         0x0118
#define PLL_PERI1PAT        0x0128
#define PLL_GPUPAT          0x0130
#define PLL_VIDEO0PAT       0x0140
#define PLL_VIDEO1PAT       0x0148
#define PLL_VIDEO2PAT       0x0150
#define PLL_VEPAT           0x0158
#define PLL_DEPAT           0x0160
#define PLL_ISPPAT          0x0168
#define PLL_HSICPAT         0x0170
#define PLL_AUDIOPAT        0x0178
#define PLL_MIPIPAT         0x0180
#define PLL_EVEPAT          0x01C0

#define PLL_DDR1_SSC_LIN    0x0218

#define CPU0_CFG            0x0500
#define CPU1_CFG            0x0504
#define PSI_CFG             0x0510
#define AHB3_CFG            0x051C
#define APB1_CFG            0x0520
#define APB2_CFG            0x0524
#define CCI400_CFG          0x0530
#define MBUS_CFG            0x0540

/* Accelerator */
#define DE_CFG              0x0600
#define DE_GATE             0x060C
#define DI_CFG              0x0620
#define DI_GATE             0x062C
#define G2D_CFG             0x0630
#define G2D_GATE            0x063C
#define EVE_CFG             0x0650
#define EVE_GATE            0x065C
#define GPU_CFG             0x0670
#define GPU_GATE            0x067C
#define CE_CFG              0x0680
#define CE_GATE             0x068C
#define VE_CFG              0x0690
#define VE_GATE             0x069C

/* SYS Resource */
#define DMA_GATE            0x070C
#define MSGBOX_GATE         0x071C
#define SPINLOCK_GATE       0x072C
#define HSTIMER_GATE        0x073C
#define AVS_CFG             0x0740
#define DBGSYS_GATE         0x078C
#define PSI_GATE            0x079C
#define PWM_GATE            0x07AC

/* Storage Medium */
#define DRAM_CFG            0x0800
#define MBUS_GATE           0x0804
#define DRAM_GATE           0x080C
#define NAND0_CFG           0x0810
#define NAND1_CFG           0x0814
#define NAND_GATE           0x082C
#define SMHC0_CFG           0x0830
#define SMHC1_CFG           0x0834
#define SMHC2_CFG           0x0838
#define SMHC3_CFG           0x083C
#define SMHC_GATE           0x084C

/* Common Interface */
#define UART_GATE           0x090C
#define TWI_GATE            0x091C
#define CAN_GATE            0x092C
#define SCR_GATE            0x093C
#define SPI0_CFG            0x0940
#define SPI1_CFG            0x0944
#define SPI_GATE            0x096C
#define GMAC_GATE           0x097C
#define TS_CFG              0x09B0
#define TS_GATE             0x09BC
#define GPADC_GATE          0x09EC
#define I2S0_CFG            0x0A10
#define I2S1_CFG            0x0A14
#define I2S2_CFG            0x0A18
#define I2S_GATE            0x0A1C
#define SPDIF_CFG           0x0A20
#define SPDIF_GATE          0x0A2C
#define DMIC_CFG            0x0A40
#define DMIC_GATE           0x0A4C
#define CODEC_1X_CFG        0x0A50
#define CODEC_4X_CFG        0x0A54
#define CODEC_GATE          0x0A5C
#define USB0_CFG            0x0A70
#define USB1_CFG            0x0A74
#define USB2_CFG            0x0A78
#define USB3_CFG            0x0A7C
#define USB_GATE            0x0A8C

/* Display Interface */
#define MIPI_DSI_HOST_CFG   0x0B24
#define MIPI_GATE           0x0B4C
#define DISPLAY_TOP_GATE    0x0B5C
#define TCON_LCD0_CFG       0x0B60
#define TCON_LCD1_CFG       0x0B64
#define TCON_LCD_GATE       0x0B7C
#define TCON_TV_GATE        0x0B9C
#define TVE_CFG             0x0BA0
#define TVE_GATE            0x0BAC
#define LVDS_GATE           0x0BB0
#define TVD0_CFG            0x0BC0
#define TVD1_CFG            0x0BC4
#define TVD2_CFG            0x0BC8
#define TVD3_CFG            0x0BCC
#define TVD_GATE            0x0BDC
#define CSI_MISC_CFG        0x0C00
#define CSI_TOP_CFG         0x0C04
#define CSI_MASTER0_CFG     0x0C08
#define CSI_MASTER1_CFG     0x0C0C
#define CSI_MASTER2_CFG     0x0C10
#define CSI_MASTER3_CFG     0x0C14
#define CSI_GATE            0x0C2C
#define MIPI_RX_CFG         0x0C30

/* others */
#define CCMU_SEC_SWITCH     0x0F00
#define PLL_LOCK_CTRL       0x0F04
#define FRE_DET_CTRL        0x0F08
#define FRE_UP_LIM_CTRL     0x0F0C
#define FRE_DOWN_LIM_CTRL   0x0F10
#define HOSC_CLK_OUTPUT     0x0F20
#define CCMU_VERSION        0x0FF0
#define SUNXI_CLK_MAX_REG   0x0FF0

/* PRCM Register List */
#define CPUS_CFG            0x0000
#define CPUS_APBS1_CFG      0x000C
#define CPUS_APBS2_CFG      0x0010
#define CPUS_TIMER_GATE     0x011C
#define CPUS_TWDOG_GATE     0x012C
#define CPUS_PWM_GATE       0x013C
#define CPUS_VM_CFG         0x0140
#define CPUS_VM_GATE        0x014C
#define CPUS_THS_GATE       0x015C
#define CPUS_LRADC_GATE     0x016C
#define CPUS_UART_GATE      0x018C
#define CPUS_TWI_GATE       0x019C
#define CPUS_CAN_GATE       0x01AC
#define CPUS_RSB_GATE       0x01BC
#define CPUS_CIR_CFG        0x01C0
#define CPUS_CIR_GATE       0x01CC
#define CPUS_SPI_CFG        0x01F0
#define CPUS_SPI_GATE       0x01FC
#define CPUS_RTC_GATE       0x020C
#define CPUS_PLL_CTRL0      0x0240
#define CPUS_PLL_CTRL1      0x0244
#define CPUS_CLK_MAX_REG    0x0244

/* RTC Register List */
#define LOSC_OUT_GATE       0x0060

#define F_N8X7_M0X4(nv, mv) FACTOR_ALL(nv, 8, 7, 0, 0, 0, mv, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0)
#define F_N8X5_K4X2(nv, kv) FACTOR_ALL(nv, 8, 5, kv, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
#define F_N8X7_M0X2(nv, mv) FACTOR_ALL(nv, 8, 7, 0, 0, 0, mv, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0)
#define F_N8X5_K4X2_M0X2(nv, kv, mv) FACTOR_ALL(nv, 8, 5, kv, 4, 2, mv, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0)
#define F_N8X5_K4X2_M0X2_P16x2(nv, kv, mv, pv) \
		    FACTOR_ALL(nv, 8, 5, \
			       kv, 4, 2, \
			       mv, 0, 2, \
			       pv, 16, 2, \
			       0, 0, 0, 0, 0, 0)
#define F_N8X7_M0X5_P16x4(nv, mv, pv) \
	       FACTOR_ALL(nv, 8, 7, 0, 0, 0, mv, 0, 5, \
			pv, 16, 4, 0, 0, 0, 0, 0, 0)

#define PLLCPU0(n, k, m, p, freq)   {F_N8X5_K4X2_M0X2_P16x2(n, k, m, p),  freq}
#define PLLCPU1(n, k, m, p, freq)   {F_N8X5_K4X2_M0X2_P16x2(n, k, m, p),  freq}
#define PLLDDR0(n, m, freq)       {F_N8X7_M0X2(n, m),  freq}
#define PLLDDR1(n, m, freq)       {F_N8X7_M0X2(n, m),  freq}
#define PLLPERIPH0(n, k, freq)    {F_N8X5_K4X2(n, k),  freq}
#define PLLPERIPH1(n, k, freq)    {F_N8X5_K4X2(n, k),  freq}
#define PLLGPU(n, m, freq)        {F_N8X7_M0X4(n, m),  freq}
#define PLLVIDEO0(n, m, freq)     {F_N8X7_M0X4(n, m),  freq}
#define PLLVIDEO1(n, m, freq)     {F_N8X7_M0X4(n, m),  freq}
#define PLLVE(n, m, freq)         {F_N8X7_M0X4(n, m),  freq}
#define PLLDE(n, m, freq)         {F_N8X7_M0X4(n, m),  freq}
#define PLLISP(n, m, freq)        {F_N8X7_M0X4(n, m),  freq}
#define PLLHSIC(n, m, freq)       {F_N8X7_M0X4(n, m),  freq}
#define PLLAUDIO(n, m, p, freq)    {F_N8X7_M0X5_P16x4(n, m, p),  freq}
#define PLLEVE(n, m, freq)        {F_N8X7_M0X4(n, m),  freq}

#endif
