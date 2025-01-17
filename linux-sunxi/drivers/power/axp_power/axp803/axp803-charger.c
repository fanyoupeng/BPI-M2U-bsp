/*
 * drivers/power/axp/axp803/axp803-charger.c
 * (C) Copyright 2010-2016
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Pannan <pannan@allwinnertech.com>
 *
 * charger driver of axp803
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/workqueue.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/power_supply.h>
#include "../axp-core.h"
#include "../axp-charger.h"
#include "axp803-charger.h"

static int axp803_get_ac_voltage(struct axp_charger_dev *cdev)
{
	return 0;
}

static int axp803_get_ac_current(struct axp_charger_dev *cdev)
{
	return 0;
}

static int axp803_set_ac_vhold(struct axp_charger_dev *cdev, int vol)
{
	u8 tmp;
	struct axp_regmap *map = cdev->chip->regmap;

	if (vol) {
		axp_regmap_set_bits(map, AXP803_CHARGE_AC_SET, 0x40);
		if (vol >= 4000 && vol <= 4700) {
			tmp = (vol - 4000)/100;
			axp_regmap_update(map, AXP803_CHARGE_AC_SET,
				tmp<<3, 0x7<<3);
		} else {
			pr_err("set ac limit voltage error, %d mV\n",
				axp803_config.pmu_ac_vol);
		}
	} else {
		axp_regmap_clr_bits(map, AXP803_CHARGE_AC_SET, 0x40);
	}

	return 0;

}

static int axp803_get_ac_vhold(struct axp_charger_dev *cdev)
{
	u8 tmp;
	struct axp_regmap *map = cdev->chip->regmap;

	axp_regmap_read(map, AXP803_CHARGE_AC_SET, &tmp);
	tmp = (tmp >> 3) & 0x7;

	return 4000 + tmp * 100;
}

static int axp803_set_ac_ihold(struct axp_charger_dev *cdev, int cur)
{
	struct axp_regmap *map = cdev->chip->regmap;
	u8 tmp;

	if (cur) {
		if (cur >= 1500 && cur <= 4000) {
			tmp = (cur - 1500) / 500;
			axp_regmap_update(map, AXP803_CHARGE_AC_SET,
				tmp, 0x7);
		} else {
			pr_err("set ac limit current error, %d mA\n",
				axp803_config.pmu_ac_cur);
		}
	} else {
		axp_regmap_set_bits(map, AXP803_CHARGE_AC_SET, 0x40);
	}

	return 0;
}

static int axp803_get_ac_ihold(struct axp_charger_dev *cdev)
{
	u8 tmp;
	struct axp_regmap *map = cdev->chip->regmap;

	axp_regmap_read(map, AXP803_CHARGE_AC_SET, &tmp);
	tmp = tmp & 0x7;

	return 1500 + tmp * 500;

}

static struct axp_ac_info axp803_ac_info = {
	.det_bit         = 7,
	.det_offset      = AXP803_STATUS,
	.valid_bit       = 6,
	.valid_offset    = AXP803_STATUS,
	.in_short_bit    = 1,
	.in_short_offset = AXP803_STATUS,
	.get_ac_voltage  = axp803_get_ac_voltage,
	.get_ac_current  = axp803_get_ac_current,
	.set_ac_vhold    = axp803_set_ac_vhold,
	.get_ac_vhold    = axp803_get_ac_vhold,
	.set_ac_ihold    = axp803_set_ac_ihold,
	.get_ac_ihold    = axp803_get_ac_ihold,
};

static int axp803_get_usb_voltage(struct axp_charger_dev *cdev)
{
	return 0;
}

static int axp803_get_usb_current(struct axp_charger_dev *cdev)
{
	return 0;
}

static int axp803_set_usb_vhold(struct axp_charger_dev *cdev, int vol)
{
	u8 tmp;
	struct axp_regmap *map = cdev->chip->regmap;

	if (vol) {
		axp_regmap_set_bits(map, AXP803_IPS_SET, 0x40);
		if (vol >= 4000 && vol <= 4700) {
			tmp = (vol - 4000)/100;
			axp_regmap_update(map, AXP803_IPS_SET,
				tmp<<3, 0x7<<3);
		} else {
			pr_err("set usb limit voltage error, %d mV\n",
				axp803_config.pmu_usbpc_vol);
		}
	} else {
		axp_regmap_clr_bits(map, AXP803_IPS_SET, 0x40);
	}

	return 0;

}

static int axp803_get_usb_vhold(struct axp_charger_dev *cdev)
{
	u8 tmp;
	struct axp_regmap *map = cdev->chip->regmap;

	axp_regmap_read(map, AXP803_IPS_SET, &tmp);
	tmp = (tmp >> 3) & 0x7;

	return 4000 + tmp * 100;
}

static int axp803_set_usb_ihold(struct axp_charger_dev *cdev, int cur)
{
	struct axp_regmap *map = cdev->chip->regmap;

	if (cur) {
		if (cur < 1500)
			axp_regmap_set_bits(map, AXP803_IPS_SET, 0x3);
		else if (cur >= 1500 && cur < 2000)
			axp_regmap_update(map, AXP803_IPS_SET, 0x1, 0x3);
		else if (cur >= 2000 && cur < 2500)
			axp_regmap_update(map, AXP803_IPS_SET, 0x2, 0x3);
		else
			axp_regmap_update(map, AXP803_IPS_SET, 0x3, 0x3);
	} else {
		axp_regmap_set_bits(map, AXP803_IPS_SET, 0x3);
	}

	return 0;
}

static int axp803_get_usb_ihold(struct axp_charger_dev *cdev)
{
	u8 tmp;
	struct axp_regmap *map = cdev->chip->regmap;

	axp_regmap_read(map, AXP803_IPS_SET, &tmp);
	tmp = tmp & 0x3;
	if (tmp == 0x0)
		return 900;
	else if (tmp == 0x1)
		return 1500;
	else if (tmp == 0x2)
		return 2000;
	else
		return 2500;
}

static struct axp_usb_info axp803_usb_info = {
	.det_bit         = 5,
	.det_offset      = AXP803_STATUS,
	.valid_bit       = 4,
	.valid_offset    = AXP803_STATUS,
	.get_usb_voltage = axp803_get_usb_voltage,
	.get_usb_current = axp803_get_usb_current,
	.set_usb_vhold   = axp803_set_usb_vhold,
	.get_usb_vhold   = axp803_get_usb_vhold,
	.set_usb_ihold   = axp803_set_usb_ihold,
	.get_usb_ihold   = axp803_get_usb_ihold,
};

static int axp803_get_rest_cap(struct axp_charger_dev *cdev)
{
	u8 val, temp_val[2], batt_max_cap_val[2];
	int batt_max_cap, coulumb_counter;
	int rest_vol = 0;
	struct axp_regmap *map = cdev->chip->regmap;

	axp_regmap_read(map, AXP803_CAP, &val);
	if (val & 0x80)
		rest_vol = (int) (val & 0x7F);

	axp_regmap_reads(map, AXP803_COUCNT0, 2, temp_val);
	coulumb_counter = (((temp_val[0] & 0x7f) << 8) + temp_val[1])
						* 1456 / 1000;

	axp_regmap_reads(map, AXP803_BATCAP0, 2, temp_val);
	batt_max_cap = (((temp_val[0] & 0x7f) << 8) + temp_val[1])
						* 1456 / 1000;

	/* Avoid the power stay in 100% for a long time. */
	if (coulumb_counter > batt_max_cap) {
		batt_max_cap_val[0] = temp_val[0] | (0x1<<7);
		batt_max_cap_val[1] = temp_val[1];
		axp_regmap_writes(map, AXP803_COUCNT0, 2, batt_max_cap_val);
		AXP_DEBUG(AXP_SPLY, cdev->chip->pmu_num,
				"Axp803 coulumb_counter = %d\n", batt_max_cap);
	}

	return rest_vol;
}

