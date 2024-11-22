#include <genesis.h>
#include "resource.h"
#define start_cut_do_sram_offset 0
#define pal_do_sram_offset 1
#define story_been_sram_offset 2
#define room_sram_offset 2
const Palette *room_palette_pointer[]={
    &rb_krisroom_pal, &rb_krishallway_pal, &rb_torhouse_pal
};

const TileSet *room_tileset_pointer[]={
    &rb_krisroom_tilset, &rb_krishallway_tilset, &rb_torhouse_tilset
};

const Map *room_map_pointer[]={
    &rb_krisroom_map, &rb_krishallway_map, &rb_torhouse_map
};

const u8 room_door_box_num[][2]={
    {0, 0}, {1, 2}, {3, 3}
};
const s32 room_door_rects[][7]={
    //my room
    {145, 235, 40, 5, 1, 0, 0},
    //my hallway
    {282, 125, 32, 16, 0, 0, 0},
    {418, 59, 40, 52, 2, 0, 0},
    //toriel house
    {282, 125, 32, 16, 1, 0, 0}
};
const u8 main_character_frames[4]={0, 1, 0, 2};
s32 temp0=0;
s32 temp1=0;
s32 temp2=0;
u8 pad_state = 0;
u16 room=0;
s16 kris_postion[2] = {0, 0};
s16 susie_postion[2] = {0, 0};
s16 ralsei_postion[2] = {0, 0};
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
Map* map = NULL;
void tick(){
    VDP_waitVSync();
    SYS_doVBlankProcess();
}
void set_scroll(){
    if((kris_postion[0]-145)>=0){
        planescrollx=kris_postion[0]-145;
    }
    if((kris_postion[1]-100)>=0){
        planescrolly=kris_postion[1]-100;
    }
    VDP_setHorizontalScroll(BG_B, -planescrollx);
    VDP_setVerticalScroll(BG_B, planescrolly);
}
//room functions
void draw_room(u8 set_pal){
    VDP_setPlaneSize(128, 128, TRUE);
    MEM_free(map);
    map = MAP_create(room_map_pointer[room], BG_B, TILE_ATTR(PAL0, 0, 0, 0));
    //if(set_pal)PAL_setPalette(PAL0, room_palette_pointer[room]->data, DMA);
    //VDP_loadTileSet(room_tileset_pointer[room], 0, DMA);
}

