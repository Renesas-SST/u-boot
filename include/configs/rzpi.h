/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2015 Renesas Electronics Corporation
 */

#ifndef __RZPI_H
#define __RZPI_H

#include <asm/arch/rmobile.h>

//Set the correct memory of the board
#define ISSI_4Gb_DDR 0
#define ISSI_8Gb_DDR 1

#define CONFIG_REMAKE_ELF

#ifdef CONFIG_SPL
#define CONFIG_SPL_TARGET	"spl/u-boot-spl.scif"
#endif

/* boot option */

#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG

/* Generic Interrupt Controller Definitions */
/* RZ/G2L use GIC-v3 */
#define CONFIG_GICV3
#define GICD_BASE	0x11900000
#define GICR_BASE	0x11960000

/* console */
#define CONFIG_SYS_CBSIZE		2048
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE
#define CONFIG_SYS_MAXARGS		64
#define CONFIG_SYS_BAUDRATE_TABLE	{ 115200, 38400 }

/* PHY needs a longer autoneg timeout */
#define PHY_ANEG_TIMEOUT		20000

/* MEMORY */
#define CONFIG_SYS_INIT_SP_ADDR		CONFIG_SYS_TEXT_BASE

/* SDHI clock freq */
#define CONFIG_SH_SDHI_FREQ		133000000

#define DRAM_RSV_SIZE			0x08000000
#define CONFIG_SYS_SDRAM_BASE		(0x40000000 + DRAM_RSV_SIZE)
#define CONFIG_SYS_SDRAM_SIZE		(0x80000000u - DRAM_RSV_SIZE) //total 2GB
#define CONFIG_SYS_LOAD_ADDR		0x58000000
#define CONFIG_LOADADDR			CONFIG_SYS_LOAD_ADDR // Default load address for tfpt,bootp...
#define CONFIG_VERY_BIG_RAM
#define CONFIG_MAX_MEM_MAPPED		(0x80000000u - DRAM_RSV_SIZE)

#define CONFIG_SYS_MONITOR_BASE		0x00000000
#define CONFIG_SYS_MONITOR_LEN		(1 * 1024 * 1024)
#define CONFIG_SYS_MALLOC_LEN		(64 * 1024 * 1024)
#define CONFIG_SYS_BOOTM_LEN		(64 << 20)

/* The HF/QSPI layout permits up to 1 MiB large bootloader blob */
#define CONFIG_BOARD_SIZE_LIMIT		1048576

/* ENV setting */
/* Support uEnv.txt to pass environment variables (and device tree overlays) to the kernel */
#ifndef CONFIG_CMD_IMPORTENV
#define CONFIG_CMD_IMPORTENV
#endif
#define RZPI_UENV_FDTO_SUPPORT

#ifndef RZPI_UENV_FDTO_SUPPORT

#define CONFIG_EXTRA_ENV_SETTINGS \
	"bootm_size=0x10000000 \0" \
	"prodsdbootargs=setenv bootargs rw rootwait earlycon root=/dev/mmcblk0p2\0" \
	"bootimage=booti 0x48080000 - 0x48000000\0" \
	"bootcmd_load=ext4load mmc 0:2 0x48080000 boot/Image;ext4load mmc 0:2 0x48000000 boot/rzpi.dtb;run prodsdbootargs\0" \
	"fiperase=sf erase 1d000 b0000\0" \
	"fipload=mmc rescan;sf probe; fatload mmc 0:1 $loadaddr fip.bin\0" \
	"fipwrite=sf write $loadaddr 1d200 $filesize\0" \
	"ethrotate=no\0" \
	"ethact=ethernet@11c30000\0" /* The short connector. */

#define CONFIG_BOOTCOMMAND	"run bootcmd_load;run bootimage"

#else

#define CONFIG_EXTRA_ENV_SETTINGS \
	"fdtfile=boot/uEnv.txt\0" \
	"image=Image \0" \
	"mmcdev=0\0" \
	"mmcpart=2\0" \
	"dtb_addr=0x48000000 \0" \
	"dtbo_addr=0x48010000\0" \
	"image_addr=0x48080000 \0" \
	"env_addr=0x58000000 \0" \
	"importbootenv=echo Importing environment from mmc${mmcdev} ...; " \
		"env import -t ${env_addr} ${filesize}\0" \
	"loadbootenv=ext4load mmc ${mmcdev}:${mmcpart} ${env_addr} ${fdtfile}\0" \
	"envboot=mmc dev ${mmcdev}; " \
		"if mmc rescan; then " \
			"echo SD/MMC found on device ${mmcdev};" \
			"if run loadbootenv; then " \
				"echo Loaded env from ${fdtfile};" \
				"run importbootenv;" \
			"fi;" \
		"fi;\0" \
	"bootimage=booti ${image_addr} - ${dtb_addr} \0"

#define CONFIG_BOOTCOMMAND	"run envboot;run prodsdboot"

#endif

/* For board */
/* Ethernet RAVB */
#define CONFIG_BITBANGMII_MULTI

#endif /* __RZPI_H */
