/*
 * include/linux/arisc/arisc.h
 *
 * Copyright 2012 (c) Allwinner.
 * sunny (sunny@allwinnertech.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 */

#ifndef	__ASM_ARCH_ARISC_H
#define	__ASM_ARCH_ARISC_H

#include <linux/power/aw_pm.h>

/* the base of messages */
#define ARISC_MESSAGE_BASE          (0x10)

/* standby commands */
#define ARISC_SSTANDBY_ENTER_REQ        (ARISC_MESSAGE_BASE + 0x00)  /* request to enter       (ac327 to arisc) */
#define ARISC_SSTANDBY_RESTORE_NOTIFY   (ARISC_MESSAGE_BASE + 0x01)  /* restore finished       (ac327 to arisc) */
#define ARISC_NSTANDBY_ENTER_REQ        (ARISC_MESSAGE_BASE + 0x02)  /* request to enter       (ac327 to arisc) */
#define ARISC_NSTANDBY_WAKEUP_NOTIFY    (ARISC_MESSAGE_BASE + 0x03)  /* wakeup notify          (arisc to ac327) */
#define ARISC_NSTANDBY_RESTORE_REQ      (ARISC_MESSAGE_BASE + 0x04)  /* request to restore     (ac327 to arisc) */
#define ARISC_NSTANDBY_RESTORE_COMPLETE (ARISC_MESSAGE_BASE + 0x05)  /* arisc restore complete (arisc to ac327) */
#define ARISC_ESSTANDBY_ENTER_REQ       (ARISC_MESSAGE_BASE + 0x06)  /* request to enter       (ac327 to arisc) */
#define ARISC_TSTANDBY_ENTER_REQ        (ARISC_MESSAGE_BASE + 0x07)  /* request to enter       (ac327 to arisc) */
#define ARISC_TSTANDBY_RESTORE_NOTIFY   (ARISC_MESSAGE_BASE + 0x08)  /* restore finished       (ac327 to arisc) */
#define ARISC_FAKE_POWER_OFF_REQ        (ARISC_MESSAGE_BASE + 0x09)  /* request to enter       (ac327 to arisc) */
#define ARISC_CPUIDLE_ENTER_REQ         (ARISC_MESSAGE_BASE + 0x0a)  /* request to enter       (ac327 to arisc) */
#define ARISC_STANDBY_INFO_REQ          (ARISC_MESSAGE_BASE + 0x10)  /* request sst info       (ac327 to arisc) */
#define ARISC_CPUIDLE_CFG_REQ           (ARISC_MESSAGE_BASE + 0x11)  /* request to config      (ac327 to arisc) */
#define ARISC_CPU_OP_REQ                (ARISC_MESSAGE_BASE + 0x12)  /* cpu operations         (ac327 to arisc) */
#define ARISC_QUERY_WAKEUP_SRC_REQ      (ARISC_MESSAGE_BASE + 0x13)  /* query wakeup source    (ac327 to arisc) */
#define ARISC_SYS_OP_REQ                (ARISC_MESSAGE_BASE + 0x14)  /* system operations      (ac327 to arisc) */
#define ARISC_CLEAR_WAKEUP_SRC_REQ      (ARISC_MESSAGE_BASE + 0x15)  /* query wakeup source    (ac327 to arisc) */

/* dvfs commands */
#define ARISC_CPUX_DVFS_REQ              (ARISC_MESSAGE_BASE + 0x20)  /* request dvfs           (ac327 to arisc) */
#define ARISC_CPUX_DVFS_CFG_VF_REQ       (ARISC_MESSAGE_BASE + 0x21)  /* request config dvfs v-f table(ac327 to arisc) */

/* pmu commands */
#define ARISC_AXP_INT_COMING_NOTIFY      (ARISC_MESSAGE_BASE + 0x40)  /* interrupt coming notify(arisc to ac327) */
#define ARISC_AXP_DISABLE_IRQ            (ARISC_MESSAGE_BASE + 0x41)  /* disable axp irq of arisc                */
#define ARISC_AXP_ENABLE_IRQ             (ARISC_MESSAGE_BASE + 0x42)  /* enable axp irq of arisc                 */
#define ARISC_AXP_GET_CHIP_ID            (ARISC_MESSAGE_BASE + 0x43)  /* axp get chip id                         */
#define ARISC_AXP_SET_PARAS              (ARISC_MESSAGE_BASE + 0x44)  /* config axp parameters (ac327 to arisc)  */
#define ARISC_SET_PMU_VOLT               (ARISC_MESSAGE_BASE + 0x45)  /* set pmu volt (ac327 to arisc)           */
#define ARISC_GET_PMU_VOLT               (ARISC_MESSAGE_BASE + 0x46)  /* get pmu volt (ac327 to arisc)           */
#define ARISC_SET_LED_BLN                (ARISC_MESSAGE_BASE + 0x47)  /* set led bln (ac327 to arisc)            */
#define ARISC_AXP_REBOOT                 (ARISC_MESSAGE_BASE + 0x48)  /* reboot system for no pmu protocols      */
#define ARISC_SET_PWR_TREE               (ARISC_MESSAGE_BASE + 0x49)  /* set power tree (ac327 to arisc)         */
#define ARISC_CLR_NMI_STATUS             (ARISC_MESSAGE_BASE + 0x4a)  /* clear nmi status (ac327 to arisc)       */
#define ARISC_SET_NMI_TRIGGER            (ARISC_MESSAGE_BASE + 0x4b)  /* set nmi trigger (ac327 to arisc)        */
#define ARISC_SET_PMU_VOLT_STA           (ARISC_MESSAGE_BASE + 0x4c)  /* set pmu volt state(ac327 to arisc)      */
#define ARISC_GET_PMU_VOLT_STA           (ARISC_MESSAGE_BASE + 0x4d)  /* get pmu volt state(ac327 to arisc)      */

