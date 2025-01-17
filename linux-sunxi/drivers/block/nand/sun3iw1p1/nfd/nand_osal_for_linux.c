/*
******************************************************************************
*						        eBIOS
*           the Easy Portable/Player Develop Kits
*				           dma sub system
*
*	        (c) Copyright 2006-2008, David China
*							All	Rights Reserved
*
* File    : clk_for_nand.c
* By      : Richard
* Version : V1.00
*****************************************************************************
*/

#include "nand_osal_for_linux.h"

#define  NAND_DRV_VERSION_0		0x03
#define  NAND_DRV_VERSION_1		0x6001
#define  NAND_DRV_DATE			0x20171031
#define  NAND_DRV_TIME			0x1300

int nand_type;

int NAND_Print(const char *fmt, ...)
{
	struct va_format vaf;
	va_list args;
	int rtn;

	va_start(args, fmt);

	vaf.fmt = fmt;
	vaf.va = &args;

	rtn = printk(KERN_ERR "%pV", &vaf);

	va_end(args);

	return rtn;
}

int NAND_Print_DBG(const char *fmt, ...)
{
	struct va_format vaf;
	va_list args;
	int rtn;

	va_start(args, fmt);

	vaf.fmt = fmt;
	vaf.va = &args;

	rtn = printk(KERN_DEBUG "%pV", &vaf);

	va_end(args);

	return rtn;
}

int NAND_ClkRequest(__u32 nand_index)
{
	long rate;

	NAND_Print_DBG("NAND_ClkRequest\n");

	pll6 = of_clk_get(ndfc_dev->of_node, 0);
	if ((pll6 == NULL) || IS_ERR(pll6)) {
		NAND_Print("%s: pll6 clock handle invalid!\n", __func__);
		return -1;
	}

	rate = clk_get_rate(pll6);
	NAND_Print_DBG("%s: get pll6 rate %dHZ\n", __func__, (__u32) rate);

	if (nand_index == 0) {
		nand0_dclk = of_clk_get(ndfc_dev->of_node, 1);

		if ((nand0_dclk == NULL) || IS_ERR(nand0_dclk)) {
			NAND_Print("%s: nand0 clock handle invalid!\n",
				__func__);
			return -1;
		}

		if (clk_set_parent(nand0_dclk, pll6))
			NAND_Print("%s:set nand0_dclk parent to pll6 failed\n",
				__func__);

		rate = clk_round_rate(nand0_dclk, 20000000);
		if (clk_set_rate(nand0_dclk, rate))
			NAND_Print("%s: set nand0_dclk rate to %dHZ failed!\n",
			       __func__, (__u32) rate);

		if (clk_prepare_enable(nand0_dclk))
			NAND_Print("%s: enable nand0_dclk failed!\n",
			       __func__);

		if (get_storage_type() == 1) {
			nand0_cclk = of_clk_get(ndfc_dev->of_node, 2);

			if ((nand0_cclk == NULL) || IS_ERR(nand0_cclk)) {
				NAND_Print("%s: nand0 cclock handle invalid!\n", __func__);
				return -1;
			}

			if (clk_set_parent(nand0_cclk, pll6))
				NAND_Print("%s:set nand0_cclk parent to pll6 failed\n", __func__);

			rate = clk_round_rate(nand0_cclk, 20000000);
			if (clk_set_rate(nand0_cclk, rate))
				NAND_Print("%s: set nand0_cclk rate to %dHZ failed!\n", __func__, (__u32) rate);

			if (clk_prepare_enable(nand0_cclk))
				NAND_Print("%s: enable nand0_cclk failed!\n", __func__);
		}
	} else {
		NAND_Print("NAND_ClkRequest, nand_index error: 0x%x\n",
		       nand_index);
		return -1;
	}

	return 0;
}

void NAND_ClkRelease(__u32 nand_index)
{
	if (nand_index == 0) {
		if (nand0_dclk && !IS_ERR(nand0_dclk)) {
			clk_disable_unprepare(nand0_dclk);

			clk_put(nand0_dclk);
			nand0_dclk = NULL;
		}

		if (get_storage_type() == 1) {
			if (nand0_cclk && !IS_ERR(nand0_cclk)) {
				clk_disable_unprepare(nand0_cclk);

				clk_put(nand0_cclk);
				nand0_cclk = NULL;
			}
		}
	} else
		NAND_Print("NAND_ClkRequest, nand_index error: 0x%x\n", nand_index);

	if (pll6 && !IS_ERR(pll6)) {
		clk_put(pll6);
		pll6 = NULL;
	}
}

