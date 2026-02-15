// Minimal kernel for simple OS
// Compile with: gcc -m32 -fno-builtin -nostdlib -c kernel.c

#include <stdint.h>
#include <stddef.h>

// Port I/O functions
void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// VGA text mode buffer
volatile uint16_t *vga_buffer = (volatile uint16_t *)0xB8000;
int cursor_x = 0;
int cursor_y = 0;

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_COLOR_WHITE 0x0F
#define VGA_COLOR_BLACK 0x00
#define VGA_COLOR_GREEN 0x0A
#define VGA_COLOR_RED 0x04

// Write a character to VGA buffer
void vga_write_char(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= VGA_HEIGHT) {
            cursor_y = VGA_HEIGHT - 1;
            // Scroll screen
            for (int i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++) {
                vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
            }
            // Clear last line
            for (int i = VGA_WIDTH * (VGA_HEIGHT - 1); i < VGA_WIDTH * VGA_HEIGHT; i++) {
                vga_buffer[i] = 0;
            }
        }
        return;
    }
    
    uint16_t attribute = (VGA_COLOR_BLACK << 4) | VGA_COLOR_GREEN;
    uint16_t entry = ((uint16_t)c) | (attribute << 8);
    vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = entry;
    
    cursor_x++;
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= VGA_HEIGHT) {
            cursor_y = VGA_HEIGHT - 1;
        }
    }
}

// Print string
void print(const char *str) {
    while (*str) {
        vga_write_char(*str);
        str++;
    }
}

// Print number in hex
void print_hex(uint32_t num) {
    char *hex_chars = "0123456789ABCDEF";
    print("0x");
    
    for (int i = 28; i >= 0; i -= 4) {
        uint8_t digit = (num >> i) & 0xF;
        vga_write_char(hex_chars[digit]);
    }
}

// Clear VGA screen
void clear_screen(void) {
    uint16_t attribute = (VGA_COLOR_BLACK << 4) | VGA_COLOR_GREEN;
    uint16_t blank = 0x20 | (attribute << 8);
    
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = blank;
    }
    
    cursor_x = 0;
    cursor_y = 0;
}

// Update cursor position in VGA hardware
void update_cursor(void) {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    
    // Set high byte of cursor position
    outb(0x3D4, 0x0E);
    outb(0x3D5, (pos >> 8) & 0xFF);
    
    // Set low byte of cursor position
    outb(0x3D4, 0x0F);
    outb(0x3D5, pos & 0xFF);
}

// Simple memory info
typedef struct {
    uint32_t size;
    uint32_t addr;
    uint32_t type;
} memory_map_entry_t;

// Main kernel entry point
void kernel_main(void) {
    clear_screen();
    
    print("===================================\n");
    print("  Welcome to SimpleOS\n");
    print("===================================\n\n");
    
    print("Kernel loaded and running!\n\n");
    
    print("System Information:\n");
    print("- CPU Mode: 32-bit Protected Mode\n");
    print("- VGA Text Mode: 80x25\n");
    print("- Video Memory: ");
    print_hex((uint32_t)vga_buffer);
    print("\n\n");
    
    print("Available Commands:\n");
    print("- Type 'help' for commands\n");
    print("- Type 'clear' to clear screen\n");
    print("- Type 'reboot' to restart\n\n");
    
    print("> ");
    update_cursor();
    
    // Simple command loop
    char command[256] = {0};
    int cmd_index = 0;
    
    while (1) {
        // Simple halt - real implementation would have keyboard driver
        asm volatile ("hlt");
    }
}
