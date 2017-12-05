/*
 * Memory Spaces Definitions.
 *
 * Need modifying for a specific board. 
 *   FLASH.ORIGIN: starting address of flash
 *   FLASH.LENGTH: length of flash
 *   RAM.ORIGIN: starting address of RAM bank 0
 *   RAM.LENGTH: length of RAM bank 0
 *
 * The values below can be addressed in further linker scripts
 * using functions like 'ORIGIN(RAM)' or 'LENGTH(RAM)'.
 */

MEMORY
{
  RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
  CCMRAM (xrw) : ORIGIN = 0x10000000, LENGTH = 64K
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 1024K
  FLASHB1 (rx) : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB0 (rx) : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB1 (rx) : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB2 (rx) : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB3 (rx) : ORIGIN = 0x00000000, LENGTH = 0
  MEMORY_ARRAY (xrw)  : ORIGIN = 0x20002000, LENGTH = 32
}

/* Variables used by FreeRTOS-MPU. */
_Privileged_Functions_Region_Size = 16K;
_Privileged_Data_Region_Size = 1024;

__FLASH_segment_start__ = ORIGIN( FLASH );
__FLASH_segment_end__ = __FLASH_segment_start__ + LENGTH( FLASH );

__privileged_functions_start__ = ORIGIN( FLASH );
__privileged_functions_end__ = __privileged_functions_start__ + _Privileged_Functions_Region_Size;

__SRAM_segment_start__ = ORIGIN( RAM );
__SRAM_segment_end__ = __SRAM_segment_start__ + LENGTH( RAM );

__privileged_data_start__ = ORIGIN( RAM );
__privileged_data_end__ = ORIGIN( RAM ) + _Privileged_Data_Region_Size;

/*
 * For external ram use something like:

   RAM (xrw) : ORIGIN = 0x64000000, LENGTH = 2048K

 */
