/*
 * drivers/power/axp_power/axp259/axp259-gpio.c
 * (C) Copyright 2010-2016
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Pannan <pannan@allwinnertech.com>
 *
 * gpio driver of axp259
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#include <linux/io.h>
#include <linux/clk.h>
#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/pinctrl/consumer.h>
#include <linux/pinctrl/machine.h>
#include <linux/pinctrl/pinctrl.h>
#include <linux/pinctrl/pinconf-generic.h>
#include <linux/pinctrl/pinmux.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/sys_config.h>
#include <linux/slab.h>
#include <linux/mfd/core.h>
#include <linux/seq_file.h>
#include <linux/i2c.h>
#include "../../../pinctrl/core.h"
#include "../axp-core.h"
#include "../axp-gpio.h"
#include "axp259-gpio.h"

static int axp259_gpio_get_data(struct axp_dev *axp_dev, int gpio)
{
	u8 ret;
	struct axp_regmap *map;

	if (gpio == 1) {
		map = axp_dev->regmap;
		if (!map) {
			pr_err("%s: %d axp regmap is null\n",
					__func__, __LINE__);
			return -ENXIO;
		}
	} else {
		pr_err("%s gpio%d is not valid\n",
				axp_name[axp_dev->pmu_num], gpio);
		return -ENXIO;
	}

	axp_regmap_read(map, AXP_GPIO_STATE, &ret);
	ret &= 0x1;

	return ret;
}

static int axp259_gpio_set_data(struct axp_dev *axp_dev, int gpio, int value)
{
	struct axp_regmap *map;
	int ret = 0;

	if (gpio == 1 || gpio == 2) {
		map = axp_dev->regmap;
		if (!map) {
			pr_err("%s: %d axp regmap is null\n",
					__func__, __LINE__);
			return -ENXIO;
		}
	} else {
		pr_err("%s gpio%d is not valid\n",
				axp_name[axp_dev->pmu_num], gpio);
		return -ENXIO;
	}

	/* high */
	if (value) {
		if (gpio == 1)
			ret = axp_regmap_update_sync(map,
				AXP_GPIO1_CFG, 0x03, 0x07);
		else if (gpio == 2)
			ret = axp_regmap_update_sync(map,
				AXP_GPIO2_CFG, 0x03, 0x07);
	} else {
		if (gpio == 1)
			ret = axp_regmap_update_sync(map,
				AXP_GPIO1_CFG, 0x02, 0x07);
		else if (gpio == 2)
			ret = axp_regmap_update_sync(map,
				AXP_GPIO2_CFG, 0x02, 0x07);
	}

	return ret;
}

static int axp259_pmx_set(struct axp_dev *axp_dev, int gpio, int mux)
{
	struct axp_regmap *map;
	int ret = 0;

	if (gpio == 1 || gpio == 2) {
		map = axp_dev->regmap;
		if (!map) {
			pr_err("%s: %d axp regmap is null\n",
				__func__, __LINE__);
			return -ENXIO;
		}
	} else {
		pr_err("%s gpio%d is not valid\n",
				axp_name[axp_dev->pmu_num], gpio);
		return -ENXIO;
	}

	/* output */
	if (mux == 1) {
		if (gpio == 1)
			ret = axp_regmap_update_sync(map,
				AXP_GPIO1_CFG, 0x02, 0x06);
		else if (gpio == 2)
			ret = axp_regmap_update_sync(map,
				AXP_GPIO2_CFG, 0x02, 0x06);
	} else {
		return -ENXIO;
	}

	return ret;
}

static int axp259_pmx_get(struct axp_dev *axp_dev, int gpio)
{
	u8 ret = 0;
	struct axp_regmap *map;

	if (gpio == 1 || gpio == 2) {
		map = axp_dev->regmap;
		if (!map) {
			pr_err("%s: %d axp regmap is null\n",
				__func__, __LINE__);
			return -ENXIO;
		}
	} else {
		pr_err("%s gpio%d is not valid\n",
				axp_name[axp_dev->pmu_num], gpio);
		return -ENXIO;
	}

	if (gpio == 1)
		axp_regmap_read(map, AXP_GPIO1_CFG, &ret);
	else if (gpio == 2)
		axp_regmap_read(map, AXP_GPIO2_CFG, &ret);

	if ((ret & 0x06) == 0x2)
		return 1;
	else
		return -ENXIO;
}

static const struct axp_desc_pin axp259_pins[] = {
	AXP_PIN_DESC(AXP_PINCTRL_GPIO(0),
			 AXP_FUNCTION(0x0, "gpio_in"),
			 AXP_FUNCTION(0x1, "gpio_out"),
			 AXP_FUNCTION_IRQ(1)),
	AXP_PIN_DESC(AXP_PINCTRL_GPIO(1),
			 AXP_FUNCTION(0x0, "gpio_in"),
			 AXP_FUNCTION(0x1, "gpio_out"),
			 AXP_FUNCTION_IRQ(1)),
	AXP_PIN_DESC(AXP_PINCTRL_GPIO(2),
			 AXP_FUNCTION(0x0, "gpio_in"),
			 AXP_FUNCTION(0x1, "gpio_out"),
			 AXP_FUNCTION_IRQ(1)),
};

static struct axp_pinctrl_desc axp259_pinctrl_pins_desc = {
	.pins  = axp259_pins,
	.npins = ARRAY_SIZE(axp259_pins),
};

static struct axp_gpio_ops axp259_gpio_ops = {
	.gpio_get_data = axp259_gpio_get_data,
	.gpio_set_data = axp259_gpio_set_data,
	.pmx_set = axp259_pmx_set,
	.pmx_get = axp259_pmx_get,
};

static int axp259_gpio_probe(struct platform_device *pdev)
{
	struct axp_dev *axp_dev = dev_get_drvdata(pdev->dev.parent);
	struct axp_pinctrl *axp259_pin;

	axp259_pin = axp_pinctrl_register(&pdev->dev,
				axp_dev, &axp259_pinctrl_pins_desc,
				&axp259_gpio_ops, axp_dev->pmu_num);
	if (IS_ERR_OR_NULL(axp259_pin))
		return -1;

	platform_set_drvdata(pdev, axp259_pin);

	return 0;
}

static int axp259_gpio_remove(struct platform_device *pdev)
{
	struct axp_pinctrl *axp259_pin = platform_get_drvdata(pdev);

	return axp_pinctrl_unregister(axp259_pin);
}

static const struct of_device_id axp259_gpio_dt_ids[] = {
	{ .compatible = "axp259-gpio", },
	{},
};
MODULE_DEVICE_TABLE(of, axp259_gpio_dt_ids);

static struct platform_driver axp259_gpio_driver = {
	.driver = {
		.name = "axp259-gpio",
		.of_match_table = axp259_gpio_dt_ids,
	},
	.probe  = axp259_gpio_probe,
	.remove = axp259_gpio_remove,
};

static int __init axp259_gpio_initcall(void)
{
	int ret;

	ret = platform_driver_register(&axp259_gpio_driver);
	if (IS_ERR_VALUE(ret)) {
		pr_err("%s: failed, errno %d\n", __func__, ret);
		return -EINVAL;
	}

	return 0;
}
fs_initcall(axp259_gpio_initcall);

MODULE_DESCRIPTION("Gpio driver of axp259");
MODULE_AUTHOR("pannan");
MODULE_LICENSE("GPL");
