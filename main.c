// original gameboy
#include <gb/gb.h>
// gameboy color
#include <gb/cgb.h>
#include <stdio.h>
#include <gb/hardware.h>
#include <stdio.h>

const UINT8 empty = 0x00;
const UINT8 full = 0x00;
const UINT8 left = 0x80;
const UINT8 right = 0x01;

INT8 scanline_offsets[8];
UINT8 scanline_counter;

void scanline_isr() {
    //SCX_REG += scanline_offsets[scanline_counter];
    SCX_REG = (INT8)SCX_REG + scanline_offsets[scanline_counter];
    scanline_counter = (UINT8)(scanline_counter+1)%8;
    ++LYC_REG;
}

void screen_wobble() {
    scanline_offsets[0] = 1;
    scanline_offsets[1] = 1;
    scanline_offsets[2] = 1;
    scanline_offsets[3] = 1;
    scanline_offsets[4] = -1;
    scanline_offsets[5] = -1;
    scanline_offsets[6] = -1;
    scanline_offsets[7] = -1;
    STAT_REG = 0x45;
    LYC_REG = 0x00;
    scanline_counter = 0;

    set_interrupts(VBL_IFLAG | LCD_IFLAG);
    for(UINT8 i = 0; i < 20; ++i){
        wait_vbl_done();
        wait_vbl_done();
        wait_vbl_done();
        wait_vbl_done();
        wait_vbl_done();
        wait_vbl_done();
        ++scanline_counter;
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
    DISPLAY_ON;

    disable_interrupts();
    add_LCD(scanline_isr);
    enable_interrupts();
    set_interrupts(VBL_IFLAG);

    while (1) {

        switch (joypad()) {
        case J_A:
        case J_B:
        case J_START:
            screen_wobble();
            delay(100);
            break;
        default:
            break;
        }
        wait_vbl_done();
    }
}
