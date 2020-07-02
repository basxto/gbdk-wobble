// original gameboy
#include <gb/gb.h>
// gameboy color
#include <gb/cgb.h>
#include <stdio.h>
#include <gb/hardware.h>
#include <stdio.h>

extern UINT8 scanline_wobble[];
extern UINT8 scanline_checkered[];
const UINT8 *scanline_offsets;
UINT8 scanline_ly_offset;

void scanline_isr() {
    SCX_REG = scanline_offsets[LY_REG%8];
}

void screen_wobble() {
    scanline_ly_offset = 0;
    scanline_offsets = &scanline_wobble[0];

    set_interrupts(VBL_IFLAG | LCD_IFLAG);
    for(UINT8 i = 0; i < 40; ++i){
        wait_vbl_done();
        wait_vbl_done();
        wait_vbl_done();
        scanline_offsets = &scanline_wobble[(++scanline_ly_offset)%8];
    }
    set_interrupts(VBL_IFLAG);
    SCX_REG = 4;
}

void screen_wobble_in() {
    scanline_ly_offset = 0;
    scanline_offsets = &scanline_wobble[0];

    set_interrupts(VBL_IFLAG | LCD_IFLAG);
    for(UINT8 i = 2; i < 42; ++i){
        wait_vbl_done();
        wait_vbl_done();
        scanline_offsets = &scanline_wobble[(++scanline_ly_offset)%8];
    }
    set_interrupts(VBL_IFLAG);
    SCX_REG = 4;
}

void screen_checkered() {
    scanline_ly_offset = 0;
    scanline_offsets = &scanline_checkered[0];

    set_interrupts(VBL_IFLAG | LCD_IFLAG);
    for(UINT8 i = 0; i < 50; ++i){
        wait_vbl_done();
        wait_vbl_done();
        scanline_offsets = &scanline_checkered[(++scanline_ly_offset)%4];
    }
    set_interrupts(VBL_IFLAG);
    SCX_REG = 4;
}

void main() {
    DISPLAY_OFF;
    SHOW_BKG;
    HIDE_WIN;
    printf(" \n\n\n      TEST FOR\n\n    WOBBLES WITH\n\n      GBDK 2020");
    // default offset
    move_bkg(4, 0);
    // configure scanline interrupts
    STAT_REG = 0x18;
    LYC_REG = 0x00;
    DISPLAY_ON;

    disable_interrupts();
    add_LCD(scanline_isr);
    enable_interrupts();
    set_interrupts(VBL_IFLAG);

    while (1) {

        switch (joypad()) {
        case J_A:
            screen_wobble();
            delay(100);
            break;
        case J_B:
            screen_wobble_in();
            delay(100);
            break;
        case J_SELECT:
        case J_START:
            screen_checkered();
            delay(100);
            break;
        default:
            break;
        }
        wait_vbl_done();
    }
}