/* set arisc debug commands */
#define ARISC_SET_DEBUG_LEVEL            (ARISC_MESSAGE_BASE + 0x50)  /* set arisc debug level  (ac327 to arisc)     */
#define ARISC_MESSAGE_LOOPBACK           (ARISC_MESSAGE_BASE + 0x51)  /* loopback message  (ac327 to arisc)          */
#define ARISC_SET_UART_BAUDRATE          (ARISC_MESSAGE_BASE + 0x52)  /* set uart baudrate (ac327 to arisc)          */
#define ARISC_SET_DRAM_PARAS             (ARISC_MESSAGE_BASE + 0x53)  /* config dram parameter (ac327 to arisc)      */
#define ARISC_SET_DEBUG_DRAM_CRC_PARAS   (ARISC_MESSAGE_BASE + 0x54)  /* config dram crc parameters (ac327 to arisc) */
#define ARISC_SET_IR_PARAS               (ARISC_MESSAGE_BASE + 0x55)  /* config ir parameter (ac327 to arisc)        */
#define ARISC_REPORT_ERR_INFO            (ARISC_MESSAGE_BASE + 0x56)  /* report arisc error info (arisc to ac327)    */

/* audio commands */
#define ARISC_AUDIO_START                (ARISC_MESSAGE_BASE + 0x60)  /* audio start play/capture(ac327 to arisc) */
#define ARISC_AUDIO_STOP                 (ARISC_MESSAGE_BASE + 0x61)  /* audio stop  play/capture(ac327 to arisc) */
#define ARISC_AUDIO_SET_BUF_PER_PARAS    (ARISC_MESSAGE_BASE + 0x62)  /* set audio buffer and peroid paras(ac327 to arisc) */
#define ARISC_AUDIO_GET_POSITION         (ARISC_MESSAGE_BASE + 0x63)  /* get audio buffer position(ac327 to arisc) */
#define ARISC_AUDIO_SET_TDM_PARAS        (ARISC_MESSAGE_BASE + 0x64)  /* set audio tdm parameters(ac327 to arisc) */
#define ARISC_AUDIO_PERDONE_NOTIFY       (ARISC_MESSAGE_BASE + 0x65)  /* audio period done notify(arisc to ac327) */
#define ARISC_AUDIO_ADD_PERIOD           (ARISC_MESSAGE_BASE + 0x66)  /* audio period done notify(arisc to ac327) */

/* rsb commands */
#define ARISC_RSB_READ_BLOCK_DATA        (ARISC_MESSAGE_BASE + 0x70)  /* rsb read block data        (ac327 to arisc) */
#define ARISC_RSB_WRITE_BLOCK_DATA       (ARISC_MESSAGE_BASE + 0x71)  /* rsb write block data       (ac327 to arisc) */
#define ARISC_RSB_BITS_OPS_SYNC          (ARISC_MESSAGE_BASE + 0x72)  /* rsb clear bits sync        (ac327 to arisc) */
#define ARISC_RSB_SET_INTERFACE_MODE     (ARISC_MESSAGE_BASE + 0x73)  /* rsb set interface mode     (ac327 to arisc) */
#define ARISC_RSB_SET_RTSADDR            (ARISC_MESSAGE_BASE + 0x74)  /* rsb set runtime slave addr (ac327 to arisc) */
#define ARISC_TWI_READ_BLOCK_DATA        (ARISC_MESSAGE_BASE + 0x70)  /* twi read block data        (ac327 to arisc) */
#define ARISC_TWI_WRITE_BLOCK_DATA       (ARISC_MESSAGE_BASE + 0x71)  /* twi write block data       (ac327 to arisc) */
#define ARISC_TWI_BITS_OPS_SYNC          (ARISC_MESSAGE_BASE + 0x72)  /* twi clear bits sync        (ac327 to arisc) */

/* arisc initialize state notify commands */
#define ARISC_STARTUP_NOTIFY             (ARISC_MESSAGE_BASE + 0x80)  /* arisc init state notify(arisc to ac327) */
#define ARISC_CRASHDUMP                  (ARISC_MESSAGE_BASE + 0x85)

/* sensorhub commands */
#define ARISC_AP_READ_DATA               (ARISC_MESSAGE_BASE + 0x90)  /* ap read data        (ac327 to arisc) */
#define ARISC_AP_WRITE_DATA              (ARISC_MESSAGE_BASE + 0x91)  /* ap write data       (ac327 to arisc) */
#define ARISC_SH_READ_DATA               (ARISC_MESSAGE_BASE + 0x92)  /* sh read data        (arisc to ac327) */
#define ARISC_SH_WRITE_DATA              (ARISC_MESSAGE_BASE + 0x93)  /* sh write data       (arisc to ac327) */
#define ARISC_SET_MSGBOX_RX_INT          (ARISC_MESSAGE_BASE + 0x94)  /* set msgbox rx int  (ac327 to arisc) */
#define ARISC_GET_MSGBOX_RX_PEND         (ARISC_MESSAGE_BASE + 0x95)  /* get msgbox rx pend  (ac327 to arisc) */
#define ARISC_CLR_MSGBOX_RX_PEND         (ARISC_MESSAGE_BASE + 0x96)  /* clr msgbox rx pend  (ac327 to arisc) */
#define ARISC_AP_WAKEUP_SH               (ARISC_MESSAGE_BASE + 0x97)  /* ap wakeup sh  (ac327 to arisc) */

#ifdef CONFIG_ARM
/* the base of ARM SVC ARISC */
#define ARM_SVC_ARISC_BASE          (0x80000000)
#endif

