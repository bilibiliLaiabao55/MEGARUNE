#include <genesis.h>
#include "resource.h"
const u8 main_character_frames[4]={0, 1 , 0, 2};
const u16 krisroomrect[4][4]={
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};
u16 temp0=0;
u16 temp1=0;
u8 pad_state = 0;
u16 kris_postion[2] = {0, 0};
u16 susie_postion[2] = {0, 0};
u16 ralsei_postion[2] = {0, 0};
u8 kris_frame=0;
u8 susie_frame=0;
u8 ralsei_frame=0;
u8 kris_delay=0;
u8 susie_delay=0;
u8 ralsei_delay=0;
u8 kris_face = 0;
u8 world=0;
u16 planescrollx=0;
u16 planescrolly=0;
bool susie_in_party = FALSE;
bool ralsei_in_party = FALSE;
Sprite* kris = NULL;
Sprite* susie = NULL;
Sprite* ralsei = NULL;
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
        PAL_setPalette(PAL1, heart_sprite_intro.palette->data, DMA);
        u8 line = 0;
        SND_startPlay_XGM(MUS_AUDIO_DRONE);
        VDP_clearPlane(BG_A, TRUE);
        VDP_clearPlane(BG_B, TRUE);
        set_plane_scroll(BG_A, 0, 0);
        set_plane_scroll(BG_B, 0, 0);
        Sprite* heart = SPR_addSpriteSafe(&heart_sprite_intro, 130, 90+8, TILE_ATTR(PAL1, 0, FALSE, FALSE));
        while(TRUE){
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
                    temp0 = 0+16;
                    while(TRUE){
                        if((temp0 >= 105)||(pad_state & BUTTON_B)){
                            temp0 = 110;
                            SPR_setPosition(heart, 145+8, temp0);
                            SPR_update();
                            break;
                        }
                        temp0 += (110-temp0)/6;
                        SPR_setPosition(heart, 145+8, temp0);
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
                    temp0 = 105+16;
                    while(TRUE){
                        if((temp0 <= 5)||(pad_state&BUTTON_B)){
                            temp0 = 0;
                            break;
                        }
                        temp0 += (0-temp0)/6;
                        SPR_setPosition(heart, 145+8, temp0);
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
}
void play_kris_anim(){
    if(kris_delay==0){
        kris_delay = 8;
        ++kris_frame;
        if(kris_frame==4) kris_frame = 0;
    }else --kris_delay;
}
void play_susie_anim(){
    if(susie_delay==0){
        susie_delay = 8;
        ++susie_frame;
        if(susie_frame==4) susie_frame = 0;
    }else --susie_delay;
}
void play_ralsei_anim(){
    if(ralsei_delay==0){
        ralsei_delay = 8;
        ++ralsei_frame;
        if(ralsei_frame==4) ralsei_frame = 0;
    }else --ralsei_delay;
}

void draw_battle_back(){
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);
    VDP_setPlaneSize(32, 32, TRUE);
    PAL_setPalette(PAL0, battle_back.palette->data, DMA);
    for(temp0 = 0;temp0<4; ++temp0){
        for(temp1 = 0;temp1<5; ++temp1){
            VDP_drawImageEx(BG_A, &battle_back, TILE_ATTR(PAL0, 0, FALSE, FALSE), temp0*8, temp1*8, FALSE, FALSE);
        }
    }
}

void battle_init(){
    set_plane_scroll(BG_A, 0, 0);
    set_plane_scroll(BG_B, 0, 0);
    draw_battle_back();
    SND_startPlay_XGM(mus_battle);
}
void battle_logic(){
    SND_startPlay_PCM(snd_battle_start, 10752, SOUND_RATE_13400, SOUND_PAN_CENTER, 0);
    temp0 = 30;
    while(temp0>0){--temp0;tick();}
    SND_startPlay_PCM(snd_weaponpull, 14592, SOUND_RATE_13400, SOUND_PAN_CENTER, 0);
    u16 kris_postion2[2]={0, 0};
    kris_postion2[0] = kris_postion[0];
    kris_postion2[1] = kris_postion[1];
    u16 kris_to_speed_x = 0;
    u16 kris_to_speed_y = 0;

    u16 susie_postion2[2]={0, 0};
    susie_postion2[0] = susie_postion[0];
    susie_postion2[1] = susie_postion[1];
    u16 susie_to_speed_x = 0;
    u16 susie_to_speed_y = 0;

    u16 ralsei_postion2[2]={0, 0};
    ralsei_postion2[0] = ralsei_postion[0];
    ralsei_postion2[1] = ralsei_postion[1];
    u16 ralsei_to_speed_x = 0;
    u16 ralsei_to_speed_y = 0;  
    temp0 = 15;
    while(temp0>0){
        kris_to_speed_x = (0-kris_postion2[0])/temp0;
        kris_to_speed_y = (30-kris_postion2[1])/temp0;
        kris_postion2[0]+=kris_to_speed_x;
        kris_postion2[1]+=kris_to_speed_y;
        if(susie_in_party){
            susie_to_speed_x = (0-susie_postion2[0])/temp0;
            susie_to_speed_y = (120-susie_postion2[1])/temp0;
            susie_postion2[0]+=susie_to_speed_x;
            susie_postion2[1]+=susie_to_speed_y;
            SPR_setPosition(susie, susie_postion2[0], susie_postion2[1]-7);
        }
        if(ralsei_in_party){
            ralsei_to_speed_x = (0-ralsei_postion2[0])/temp0;
            ralsei_to_speed_y = (70-ralsei_postion2[1])/temp0;
            ralsei_postion2[0]+=ralsei_to_speed_x;
            ralsei_postion2[1]+=ralsei_to_speed_y;
            SPR_setPosition(ralsei, ralsei_postion2[0], ralsei_postion2[1]-6);
        }
        SPR_setPosition(kris, kris_postion2[0]-2, kris_postion2[1]);
        SPR_update();
        --temp0;
        tick();
    }
    temp0 = 15;
    while(temp0>0){--temp0;tick();}
    battle_init();
    while(TRUE){
        set_plane_scroll(BG_A, planescrollx-1, planescrolly+1);
        tick();
    }
}
void init_all(){
    VDP_init();
    VDP_resetScreen();
    VDP_resetSprites();
    JOY_init();
    SPR_init();
}
int main(u16 isHardReset){
    init_all();
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
    SPR_end();
    SPR_init();
    SND_startPlay_PCM(snd_appear, 33536, SOUND_RATE_13400, SOUND_PAN_CENTER, 0);
    VDP_drawImageEx(BG_A, &deltarune_logo, TILE_ATTR(PAL0, 0, FALSE, FALSE), 6, 10, FALSE, FALSE);
    PAL_setPalette(PAL0, palette_black, DMA);
    PAL_fadeIn(0, 63, deltarune_logo.palette->data, 15, TRUE);
    temp0 = 120-15;
    while(temp0 > 0){--temp0;tick();}
    VDP_clearPlane(BG_A, TRUE);

    kris = SPR_addSpriteSafe(&spr_kris, 0, 0, TILE_ATTR(PAL1, 1, FALSE, FALSE));
    PAL_setPalette(PAL1, spr_kris.palette->data, DMA);
    if(susie_in_party){
        susie = SPR_addSpriteSafe(&spr_susie, 0, 0, TILE_ATTR(PAL2, 1, FALSE, FALSE));
        PAL_setPalette(PAL2, spr_susie.palette->data, DMA);
        SPR_setAlwaysOnTop(susie, TRUE);
    }
    if(ralsei_in_party){
        ralsei = SPR_addSpriteSafe(&spr_ralsei, 0, 0, TILE_ATTR(PAL3, 1, FALSE, FALSE));
        PAL_setPalette(PAL3, spr_ralsei.palette->data, DMA);
        SPR_setAlwaysOnTop(ralsei, TRUE);
    }
    SPR_setAlwaysOnTop(kris, TRUE);

    VDP_setPlaneSize(128, 128, TRUE);
    PAL_setPalette(PAL0, rb_krisroom.palette->data, DMA);
    VDP_drawImageEx(BG_A, &rb_krisroom, TILE_ATTR(PAL0, 0, FALSE, FALSE), 0, 0, TRUE, FALSE);
    while(TRUE){
        pad_state = JOY_readJoypad(JOY_1);
        if(pad_state & BUTTON_LEFT){
            kris_face = 0;
            kris_postion[0]-=1;
            if(pad_state & BUTTON_B) kris_postion[0]-=1;
            SPR_setHFlip(kris, TRUE);
            SPR_setAnim(kris, 4-world*3);
        }
        else if(pad_state & BUTTON_RIGHT){
            kris_face = 1;
            kris_postion[0]+=1;
            if(pad_state & BUTTON_B) kris_postion[0]+=1;
            SPR_setHFlip(kris, FALSE);
            SPR_setAnim(kris, 4-world*3);
        }
        if(pad_state & BUTTON_UP){
            kris_face = 2;
            kris_postion[1]-=1;
            if(pad_state & BUTTON_B) kris_postion[1]-=1;
            SPR_setHFlip(kris, FALSE);
            SPR_setAnim(kris, 5-world*3);
        }
        else if(pad_state & BUTTON_DOWN){
            kris_face = 3;
            kris_postion[1]+=1;
            if(pad_state & BUTTON_B) kris_postion[1]+=1;
            SPR_setHFlip(kris, FALSE);
            SPR_setAnim(kris, 3-world*3);
        }
        if(pad_state & BUTTON_C) battle_logic();
        SPR_setFrame(kris, main_character_frames[kris_frame]);
        if(!(pad_state & BUTTON_DIR)){
            kris_frame = 0;
            kris_delay = 0;
        }else play_kris_anim();
        //face: 0 left 1 right 2 up 3 down
        if(ralsei_in_party){
            if(kris_face==0){
                temp0 = kris_postion[0]+25;
                temp1 = kris_postion[1];
            }
            if(kris_face==1){
                temp0 = kris_postion[0]-25;
                temp1 = kris_postion[1];
            }
            if(kris_face==2){
                temp0 = kris_postion[0];
                temp1 = kris_postion[1]+25;
            }
            if(kris_face==3){
                temp0 = kris_postion[0];
                temp1 = kris_postion[1]-25;
            }
            if(pad_state & BUTTON_DIR){
                play_ralsei_anim();
                if(ralsei_postion[0]>temp0){
                    SPR_setHFlip(ralsei, TRUE);
                    ralsei_postion[0]-=1;
                    if(pad_state & BUTTON_B) ralsei_postion[0]-=1;
                    SPR_setAnim(ralsei, 1);
                }
                if(ralsei_postion[0]<temp0){
                    SPR_setHFlip(ralsei, FALSE);
                    ralsei_postion[0]+=1;
                    if(pad_state & BUTTON_B) ralsei_postion[0]+=1;
                    SPR_setAnim(ralsei, 1);
                }
                if(ralsei_postion[1]>temp1){
                    SPR_setHFlip(ralsei, FALSE);
                    ralsei_postion[1]-=1;
                    if(pad_state & BUTTON_B) ralsei_postion[1]-=1;
                    SPR_setAnim(ralsei, 2);
                }
                if(ralsei_postion[1]<temp1){
                    SPR_setHFlip(ralsei, FALSE);
                    ralsei_postion[1]+=1;
                    if(pad_state & BUTTON_B) ralsei_postion[1]+=1;
                    SPR_setAnim(ralsei, 0);
                }
            }else ralsei_frame = 0;
            SPR_setFrame(ralsei, main_character_frames[ralsei_frame]);
            SPR_setPosition(ralsei, ralsei_postion[0], ralsei_postion[1]-6);
        }
        if(susie_in_party){
            if(kris_face==0){
                temp0 = kris_postion[0]+50;
                temp1 = kris_postion[1];
            }
            if(kris_face==1){
                temp0 = kris_postion[0]-50;
                temp1 = kris_postion[1];
            }
            if(kris_face==2){
                temp0 = kris_postion[0];
                temp1 = kris_postion[1]+50;
            }
            if(kris_face==3){
                temp0 = kris_postion[0];
                temp1 = kris_postion[1]-50;
            }
            if(pad_state & BUTTON_DIR){
                play_susie_anim();
                if(susie_postion[0]>temp0){
                    SPR_setHFlip(susie, TRUE);
                    susie_postion[0]-=1;
                    if(pad_state & BUTTON_B) susie_postion[0]-=1;
                    SPR_setAnim(susie, 4-world*3);
                }
                if(susie_postion[0]<temp0){
                    SPR_setHFlip(susie, FALSE);
                    susie_postion[0]+=1;
                    if(pad_state & BUTTON_B) susie_postion[0]+=1;
                    SPR_setAnim(susie, 4-world*3);
                }
                if(susie_postion[1]>temp1){
                    SPR_setHFlip(susie, FALSE);
                    susie_postion[1]-=1;
                    if(pad_state & BUTTON_B) susie_postion[1]-=1;
                    SPR_setAnim(susie, 5-world*3);
                }
                if(susie_postion[1]<temp1){
                    SPR_setHFlip(susie, FALSE);
                    susie_postion[1]+=1;
                    if(pad_state & BUTTON_B) susie_postion[1]+=1;
                    SPR_setAnim(susie, 3-world*3);
                }
            }else susie_frame = 0;
            SPR_setFrame(susie, main_character_frames[susie_frame]);
            SPR_setPosition(susie, susie_postion[0], susie_postion[1]-7);
        }
        SPR_setPosition(kris, kris_postion[0]-2, kris_postion[1]);
        SPR_update();
        tick();
    }
    return 0;
}