static int axp803_get_bat_health(struct axp_charger_dev *cdev)
{
	u8 val;
	struct axp_regmap *map = cdev->chip->regmap;

	axp_regmap_read(map, AXP803_MODE_CHGSTATUS, &val);
	if (val & AXP803_FAULT_LOG_BATINACT)
		return POWER_SUPPLY_HEALTH_DEAD;
	else if (val & AXP803_FAULT_LOG_OVER_TEMP)
		return POWER_SUPPLY_HEALTH_OVERHEAT;
	else if (val & AXP803_FAULT_LOG_COLD)
		return POWER_SUPPLY_HEALTH_COLD;
	else
		return POWER_SUPPLY_HEALTH_GOOD;
}

static inline int axp803_vbat_to_mV(u32 reg)
{
	return ((int)(((reg >> 8) << 4) | (reg & 0x000F))) * 1100 / 1000;
}

static int axp803_get_vbat(struct axp_charger_dev *cdev)
{
	u8 tmp[2];
	u32 res;
	struct axp_regmap *map = cdev->chip->regmap;

	axp_regmap_reads(map, AXP803_VBATH_RES, 2, tmp);
	res = (tmp[0] << 8) | tmp[1];

	return axp803_vbat_to_mV(res);
}

static inline int axp803_icharge_to_mA(u32 reg)
{
	return (int)(((reg >> 8) << 4) | (reg & 0x000F));
}