#ifdef CONFIG_ARM64
/* the base of ARM SVC ARISC */
#define ARM_SVC_ARISC_BASE          (0xc0000000)
#endif

/* standby commands */
#define ARM_SVC_ARISC_SSTANDBY_ENTER_REQ        (ARM_SVC_ARISC_BASE + ARISC_SSTANDBY_ENTER_REQ)        /* request to enter       (ac327 to arisc) */
#define ARM_SVC_ARISC_SSTANDBY_RESTORE_NOTIFY   (ARM_SVC_ARISC_BASE + ARISC_SSTANDBY_RESTORE_NOTIFY)   /* restore finished       (ac327 to arisc) */
#define ARM_SVC_ARISC_NSTANDBY_ENTER_REQ        (ARM_SVC_ARISC_BASE + ARISC_NSTANDBY_ENTER_REQ)        /* request to enter       (ac327 to arisc) */
#define ARM_SVC_ARISC_NSTANDBY_WAKEUP_NOTIFY    (ARM_SVC_ARISC_BASE + ARISC_NSTANDBY_WAKEUP_NOTIFY)    /* wakeup notify          (arisc to ac327) */
#define ARM_SVC_ARISC_NSTANDBY_RESTORE_REQ      (ARM_SVC_ARISC_BASE + ARISC_NSTANDBY_RESTORE_REQ)      /* request to restore     (ac327 to arisc) */
#define ARM_SVC_ARISC_NSTANDBY_RESTORE_COMPLETE (ARM_SVC_ARISC_BASE + ARISC_NSTANDBY_RESTORE_COMPLETE) /* arisc restore complete (arisc to ac327) */
#define ARM_SVC_ARISC_ESSTANDBY_ENTER_REQ       (ARM_SVC_ARISC_BASE + ARISC_ESSTANDBY_ENTER_REQ)       /* request to enter       (ac327 to arisc) */
#define ARM_SVC_ARISC_TSTANDBY_ENTER_REQ        (ARM_SVC_ARISC_BASE + ARISC_TSTANDBY_ENTER_REQ)        /* request to enter       (ac327 to arisc) */
#define ARM_SVC_ARISC_TSTANDBY_RESTORE_NOTIFY   (ARM_SVC_ARISC_BASE + ARISC_TSTANDBY_RESTORE_NOTIFY)   /* restore finished       (ac327 to arisc) */
#define ARM_SVC_ARISC_FAKE_POWER_OFF_REQ        (ARM_SVC_ARISC_BASE + ARISC_FAKE_POWER_OFF_REQ)        /* request to enter       (ac327 to arisc) */
#define ARM_SVC_ARISC_CPUIDLE_ENTER_REQ         (ARM_SVC_ARISC_BASE + ARISC_CPUIDLE_ENTER_REQ)         /* request to enter       (ac327 to arisc) */
#define ARM_SVC_ARISC_STANDBY_INFO_REQ          (ARM_SVC_ARISC_BASE + ARISC_STANDBY_INFO_REQ)          /* request sst info       (ac327 to arisc) */
#define ARM_SVC_ARISC_CPUIDLE_CFG_REQ           (ARM_SVC_ARISC_BASE + ARISC_CPUIDLE_CFG_REQ)           /* request to config      (ac327 to arisc) */
#define ARM_SVC_ARISC_CPU_OP_REQ                (ARM_SVC_ARISC_BASE + ARISC_CPU_OP_REQ)                /* cpu operations         (ac327 to arisc) */
#define ARM_SVC_ARISC_QUERY_WAKEUP_SRC_REQ      (ARM_SVC_ARISC_BASE + ARISC_QUERY_WAKEUP_SRC_REQ)      /* query wakeup source    (ac327 to arisc) */
#define ARM_SVC_ARISC_CLEAR_WAKEUP_SRC_REQ      (ARM_SVC_ARISC_BASE + ARISC_CLEAR_WAKEUP_SRC_REQ)      /* query wakeup source    (ac327 to arisc) */
#define ARM_SVC_ARISC_SYS_OP_REQ                (ARM_SVC_ARISC_BASE + ARISC_SYS_OP_REQ)                /* system operations      (ac327 to arisc) */

/* dvfs commands */
#define ARM_SVC_ARISC_CPUX_DVFS_REQ              (ARM_SVC_ARISC_BASE + ARISC_CPUX_DVFS_REQ)            /* request dvfs           (ac327 to arisc) */
#define ARM_SVC_ARISC_CPUX_DVFS_CFG_VF_REQ       (ARM_SVC_ARISC_BASE + ARISC_CPUX_DVFS_CFG_VF_REQ)     /* request config dvfs v-f table(ac327 to arisc) */

