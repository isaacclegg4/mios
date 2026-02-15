; Minimal x86 bootloader for a simple OS
; This code runs at 0x7C00 when the system boots
; Compile with: nasm -f bin boot.asm -o boot.bin

[BITS 16]                    ; 16-bit real mode
[ORG 0x7C00]                 ; Bootloader origin at 0x7C00

; Boot sector entry point
boot_start:
    ; Disable interrupts during setup
    cli
    
    ; Clear registers
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    
    ; Set stack pointer
    mov sp, 0x7C00
    
    ; Enable A20 line (allows access to all memory)
    call enable_a20
    
    ; Load GDT and enter protected mode
    call load_gdt
    call enter_protected_mode
    
    ; Jump to kernel (loaded at 0x1000)
    jmp 0x08:kernel_start
    
; Enable A20 line using keyboard controller
enable_a20:
    push ax
    
    ; Wait for input buffer to be empty
.wait_input:
    in al, 0x64
    test al, 0x02
    jnz .wait_input
    
    ; Send "write output port" command
    mov al, 0xD1
    out 0x64, al
    
    ; Wait for input buffer to be empty again
.wait_input2:
    in al, 0x64
    test al, 0x02
    jnz .wait_input2
    
    ; Write output port with A20 bit set
    mov al, 0xDF
    out 0x60, al
    
    pop ax
    ret

; Load Global Descriptor Table (GDT)
load_gdt:
    cli
    lgdt [gdt_descriptor]
    ret

; Enter protected mode
enter_protected_mode:
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    ret

; Global Descriptor Table
gdt_start:
    ; Null descriptor (required)
    dq 0x0000000000000000
    
    ; Code segment descriptor
    dq 0x00cf9a000000ffff
    
    ; Data segment descriptor
    dq 0x00cf92000000ffff

gdt_end:
    
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

kernel_start:
    [BITS 32]
    ; Zero out registers
    xor eax, eax
    xor ebx, ebx
    xor ecx, ecx
    xor edx, edx
    
    ; Set up segments
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    
    ; Jump to kernel C code
    extern kernel_main
    call kernel_main
    
    ; Halt if kernel returns
    cli
    hlt
    jmp $

; Fill remaining space with zeros until 510 bytes
times 510 - ($ - $$) db 0

; Boot signature (required for BIOS)
dw 0xAA55
