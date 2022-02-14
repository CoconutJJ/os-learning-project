# OS Learning Project

This project is the start of learning how Operating Systems work at a technical
level and wanting to build a *very* simple OS from scratch.

I may make things, I may break things.. who really knows? I'm not expecting this
OS to really function coherently, or really even do anything useful at all. 

Targets i686-elf.

## Resources

This is a list of resources I have been using to feed by learning, and notes I'm
leaving myself so I don't forget.

- OSDev
  - Installing a cross compiler: https://wiki.osdev.org/GCC_Cross-Compiler
  - Bare Bones: https://wiki.osdev.org/Bare_Bones
  - PC Screen Fonts: https://wiki.osdev.org/PC_Screen_Font
    - Apparently VGA text-mode is deprecated in favour of framebuffers.
- GRUB
  - Multiboot Specification:
    https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
    - Contains the struct needed to find framebuffer address

Must compile with `-ffreestanding` flag.