/* pmu commands */
#define ARM_SVC_ARISC_AXP_INT_COMING_NOTIFY      (ARM_SVC_ARISC_BASE + ARISC_AXP_INT_COMING_NOTIFY)    /* interrupt coming notify(arisc to ac327) */
#define ARM_SVC_ARISC_AXP_DISABLE_IRQ            (ARM_SVC_ARISC_BASE + ARISC_AXP_DISABLE_IRQ)          /* disable axp irq of arisc                */
#define ARM_SVC_ARISC_AXP_ENABLE_IRQ             (ARM_SVC_ARISC_BASE + ARISC_AXP_ENABLE_IRQ)           /* enable axp irq of arisc                 */
#define ARM_SVC_ARISC_AXP_GET_CHIP_ID            (ARM_SVC_ARISC_BASE + ARISC_AXP_GET_CHIP_ID)          /* axp get chip id                         */
#define ARM_SVC_ARISC_AXP_SET_PARAS              (ARM_SVC_ARISC_BASE + ARISC_AXP_SET_PARAS)            /* config axp parameters (ac327 to arisc)  */
#define ARM_SVC_ARISC_SET_PMU_VOLT               (ARM_SVC_ARISC_BASE + ARISC_SET_PMU_VOLT)             /* set pmu volt (ac327 to arisc)           */
#define ARM_SVC_ARISC_GET_PMU_VOLT               (ARM_SVC_ARISC_BASE + ARISC_GET_PMU_VOLT)             /* get pmu volt (ac327 to arisc)           */
#define ARM_SVC_ARISC_SET_LED_BLN                (ARM_SVC_ARISC_BASE + ARISC_SET_LED_BLN)              /* set led bln (ac327 to arisc)            */
#define ARM_SVC_ARISC_AXP_REBOOT                 (ARM_SVC_ARISC_BASE + ARISC_AXP_REBOOT)               /* reboot system for no pmu protocols      */
#define ARM_SVC_ARISC_SET_PWR_TREE               (ARM_SVC_ARISC_BASE + ARISC_SET_PWR_TREE)             /* set power tree (ac327 to arisc)         */
#define ARM_SVC_ARISC_CLR_NMI_STATUS             (ARM_SVC_ARISC_BASE + ARISC_CLR_NMI_STATUS)           /* clear nmi status (ac327 to arisc)       */
#define ARM_SVC_ARISC_SET_NMI_TRIGGER            (ARM_SVC_ARISC_BASE + ARISC_SET_NMI_TRIGGER)          /* set nmi trigger (ac327 to arisc)        */
#define ARM_SVC_ARISC_SET_PMU_VOLT_STA           (ARM_SVC_ARISC_BASE + ARISC_SET_PMU_VOLT_STA)         /* set pmu volt state(ac327 to arisc)      */
#define ARM_SVC_ARISC_GET_PMU_VOLT_STA           (ARM_SVC_ARISC_BASE + ARISC_GET_PMU_VOLT_STA)          /* get pmu volt state(ac327 to arisc)     */

/* set arisc debug commands */
#define ARM_SVC_ARISC_SET_DEBUG_LEVEL            (ARM_SVC_ARISC_BASE + ARISC_SET_DEBUG_LEVEL)          /* set arisc debug level  (ac327 to arisc)     */
#define ARM_SVC_ARISC_MESSAGE_LOOPBACK           (ARM_SVC_ARISC_BASE + ARISC_MESSAGE_LOOPBACK)         /* loopback message  (ac327 to arisc)          */
#define ARM_SVC_ARISC_SET_UART_BAUDRATE          (ARM_SVC_ARISC_BASE + ARISC_SET_UART_BAUDRATE)        /* set uart baudrate (ac327 to arisc)          */
#define ARM_SVC_ARISC_SET_DRAM_PARAS             (ARM_SVC_ARISC_BASE + ARISC_SET_DRAM_PARAS)           /* config dram parameter (ac327 to arisc)      */
#define ARM_SVC_ARISC_SET_DEBUG_DRAM_CRC_PARAS   (ARM_SVC_ARISC_BASE + ARISC_SET_DEBUG_DRAM_CRC_PARAS) /* config dram crc parameters (ac327 to arisc) */
#define ARM_SVC_ARISC_SET_IR_PARAS               (ARM_SVC_ARISC_BASE + ARISC_SET_IR_PARAS)             /* config ir parameter (ac327 to arisc)        */
#define ARM_SVC_ARISC_REPORT_ERR_INFO            (ARM_SVC_ARISC_BASE + ARISC_REPORT_ERR_INFO)          /* report arisc error info (arisc to ac327)    */

/* audio commands */
#define ARM_SVC_ARISC_AUDIO_START                (ARM_SVC_ARISC_BASE + ARISC_AUDIO_START)              /* audio start play/capture(ac327 to arisc) */
#define ARM_SVC_ARISC_AUDIO_STOP                 (ARM_SVC_ARISC_BASE + ARISC_AUDIO_STOP)               /* audio stop  play/capture(ac327 to arisc) */
#define ARM_SVC_ARISC_AUDIO_SET_BUF_PER_PARAS    (ARM_SVC_ARISC_BASE + ARISC_AUDIO_SET_BUF_PER_PARAS)  /* set audio buffer and peroid paras(ac327 to arisc) */
#define ARM_SVC_ARISC_AUDIO_GET_POSITION         (ARM_SVC_ARISC_BASE + ARISC_AUDIO_GET_POSITION)       /* get audio buffer position(ac327 to arisc) */
#define ARM_SVC_ARISC_AUDIO_SET_TDM_PARAS        (ARM_SVC_ARISC_BASE + ARISC_AUDIO_SET_TDM_PARAS)      /* set audio tdm parameters(ac327 to arisc) */
#define ARM_SVC_ARISC_AUDIO_PERDONE_NOTIFY       (ARM_SVC_ARISC_BASE + ARISC_AUDIO_PERDONE_NOTIFY)     /* audio period done notify(arisc to ac327) */
#define ARM_SVC_ARISC_AUDIO_ADD_PERIOD           (ARM_SVC_ARISC_BASE + ARISC_AUDIO_ADD_PERIOD)         /* audio period done notify(arisc to ac327) */