int NAND_SetClk(__u32 nand_index, __u32 nand_clk0, __u32 nand_clk1)
{
	long rate;

	if (nand_index == 0) {
		if ((nand0_dclk == NULL) || IS_ERR(nand0_dclk)) {
			NAND_Print("%s: clock handle invalid!\n", __func__);
			return -1;
		}

		rate = clk_round_rate(nand0_dclk, nand_clk0 * 2000000);
		if (clk_set_rate(nand0_dclk, rate))
			NAND_Print("%s: set nand0_dclk to %dHZ failed! nand_clk: 0x%x\n",
			       __func__, (__u32) rate, nand_clk0);

		if (get_storage_type() == 1) {
			if ((nand0_cclk == NULL) || IS_ERR(nand0_cclk)) {
				NAND_Print("%s: clock handle invalid!\n", __func__);
				return -1;
			}

			rate = clk_round_rate(nand0_cclk, nand_clk1 * 1000000);
			if (clk_set_rate(nand0_cclk, rate))
				NAND_Print("%s: set nand0_cclk to %dHZ failed! nand_clk: 0x%x\n",
				       __func__, (__u32) rate, nand_clk1);
		}
	} else {
		NAND_Print("NAND_SetClk, nand_index error: 0x%x\n", nand_index);
		return -1;
	}

	return 0;
}

int NAND_GetClk(__u32 nand_index, __u32 *pnand_clk0, __u32 *pnand_clk1)
{
	long rate = 0;

	if (nand_index == 0) {
		if ((nand0_dclk == NULL) || IS_ERR(nand0_dclk)) {
			NAND_Print("%s: clock handle invalid!\n", __func__);
			return -1;
		}
		rate = clk_get_rate(nand0_dclk);
		*pnand_clk0 = (rate / 2000000);

		rate = 0;
		if (get_storage_type() == 1) {
			if ((nand0_cclk == NULL) || IS_ERR(nand0_cclk)) {
				NAND_Print("%s: clock handle invalid!\n", __func__);
				return -1;
			}
			rate = clk_get_rate(nand0_cclk);
		}
		*pnand_clk1 = (rate / 1000000);
	} else {
		NAND_Print("NAND_GetClk, nand_index error: 0x%x\n", nand_index);
		return -1;
	}

	return 0;
}

void eLIBs_CleanFlushDCacheRegion_nand(void *adr, size_t bytes)
{
/*  __flush_dcache_area(adr, bytes + (1 << 5) * 2 - 2);*/
}

__s32 NAND_CleanFlushDCacheRegion(void *buff_addr, __u32 len)
{
	eLIBs_CleanFlushDCacheRegion_nand((void *)buff_addr, (size_t) len);
	return 0;
}

__s32 NAND_InvaildDCacheRegion(__u32 rw, __u32 buff_addr, __u32 len)
{
	return 0;
}

void *NAND_DMASingleMap(__u32 rw, void *buff_addr, __u32 len)
{
	void *mem_addr;

	if (rw == 1) {
		mem_addr = (void *)dma_map_single(ndfc_dev, (void *)buff_addr,
						len, DMA_TO_DEVICE);
	} else {
		mem_addr = (void *)dma_map_single(ndfc_dev, (void *)buff_addr,
						len, DMA_BIDIRECTIONAL);
	}
	if (dma_mapping_error(ndfc_dev, (dma_addr_t) mem_addr))
		NAND_Print("dma mapping error\n");

	return mem_addr;
}

void *NAND_DMASingleUnmap(__u32 rw, void *buff_addr, __u32 len)
{
	void *mem_addr = buff_addr;

	if (rw == 1) {
		dma_unmap_single(ndfc_dev, (dma_addr_t) mem_addr, len,
				 DMA_TO_DEVICE);
	} else {
		dma_unmap_single(ndfc_dev, (dma_addr_t) mem_addr, len,
				 DMA_BIDIRECTIONAL);
	}

	return mem_addr;
}