static int axp803_get_ibat(struct axp_charger_dev *cdev)
{
	u8 tmp[2];
	u32 res;
	struct axp_regmap *map = cdev->chip->regmap;

	axp_regmap_reads(map, AXP803_IBATH_REG, 2, tmp);
	res = (tmp[0] << 8) | tmp[1];

	return axp803_icharge_to_mA(res);
}

static int axp803_get_disibat(struct axp_charger_dev *cdev)
{
	u8 tmp[2];
	u32 dis_res;
	struct axp_regmap *map = cdev->chip->regmap;

	axp_regmap_reads(map, AXP803_DISIBATH_REG, 2, tmp);
	dis_res = (tmp[0] << 8) | tmp[1];

	return axp803_icharge_to_mA(dis_res);
}

static inline int axp803_vts_to_mv(u16 reg)
{
	return ((int)(((reg >> 8) << 4) | (reg & 0x000F))) * 800 / 1000;
}

static int axp803_get_bat_temp(struct axp_charger_dev *cdev)
{
	u8 tmp[2];
	u16 vts_res;
	struct axp_regmap *map = cdev->chip->regmap;
	int bat_temp_mv;

	axp_regmap_reads(map, AXP803_VTS_RES, 2, tmp);
	vts_res = ((u16) tmp[0] << 8) | tmp[1];
	bat_temp_mv = axp803_vts_to_mv(vts_res);

	return axp_charger_vts_to_temp(bat_temp_mv, &axp803_config);
}

static int axp803_set_chg_cur(struct axp_charger_dev *cdev, int cur)
{
	uint8_t tmp = 0;
	struct axp_regmap *map = cdev->chip->regmap;

	if (cur == 0)
		axp_regmap_clr_bits(map, AXP803_CHARGE1, 0x80);
	else
		axp_regmap_set_bits(map, AXP803_CHARGE1, 0x80);

	if (cur >= 200 && cur <= 2800) {
		tmp = (cur - 200) / 200;
		axp_regmap_update(map, AXP803_CHARGE1, tmp, 0x0F);
	} else if (cur < 200) {
		axp_regmap_clr_bits(map, AXP803_CHARGE1, 0x0F);
	} else {
		axp_regmap_update(map, AXP803_CHARGE1, 0xd, 0x0F);
	}

	return 0;
}