/* rsb commands */
#define ARM_SVC_ARISC_RSB_READ_BLOCK_DATA        (ARM_SVC_ARISC_BASE + ARISC_RSB_READ_BLOCK_DATA)      /* rsb read block data        (ac327 to arisc) */
#define ARM_SVC_ARISC_RSB_WRITE_BLOCK_DATA       (ARM_SVC_ARISC_BASE + ARISC_RSB_WRITE_BLOCK_DATA)     /* rsb write block data       (ac327 to arisc) */
#define ARM_SVC_ARISC_RSB_BITS_OPS_SYNC          (ARM_SVC_ARISC_BASE + ARISC_RSB_BITS_OPS_SYNC)        /* rsb clear bits sync        (ac327 to arisc) */
#define ARM_SVC_ARISC_RSB_SET_INTERFACE_MODE     (ARM_SVC_ARISC_BASE + ARISC_RSB_SET_INTERFACE_MODE)   /* rsb set interface mode     (ac327 to arisc) */
#define ARM_SVC_ARISC_RSB_SET_RTSADDR            (ARM_SVC_ARISC_BASE + ARISC_RSB_SET_RTSADDR)          /* rsb set runtime slave addr (ac327 to arisc) */
#define ARM_SVC_ARISC_TWI_READ_BLOCK_DATA        (ARM_SVC_ARISC_BASE + ARISC_TWI_READ_BLOCK_DATA)      /* rsb read block data        (ac327 to arisc) */
#define ARM_SVC_ARISC_TWI_WRITE_BLOCK_DATA       (ARM_SVC_ARISC_BASE + ARISC_TWI_WRITE_BLOCK_DATA)     /* rsb write block data       (ac327 to arisc) */
#define ARM_SVC_ARISC_TWI_BITS_OPS_SYNC          (ARM_SVC_ARISC_BASE + ARISC_TWI_BITS_OPS_SYNC)        /* rsb clear bits sync        (ac327 to arisc) */

/* arisc initialize state notify commands */
#define ARM_SVC_ARISC_STARTUP_NOTIFY             (ARM_SVC_ARISC_BASE + ARISC_STARTUP_NOTIFY)           /* arisc init state notify(arisc to ac327) */
#define ARM_SVC_ARISC_CRASHDUMP_START            (ARM_SVC_ARISC_BASE + ARISC_CRASHDUMP)

/* sensorhub commands */
#define ARM_SVC_ARISC_AP_READ_DATA               (ARM_SVC_ARISC_BASE + ARISC_AP_READ_DATA)             /* ap read data        (ac327 to arisc) */
#define ARM_SVC_ARISC_AP_WRITE_DATA              (ARM_SVC_ARISC_BASE + ARISC_AP_WRITE_DATA)            /* ap write data       (ac327 to arisc) */
#define ARM_SVC_ARISC_SH_READ_DATA               (ARM_SVC_ARISC_BASE + ARISC_SH_READ_DATA)             /* sh read data        (arisc to ac327) */
#define ARM_SVC_ARISC_SH_WRITE_DATA              (ARM_SVC_ARISC_BASE + ARISC_SH_WRITE_DATA)            /* sh write data       (arisc to ac327) */
#define ARM_SVC_ARISC_SET_MSGBOX_RX_INT          (ARM_SVC_ARISC_BASE + ARISC_SET_MSGBOX_RX_INT)        /* set msgbox rx int (ac327 to arisc) */
#define ARM_SVC_ARISC_GET_MSGBOX_RX_PEND         (ARM_SVC_ARISC_BASE + ARISC_GET_MSGBOX_RX_PEND)       /* get msgbox rx pend (ac327 to arisc) */
#define ARM_SVC_ARISC_CLR_MSGBOX_RX_PEND         (ARM_SVC_ARISC_BASE + ARISC_CLR_MSGBOX_RX_PEND)       /* clr msgbox rx pend (ac327 to arisc) */
#define ARM_SVC_ARISC_AP_WAKEUP_SH               (ARM_SVC_ARISC_BASE + ARISC_AP_WAKEUP_SH)             /* ap wakeup sh (ac327 to arisc) */

#define NMI_INT_TYPE_PMU (0)
#define NMI_INT_TYPE_RTC (1)
#define NMI_INT_TYPE_PMU_OFFSET (0x1 << NMI_INT_TYPE_PMU)
#define NMI_INT_TYPE_RTC_OFFSET (0x1 << NMI_INT_TYPE_RTC)

/* the modes of arisc dvfs */
#define	ARISC_DVFS_SYN		(1<<0)

/* message attributes(only use 8bit) */
#define	ARISC_MESSAGE_ATTR_ASYN		    (0<<0)	/* need asyn with another cpu     */
#define	ARISC_MESSAGE_ATTR_SOFTSYN	    (1<<0)	/* need soft syn with another cpu */
#define	ARISC_MESSAGE_ATTR_HARDSYN	    (1<<1)	/* need hard syn with another cpu */

/* axp driver interfaces */
#define AXP_TRANS_BYTE_MAX	(4)
#define RSB_TRANS_BYTE_MAX	(4)
#define P2WI_TRANS_BYTE_MAX	(8)
#define TWI_TRANS_BYTE_MAX	(8)

/* RSB devices' address */
#define RSB_DEVICE_SADDR1   	(0x3A3) /* (0x01d1)AXP22x(AW1669) */
#define RSB_DEVICE_SADDR3  		(0x745) /* (0x03a2)AXP15x(AW1657) */
#define RSB_DEVICE_SADDR7  		(0xE89) /* (0x0744)Audio codec, AC100 */

/* RSB run time address */
#define RSB_RTSADDR_AXP809  (0x2d)
#define RSB_RTSADDR_AXP806  (0x3a)
#define RSB_RTSADDR_AC100   (0x4e)

/* audio sram base address */
#define AUDIO_SRAM_BASE_PALY            (0x08117000)
#define AUDIO_SRAM_BASE_CAPTURE         (0x0811f000)

#define AUDIO_SRAM_BUF_SIZE_02K  (2048)     /* buffer size 2k  = 0x800  = 2048  */
#define AUDIO_SRAM_BUF_SIZE_04K  (4096)     /* buffer size 4k  = 0x1000 = 4096  */
#define AUDIO_SRAM_BUF_SIZE_08K  (8192)     /* buffer size 8k  = 0x2000 = 8192  */
#define AUDIO_SRAM_BUF_SIZE_16K (16384)     /* buffer size 16k = 0x4000 = 16384 */
#define AUDIO_SRAM_BUF_SIZE_32K (32768)     /* buffer size 32k = 0x8000 = 32768 */

