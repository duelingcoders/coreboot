/*
 * This file is part of the coreboot project.
 *
 * Copyright 2013 Google Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <bootblock_common.h>
#include <arch/cache.h>

#include "clk.h"
#include "wakeup.h"
#include "cpu.h"

/* convenient shorthand (in MB) */
#define SRAM_START	(EXYNOS5_SRAM_BASE >> 20)
#define SRAM_SIZE	1
#define SRAM_END	(SRAM_START + SRAM_SIZE)	/* plus one... */

void bootblock_soc_init(void)
{
	/* kick off the multi-core timer.
	 * We want to do this as early as we can.
	 */
	mct_start();

	if (get_wakeup_state() == WAKEUP_DIRECT) {
		wakeup();
		/* Never returns. */
	}

	/* set up dcache and MMU */
	mmu_init();
	mmu_disable_range(0, SRAM_START);
	mmu_config_range(SRAM_START, SRAM_SIZE, DCACHE_WRITEBACK);
	mmu_config_range(SRAM_END, 4096 - SRAM_END, DCACHE_OFF);
	dcache_mmu_enable();

	/* For most ARM systems, we have to initialize firmware media source
	 * (ex, SPI, SD/MMC, or eMMC) now; but for Exynos platform, that is
	 * already handled by iROM so there's no need to setup again.
	 */
}