static int axp803_set_chg_vol(struct axp_charger_dev *cdev, int vol)
{
	uint8_t tmp = 0x0;
	struct axp_regmap *map = cdev->chip->regmap;

	if (vol < 4150) {
		tmp &= ~(3 << 5);
	} else if (vol < 4200) {
		tmp &= ~(0x3 << 5);
		tmp |= 0x1 << 5;
	} else if (vol < 4350) {
		tmp &= ~(0x3 << 5);
		tmp |= 0x2 << 5;
	} else {
		tmp |= 0x3 << 5;
	}

	axp_regmap_update(map, AXP803_CHARGE1, tmp, 0x3<<5);

	return 0;
}

static struct axp_battery_info axp803_batt_info = {
	.chgstat_bit          = 6,
	.chgstat_offset       = AXP803_MODE_CHGSTATUS,
	.det_bit              = 5,
	.det_valid_bit        = 4,
	.det_valid            = 1,
	.det_offset           = AXP803_MODE_CHGSTATUS,
	.cur_direction_bit    = 2,
	.cur_direction_offset = AXP803_STATUS,
	.get_rest_cap         = axp803_get_rest_cap,
	.get_bat_health       = axp803_get_bat_health,
	.get_vbat             = axp803_get_vbat,
	.get_ibat             = axp803_get_ibat,
	.get_disibat          = axp803_get_disibat,
	.get_bat_temp         = axp803_get_bat_temp,
	.set_chg_cur          = axp803_set_chg_cur,
	.set_chg_vol          = axp803_set_chg_vol,
};

static struct power_supply_info battery_data = {
	.name = "PTI PL336078",
	.technology = POWER_SUPPLY_TECHNOLOGY_LiFe,
	.voltage_max_design = 4200000,
	.voltage_min_design = 3500000,
	.use_for_apm = 1,
};

static struct axp_supply_info axp803_spy_info = {
	.ac   = &axp803_ac_info,
	.usb  = &axp803_usb_info,
	.batt = &axp803_batt_info,
};

