/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * include/configs/sparrowhawk.h
 *     This file is Sparrow Hawk board configuration.
 *
 * Copyright (C) 2025 Marek Vasut <marek.vasut+renesas@mailbox.org>
 */

#ifndef __SPARROWHAWK_H
#define __SPARROWHAWK_H

#include "rcar-gen4-common.h"

/* Sparrow-Hawk BID/platform settings are stored at xSPI offset 0x2c0000. */
#define RCAR_V4H_SPL_PLATFORM_SETTINGS_OFFSET	0x2C0000

#endif /* __SPARROWHAWK_H */