void *NAND_VA_TO_PA(void *buff_addr)
{
	return (void *)(__pa((void *)buff_addr));
}

__s32 NAND_PIORequest(__u32 nand_index)
{
	struct pinctrl *pinctrl = NULL;

	PRINT_LEVEL = NAND_Print_level();

	pinctrl = pinctrl_get_select(ndfc_dev, "default");
	if (!pinctrl || IS_ERR(pinctrl)) {
		NAND_Print("NAND_PIORequest: set nand0 pin error!\n");
		return -1;
	}

	return 0;
}

void NAND_PIORelease(__u32 nand_index)
{

	struct pinctrl *pinctrl = NULL;

	pinctrl = pinctrl_get_select(ndfc_dev, "sleep");
	if (!pinctrl || IS_ERR(pinctrl))
		NAND_Print("NAND_PIORelease: set nand0 pin error!\n");

}

void NAND_Memset(void *pAddr, unsigned char value, unsigned int len)
{
	memset(pAddr, value, len);
}

void NAND_Memcpy(void *pAddr_dst, void *pAddr_src, unsigned int len)
{
	memcpy(pAddr_dst, pAddr_src, len);
}

void *NAND_Malloc(unsigned int Size)
{
	return kmalloc(Size, GFP_KERNEL);
}

void NAND_Free(void *pAddr, unsigned int Size)
{
	kfree(pAddr);
}

void *NAND_IORemap(void *base_addr, unsigned int size)
{
	return base_addr;
}

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
DEFINE_SEMAPHORE(nand_physic_mutex);

int NAND_PhysicLockInit(void)
{
	return 0;
}

int NAND_PhysicLock(void)
{
	down(&nand_physic_mutex);
	return 0;
}

int NAND_PhysicUnLock(void)
{
	up(&nand_physic_mutex);
	return 0;
}

int NAND_PhysicLockExit(void)
{
	return 0;
}

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
/* request dma channel and set callback function */
int spinand_request_tx_dma(void)
{
	dma_cap_mask_t mask;

	NAND_Print_DBG("request tx DMA\n");

	/* Try to acquire a generic DMA engine slave channel */
	dma_cap_zero(mask);
	dma_cap_set(DMA_SLAVE, mask);

	if (dma_hdl_tx == NULL) {
		dma_hdl_tx = dma_request_channel(mask, NULL, NULL);
		if (dma_hdl_tx == NULL) {
			NAND_Print_DBG("Request tx DMA failed!\n");
			return -EINVAL;
	    }
	}

	return 0;
}

int spinand_request_rx_dma(void)
{
	dma_cap_mask_t mask;

	NAND_Print_DBG("request rx DMA\n");

	/* Try to acquire a generic DMA engine slave channel */
	dma_cap_zero(mask);
	dma_cap_set(DMA_SLAVE, mask);

	if (dma_hdl_rx == NULL) {
		dma_hdl_rx = dma_request_channel(mask, NULL, NULL);
		if (dma_hdl_rx == NULL) {
			NAND_Print_DBG("Request rx DMA failed!\n");
			return -EINVAL;
	    }
	}

	return 0;
}

int spinand_releasetxdma(void)
{
	if (dma_hdl_tx != NULL) {
		printk("spinand release tx dma\n");
		dma_release_channel(dma_hdl_tx);
		dma_hdl_tx = NULL;
		return 0;
	}

	return 0;
}

int spinand_releaserxdma(void)
{
	if (dma_hdl_rx != NULL) {
		NAND_Print_DBG("spinand release rx dma\n");
		dma_release_channel(dma_hdl_rx);
		dma_hdl_rx = NULL;
		return 0;
	}

	return 0;
}

void spinand_dma_callback(void *arg)
{
	complete(&spinand_dma_done);
}

