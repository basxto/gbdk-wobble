// original gameboy
#include <gb/gb.h>
// gameboy color
#include <gb/cgb.h>
#include <stdio.h>
#include <gb/hardware.h>
#include <stdio.h>

UINT8 scanline_offsets[8];
UINT8 scanline_ly_offset;

void scanline_isr() {
    SCX_REG = scanline_offsets[(UINT8)((UINT8)LY_REG + scanline_ly_offset)%8];
    LYC_REG = LY_REG+1;
}

void screen_wobble() {
    scanline_offsets[0] = 4;
    scanline_offsets[1] = 5;
    scanline_offsets[2] = 6;
    scanline_offsets[3] = 7;
    scanline_offsets[4] = 7;
    scanline_offsets[5] = 6;
    scanline_offsets[6] = 5;
    scanline_offsets[7] = 4;
    STAT_REG = 0x45;
    LYC_REG = 0x00;
    scanline_ly_offset = 0;

    set_interrupts(VBL_IFLAG | LCD_IFLAG);
    for(UINT8 i = 0; i < 40; ++i){
        wait_vbl_done();
        wait_vbl_done();
        wait_vbl_done();
        wait_vbl_done();
        ++scanline_ly_offset;
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
