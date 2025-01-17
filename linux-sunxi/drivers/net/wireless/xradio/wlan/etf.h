/*
 * Common interfaces for XRadio drivers
 *
 * Copyright (c) 2013
 * Xradio Technology Co., Ltd. <www.xradiotech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef XRADIO_ETF_H
#define XRADIO_ETF_H
#include <linux/version.h>

/* ETF Adapter layer */
#define  NL_FOR_XRADIO  30
#define  ETF_QUEUEMODE  1

#define  ADAPTER_RX_BUF_LEN  (528*3) /* pay respect for SDIO_BLOCK_SIZE*/
#if (ETF_QUEUEMODE)
#define  ADAPTER_ITEM_MAX    (0x1f)
#define  XRADIO_ADAPTER  ("xradio_etf")
struct  adapter_item {
	struct list_head head;
	void   *skb;
};
#endif

typedef int (*msg_proc)(void *data, int len);
struct xradio_adapter {
	struct sock *sock;
	msg_proc  handler;
	void     *msg_buf;
	int       msg_len;
#if (ETF_QUEUEMODE)
	struct adapter_item rx_items[ADAPTER_ITEM_MAX];
	struct list_head rx_queue;
	struct list_head rx_pool;
	spinlock_t  recv_lock;
	atomic_t    rx_cnt;
	struct semaphore proc_sem;
	struct task_struct *thread_tsk;
	int    exit;
#endif
	int       send_pid;
	struct    semaphore send_lock;
};

/* ETF internal connect state*/
#define ETF_STAT_NULL        0
#define ETF_STAT_CONNECTING  1
#define ETF_STAT_CONNECTED   2
struct xradio_etf {
	int    etf_state;
	int    is_wlan;
	void  *core_priv;
	struct semaphore etf_lock;
	struct xradio_adapter *adapter;
	int    seq_send;
	int    seq_recv;
	u8    *fw_path;
	u8    *sdd_path;
};

/* ETF fw cmd defines*/
#define  ETF_REQ_BASE               0x0000
#define  ETF_CNF_BASE               0x0400
#define  ETF_IND_BASE               0x0800
#define  ETF_MSG_STARTUP_IND_ID     0x0801
#define  ETF_DOWNLOAD_SDD          (0x20 + 22)
#define  ETF_HWT_REQ                0x0004
struct etf_sdd_req {
	u16    len;
	u16    id;
	u32    sdd_cmd;
};

/* ETF driver cmd defines */
#define  ETF_DRIVER_CMD_START_ID  (ETF_REQ_BASE + 0x03F0)
#define  ETF_GET_API_CONTEXT_ID   (ETF_REQ_BASE + 0x03F8)
#define  ETF_GET_SDD_PARAM_ID     (ETF_REQ_BASE + 0x03F9)
#define  ETF_CONNECT_ID           (ETF_REQ_BASE + 0x03FA)
#define  ETF_DISCONNECT_ID        (ETF_REQ_BASE + 0x03FB)
#define  ETF_RECONNECT_ID         (ETF_REQ_BASE + 0x03FC)
#define  ETF_DOWNLOAD_SDD_ID      (ETF_REQ_BASE + 0x03FD)
#define  ETF_DOWNLOAD_ID          (ETF_REQ_BASE + 0x03FE)
#define  ETF_SOFT_RESET_REQ_ID    (ETF_REQ_BASE + 0x03FF)
#define  ETF_DRIVER_IND_ID        (ETF_IND_BASE + 0x03FF)
#define  ETF_GET_SDD_POWER_DEFT   (ETF_REQ_BASE + 0x1FFF)

#define  FLAG_GET_SDD_ALL         0x1
struct get_sdd_param_req {
	u16    len;
	u16    id;
	u8     flags;
	u8     ies;
};
struct get_sdd_result {
	u16    len;
	u16    id;
	u32    result;
	u32    length;
	/* sdd data follow */
};

struct etf_api_context_req {
	u16    len;
	u16    id;
	u32    param;
};

#define HI_SW_LABEL_MAX	     128
struct etf_api_context_result {
	u16    len;
	u16    id;
	u32    result;
	u8     is_etf_fw_run;
	u8     reversed[3];
	u32    mib_baseaddr;
	u8     fw_label[HI_SW_LABEL_MAX];
	u16    fw_api_ver;
};

#define HI_MAX_CONFIG_TABLES	4
struct etf_api_context {
	u32    config[HI_MAX_CONFIG_TABLES];
};

/* boot State, for download file from GUI */
#define BOOT_STATE_NULL              0
#define BOOT_WAITING_DOWNLOAD        1
#define BOOT_IN_PROGRESS             2
#define BOOT_COMPLETE                3
#define BOOT_SDD_COMPLETE            4

/* Driver Result code */
#define BOOT_SUCCESS                 (0)
#define BOOT_ERR_DECODE              (1)
#define BOOT_ERR_CHECKSUM            (2)
#define BOOT_ERR_FILE_SIZE           (3)
#define BOOT_ERR_BAD_OP              (4)
#define ETF_ERR_CONNECTED            (5)
#define ETF_ERR_WLAN_MODE            (6)
#define ETF_ERR_NOT_CONNECT          (7)
#define ETF_ERR_IO_FAILED            (8)
#define ETF_ERR_DRIVER_HANG          (9)

struct drv_resp {
	u16    len;
	u16    id;
	u32    state;
	u32    result;
};

#define  MSG_ID(x)     ((u16)((x)&0x1fff))
#define  MSG_SEQ(x)    (((x)>>13)&0x0007)
#define  SEQ_MASK(x)   ((x) & 0x0007)

#define DOWNLOAD_F_START          0x01
#define DOWNLOAD_F_END            0x02
#define DOWNLOAD_F_PATH_ONLY      0x04
struct drv_download {
	unsigned long   offset;
	unsigned long   flags;
	/* data followed, max size is HI_MEM_BLK_BYTES */
};

#ifdef USE_VFS_FIRMWARE
#define XR819_ETF_FIRMWARE   ("/lib/firmware/etf_xr819.bin")
#else
#define XR819_ETF_FIRMWARE   ("etf_xr819.bin")
#endif

/* ETF interfaces called by WLAN core */
int xradio_etf_init(void);
void xradio_etf_deinit(void);
const char *etf_get_fwpath(void);
const char *etf_get_sddpath(void);
bool etf_is_connect(void);
void etf_set_core(void *core_priv);
int xradio_etf_from_device(struct sk_buff **skb);
void xradio_etf_save_context(void *buf, int len);
int xradio_etf_suspend(void);
int xradio_etf_resume(void);

/* External interfaces called by etf */
extern int  xradio_core_init(void);
extern void xradio_core_deinit(void);

#endif /*XRADIO_ETF_H*/
