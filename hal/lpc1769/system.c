/* -----------------------------------------------------------------------------
 * hal.c                                           (c) 2017 Rocks
 * -----------------------------------------------------------------------------
 * Author: Gertjan Rocks
 * Web:    www.gertjanrocks.com
 * Mail:   gertjanrocks@outlook.com
 * -----------------------------------------------------------------------------
 * Description: 
 * -----------------------------------------------------------------------------
 * Created on: 19 mrt. 2017
 * -----------------------------------------------------------------------------
 */


/* --------------*
 * Include files *
 * --------------*
 */

#include "device.h"

#ifdef LPC1769
#include "system.h"

/* -------------------------------*
 * Constant and macro definitions *
 * -------------------------------*
 */
const uint32_t OscRateIn = 12000000;
const uint32_t RTCOscRateIn = 32768;

/* -----------------*
 * Type definitions *
 * -----------------*
 */


/* ---------------------*
 * File-scope variables *
 * ---------------------*
 */


/* ----------------------*
 * Function declarations *
 * ----------------------*
 */
static void Board_SetupClocking(void);

/* ----------------------*
 * Function definitions  *
 * ----------------------*
 */

/**
 * init all system functions
 */
void system_init(void){
	//Init LPC1769 specific
	unsigned int *pSCB_VTOR = (unsigned int *) 0xE000ED08;

	#if defined(__IAR_SYSTEMS_ICC__)
		extern void *__vector_table;

		*pSCB_VTOR = (unsigned int) &__vector_table;
	#elif defined(__CODE_RED)
		extern void *g_pfnVectors;

		*pSCB_VTOR = (unsigned int) &g_pfnVectors;
	#elif defined(__ARMCC_VERSION)
		extern void *__Vectors;

		*pSCB_VTOR = (unsigned int) &__Vectors;
	#endif

	#if defined(__FPU_PRESENT) && __FPU_PRESENT == 1
		fpuInit();
	#endif
	//Init clocking of the LPC1769
	Board_SetupClocking();
	SystemCoreClockUpdate();
}

/* Setup system clocking */
void Board_SetupClocking(void)
{
	Chip_SetupXtalClocking();

	/* Setup FLASH access to 4 clocks (100MHz clock) */
	Chip_SYSCTL_SetFLASHAccess(FLASHTIM_100MHZ_CPU);
}

#endif /* LPC1769 */