static int axp803_charger_init(struct axp_dev *axp_dev)
{
	u8 ocv_cap[32];
	u8 val = 0;
	int cur_coulomb_counter, rdc;
	struct axp_regmap *map = axp_dev->regmap;
	int i, update_min_times[8] = {30, 60, 120, 164, 0, 5, 10, 20};

	if (axp803_config.pmu_init_chgend_rate == 10)
		val &= ~(1 << 4);
	else
		val |= 1 << 4;

	val &= 0x7F;
	axp_regmap_write(map, AXP803_CHARGE1, val);

	if (axp803_config.pmu_init_chg_pretime < 40)
		axp803_config.pmu_init_chg_pretime = 40;

	if (axp803_config.pmu_init_chg_csttime < 360)
		axp803_config.pmu_init_chg_csttime = 360;

	val = ((((axp803_config.pmu_init_chg_pretime - 40) / 10) << 6)
			| ((axp803_config.pmu_init_chg_csttime - 360) / 120));
	axp_regmap_update(map, AXP803_CHARGE2, val, 0xC2);

	/* adc set */
	val = AXP803_ADC_BATVOL_ENABLE | AXP803_ADC_BATCUR_ENABLE;
	if (0 != axp803_config.pmu_bat_temp_enable)
		val = val | AXP803_ADC_TSVOL_ENABLE;
	axp_regmap_update(map, AXP803_ADC_EN, val,
						AXP803_ADC_BATVOL_ENABLE
						| AXP803_ADC_BATCUR_ENABLE
						| AXP803_ADC_TSVOL_ENABLE);

	axp_regmap_read(map, AXP803_ADC_SPEED_SET, &val);
	switch (axp803_config.pmu_init_adc_freq / 100) {
	case 1:
		val &= ~(0x3 << 4);
		break;
	case 2:
		val &= ~(0x3 << 4);
		val |= 0x1 << 4;
		break;
	case 4:
		val &= ~(0x3 << 4);
		val |= 0x2 << 4;
		break;
	case 8:
		val |= 0x3 << 4;
		break;
	default:
		break;
	}

	if (0 != axp803_config.pmu_bat_temp_enable)
		val &= (~(0x1 << 2));
	axp_regmap_write(map, AXP803_ADC_SPEED_SET, val);

	/* bat para */
	axp_regmap_write(map, AXP803_WARNING_LEVEL,
		((axp803_config.pmu_battery_warning_level1 - 5) << 4)
		+ axp803_config.pmu_battery_warning_level2);

	ocv_cap[0]  = axp803_config.pmu_bat_para1;
	ocv_cap[1]  = axp803_config.pmu_bat_para2;
	ocv_cap[2]  = axp803_config.pmu_bat_para3;
	ocv_cap[3]  = axp803_config.pmu_bat_para4;
	ocv_cap[4]  = axp803_config.pmu_bat_para5;
	ocv_cap[5]  = axp803_config.pmu_bat_para6;
	ocv_cap[6]  = axp803_config.pmu_bat_para7;
	ocv_cap[7]  = axp803_config.pmu_bat_para8;
	ocv_cap[8]  = axp803_config.pmu_bat_para9;
	ocv_cap[9]  = axp803_config.pmu_bat_para10;
	ocv_cap[10] = axp803_config.pmu_bat_para11;
	ocv_cap[11] = axp803_config.pmu_bat_para12;
	ocv_cap[12] = axp803_config.pmu_bat_para13;
	ocv_cap[13] = axp803_config.pmu_bat_para14;
	ocv_cap[14] = axp803_config.pmu_bat_para15;
	ocv_cap[15] = axp803_config.pmu_bat_para16;
	ocv_cap[16] = axp803_config.pmu_bat_para17;
	ocv_cap[17] = axp803_config.pmu_bat_para18;
	ocv_cap[18] = axp803_config.pmu_bat_para19;
	ocv_cap[19] = axp803_config.pmu_bat_para20;
	ocv_cap[20] = axp803_config.pmu_bat_para21;
	ocv_cap[21] = axp803_config.pmu_bat_para22;
	ocv_cap[22] = axp803_config.pmu_bat_para23;
	ocv_cap[23] = axp803_config.pmu_bat_para24;
	ocv_cap[24] = axp803_config.pmu_bat_para25;
	ocv_cap[25] = axp803_config.pmu_bat_para26;
	ocv_cap[26] = axp803_config.pmu_bat_para27;
	ocv_cap[27] = axp803_config.pmu_bat_para28;
	ocv_cap[28] = axp803_config.pmu_bat_para29;
	ocv_cap[29] = axp803_config.pmu_bat_para30;
	ocv_cap[30] = axp803_config.pmu_bat_para31;
	ocv_cap[31] = axp803_config.pmu_bat_para32;
	axp_regmap_writes(map, 0xC0, 32, ocv_cap);

	/* Init CHGLED function */
	if (axp803_config.pmu_chgled_func)
		axp_regmap_set_bits(map, AXP803_OFF_CTL, 0x08); /* by charger */
	else
		axp_regmap_clr_bits(map, AXP803_OFF_CTL, 0x08); /* drive MOTO */

	/* set CHGLED Indication Type */
	if (axp803_config.pmu_chgled_type)
		axp_regmap_set_bits(map, AXP803_CHARGE2, 0x10); /* Type B */
	else
		axp_regmap_clr_bits(map, AXP803_CHARGE2, 0x10); /* Type A */

	/* Init battery capacity correct function */
	if (axp803_config.pmu_batt_cap_correct)
		axp_regmap_set_bits(map, AXP803_COULOMB_CTL, 0x20);
	else
		axp_regmap_clr_bits(map, AXP803_COULOMB_CTL, 0x20);

	/* Init battery regulator enable or not when charge finish */
	if (axp803_config.pmu_chg_end_on_en)
		axp_regmap_set_bits(map, AXP803_CHARGE2, 0x20);
	else
		axp_regmap_clr_bits(map, AXP803_CHARGE2, 0x20);

	if (axp803_config.pmu_batdeten)
		axp_regmap_set_bits(map, AXP803_OFF_CTL, 0x40);
	else
		axp_regmap_clr_bits(map, AXP803_OFF_CTL, 0x40);

	/* RDC initial */
	axp_regmap_read(map, AXP803_RDC0, &val);
	if ((axp803_config.pmu_battery_rdc) && (!(val & 0x40))) {
		rdc = (axp803_config.pmu_battery_rdc * 10000 + 5371) / 10742;
		axp_regmap_write(map, AXP803_RDC0, ((rdc >> 8) & 0x1F)|0x80);
		axp_regmap_write(map, AXP803_RDC1, rdc & 0x00FF);
	}

	axp_regmap_read(map, AXP803_BATCAP0, &val);
	if ((axp803_config.pmu_battery_cap) && (!(val & 0x80))) {
		cur_coulomb_counter = axp803_config.pmu_battery_cap
					* 1000 / 1456;
		axp_regmap_write(map, AXP803_BATCAP0,
					((cur_coulomb_counter >> 8) | 0x80));
		axp_regmap_write(map, AXP803_BATCAP1,
					cur_coulomb_counter & 0x00FF);
	} else if (!axp803_config.pmu_battery_cap) {
		axp_regmap_write(map, AXP803_BATCAP0, 0x00);
		axp_regmap_write(map, AXP803_BATCAP1, 0x00);
	}

	if (axp803_config.pmu_bat_unused == 1)
		axp803_spy_info.batt->det_unused = 1;
	else
		axp803_spy_info.batt->det_unused = 0;
	/*As datasheet decripted:
	  TS_VOL = reg_value * 16 * 10K * 80ua
	 */
	if (axp803_config.pmu_bat_temp_enable == 1) {
		axp_regmap_write(map, AXP803_VLTF_CHARGE,
				axp803_config.pmu_bat_charge_ltf * 10 / 128);
		axp_regmap_write(map, AXP803_VHTF_CHARGE,
				axp803_config.pmu_bat_charge_htf * 10 / 128);
		axp_regmap_write(map, AXP803_VLTF_WORK,
				axp803_config.pmu_bat_shutdown_ltf * 10 / 128);
		axp_regmap_write(map, AXP803_VHTF_WORK,
				axp803_config.pmu_bat_shutdown_htf * 10 / 128);
	}

	if (axp803_config.pmu_ocv_en == 0) {
		pr_warn("axp803 ocv must be enabled\n");
		axp803_config.pmu_ocv_en = 1;
	}

	if (axp803_config.pmu_cou_en == 1) {
		/* use ocv and cou */
		axp_regmap_set_bits(map, AXP803_COULOMB_CTL, 0x80);
		axp_regmap_set_bits(map, AXP803_COULOMB_CTL, 0x40);
	} else if (axp803_config.pmu_cou_en == 0) {
		/* only use ocv */
		axp_regmap_set_bits(map, AXP803_COULOMB_CTL, 0x80);
		axp_regmap_clr_bits(map, AXP803_COULOMB_CTL, 0x40);
	}

	for (i = 0; i < ARRAY_SIZE(update_min_times); i++) {
		if (update_min_times[i] == axp803_config.pmu_update_min_time)
			break;
	}
	axp_regmap_update(map, AXP803_ADJUST_PARA, i, 0x7);

	return 0;
}

