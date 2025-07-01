/*-------------------------------------------------------------------*
 *                                                                   *
 *                       Bihl+Wiedemann GmbH                         *
 *                                                                   *
 *                                                                   *
 *       project: Control_III                                        *
 *   module name: pnio.ld                                            *
 *        author: Christian Sommerfeld                               *
 *          date: 2015-02-26                                         *
 *                                                                   *
 *  version: 1.0 first version                                       *
 *   			                                                     *
 *                                                                   *                                                                *
 *-------------------------------------------------------------------*/
 
MEMORY
{
	func(rw)    : ORIGIN = 0x20F80000, LENGTH = 0x00002000
	ram (rx)	: ORIGIN = 0x20F82000, LENGTH = 0x00040000
}



/* Section Definitions */

SECTIONS
{ 
    .text : 
    { 
		. = ALIGN(4);
		CREATE_OBJECT_SYMBOLS
		KEEP(*(.startup))
    	*(.text*) 
    	*(.gnu.warning) 
    	*(.gnu.linkonce.t.*) 
    	*(.init) 
    	*(.glue_7) 
    	*(.glue_7t)
    	. = ALIGN(0x4); 
    } >ram
   
    .rodata : 
    { 	
    	. = .; 
    	*(.rodata*) 
    	*(.gnu.linkonce.r.*) 
    } >ram
    
    __etext = .;
    PROVIDE (__etext = .);   
   
	.data : AT (__etext)
	{
		. = ALIGN(4);
		__data_start = .;
		*(.data .data.*)
		*(.gnu.linkonce.d.*)
	} >ram
	. = ALIGN(4);
	
	__edata = .;
	PROVIDE (__edata = .); 
    
    .bss : 
    { 
		__bss_start = .;
		__bss_start__ = .;
		*(.bss .bss.*)
		*(.gnu.linkonce.b.*)
		*(COMMON)
		. = ALIGN(4); 
    } > ram
	__bss_end__ = .;

	.funcarea (NOLOAD) :
	{
		*(.funcarea)
	} > func
         
    _end = .; 
    PROVIDE (end = .);

}
