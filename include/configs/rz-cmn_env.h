/*
 * Common env helpers for Renesas RZ boards:
 * - Board DTB selection (table-driven)
 * - Device Tree overlay application (flag-driven + list)
 *
 * Copyright (c) 2026, Renesas Electronics Corporation. All rights reserved.
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __RZ_CMN_ENV_H__
#define __RZ_CMN_ENV_H__

/* OpenCVA firmware load support (V2H only) */
#define RZ_OPENCVA_ADDR        "0xC0000000"
#define RZ_OPENCVA_FILE        "OpenCV_Bin.bin"

/* Load OpenCVA firmware to reserved memory */
#define RZ_OPENCVA_LOAD_CMD \
		"echo Loading OpenCVA firmware: " RZ_OPENCVA_FILE " -> " RZ_OPENCVA_ADDR "; " \
		"if ext4load mmc ${rootfs_mmcdev}:${rootfs_mmcpart} " RZ_OPENCVA_ADDR " boot/"RZ_OPENCVA_FILE "; then " \
				"echo OpenCVA firmware loaded.; " \
		"else " \
				"echo WARN: missing OpenCVA firmware: " RZ_OPENCVA_FILE "; " \
		"fi; "

/* Load OpenCVA firmware only on V2H boards */
#define RZ_OPENCVA_LOAD_IF_V2H \
		"if test \"${model_string}\" = \"imdt-v2h-sbc\" -o " \
				"\"${model_string}\" = \"rzv2h-evk\" -o " \
				"\"${model_string}\" = \"rzv2h-rdk\"; then " \
				RZ_OPENCVA_LOAD_CMD \
		"fi; "

/* Apply a single overlay by filename (relative to ${overlaydir}) */
#define RZ_OVERLAY_APPLY_ONE(dtbo_name) \
	"echo Applying DT overlay: " dtbo_name "; " \
	"if fatload mmc ${mmcdev}:${mmcpart} ${dtbo_addr} ${overlaydir}/" dtbo_name "; then " \
		"fdt addr ${dtb_addr}; fdt apply ${dtbo_addr}; " \
	"else " \
		"echo WARN: missing overlay: ${overlaydir}/" dtbo_name "; " \
	"fi; "

/* Apply overlay if env flag exists and is 1/yes */
#define RZ_OVERLAY_IF_FLAG(flag, dtbo_name) \
	"if env exists " flag " && test ${" flag "} = 1 -o ${" flag "} = yes; then " \
		RZ_OVERLAY_APPLY_ONE(dtbo_name) \
	"fi; "

/* Loop through optional list in $fdt_extra_overlays */
#define RZ_OVERLAY_APPLY_LIST \
	"if env exists fdt_extra_overlays && test -n ${fdt_extra_overlays}; then " \
		"for dtbo_file in ${fdt_extra_overlays}; do " \
			"echo Applying DT overlay: ${dtbo_file}; " \
			"if fatload mmc ${mmcdev}:${mmcpart} ${dtbo_addr} ${overlaydir}/${dtbo_file}; then " \
				"fdt addr ${dtb_addr}; fdt apply ${dtbo_addr}; " \
			"else " \
				"echo WARN: missing overlay: ${overlaydir}/${dtbo_file}; " \
			"fi; " \
		"done; " \
	"fi; "

/* Board selection cases */
#define RZ_FDT_CASE(model, maj, min, dtb) \
	"elif test \"${model_string}\" = \"" model "\" && " \
		"test \"${revision_major}\" = \"" maj "\" && " \
		"test \"${revision_minor}\" = \"" min "\"; then " \
		"setenv fdtfile " dtb "; "

#define RZ_FDT_SELECT_BEGIN "if false; then ; "
#define RZ_FDT_SELECT_END   "else echo WARN: unknown board ${model_string}-${revision_major}.${revision_minor}; fi; "