static struct axp_interrupts axp_charger_irq[] = {
	{"usb in",               axp_usb_in_isr},
	{"usb out",              axp_usb_out_isr},
	{"ac in",                axp_ac_in_isr},
	{"ac out",               axp_ac_out_isr},
	{"bat in",               axp_capchange_isr},
	{"bat out",              axp_capchange_isr},
	{"bat untemp work",      axp_change_isr},
	{"bat ovtemp work",      axp_change_isr},
	{"quit bat untemp chg",  axp_change_isr},
	{"bat untemp chg",       axp_change_isr},
	{"quit bat ovtemp chg",  axp_change_isr},
	{"bat ovtemp chg",       axp_change_isr},
	{"charging",             axp_change_isr},
	{"charge over",          axp_change_isr},
	{"low warning1",         axp_low_warning1_isr},
	{"low warning2",         axp_low_warning2_isr},
};

static void axp803_private_debug(struct axp_charger_dev *cdev)
{
	u8 tmp[2];
	struct axp_regmap *map = cdev->chip->regmap;

	axp_regmap_reads(map, AXP803_OCVBATH_RES, 2, tmp);
	AXP_DEBUG(AXP_SPLY, cdev->chip->pmu_num,
			"ocv_vol = %d\n", ((tmp[0] << 4) | (tmp[1] & 0xF))
			* 1100 / 1000);

	axp_regmap_read(map, AXP803_OCV_PERCENT, &tmp[0]);
	if (tmp[0] & 0x80)
		AXP_DEBUG(AXP_SPLY, cdev->chip->pmu_num,
			"ocv_percent = %d\n", tmp[0] & 0x7f);

	axp_regmap_read(map, AXP803_COU_PERCENT, &tmp[0]);
	if (tmp[0] & 0x80)
		AXP_DEBUG(AXP_SPLY, cdev->chip->pmu_num,
			"coulomb_percent = %d\n", tmp[0] & 0x7f);
}

