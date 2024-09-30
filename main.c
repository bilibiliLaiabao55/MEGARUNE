#include <genesis.h>
#include "resource.h"

const u8 kris_frames[4]={0, 1 , 0, 2};
u16 temp0=0;
u16 temp1=0;
u8 pad_state = 0;
u32 kris_postion[2] = {0, 0};
u8 world=0;
u8 kris_frame=0;
u8 kris_delay=0;
u16 TILE_USED = TILE_USERINDEX;
u16 planescrollx=0;
u16 planescrolly=0;
void tick(){
    VDP_waitVSync();
    SYS_doVBlankProcess();
}

void set_plane_scroll(VDPPlane plane, u16 scrollx, u16 scrolly){
    planescrollx = scrollx;
    planescrolly = scrolly;
    VDP_setHorizontalScroll(plane, scrollx);
    VDP_setVerticalScroll(plane, scrolly);
}

void drawTextUT(VDPPlane plane, const char* text, u16 x, u16 y, u16 delay){
    u16 timerDelay = 0;
    u16 len = 0;
    u8 temp = 0;
    while (text[len] != '\0') len++;
    for(temp = 0;temp<len; ++temp){
        if(JOY_readJoypad(JOY_1)&BUTTON_B){
            VDP_drawTextBG(plane, text, x, y);
            break;
        }
        char this;
        memcpy(&this, &text[temp], 1);
        VDP_drawTextBG(plane, &this, x+temp, y);
        while(timerDelay > 0){
            --timerDelay;
            tick();
        }
        timerDelay = delay;
        tick();
    }
}
void playCutscences(u8 cutscences){
    if(cutscences==0){
        PAL_setPalette(PAL1, heart_sprite_intro.palette->data, CPU);
        u8 line = 0;
        XGM_startPlay(MUS_AUDIO_DRONE);
        while(TRUE){
            VDP_clearPlane(BG_A, TRUE);
            VDP_clearPlane(BG_B, TRUE);
            set_plane_scroll(BG_A, 0, 0);
            set_plane_scroll(BG_B, 0, 0);
            Sprite* heart = SPR_addSpriteSafe(&heart_sprite_intro, 130, 90, TILE_ATTR(PAL1, 0, FALSE, FALSE));
            pad_state = JOY_readJoypad(JOY_1);
            if(temp1 == 0){
                VDP_clearPlane(BG_B, FALSE);
                if(line == 0){
                    drawTextUT(BG_B, "ARE YOU", 14, 10, 5);
                    drawTextUT(BG_B, "THERE?", 14, 11, 5);
                }
                else if(line == 1){
                    drawTextUT(BG_B, "ARE WE", 14, 10, 5);
                    drawTextUT(BG_B, "CONNECTED?", 14, 11, 5);
                }
                else if(line == 2){
                    temp0 = 0;
                    while(TRUE){
                        if((temp0 >= 105)||(pad_state & BUTTON_B)){
                            temp0 = 110;
                            break;
                        }
                        temp0 += (110-temp0)/6;
                        SPR_setPosition(heart, 145, temp0);
                        SPR_update();
                        tick();
                    }
                }
                else if(line == 3){
                    drawTextUT(BG_B, "EXCELLENT.", 15, 8, 5);
                }else if(line == 4){
                    drawTextUT(BG_B, "TRULY", 17, 8, 5);
                    drawTextUT(BG_B, "EXCELLENT.", 15, 9, 5);
                }else if(line == 5){
                    drawTextUT(BG_B, "NOW.", 17, 8, 5);
                }else if(line == 6){
                    drawTextUT(BG_B, "WE MAY", 17, 8, 5);
                    drawTextUT(BG_B, "BEGIN.", 17, 9, 5);
                }else if(line == 7){
                    temp0 = 105;
                    while(TRUE){
                        if((temp0 <= 5)||(pad_state&BUTTON_B)){
                            temp0 = 0;
                            break;
                        }
                        temp0 += (0-temp0)/6;
                        SPR_setPosition(heart, 145, temp0);
                        SPR_update();
                        tick();
                    }
                    SPR_releaseSprite(heart);
                    SPR_clear();
                }else if(line == 8){
                    break;
                }
                temp1 = 60;
                ++line;
            }else --temp1;
            if(pad_state & BUTTON_A){
                temp1 = 0;
            }
            tick();
        }
        XGM_stopPlay();
    }
    if(cutscences==1){
        VDP_clearPlane(BG_A, TRUE);
        VDP_clearPlane(BG_B, TRUE);
        set_plane_scroll(BG_A, 0, 0);
        set_plane_scroll(BG_B, 0, 0);
        XGM_startPlay(mus_story);
        u8 line = 0;
        while(TRUE){
            if(line==0){
                drawTextUT(BG_B, "adadad", 0, 0, 3);
            }
        }
    }
}
void play_kris_anim(){
    if(kris_delay==0){
        kris_delay = 8;
        ++kris_frame;
        if(kris_frame==4) kris_frame = 0;
    }else --kris_delay;
}