#define AUDIO_SRAM_PER_SIZE_02K  (2048)     /* period size 2k  = 0x800  = 2048  */
#define AUDIO_SRAM_PER_SIZE_04K  (4096)     /* period size 4k  = 0x1000 = 4096  */
#define AUDIO_SRAM_PER_SIZE_08K  (8192)     /* period size 8k  = 0x2000 = 8192  */
#define AUDIO_SRAM_PER_SIZE_16K (16384)     /* period size 16k = 0x4000 = 16384 */
#define AUDIO_SRAM_PER_SIZE_32K (32768)     /* period size 32k = 0x8000 = 32768 */

//pmu voltage types
typedef enum power_voltage_type
{
	AW1657_POWER_DCDCA = 0x0,
	AW1657_POWER_DCDCB,
	AW1657_POWER_DCDCC,
	AW1657_POWER_DCDCD,
	AW1657_POWER_DCDCE,
	AW1657_POWER_ALDO1,
	AW1657_POWER_ALDO2,
	AW1657_POWER_ALDO3,
	AW1657_POWER_BLDO1,
	AW1657_POWER_BLDO2,
	AW1657_POWER_BLDO3,
	AW1657_POWER_BLDO4,
	AW1657_POWER_CLDO1,
	AW1657_POWER_CLDO2,
	AW1657_POWER_CLDO3,
	AW1657_POWER_DC1SW,
	AW1657_POWER_MAX,
	DUMMY_REGULATOR1, /* AVCC/VCC3V3-PLL/VCC3V3-TV */
	DUMMY_REGULATOR2, /* DRAM */
	DUMMY_REGULATOR3, /* SYSTEM */
	DUMMY_REGULATOR4, /* VCC-CPUX */
	DUMMY_REGULATOR5, /* WIFI */
	DUMMY_REGULATOR6, /* VCC-IO */
	DUMMY_REGULATOR_MAX,
} power_voltage_type_e;

/* the pll of arisc dvfs */
typedef enum arisc_pll_no {
	ARISC_DVFS_PLL1 = 1,
	ARISC_DVFS_PLL2 = 2
} arisc_pll_no_e;

/* rsb transfer data type */
typedef enum arisc_rsb_datatype {
	RSB_DATA_TYPE_BYTE  = 1,
	RSB_DATA_TYPE_HWORD = 2,
	RSB_DATA_TYPE_WORD  = 4
} arisc_rsb_datatype_e;

/* rsb transfer data type */
typedef enum arisc_rsb_bits_ops {
	RSB_CLR_BITS,
	RSB_SET_BITS
} arisc_rsb_bits_ops_e;

typedef enum arisc_audio_mode {
	AUDIO_PLAY,                   /* play    mode */
	AUDIO_CAPTURE                 /* capture mode */
} arisc_audio_mode_e;

typedef struct arisc_audio_mem {
	unsigned int mode;
	unsigned int sram_base_addr;
	unsigned int buffer_size;
	unsigned int period_size;
} arisc_audio_mem_t;


typedef struct arisc_audio_tdm {
	unsigned int mode;
	unsigned int samplerate;
	unsigned int channum;
} arisc_audio_tdm_t;

/* arisc call-back */
typedef int (*arisc_cb_t)(void *arg);

/* sunxi_perdone_cbfn
 *
 * period done callback routine type
*/
/* audio callback struct */
typedef struct audio_cb {
	arisc_cb_t	handler;	/* dma callback fuction */
	void 		*arg;		/* args of func         */
} audio_cb_t;

/*
 * @len :       number of read registers, max len:4;
 * @datatype:   type of the data, 0:byte(8bits), 1:halfword(16bits), 2:word(32bits)
 * @msgattr:    message attribute, 0:async, 1:soft sync, 2:hard aync
 * @devaddr:    devices address;
 * @regaddr:    array of registers address;
 * @data:       array of registers data;
 */
typedef struct arisc_rsb_block_cfg
{
	unsigned int len;
	unsigned int datatype;
	unsigned int msgattr;
	unsigned int devaddr;
	unsigned char *regaddr;
	unsigned int *data;
} arisc_rsb_block_cfg_t;

/*
 * @len  :       number of operate registers, max len:4;
 * @datatype:    type of the data, 0:byte(8bits), 1:halfword(16bits), 2:word(32bits)
 * @msgattr:     message attribute, 0:async, 1:soft sync, 2:hard aync
 * @ops:         bits operation, 0:clear bits, 1:set bits
 * @devaddr :    devices address;
 * @regaddr :    point of registers address;
 * @mask :       point of mask bits data;
 * @delay:       point of delay times;
 */
typedef struct arisc_rsb_bits_cfg {
	unsigned int len;
	unsigned int datatype;
	unsigned int msgattr;
	unsigned int ops;
	unsigned int devaddr;
	unsigned char *regaddr;
	unsigned char *delay;
	unsigned int *mask;
} arisc_rsb_bits_cfg_t;

typedef enum arisc_rw_type {
	ARISC_READ = 0x0,
	ARISC_WRITE = 0x1,
} arisc_rw_type_e;

typedef struct nmi_isr {
	arisc_cb_t   handler;
	void        *arg;
} nmi_isr_t;

extern nmi_isr_t nmi_isr_node[2];

/*
 * @flags: 0x01-clean pendings, 0x10-enter cupidle.
 * @resume_addr: resume address for cpu0 out of idle.
 */
typedef struct sunxi_enter_idle_para {
	unsigned long flags;
	void *resume_addr;
} sunxi_enter_idle_para_t;