static int axp803_charger_probe(struct platform_device *pdev)
{
	int ret, i, irq;
	struct axp_charger_dev *chg_dev;
	struct axp_dev *axp_dev = dev_get_drvdata(pdev->dev.parent);

	if (pdev->dev.of_node) {
		/* get dt and sysconfig */
		ret = axp_charger_dt_parse(pdev->dev.of_node, &axp803_config);
		if (ret) {
			pr_err("%s parse device tree err\n", __func__);
			return -EINVAL;
		}
	} else {
		pr_err("axp803 charger device tree err!\n");
		return -EBUSY;
	}

	axp803_ac_info.ac_vol = axp803_config.pmu_ac_vol;
	axp803_ac_info.ac_cur = axp803_config.pmu_ac_cur;
	axp803_usb_info.usb_pc_vol = axp803_config.pmu_usbpc_vol;
	axp803_usb_info.usb_pc_cur = axp803_config.pmu_usbpc_cur;
	axp803_usb_info.usb_ad_vol = axp803_config.pmu_ac_vol;
	axp803_usb_info.usb_ad_cur = axp803_config.pmu_ac_cur;
	axp803_batt_info.runtime_chgcur = axp803_config.pmu_runtime_chgcur;
	axp803_batt_info.suspend_chgcur = axp803_config.pmu_suspend_chgcur;
	axp803_batt_info.shutdown_chgcur = axp803_config.pmu_shutdown_chgcur;
	battery_data.voltage_max_design = axp803_config.pmu_init_chgvol
								* 1000;
	battery_data.voltage_min_design = axp803_config.pmu_pwroff_vol
								* 1000;
	battery_data.energy_full_design = axp803_config.pmu_battery_cap;

	axp803_charger_init(axp_dev);

	chg_dev = axp_power_supply_register(&pdev->dev, axp_dev,
					&battery_data, &axp803_spy_info);
	if (IS_ERR_OR_NULL(chg_dev))
		goto fail;
	chg_dev->private_debug = axp803_private_debug;

	for (i = 0; i < ARRAY_SIZE(axp_charger_irq); i++) {
		irq = platform_get_irq_byname(pdev, axp_charger_irq[i].name);
		if (irq < 0)
			continue;

		ret = axp_request_irq(axp_dev, irq,
				axp_charger_irq[i].isr, chg_dev);
		if (ret != 0) {
			dev_err(&pdev->dev, "failed to request %s IRQ %d: %d\n",
					axp_charger_irq[i].name, irq, ret);
			goto out_irq;
		}

		dev_dbg(&pdev->dev, "Requested %s IRQ %d: %d\n",
			axp_charger_irq[i].name, irq, ret);
	}

	platform_set_drvdata(pdev, chg_dev);

	return 0;

out_irq:
	for (i = i - 1; i >= 0; i--) {
		irq = platform_get_irq_byname(pdev, axp_charger_irq[i].name);
		if (irq < 0)
			continue;
		axp_free_irq(axp_dev, irq);
	}
fail:
	return -1;
}

