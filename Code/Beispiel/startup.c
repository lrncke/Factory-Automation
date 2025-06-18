/*-------------------------------------------------------------------*
 *                                                                   *
 *                       Bihl+Wiedemann GmbH                         *
 *                                                                   *
 *                                                                   *
 *       project: Control_III                                        *
 *   module name: startup.c                                          *
 *        author: Christian Sommerfeld                               *
 *          date: 2015-02-26                                         *
 *                                                                   *
 *  version: 1.0 first version                                       *
 *   			                                                     *
 *                                                                   *                                                                *
 *-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*
 *  include files                                                    *
 *-------------------------------------------------------------------*/

#include "control.h"


/*-------------------------------------------------------------------*
 *  local definitions                                                *
 *-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*
 *  external declarations                                            *
 *-------------------------------------------------------------------*/
extern unsigned long __etext;
extern unsigned long __data_start;
extern unsigned long __edata;
extern unsigned long __bss_start__;
extern unsigned long __bss_end__;

/*-------------------------------------------------------------------*
 *  function prototypes                                              *
 *-------------------------------------------------------------------*/
static void start_up ( void );


/*-------------------------------------------------------------------*
 *  public data                                                      *
 *-------------------------------------------------------------------*/
volatile ccontrol_func_t cctrl_func __attribute__ ((__section__ (".funcarea")));

unsigned long start_vector[] __attribute__ ((__section__ (".startup"), __used__ )) =
{
	(unsigned long) start_up,
	(unsigned long) SIGNATURE,
};

/*-------------------------------------------------------------------*
 *  private functions                                                *
 *-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*
 *  public functions                                                 *
 *-------------------------------------------------------------------*/

static void start_up ( void )
{
	unsigned long *src;
	unsigned long *dst;

	/*	initialize the 'data' segment with values from flash
	 */

	src = &__etext;
	dst = &__data_start;

	while (dst < &__edata)
	{
		*dst = *src;

		src++;
		dst++;
	}

	/*	initialize the 'bss' segment with zero
	 */

	dst = &__bss_start__;

	while (dst < &__bss_end__)
	{
		*dst = 0;
		dst++;
	}

	main ();
}

/*---------------------------------------------------------------------*
 *  eof                                                                *
 *---------------------------------------------------------------------*/