/* Built-in DTB table — add more boards here */
#ifndef RZ_FDT_SELECT_TABLE
#define RZ_FDT_SELECT_TABLE \
	RZ_FDT_CASE("rs-g2l100", "1", "0", "rs-g2l100.dtb") \
	RZ_FDT_CASE("rzg2l-sbc", "1", "0", "rzg2l-sbc.dtb") \
	RZ_FDT_CASE("rzg2l-evk", "1", "0", "rzg2l-evk.dtb") \
	RZ_FDT_CASE("rzv2l-evk", "1", "0", "rzv2l-evk.dtb") \
	RZ_FDT_CASE("rzv2h-evk", "1", "0", "rzv2h-evk-ver1.dtb") \
	RZ_FDT_CASE("rzv2h-evk", "2", "0", "rzv2h-evk-ver1.dtb") \
	RZ_FDT_CASE("rzv2h-rdk", "1", "0", "rzv2h-rdk-ver1.dtb") \
	RZ_FDT_CASE("imdt-v2h-sbc", "1", "0", "imdt-v2h-sbc.dtb") \
	RZ_FDT_CASE("sparrow-hawk", "1", "0", "sparrow-hawk.dtb")
#endif

/* Built-in overlay flags — add more overlays here */
#ifndef RZ_OVERLAY_FLAGS_TABLE
#define RZ_OVERLAY_FLAGS_TABLE \
	RZ_OVERLAY_IF_FLAG("enable_overlay_i2c",          "${model_string}-${revision_major}.${revision_minor}-ext-i2c.dtbo") \
	RZ_OVERLAY_IF_FLAG("enable_overlay_spi",          "${model_string}-${revision_major}.${revision_minor}-ext-spi.dtbo") \
	RZ_OVERLAY_IF_FLAG("enable_overlay_can",          "${model_string}-${revision_major}.${revision_minor}-can.dtbo") \
	RZ_OVERLAY_IF_FLAG("enable_overlay_dsi",          "${model_string}-${revision_major}.${revision_minor}-dsi.dtbo") \
	RZ_OVERLAY_IF_FLAG("enable_overlay_audio_codec",  "${model_string}-${revision_major}.${revision_minor}-audio-codec.dtbo") \
	RZ_OVERLAY_IF_FLAG("enable_overlay_csi_ov5640",   "${model_string}-${revision_major}.${revision_minor}-ov5640.dtbo") \
	RZ_OVERLAY_IF_FLAG("enable_overlay_csi_ov5645",   "${model_string}-${revision_major}.${revision_minor}-cru-csi-ov5645.dtbo") \
	RZ_OVERLAY_IF_FLAG("enable_overlay_csi22_ar1335",  "${model_string}-${revision_major}.${revision_minor}-cru-csi22-ar1335.dtbo") \
	RZ_OVERLAY_IF_FLAG("enable_overlay_csi23_ar1335",  "${model_string}-${revision_major}.${revision_minor}-cru-csi23-ar1335.dtbo") \
	RZ_OVERLAY_IF_FLAG("enable_overlay_csi_j1_imx219",   "${model_string}-${revision_major}.${revision_minor}-cru-csi-j1-imx219.dtbo") \
	RZ_OVERLAY_IF_FLAG("enable_overlay_csi_j2_imx219",   "${model_string}-${revision_major}.${revision_minor}-cru-csi-j2-imx219.dtbo") \
	RZ_OVERLAY_IF_FLAG("enable_overlay_csi_j2_imx708",   "${model_string}-${revision_major}.${revision_minor}-cru-csi-j2-imx708.dtbo") \
	RZ_OVERLAY_IF_FLAG("enable_overlay_uio",   "${model_string}-${revision_major}.${revision_minor}-uio.dtbo")
#endif

/* Sparrow-Hawk V4H secure-monitor handoff (partition 1) */
#define RCAR_V4H_BL31_ADDR       "0x46400000"
#define RCAR_V4H_BL31_FILE       "bl31-sparrow-hawk.bin"