void door_collision(){
    for(u16 i=room_door_box_num[room][0];i<=room_door_box_num[room][1]; ++i){
        if(kris_postion[0]>room_door_rects[i][0]){
            if(kris_postion[0]<(room_door_rects[i][0]+room_door_rects[i][2])){
                if((kris_postion[1]+48)>(room_door_rects[i][1])){
                    if((kris_postion[1]+48)<(room_door_rects[i][1]+room_door_rects[i][3])){
                        PAL_fadeOutAll(10, FALSE);
                        for(u16 j=0;j<15;++j)tick();
                        kris_postion[0]=room_door_rects[i][5];
                        kris_postion[1]=room_door_rects[i][6];
                        susie_postion[0]=room_door_rects[i][5];
                        susie_postion[1]=room_door_rects[i][6];
                        ralsei_postion[0]=room_door_rects[i][5];
                        ralsei_postion[1]=room_door_rects[i][6];
                        planescrollx=0;
                        planescrolly=0;
                        room=room_door_rects[i][4];
                        set_scroll();
                        SPR_setPosition(kris, kris_postion[0]-planescrollx, kris_postion[1]-planescrolly);
                        SPR_setPosition(susie, susie_postion[0]-planescrollx, susie_postion[1]-planescrolly);
                        SPR_setPosition(ralsei, ralsei_postion[0]-planescrollx, ralsei_postion[1]-planescrolly);
                        SPR_update();
                        draw_room(FALSE);
                        PAL_fadeIn(0, 15, room_palette_pointer[room]->data, 10, FALSE);
                        PAL_fadeIn(16, 31, spr_kris.palette->data, 10, FALSE);
                        PAL_fadeIn(32, 47, spr_susie.palette->data, 10, FALSE);
                        PAL_fadeIn(48, 63, spr_ralsei.palette->data, 10, FALSE);
                        break;
                    }
                }
            }
        }
    }
}
void drawTextUT(VDPPlane plane, const char* text, u16 x, u16 y, u16 delay,u8 play_sound, const u8 *sound_p, u32 sound_l){
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
        if(play_sound)SND_startPlay_PCM(sound_p, sound_l, SOUND_RATE_13400, SOUND_PAN_CENTER, 0);
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
        SND_startPlay_XGM(MUS_AUDIO_DRONE);
        VDP_clearPlane(BG_A, TRUE);
        VDP_clearPlane(BG_B, TRUE);
        Sprite* heart = SPR_addSpriteSafe(&heart_sprite_intro, 130, 90+8, TILE_ATTR(PAL1, 0, FALSE, FALSE));
        while(TRUE){
            pad_state = JOY_readJoypad(JOY_1);
            if(temp1 == 0){
                VDP_clearPlane(BG_B, FALSE);
                if(line == 0){
                    drawTextUT(BG_B, "ARE YOU", 14, 10, 5, FALSE, 0, 0);
                    drawTextUT(BG_B, "THERE?", 14, 11, 5, FALSE, 0, 0);
                }
                else if(line == 1){
                    drawTextUT(BG_B, "ARE WE", 14, 10, 5, FALSE, 0, 0);
                    drawTextUT(BG_B, "CONNECTED?", 14, 11, 5, FALSE, 0, 0);
                }
                else if(line == 2){
                    temp0 = -16;
                    temp1 = -54;
                    temp2 = 0;
                    while(TRUE){
                        pad_state=JOY_readJoypad(JOY_1);
                        if(temp2==4)temp2=0;
                        if(temp0>152)temp0=152;
                        if(temp1>114)temp1=114;
                        if(pad_state & BUTTON_B){
                            temp0=152;
                            temp1=114;
                        }
                        switch (temp2)
                        {
                        case 0:
                            SPR_setPosition(heart, temp0, 114);
                            break;
                        case 1:
                            SPR_setPosition(heart, 304-temp0, 114);
                            break;
                        case 2:
                            SPR_setPosition(heart, 152, temp1);
                            break;
                        case 3:
                            SPR_setPosition(heart, 152, 228-temp1);
                            break;
                        }
                        SPR_update();
                        if((temp0==152)&&(temp1==114))break;
                        temp0++;
                        temp1++;
                        temp2++;
                        tick();
                    }
                }
                else if(line == 3){
                    drawTextUT(BG_B, "EXCELLENT.", 15, 8, 5, FALSE, 0, 0);
                }else if(line == 4){
                    drawTextUT(BG_B, "TRULY", 17, 8, 5, FALSE, 0, 0);
                    drawTextUT(BG_B, "EXCELLENT.", 15, 9, 5, FALSE, 0, 0);
                }else if(line == 5){
                    drawTextUT(BG_B, "NOW.", 17, 8, 5, FALSE, 0, 0);
                }else if(line == 6){
                    drawTextUT(BG_B, "WE MAY", 17, 8, 5, FALSE, 0, 0);
                    drawTextUT(BG_B, "BEGIN.", 17, 9, 5, FALSE, 0, 0);
                }else if(line == 7){
                    temp0=152;
                    temp1=114;
                    temp2=0;
                    while(TRUE){
                        pad_state=JOY_readJoypad(JOY_1);
                        if(temp2==4)temp2=0;
                        if(temp0<-16)temp0=-16;
                        if(temp1<-16)temp1=-16;
                        if(pad_state & BUTTON_B){
                            temp0=-16;
                            temp1=-16;
                        }
                        switch (temp2)
                        {
                        case 0:
                            SPR_setPosition(heart, temp0, 114);
                            break;
                        case 1:
                            SPR_setPosition(heart, 304-temp0, 114);
                            break;
                        case 2:
                            SPR_setPosition(heart, 152, temp1);
                            break;
                        case 3:
                            SPR_setPosition(heart, 152, 228-temp1);
                            break;
                        }
                        SPR_update();
                        if((temp0==-16)&&(temp1==-16))break;
                        temp0--;
                        temp1--;
                        temp2++;
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
        SRAM_enable();
        SRAM_writeByte(start_cut_do_sram_offset, 1);
        SRAM_disable();
    }
    if(cutscences==1){
        temp0=0;
        temp1=TRUE;
        while(TRUE){
            PAL_setPalette(PAL0, palette_black, CPU);
            pad_state = JOY_readJoypad(JOY_1);
            if(pad_state & BUTTON_A)temp1 = TRUE;
            if(temp1==TRUE){
                VDP_drawImageEx(BG_A, &text_box, TILE_ATTR(PAL3, 0, FALSE, FALSE), 2, 15, FALSE, FALSE);
                switch(temp0){
                    case 0:
                        drawTextUT(BG_A, "* Kris!", 3, 18, 1, TRUE, snd_toriel, 1280);
                        break;
                    case 1:
                        drawTextUT(BG_A, "* Kris, If you don't wake up, we ", 3, 18, 1, TRUE, snd_toriel, 1280);
                        drawTextUT(BG_A, "will be late for school!", 3, 19, 1, TRUE, snd_toriel, 1280);
                        break;
                    case 2:
                        drawTextUT(BG_A, "* I will wait outside for you, ", 3, 18, 1, TRUE, snd_toriel, 1280);
                        drawTextUT(BG_A, "alright?", 3, 19, 1, TRUE, snd_toriel, 1280);
                        break;
                }
                ++temp0;
                if(temp0==4)break;
                temp1=FALSE;
            }
            tick();
        }
        VDP_clearPlane(BG_A, TRUE);
        draw_room(FALSE);
        kris_postion[0]=210;
        kris_postion[1]=100;
        SPR_setAnim(kris, 4);
        SPR_setHFlip(kris, TRUE);
        PAL_fadeTo(0, 15, room_palette_pointer[room]->data, 10, TRUE);
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
    VDP_setPlaneSize(32, 32, FALSE);
    PAL_setPalette(PAL0, battle_back.palette->data, CPU);
    for(temp0 = 0;temp0<4; ++temp0){
        for(temp1 = 0;temp1<5; ++temp1){
            VDP_drawImageEx(BG_A, &battle_back, TILE_ATTR(PAL0, 0, FALSE, FALSE), temp0*8, temp1*8, FALSE, TRUE);
        }
    }
}

void battle_init(){
    VDP_setHorizontalScroll(BG_B, 0);
    VDP_setVerticalScroll(BG_B, 0);
    VDP_setHorizontalScroll(BG_A, 0);
    VDP_setVerticalScroll(BG_A, 0);
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
    temp0=0;
    while(TRUE){
        --temp0;
        VDP_setHorizontalScroll(BG_A, temp0);
        VDP_setHorizontalScroll(BG_A, -temp0);
        tick();
    }
}
void init_all(){
    VDP_init();
    VDP_resetScreen();
    VDP_resetSprites();
    JOY_init();
    SPR_init();
    VDP_setHorizontalScroll(BG_A, 0);
    VDP_setVerticalScroll(BG_A, 3);
    setRandomSeed((IS_PALSYSTEM*5-4)/3);
}
void pause_menu(){
    if(world == 0){
        VDP_drawImageEx(WINDOW, &battle_back, TILE_ATTR(PAL0, 0, FALSE, FALSE), 1, 1, FALSE, TRUE);
        while(TRUE){
            pad_state = JOY_readJoypad(JOY_1);
            if(pad_state & BUTTON_C){
                
            }
        }
    }
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
    SRAM_enableRO();
    temp0 = SRAM_readByte(start_cut_do_sram_offset);
    SRAM_disable();
    if((isHardReset == TRUE)&&(temp0==0)){
        playCutscences(0);
    }
    SRAM_enableRO();
    temp0=SRAM_readByte(pal_do_sram_offset);
    SRAM_disable();
    if((IS_PALSYSTEM)&&(temp0==0)){
        VDP_drawText("You are useing a pal TV!", 6 ,12);
        VDP_drawText("Somthing might be wrong!", 6 ,13);
        VDP_drawText("Press any to continue", 6 ,14);
        while(!(JOY_readJoypad(JOY_1) & BUTTON_ALL)){
            tick();
        }
        SRAM_enable();
        SRAM_writeByte(pal_do_sram_offset, 1);
        SRAM_disable();
    }
    SPR_end();
    SPR_init();
    SND_startPlay_PCM(snd_appear, 33536, SOUND_RATE_13400, SOUND_PAN_CENTER, 0);
    VDP_drawImageEx(BG_A, &deltarune_logo, TILE_ATTR(PAL0, 0, FALSE, FALSE), 6, 10, FALSE, FALSE);
    PAL_setPalette(PAL0, palette_black, CPU);
    PAL_fadeIn(0, 15, deltarune_logo.palette->data, 15, TRUE);
    temp0 = 120-15;
    while(temp0 > 0){--temp0;tick();}
    VDP_clearPlane(BG_A, TRUE);
    VDP_setTextPalette(PAL3);
    kris = SPR_addSpriteSafe(&spr_kris, 0, 0, TILE_ATTR(PAL1, 1, FALSE, FALSE));
    PAL_setPalette(PAL1, spr_kris.palette->data, CPU);
    PAL_setPalette(PAL2, spr_susie.palette->data, CPU);
    PAL_setPalette(PAL3, spr_ralsei.palette->data, CPU);
    if(susie_in_party){
        susie = SPR_addSpriteSafe(&spr_susie, 0, 0, TILE_ATTR(PAL2, 1, FALSE, FALSE));
        SPR_setAlwaysOnTop(susie, TRUE);
    }
    if(ralsei_in_party){
        ralsei = SPR_addSpriteSafe(&spr_ralsei, 0, 0, TILE_ATTR(PAL3, 1, FALSE, FALSE));
        SPR_setAlwaysOnTop(ralsei, TRUE);
    }
    SPR_setAlwaysOnTop(kris, TRUE);
    //battle_logic();
    playCutscences(1);
    while(TRUE){
        pad_state = JOY_readJoypad(JOY_1);
        door_collision();
        set_scroll();
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
        if(pad_state & BUTTON_C) pause_menu();
        SPR_setFrame(kris, main_character_frames[kris_frame]);
        if(!(pad_state & BUTTON_DIR)){
            kris_frame = 0;
            kris_delay = 0;
        }else play_kris_anim();
        SPR_setPosition(kris, kris_postion[0]-2-planescrollx, kris_postion[1]-planescrolly);
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
            SPR_setPosition(ralsei, ralsei_postion[0]-planescrollx, ralsei_postion[1]-6-planescrolly);
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
            SPR_setPosition(susie, susie_postion[0]-planescrollx, susie_postion[1]-7-planescrolly);
        }
        SPR_update();
        tick();
    }
    return 0;
}
