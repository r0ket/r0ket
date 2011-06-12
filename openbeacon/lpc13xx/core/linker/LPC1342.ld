MEMORY
{
	flash	: ORIGIN = 0x00000000, LENGTH = 16K
	ram	: ORIGIN = 0x10000180, LENGTH = 4K - 0x180
}

__stack_end__ = 0x10000000 + 4K - 32;

INCLUDE "../core/linker/LPC13xx.ld"