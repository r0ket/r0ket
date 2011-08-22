
sram_top = ORIGIN(sram) + LENGTH(sram);
TheTable = 0x00000124;
ENTRY(ram)

SECTIONS
{
  .text :
  {
    *(.text*)
    *(.rodata*)
  } > sram

  /*
   * More information about Special Section Indexes is available in the
   * free "ELF for the ARM Architecture" document from ARM Limited
   * http://infocenter.arm.com/help/topic/com.arm.doc.ihi0044d/IHI0044D_aaelf.pdf
   *
   */
  .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } > sram
  __exidx_start = .;
  .ARM.exidx : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) } > sram
  __exidx_end = .;

  _etext = .;
    
  .data : AT (__exidx_end)
  {
    _data = .;
    *(vtable)
    *(.data*)
    _edata = .;
                    /* force zero initialized data to be present*/
    *(.bss*)
    *(COMMON)
    _ebss = .;
  } > sram

  end = .;

}
