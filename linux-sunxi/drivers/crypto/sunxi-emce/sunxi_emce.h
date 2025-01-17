/*
 * Some macro and struct of SUNXI EMCE.
 *
 * Copyright (C) 2016 Allwinner.
 *
 * zhouhuacai <zhouhuacai@allwinnertech.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef _SUNXI_EMCE_H_
#define _SUNXI_EMCE_H_


/*for debug*/
#define EMCE_DBG(fmt, arg...) pr_debug("%s()%d"fmt, __func__, __LINE__, ##arg);
#define EMCE_ERR(fmt, arg...) pr_err("%s()%d"fmt, __func__, __LINE__, ##arg)
#define EMCE_EXIT()				EMCE_DBG("%s\n", "Exit")
#define EMCE_ENTER()			EMCE_DBG("%s\n", "Enter ...")
#define emce_readl(emce, reg)  \
	readl((emce)->base_addr + reg)
#define emce_writel(emce, reg, value)  \
	writel((value), (emce)->base_addr + reg)


#define MAXNAME		16
#define SUNXI_EMCE_DEV_NAME	"emce"

#define EMCE_MODE				(0x0080)
#define EMCE_VER				(0x0090)

#define EMCE_SECTOR_SIZE_BIT		(16)
#define EMCE_CUR_CTR_BIT		(12)
#define EMCE_IV_LEN_BIT			(10)
#define EMCE_KEY_LEN_BIT		(4)
#define EMCE_MODE_BIT			(0)

#define EMCE_SECTOR_SIZE_MASK	(0xFFFF<<16)
#define EMCE_CUR_CTR_MASK		(0x1<<12)
#define EMCE_IV_LEN_MASK		(0x3<<10)
#define EMCE_KEY_LEN_MASK		(0x3<<4)
#define EMCE_MODE_MASK			(0x3<<0)

#define EMCE_SALT_KEY_LEN		(32)

#define EMCE_KEY_128_BIT		(0x0)
#define EMCE_KEY_192_BIT		(0x1)
#define EMCE_KEY_256_BIT		(0x2)

#define EMCE_MASTER_KEY_OFFSET	(0x0)
#define EMCE_SALT_KEY_OFFSET	(0x40)

enum cfg_cmd {
	EMCE_SET_SECTOR_SIZE,	/* default:0x200 */
	EMCE_SET_CTR,			/* 0:NDFC, 1:SMHC */
	EMCE_SET_IV_LEN,		/* 0:128bit, 1:192bit,2:256bit */
	EMCE_SET_KEY_LEN,		/* 0:128bit, 1:192bit,2:256bit */
	EMCE_SET_MODE,			/* 0:ECB, 1:CBC */
};

struct sunxi_emce_regs {
	u32 emce_key[8];
	u32 emce_salt[8];
	u32 emce_mode;
};


struct sunxi_emce_t {
	struct platform_device *pdev;
	void __iomem *base_addr; /* for register */
	struct sunxi_emce_regs regs;
	struct clk *mclk;  /* module clock */
	u32 gen_clkrate;
	s8  dev_name[8];
} sunxi_emce_t;

#endif /* _SUNXI_EMCE_ */