void draw_battle_back(){
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);
    VDP_setPlaneSize(32, 32, FALSE);
    VDP_setPalette(PAL0, battle_back.palette->data);
    for(temp0 = 0;temp0<4; ++temp0){
        for(temp1 = 0;temp1<5; ++temp1){
            VDP_drawImage(BG_A, &battle_back, temp0*8, temp1*8);
        }
    }
}

void battle_init(){
    set_plane_scroll(BG_A, 0, 0);
    set_plane_scroll(BG_B, 0, 0);
    draw_battle_back();
    XGM_startPlay(mus_battle);
}
void battle_logic(){
    battle_init();
    while(TRUE){
        set_plane_scroll(BG_A, planescrollx-1, planescrolly+1);
        tick();
    }
}

int main(u16 isHardReset){
    VDP_init();
    JOY_init();
    SPR_init();
    if(!((JOY_getJoypadType(JOY_1)==JOY_TYPE_PAD6)||(JOY_getJoypadType(JOY_1)==JOY_TYPE_PAD3))){
        VDP_drawText("This controller can't use for this game!", 0 ,12);
        VDP_drawText("Please use a 6 or 3 button pad!", 0 ,13);
        while(TRUE){
            tick();
        }
    }
    if((isHardReset == TRUE)){
        playCutscences(0);
    }
    battle_logic();
    SPR_end();
    SPR_init();
    XGM_setPCM(64, snd_appear, 35072);
    XGM_startPlayPCM(64, 15, SOUND_PCM_CH2);
    VDP_drawImage(BG_A, &deltarune_logo, 6, 10);
    TILE_USED += deltarune_logo.tileset->numTile;
    PAL_setPalette(PAL0, palette_black, CPU);
    PAL_fadeIn(0, 15, deltarune_logo.palette->data, 15, TRUE);
    temp1 = 105;
    while(temp1 > 0){
        pad_state = JOY_readJoypad(JOY_1);
        if((pad_state & BUTTON_START)||(pad_state & BUTTON_A)) temp1 = 0;
        --temp1;
        tick();
    }
    VDP_clearPlane(BG_A, TRUE);
    Sprite* kris = SPR_addSpriteSafe(&spr_kris_d, 0, 0, TILE_ATTR(PAL2, 1, FALSE, FALSE));
    PAL_setPalette(PAL2, spr_kris_d.palette->data, 16);
    while(TRUE){
        pad_state = JOY_readJoypad(JOY_1);
        SPR_clear();
        if(pad_state & BUTTON_UP){
            kris_postion[1]-=2;
            SPR_setHFlip(kris, FALSE);
            SPR_setAnim(kris, 5-world*3);
            play_kris_anim();
        }
        else if(pad_state & BUTTON_DOWN){
            kris_postion[1]+=2;
            SPR_setHFlip(kris, FALSE);
            SPR_setAnim(kris, 3-world*3);
            play_kris_anim();
        }
        if(pad_state & BUTTON_LEFT){
            kris_postion[0]-=2;
            SPR_setHFlip(kris, TRUE);
            SPR_setAnim(kris, 4-world*3);
            play_kris_anim();
        }
        else if(pad_state & BUTTON_RIGHT){
            kris_postion[0]+=2;
            SPR_setHFlip(kris, FALSE);
            SPR_setAnim(kris, 4-world*3);
            play_kris_anim();
        }
        SPR_setFrame(kris, kris_frames[kris_frame]);
        if(!(pad_state & BUTTON_DOWN))
        if(!(pad_state & BUTTON_UP))
        if(!(pad_state & BUTTON_LEFT))
        if(!(pad_state & BUTTON_RIGHT)){
            kris_frame = 0;
            kris_delay = 0;
        }
        SPR_setPosition(kris, kris_postion[0], kris_postion[1]);
        SPR_update();
        tick();
    }
    return 0;
}