int tx_dma_config_start(dma_addr_t addr, __u32 length)
{
	struct dma_slave_config dma_conf = {0};
	struct dma_async_tx_descriptor *dma_desc = NULL;

	dma_conf.direction = DMA_MEM_TO_DEV;
	dma_conf.dst_addr = SPI_BASE_ADDR + SPI_TX_DATA_REG;

	dma_conf.src_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	dma_conf.dst_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	dma_conf.src_maxburst = 1;
	dma_conf.dst_maxburst = 1;

	/* dma_conf.slave_id = sunxi_slave_id(DRQDST_SPI0_TX, DRQSRC_SDRAM); */
	dma_conf.slave_id = sunxi_slave_id(DRQDST_SPI0TX, DRQSRC_SDRAM);
	dmaengine_slave_config(dma_hdl_tx, &dma_conf);

	dma_desc = dmaengine_prep_slave_single(dma_hdl_tx, addr, length,
				DMA_TO_DEVICE, DMA_PREP_INTERRUPT|DMA_CTRL_ACK);
	if (!dma_desc) {
		NAND_Print_DBG("tx dmaengine prepare failed!\n");
		return -1;
	}

	dma_desc->callback = spinand_dma_callback;
	dma_desc->callback_param = NULL;
	dmaengine_submit(dma_desc);

	dma_async_issue_pending(dma_hdl_tx);

	return 0;
}
int rx_dma_config_start(dma_addr_t addr, __u32 length)
{
	struct dma_slave_config dma_conf = {0};
	struct dma_async_tx_descriptor *dma_desc = NULL;

	dma_conf.direction = DMA_DEV_TO_MEM;
	dma_conf.src_addr = SPI_BASE_ADDR + SPI_RX_DATA_REG;

	dma_conf.src_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	dma_conf.dst_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	dma_conf.src_maxburst = 1;
	dma_conf.dst_maxburst = 1;
	/* dma_conf.slave_id = sunxi_slave_id(DRQDST_SDRAM, DRQSRC_SPI0_RX); */
	dma_conf.slave_id = sunxi_slave_id(DRQDST_SDRAM, DRQSRC_SPI0RX);
	dmaengine_slave_config(dma_hdl_rx, &dma_conf);

	dma_desc = dmaengine_prep_slave_single(dma_hdl_rx, addr, length,
				DMA_FROM_DEVICE, DMA_PREP_INTERRUPT|DMA_CTRL_ACK);
	if (!dma_desc) {
		NAND_Print_DBG("rx dmaengine prepare failed!\n");
		return -1;
	}

	dma_desc->callback = spinand_dma_callback;
	dma_desc->callback_param = NULL;
	dmaengine_submit(dma_desc);

	dma_async_issue_pending(dma_hdl_rx);

	return 0;
}

int spinand_dma_config_start(__u32 rw, __u32 addr, __u32 length)
{
	dma_map_addr = NAND_DMASingleMap(rw, (void *)addr, length);

	if (rw)
		tx_dma_config_start((dma_addr_t)dma_map_addr, length);
	else
		rx_dma_config_start((dma_addr_t)dma_map_addr, length);

	return 0;
}

int NAND_WaitDmaFinish(__u32 tx_flag, __u32 rx_flag)
{
	__u32 timeout = 2000;
	__u32 timeout_flag = 0;

	if (tx_flag || rx_flag) {
		timeout_flag = wait_for_completion_timeout(&spinand_dma_done, msecs_to_jiffies(timeout));
		if (!timeout_flag) {
			NAND_Print_DBG("wait dma finish timeout!!\n");
			return -1;
		}
	}

	return 0;
}

int Nand_Dma_End(__u32 rw, __u32 addr, __u32 length)
{

	NAND_DMASingleUnmap(rw, dma_map_addr, length);

	return 0;
}

int rawnand_dma_config_start(__u32 rw, __u32 addr, __u32 length)
{
#if 0
/*no use extern  DMA*/
	struct dma_slave_config dma_conf = { 0 };
	struct dma_async_tx_descriptor *dma_desc = NULL;

	dma_conf.direction = DMA_DEV_TO_MEM;
	dma_conf.src_addr = 0x01c03300;
	dma_conf.dst_addr = 0x01c03300;
	dma_conf.src_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	dma_conf.dst_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	dma_conf.src_maxburst = 1;
	dma_conf.dst_maxburst = 1;
	dma_conf.slave_id =
	    rw ? sunxi_slave_id(DRQDST_NAND0,
				DRQSRC_SDRAM) : sunxi_slave_id(DRQDST_SDRAM,
							       DRQSRC_NAND0);
	dmaengine_slave_config(dma_hdl, &dma_conf);

	dma_desc = dmaengine_prep_slave_single(dma_hdl, addr, length,
					       (rw ? DMA_TO_DEVICE :
						DMA_FROM_DEVICE),
					       DMA_PREP_INTERRUPT |
					       DMA_CTRL_ACK);
	if (!dma_desc) {
		NAND_Print("dmaengine prepare failed!\n");
		return -1;
	}

	dma_desc->callback = (void *)nand_dma_callback;
	if (rw == 0)
		dma_desc->callback_param = NULL;
	else
		dma_desc->callback_param = (void *)(dma_desc);

	dmaengine_submit(dma_desc);

	dma_async_issue_pending(dma_hdl);
#endif
	return 0;
}