/*
 * @len :       number of read registers, max len:8;
 * @msgattr:    message attribute, 0:async, 1:soft sync, 2:hard aync
 * @addr:       point of registers address;
 * @data:       point of registers data;
 */
typedef struct arisc_twi_block_cfg {
	unsigned int len;
	unsigned int msgattr;
	unsigned char *addr;
	unsigned char *data;
} arisc_twi_block_cfg_t;

/*
 * @len  :       number of operate registers, max len:8;
 * @msgattr:     message attribute, 0:async, 1:soft sync, 2:hard aync
 * @ops:         bits operation, 0:clear bits, 1:set bits
 * @addr :       point of registers address;
 * @mask :       point of mask bits data;
 * @delay:       point of delay times;
 */
typedef struct arisc_twi_bits_cfg {
	unsigned int len;
	unsigned int msgattr;
	unsigned int ops;
	unsigned char *addr;
	unsigned char *mask;
	unsigned char *delay;
} arisc_twi_bits_cfg_t;

/* ====================================dvfs interface==================================== */
/*
 * set specific pll target frequency.
 * @freq:    target frequency to be set, based on KHZ;
 * @pll:     which pll will be set
 * @mode:    the attribute of message, whether syn or asyn;
 * @cb:      callback handler;
 * @cb_arg:  callback handler arguments;
 *
 * return: result, 0 - set frequency successed,
 *                !0 - set frequency failed;
 */
int arisc_dvfs_set_cpufreq(unsigned int freq, unsigned int pll, unsigned int mode, arisc_cb_t cb, void *cb_arg);
int arisc_dvfs_cfg_vf_table(unsigned int cluster, unsigned int vf_num,
				void *vf_tbl);

/**
 * query super-standby wakeup source.
 * @para:  point of buffer to store wakeup event informations.
 *
 * return: result, 0 - query successed, !0 - query failed;
 */
int arisc_query_wakeup_source(u32 *event);

int arisc_standby_super(struct super_standby_para *para, arisc_cb_t cb, void *cb_arg);

int arisc_cpux_ready_notify(void);
int arisc_clear_wakeup_source(void);

extern int arisc_query_set_standby_info(struct standby_info_para *para, arisc_rw_type_e op);

/*
 * query config of standby power state and consumer.
 * @para:  point of buffer to store power informations.
 *
 * return: result, 0 - query successed, !0 - query failed;
 */
extern int arisc_query_standby_power_cfg(struct standby_info_para *para);

/*
 * set config of standby power state and consumer.
 * @para:  point of buffer to store power informations.
 *
 * return: result, 0 - set successed, !0 - set failed;
 */
#define arisc_set_standby_power_cfg(para) \
	arisc_query_set_standby_info(para, ARISC_WRITE)

/*
 * query standby power state and consumer.
 * @para:  point of buffer to store power informations.
 *
 * return: result, 0 - query successed, !0 - query failed;
 */
#define arisc_query_standby_power(para) \
	arisc_query_set_standby_info(para, ARISC_READ)

/**
 * query super-standby dram crc result.
 * @perror:  pointer of dram crc result.
 * @ptotal_count: pointer of dram crc total count
 * @perror_count: pointer of dram crc error count
 *
 * return: result, 0 - query successed,
 *                !0 - query failed;
 */
int arisc_query_dram_crc_result(unsigned long *perror, unsigned long *ptotal_count,
	unsigned long *perror_count);

int arisc_set_dram_crc_result(unsigned long error, unsigned long total_count,
	unsigned long error_count);

/* ====================================axp interface==================================== */
/**
 * register call-back function, call-back function is for arisc notify some event to ac327,
 * axp/rtc interrupt for external interrupt NMI.
 * @type:  nmi type, pmu/rtc;
 * @func:  call-back function;
 * @para:  parameter for call-back function;
 *
 * @return: result, 0 - register call-back function successed;
 *                 !0 - register call-back function failed;
 * NOTE: the function is like "int callback(void *para)";
 *       this function will execute in system ISR.
 */
int arisc_nmi_cb_register(u32 type, arisc_cb_t func, void *para);

/**
 * unregister call-back function.
 * @type:  nmi type, pmu/rtc;
 * @func:  call-back function which need be unregister;
 */
void arisc_nmi_cb_unregister(u32 type, arisc_cb_t func);

int arisc_disable_nmi_irq(void);
int arisc_enable_nmi_irq(void);
int arisc_clear_nmi_status(void);
int arisc_set_nmi_trigger(u32 type);

int arisc_axp_get_chip_id(unsigned char *chip_id);
#if (defined CONFIG_ARCH_SUN8IW5P1) || \
	(defined CONFIG_ARCH_SUN50IW1P1) || \
	(defined CONFIG_ARCH_SUN50IW2P1) || \
	(defined CONFIG_ARCH_SUN50IW3P1)
int arisc_adjust_pmu_chgcur(unsigned int max_chgcur, unsigned int chg_ic_temp);
#endif
int arisc_set_pwr_tree(u32 *pwr_tree);
int arisc_set_led_bln(u32 led_rgb, u32 led_onms, u32 led_offms, u32 led_darkms);


/* ====================================rsb interface==================================== */
/**
 * rsb read block data.
 * @cfg:    point of arisc_rsb_block_cfg struct;
 *
 * return: result, 0 - read register successed,
 *                !0 - read register failed or the len more then max len;
 */
int arisc_rsb_read_block_data(struct arisc_rsb_block_cfg *cfg);

/**
 * rsb write block data.
 * @cfg:    point of arisc_rsb_block_cfg struct;
 *
 * return: result, 0 - write register successed,
 *                !0 - write register failedor the len more then max len;
 */
