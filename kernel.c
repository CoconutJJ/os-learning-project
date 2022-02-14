#include "multiboot.h"

multiboot_info_t * mbi;



void kernel_main (unsigned long multiboot_magic_number, unsigned long multiboot_struct_addr)
{
        if (multiboot_magic_number != MULTIBOOT_BOOTLOADER_MAGIC) {
                // shit went wrong.
                return;
        }

        mbi = (multiboot_info_t *)multiboot_struct_addr;

}