int nand_dma_config_start(__u32 rw, __u32 addr, __u32 length)
{
	int ret = 0;

	if (get_storage_type() == 1)
		ret = rawnand_dma_config_start(rw, addr, length);
	else if (get_storage_type() == 2)
		ret = spinand_dma_config_start(rw, addr, length);

	return ret;
}

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
__u32 NAND_GetNdfcDmaMode(void)
{
	return 1;
}

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
__u32 NAND_GetNandExtPara(__u32 para_num)
{
	int ret = 0;
	int nand_para = 0xffffffff;

	if (para_num == 0) {	/*frequency */
		ret = of_property_read_u32(ndfc_dev->of_node, "nand0_p0",
					 &nand_para);
		if (ret) {
			NAND_Print("Failed to get nand_p0\n");
			return 0xffffffff;
		}

		if (nand_para == 0x55aaaa55) {
			NAND_Print_DBG("nand_p0 is no used\n");
			nand_para = 0xffffffff;
		} else
			NAND_Print_DBG("nand: get nand_p0 %x\n", nand_para);
	} else if (para_num == 1) {	/*SUPPORT_TWO_PLANE */
		ret = of_property_read_u32(ndfc_dev->of_node, "nand0_p1",
					 &nand_para);
		if (ret) {
			NAND_Print("Failed to get nand_p1\n");
			return 0xffffffff;
		}

		if (nand_para == 0x55aaaa55) {
			NAND_Print_DBG("nand_p1 is no used\n");
			nand_para = 0xffffffff;
		} else
			NAND_Print_DBG("nand : get nand_p1 %x\n", nand_para);
	} else if (para_num == 2) {	/*SUPPORT_VERTICAL_INTERLEAVE */
		ret = of_property_read_u32(ndfc_dev->of_node, "nand0_p2",
					 &nand_para);
		if (ret) {
			NAND_Print("Failed to get nand_p2\n");
			return 0xffffffff;
		}

		if (nand_para == 0x55aaaa55) {
			NAND_Print_DBG("nand_p2 is no used\n");
			nand_para = 0xffffffff;
		} else
			NAND_Print_DBG("nand : get nand_p2 %x\n", nand_para);
	} else if (para_num == 3) {	/*SUPPORT_DUAL_CHANNEL */
		ret = of_property_read_u32(ndfc_dev->of_node, "nand0_p3",
					 &nand_para);
		if (ret) {
			NAND_Print("Failed to get nand_p3\n");
			return 0xffffffff;
		}

		if (nand_para == 0x55aaaa55) {
			NAND_Print_DBG("nand_p3 is no used\n");
			nand_para = 0xffffffff;
		} else
			NAND_Print_DBG("nand: get nand_p3 %x\n", nand_para);
	} else {
		NAND_Print("NAND_GetNandExtPara: wrong para num: %d\n",
		       para_num);
		return 0xffffffff;
	}
	return nand_para;
}