static int axp803_charger_remove(struct platform_device *pdev)
{
	int i, irq;
	struct axp_charger_dev *chg_dev = platform_get_drvdata(pdev);
	struct axp_dev *axp_dev = dev_get_drvdata(pdev->dev.parent);

	for (i = 0; i < ARRAY_SIZE(axp_charger_irq); i++) {
		irq = platform_get_irq_byname(pdev, axp_charger_irq[i].name);
		if (irq < 0)
			continue;
		axp_free_irq(axp_dev, irq);
	}

	axp_power_supply_unregister(chg_dev);

	return 0;
}

static int axp803_charger_suspend(struct platform_device *dev,
				pm_message_t state)
{
	struct axp_charger_dev *chg_dev = platform_get_drvdata(dev);

	axp_suspend_flag = AXP_WAS_SUSPEND;
	axp_charger_suspend(chg_dev);

	return 0;
}

static int axp803_charger_resume(struct platform_device *dev)
{
	struct axp_charger_dev *chg_dev = platform_get_drvdata(dev);
	struct axp_regmap *map = chg_dev->chip->regmap;
	int pre_rest_vol;

	if (axp_suspend_flag == AXP_SUSPEND_WITH_IRQ) {
		axp_suspend_flag = AXP_NOT_SUSPEND;
#ifdef CONFIG_AXP_NMI_USED
		enable_nmi();
#endif
	} else {
		axp_suspend_flag = AXP_NOT_SUSPEND;
	}

	pre_rest_vol = chg_dev->rest_vol;
	axp_charger_resume(chg_dev);

	if (chg_dev->rest_vol - pre_rest_vol) {
		pr_info("battery vol change: %d->%d\n",
				pre_rest_vol, chg_dev->rest_vol);
		axp_regmap_write(map, AXP803_BUFFER2, chg_dev->rest_vol | 0x80);
	}

	return 0;
}

static void axp803_charger_shutdown(struct platform_device *dev)
{
	struct axp_charger_dev *chg_dev = platform_get_drvdata(dev);
	axp_charger_shutdown(chg_dev);
}

static const struct of_device_id axp803_charger_dt_ids[] = {
	{ .compatible = "axp803-charger", },
	{ .compatible = "axp288-charger", },
	{},
};
MODULE_DEVICE_TABLE(of, axp803_charger_dt_ids);

static struct platform_driver axp803_charger_driver = {
	.driver     = {
		.name   = "axp803-charger",
		.of_match_table = axp803_charger_dt_ids,
	},
	.probe    = axp803_charger_probe,
	.remove   = axp803_charger_remove,
	.suspend  = axp803_charger_suspend,
	.resume   = axp803_charger_resume,
	.shutdown = axp803_charger_shutdown,
};

static int __init axp803_charger_initcall(void)
{
	int ret;

	ret = platform_driver_register(&axp803_charger_driver);
	if (IS_ERR_VALUE(ret)) {
		pr_err("%s: failed, errno %d\n", __func__, ret);
		return -EINVAL;
	}

	return 0;
}
fs_initcall_sync(axp803_charger_initcall);

MODULE_DESCRIPTION("Charger Driver of AXP803");
MODULE_AUTHOR("pannan");
MODULE_LICENSE("GPL");
