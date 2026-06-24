/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <fsl_clock.h>
#include <fsl_flexspi.h>
#include <soc.h>
#include <errno.h>
#include <zephyr/irq.h>
#include <zephyr/dt-bindings/clock/imx_ccm_rev2.h>

static void __flexspi_clock_root_update(FLEXSPI_Type *flexspi, uint32_t divider, bool update_mux, uint8_t mux)
{
	clock_ip_name_t clk_gate;
	clock_root_t flexspi_clk;

	if (flexspi == FLEXSPI1) {
		flexspi_clk = kCLOCK_Root_Flexspi1;
		clk_gate = kCLOCK_Flexspi1;
	} else if (flexspi == FLEXSPI2) {
		flexspi_clk = kCLOCK_Root_Flexspi2;
		clk_gate = kCLOCK_Flexspi2;
	} else {
		return;
	}

	while (FLEXSPI_GetBusIdleStatus(flexspi) == false) {
		/* Spin */
	}

	FLEXSPI_Enable(flexspi, false);

	CLOCK_DisableClock(clk_gate);

	if (update_mux)
		CLOCK_SetRootClockMux(flexspi_clk, mux);

	CLOCK_SetRootClockDiv(flexspi_clk, divider);

	CLOCK_EnableClock(clk_gate);

	FLEXSPI_Enable(flexspi, true);

	FLEXSPI_SoftwareReset(flexspi);
}

uint32_t flexspi_clock_set_freq(uint32_t clock_name, uint32_t rate)
{
	clock_name_t root;
	uint32_t root_rate;
	FLEXSPI_Type *flexspi;
	clock_root_t flexspi_clk;
	uint32_t divider;

	switch (clock_name) {
	case IMX_CCM_FLEXSPI_CLK:
		flexspi_clk = kCLOCK_Root_Flexspi1;
		flexspi = (FLEXSPI_Type *)DT_REG_ADDR(DT_NODELABEL(flexspi));
		break;
	case IMX_CCM_FLEXSPI2_CLK:
		flexspi_clk = kCLOCK_Root_Flexspi2;
		flexspi = (FLEXSPI_Type *)DT_REG_ADDR(DT_NODELABEL(flexspi2));
		break;
	default:
		return -ENOTSUP;
	}
	root = CLOCK_GetRootClockSource(flexspi_clk,
			CLOCK_GetRootClockMux(flexspi_clk));
	/* Get clock root frequency */
	root_rate = CLOCK_GetFreq(root);
	/* Select a divider based on root clock frequency. We round the
	 * divider up, so that the resulting clock frequency is lower than
	 * requested when we can't output the exact requested frequency
	 */
	divider = ((root_rate + (rate - 1)) / rate);
	/* Cap divider to max value */
	divider = MIN(divider, CCM_CLOCK_ROOT_CONTROL_DIV_MASK);

	__flexspi_clock_root_update(flexspi, divider, false, 0U);

	return 0;
}

void flexspi_clock_root_update(FLEXSPI_Type *flexspi, uint8_t src, uint32_t divider)
{
	__flexspi_clock_root_update(flexspi, divider, true, src);
}