__u32 NAND_GetNandIDNumCtrl(void)
{
	int ret;
	int id_number_ctl = 0;

	ret = of_property_read_u32(ndfc_dev->of_node, "nand0_id_number_ctl",
				 &id_number_ctl);
	if (ret) {
		NAND_Print_DBG("Failed to get id_number_ctl\n");
		id_number_ctl = 0;
	} else {
		if (id_number_ctl == 0x55aaaa55) {
			NAND_Print_DBG("id_number_ctl is no used\n");
			id_number_ctl = 0;
		} else
			NAND_Print_DBG("nand : get id_number_ctl %x\n",
				       id_number_ctl);
	}
	return id_number_ctl;
}

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
__u32 NAND_GetMaxChannelCnt(void)
{
	return 1;
}

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
int nand_request_dma(void)
{
	dma_cap_mask_t mask;

	NAND_Print_DBG("request DMA");

	/* Try to acquire a generic DMA engine slave channel */
	dma_cap_zero(mask);
	dma_cap_set(DMA_SLAVE, mask);

	if (dma_hdl == NULL) {
		dma_hdl = dma_request_channel(mask, NULL, NULL);
		if (dma_hdl == NULL) {
			NAND_Print("Request DMA failed!\n");
			return -EINVAL;
		}
	}
	NAND_Print_DBG("chan_id: %d", dma_hdl->chan_id);

	return 0;
}

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
int NAND_ReleaseDMA(__u32 nand_index)
{
	if (dma_hdl != NULL) {
		NAND_Print_DBG("nand release dma\n");
		dma_release_channel(dma_hdl);
		dma_hdl = NULL;
		return 0;
	}

	return 0;
}

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
__u32 NAND_GetNdfcVersion(void)
{
	return 1;
}

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
void *RAWNAND_GetIOBaseAddrCH0(void)
{
	return NDFC0_BASE_ADDR;
}

void *RAWNAND_GetIOBaseAddrCH1(void)
{
	return NDFC1_BASE_ADDR;
}
void *SPINAND_GetIOBaseAddrCH0(void)
{
	return SPIC0_IO_BASE;
}

void *SPINAND_GetIOBaseAddrCH1(void)
{
	return NULL;
}


/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         : wait rb
*****************************************************************************/
static DECLARE_WAIT_QUEUE_HEAD(NAND_RB_WAIT_CH0);
static DECLARE_WAIT_QUEUE_HEAD(NAND_RB_WAIT_CH1);

__s32 nand_rb_wait_time_out(__u32 no, __u32 *flag)
{
	__s32 ret;

	if (no == 0)
		ret = wait_event_timeout(NAND_RB_WAIT_CH0, *flag, HZ >> 1);
	else
		ret = wait_event_timeout(NAND_RB_WAIT_CH1, *flag, HZ >> 1);

	return ret;
}

__s32 nand_rb_wake_up(__u32 no)
{
	if (no == 0)
		wake_up(&NAND_RB_WAIT_CH0);
	else
		wake_up(&NAND_RB_WAIT_CH1);

	return 0;
}

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         : wait dma
*****************************************************************************/
static DECLARE_WAIT_QUEUE_HEAD(NAND_DMA_WAIT_CH0);
static DECLARE_WAIT_QUEUE_HEAD(NAND_DMA_WAIT_CH1);

__s32 nand_dma_wait_time_out(__u32 no, __u32 *flag)
{
	__s32 ret;

	if (no == 0)
		ret = wait_event_timeout(NAND_DMA_WAIT_CH0, *flag, HZ >> 1);
	else
		ret = wait_event_timeout(NAND_DMA_WAIT_CH1, *flag, HZ >> 1);

	return ret;
}

__s32 nand_dma_wake_up(__u32 no)
{
	if (no == 0)
		wake_up(&NAND_DMA_WAIT_CH0);
	else
		wake_up(&NAND_DMA_WAIT_CH1);

	return 0;
}

__u32 nand_dma_callback(void *para)
{
	return 0;
}

int NAND_get_storagetype(void)
{
	return 0;
}

