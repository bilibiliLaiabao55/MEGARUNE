#include<genesis.h>
#include "resource.h"
#include "texts.c"
double timer0=0;
u16 timer1=0;
u16 temp0=0;
u16 temp1=0;
void tick(){
    VDP_waitVSync();
    SYS_doVBlankProcess();
}
void drawTextUT(VDPPlane plane, const char* text, u16 x, u16 y, u16 len, u16 delay){
    u16 timerDelay = 0;
    for(temp0 = 0;temp0<len; ++temp0){
        if(JOY_readJoypad(JOY_1)&BUTTON_B){
            VDP_drawTextBG(plane, text, x, y);
            break;
        }
        char this;
        memcpy(&this, &text[temp0], 1);
        VDP_drawTextBG(plane, &this, x+temp0, y);
        while(timerDelay > 0){
            --timerDelay;
            tick();
        }
        timerDelay = delay;
        tick();
    }
}
int main(u16 isHardReset){
    JOY_init();
    SPR_init();
    Sprite* heart = SPR_addSprite(&heart_sprite0, 130, 90, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    if(isHardReset == TRUE){
        PAL_setPalette(PAL1, heart_sprite0.palette->data, CPU);
        u16 line = 0;
        XGM_startPlay(track0);
        while(TRUE){
            if(timer0 == 0){
                VDP_clearPlane(BG_B, FALSE);
                if(line == 0){
                    drawTextUT(BG_B, "ARE YOU", 12, 10, 14, 5);
                    drawTextUT(BG_B, "THERE?", 12, 11, 14, 5);
                }
                else if(line == 1){
                    drawTextUT(BG_B, "ARE WE", 12, 10, 17, 5);
                    drawTextUT(BG_B, "CONNECTED?", 12, 11, 17, 5);
                }
                else if(line == 2){
                    temp0 = 0;
                    while(TRUE){
                        if(temp0 >= 105){
                            break;
                        }
                        temp0 += (110-temp0)/6;
                        SPR_setPosition(heart, 145, temp0);
                        SPR_update();
                        tick();
                    }
                }
                else if(line == 3){
                    drawTextUT(BG_B, "EXCELLENT.", 15, 8, 10, 5);
                }else if(line == 4){
                    drawTextUT(BG_B, "TRULY", 17, 8, 5, 5);
                    drawTextUT(BG_B, "EXCELLENT.", 15, 9, 10, 5);
                }else if(line == 5){
                    drawTextUT(BG_B, "NOW.", 17, 8, 4, 5);
                }else if(line == 6){
                    drawTextUT(BG_B, "WE MAY", 17, 8, 6, 5);
                    drawTextUT(BG_B, "BEGIN.", 17, 9, 6, 5);
                }else if(line == 7){
                    temp0 = 105;
                    while(TRUE){
                        if(temp0 <= 5){
                            break;
                        }
                        temp0 += (0-temp0)/6;
                        SPR_setPosition(heart, 145, temp0);
                        SPR_update();
                        tick();
                    }
                    SPR_clear();
                }else if(line == 8){
                    XGM_startPlay(track1);
                    break;
                }
                timer0 = 60;
                ++line;
            }else --timer0;
            tick();
        }
    }
    while(TRUE){

        tick();
    }
    return 0;
}