/* Load the BL31 payload without BL32/TEE, then hand off to TF-A */
#define RCAR_V4H_BL31_PREPARE_CMD \
		"if tfa_status; then " \
			"echo Using active TF-A handoff; " \
		"elif fatload mmc ${mmcdev}:${mmcpart} " RCAR_V4H_BL31_ADDR " " \
				RCAR_V4H_BL31_FILE " && " \
			"setexpr v4h_bl31_size ${filesize}; then " \
			"tfa_prepare " RCAR_V4H_BL31_ADDR " ${v4h_bl31_size}; " \
		"else " \
			"echo ERROR: missing Sparrow-Hawk BL31 payload; " \
			"test 1 = 0; " \
		"fi; "

/* Prepare TF-A handoff only on Sparrow-Hawk, boot normally otherwise */
#define RCAR_V4H_BL31_PREPARE_IF_SPARROWHAWK \
		"if test \"${model_string}\" = \"sparrow-hawk\"; then " \
			RCAR_V4H_BL31_PREPARE_CMD \
		"else " \
			"test 0 = 0; " \
		"fi; "

/* Image selection cases */
#define RZ_IMAGE_SELECT_BEGIN \
		"if test -z \"${image_flavor}\" || test \"${image_flavor}\" = \"normal\"; then " \
				"setenv kernel_image Image; "

#define RZ_IMAGE_SELECT_END \
		"else " \
				"echo WARN: unknown image_flavor=${image_flavor}, fallback to normal; " \
				"setenv image_flavor normal; " \
				"setenv kernel_image Image; " \
		"fi; \0"

#define RZ_IMAGE_CASE(image_flavor, kernel_image) \
	"elif test \"${image_flavor}\" = \"" image_flavor "\"; then " \
		"setenv kernel_image " kernel_image "; "

#define RZ_IMAGE_SELECT_TABLE \
	RZ_IMAGE_CASE("preempt_rt", "Image-preempt_rt") \
	RZ_IMAGE_CASE("nonpreempt", "Image-nonpreempt")

#define RZ_ENV_DEFAULTS \
	"overlaydir=dtb/renesas/overlays\0" \
	"v4h_bl31_prepare=" RCAR_V4H_BL31_PREPARE_IF_SPARROWHAWK "\0" \
	"v4h_bl31_boot=" \
		"if run v4h_bl31_prepare; then " \
			"booti ${image_addr} - ${dtb_addr}; " \
		"else " \
			"echo ERROR: secure monitor is required for Sparrow-Hawk SMP; " \
		"fi; \0" \
	"image_select=" \
		RZ_IMAGE_SELECT_BEGIN \
		RZ_IMAGE_SELECT_TABLE \
		RZ_IMAGE_SELECT_END \
	"fdt_select=" \
		"if env exists fdtfile && test -n ${fdtfile}; then " \
			"; " \
		"else " \
			RZ_FDT_SELECT_BEGIN \
			RZ_FDT_SELECT_TABLE \
			RZ_FDT_SELECT_END \
			"if env exists fdt_user_cases; then run fdt_user_cases; fi; " \
		"fi; \0" \
	"fdt_load=run fdt_select; " \
		"fatload mmc ${mmcdev}:${mmcpart} ${dtb_addr} dtb/renesas/${fdtfile}; \0" \
	"fdt_ovrun=if run fdt_load; then " \
			"fdt addr ${dtb_addr}; fdt resize 0x10000; " \
			RZ_OVERLAY_FLAGS_TABLE \
			RZ_OVERLAY_APPLY_LIST \
			"if env exists overlay_user_cases; then run overlay_user_cases; fi; " \
		"else echo WARN: Cannot load base DT; fi; \0" \
	"opencva_load=" RZ_OPENCVA_LOAD_IF_V2H "\0" \
	"mmc_do_boot=run mmc_args; run image_select; " \
		"fatload mmc ${mmcdev}:${mmcpart} ${image_addr} ${kernel_image}; " \
		"run fdt_ovrun; " \
		"run opencva_load; " \
		"if test \"${model_string}\" = \"sparrow-hawk\"; then " \
			"run v4h_bl31_boot; " \
		"else " \
			"booti ${image_addr} - ${dtb_addr}; " \
		"fi\0"

#endif /* __RZ_CMN_ENV_H__ */