int NAND_GetVoltage(void)
{

	int ret = 0;
	const char *sti_vcc_nand = NULL;
	const char *sti_vcc_io = NULL;

	ret = of_property_read_string(ndfc_dev->of_node, "nand0_regulator1",
				    &sti_vcc_nand);
	NAND_Print_DBG("nand0_regulator1 %s\n", sti_vcc_nand);
	if (ret)
		NAND_Print_DBG("Failed to get vcc_nand\n");

	regu1 = regulator_get(NULL, sti_vcc_nand);
	if (IS_ERR(regu1))
		NAND_Print_DBG("nand:fail to get regulator vcc-nand!\n");
	else {
		/*enable regulator */
		ret = regulator_enable(regu1);
		if (IS_ERR(regu1)) {
			NAND_Print_DBG("nand:fail to enable regulator vcc-nand!\n");
			return -1;
		}
		NAND_Print_DBG("nand:get voltage vcc-nand ok:%p\n", regu1);
	}

	if (get_storage_type() == 1) {
		ret = of_property_read_string(ndfc_dev->of_node,
			 "nand0_regulator2", &sti_vcc_io);
		NAND_Print_DBG("nand0_regulator2 %s\n", sti_vcc_io);
		if (ret)
			NAND_Print_DBG("Failed to get vcc_io\n");

		regu2 = regulator_get(NULL, sti_vcc_io);
		if (IS_ERR(regu2))
			NAND_Print_DBG("nand:fail to get regulator vcc-io!\n");
		else {
			/*enable regulator */
			ret = regulator_enable(regu2);
			if (IS_ERR(regu2)) {
				NAND_Print
				    ("fail to enable regulator vcc-io!\n");
				return -1;
			}
			NAND_Print_DBG("nand:get voltage vcc-io ok:%p\n", regu2);
		}
	}

	NAND_Print_DBG("nand:has already get voltage\n");

	return ret;

}

int NAND_ReleaseVoltage(void)
{
	int ret = 0;

	if (!IS_ERR(regu1)) {
		NAND_Print_DBG("nand release voltage vcc-nand\n");
		ret = regulator_disable(regu1);
		if (ret)
			NAND_Print_DBG("nand: regu1 disable fail, ret 0x%x\n", ret);
		if (IS_ERR(regu1))
			NAND_Print_DBG("nand: fail to disable regulator vcc-nand!");

		/*put regulator when module exit */
		regulator_put(regu1);

		regu1 = NULL;
	}

	if (get_storage_type() == 1) {
		if (!IS_ERR(regu2)) {
			NAND_Print_DBG("nand release voltage vcc-io\n");
			ret = regulator_disable(regu2);
			if (ret)
				NAND_Print_DBG("nand: regu2 disable fail,ret 0x%x\n", ret);
			if (IS_ERR(regu2))
				NAND_Print_DBG("nand: fail to disable regulator vcc-io!\n");

			/*put regulator when module exit */
			regulator_put(regu2);

			regu2 = NULL;
		}
	}

	NAND_Print_DBG("nand had already release voltage\n");

	return ret;

}

int NAND_IS_Secure_sys(void)
{
	if (sunxi_soc_is_secure()) {
		NAND_Print_DBG("secure system\n");
		return 1;
	}
		NAND_Print_DBG("non secure\n");

		return 0;
}

__u32 NAND_Print_level(void)
{
	int ret;
	int print_level = 0xffffffff;

	ret = of_property_read_u32(ndfc_dev->of_node, "nand0_print_level",
				 &print_level);
	if (ret) {
		NAND_Print_DBG("Failed to get print_level\n");
		print_level = 0xffffffff;
	} else {
		if (print_level == 0x55aaaa55) {
			NAND_Print_DBG("print_level is no used\n");
			print_level = 0xffffffff;
		} else
			NAND_Print_DBG("nand : get print_level %x\n", print_level);
	}

	return print_level;
}

int NAND_Get_Dragonboard_Flag(void)
{
	int ret;
	int dragonboard_flag = 0;

	ret = of_property_read_u32(ndfc_dev->of_node, "nand0_dragonboard",
				 &dragonboard_flag);
	if (ret) {
		NAND_Print_DBG("Failed to get dragonboard_flag\n");
		dragonboard_flag = 0;
	} else {
		NAND_Print_DBG("nand: dragonboard_flag %x\n", dragonboard_flag);
	}

	return dragonboard_flag;
}

void NAND_Print_Version(void)
{
	int val[4] = { 0 };

	val[0] = NAND_DRV_VERSION_0;
	val[1] = NAND_DRV_VERSION_1;
	val[2] = NAND_DRV_DATE;
	val[3] = NAND_DRV_TIME;

	NAND_Print("kernel: nand version: %x %x %x %x\n", val[0],
	       val[1], val[2], val[3]);
}

int NAND_Get_Version(void)
{
	return NAND_DRV_DATE;
}

void nand_cond_resched(void)
{
	cond_resched();
}

__u32 get_storage_type(void)
{
	__u32 ret = 0;

	if (nand_type == 1)
		ret = 1;
	else if (nand_type == 2)
		ret = 2;

	return ret;
}