int arisc_rsb_write_block_data(struct arisc_rsb_block_cfg *cfg);

/**
 * rsb bits operation sync.
 * @cfg:    point of arisc_rsb_bits_cfg struct;
 *
 * return: result, 0 - bits operation successed,
 *                !0 - bits operation failed, or the len more then max len;
 *
 * rsb clear bits internal:
 * data = rsb_read(regaddr);
 * data = data & (~mask);
 * rsb_write(regaddr, data);
 *
 * rsb set bits internal:
 * data = rsb_read(addr);
 * data = data | mask;
 * rsb_write(addr, data);
 *
 */
int rsb_bits_ops_sync(struct arisc_rsb_bits_cfg *cfg);

/**
 * rsb set interface mode.
 * @devaddr:  rsb slave device address;
 * @regaddr:  register address of rsb slave device;
 * @data:     data which to init rsb slave device interface mode;
 *
 * return: result, 0 - set interface mode successed,
 *                !0 - set interface mode failed;
 */
int arisc_rsb_set_interface_mode(u32 devaddr, u32 regaddr, u32 data);

/**
 * rsb set runtime slave address.
 * @devaddr:  rsb slave device address;
 * @rtsaddr:  rsb slave device's runtime slave address;
 *
 * return: result, 0 - set rsb runtime address successed,
 *                !0 - set rsb runtime address failed;
 */
int arisc_rsb_set_rtsaddr(u32 devaddr, u32 rtsaddr);

/**
 * set pmu voltage.
 * @type:     pmu regulator type;
 * @voltage:  pmu regulator voltage;
 *
 * return: result, 0 - set pmu voltage successed,
 *                !0 - set pmu voltage failed;
 */
int arisc_pmu_set_voltage(u32 type, u32 voltage);

/**
 * get pmu voltage.
 * @type:     pmu regulator type;
 *
 * return: pmu regulator voltage;
 */
unsigned int arisc_pmu_get_voltage(u32 type);

/**
 * set pmu voltage state.
 * @type:     pmu regulator type;
 * @state:  pmu regulator voltage state;
 *
 * return: result, 0 - set pmu voltage state successed,
 *                !0 - set pmu voltage state failed;
 */
int arisc_pmu_set_voltage_state(u32 type, u32 state);

/**
 * get pmu voltage state.
 * @type:     pmu regulator type;
 *
 * return: pmu regulator voltage state;
 */
unsigned int arisc_pmu_get_voltage_state(u32 type);

/* ====================================twi interface==================================== */
/**
 * twi read block data.
 * @cfg:    point of arisc_twi_block_cfg struct;
 *
 * return: result, 0 - read register successed,
 *                !0 - read register failed or the len more then max len;
 */
int arisc_twi_read_block_data(struct arisc_twi_block_cfg *cfg);

/**
 * twi write block data.
 * @cfg:    point of arisc_twi_block_cfg struct;
 *
 * return: result, 0 - write register successed,
 *                !0 - write register failedor the len more then max len;
 */
int arisc_twi_write_block_data(struct arisc_twi_block_cfg *cfg);

/**
 * twi bits operation sync.
 * @cfg:    point of arisc_twi_bits_cfg struct;
 *
 * return: result, 0 - bits operation successed,
 *                !0 - bits operation failed, or the len more then max len;
 *
 * twi clear bits internal:
 * data = twi_read(addr);
 * data = data & (~mask);
 * twi_write(addr, data);
 *
 * twi set bits internal:
 * data = twi_read(addr);
 * data = data | mask;
 * twi_write(addr, data);
 *
 */
int twi_bits_ops_sync(struct arisc_twi_bits_cfg *cfg);

/* ====debug interface==== */
int arisc_message_loopback(void);
int arisc_config_ir_paras(u32 ir_code, u32 ir_addr);
int arisc_set_crashdump_mode(void);

/* ====standby interface==== */
/**
 * enter super standby.
 * @para:  parameter for enter normal standby.
 *
 * return: result, 0 - super standby successed, !0 - super standby failed;
 */
int arisc_standby_super(struct super_standby_para *para, arisc_cb_t cb, void *cb_arg);

/**
 * notify arisc cpux restored.
 * @para:  none.
 *
 * return: result, 0 - notify successed, !0 - notify failed;
 */
int arisc_cpux_ready_notify(void);

/**
 * ap read data.
 * @data:    point of data;
 * @length:  length of data;
 *
 * return: result, 0 - read data successed,
 *                !0 - read data failed or the len more then max len;
 */
int arisc_ap_read_data(char *data, int length);

/**
 * ap write data.
 * @data:    point of data;
 * @length:  length of data;
 *
 * return: result, 0 - write data successed,
 *                !0 - write data failed or the len more then max len;
 */
int arisc_ap_write_data(char *data, int length);

/**
 * set msgbox channel receiver interrupt.
 *
 * return: result, 0 - setting successed,
 *                !0 - setting failed;
 */
int arisc_set_msgbox_receiver_int(unsigned int channel, unsigned int user, bool enable);

/**
 * get msgbox channel receiver pend.
 *
 * return: result 1 - interrupt pending,
 *                !0 - interrupt not pending;
 */
int arisc_get_msgbox_receiver_pend(unsigned int channel, unsigned int user);

/**
 * clear msgbox channel receiver pend.
 *
 * return: result 0 - clear interrupt pending successed,
 *                !0 - clear interrupt pending  failed;;
 */
int arisc_clear_msgbox_receiver_pend(unsigned int channel, unsigned int user);

/**
 *  ap wakeup sh interrupt.
 *
 * return: result 0 - send successed,
 *                !0 - send  failed;;
 */
int arisc_ap_wakeup_sh(bool wakeup);
#endif	/* __ASM_ARCH_A100_H */
