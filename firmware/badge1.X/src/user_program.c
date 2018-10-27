/************************************
 * This is the framework for those
 * who wish to write their own C
 * code for the basic badge
 ************************************/


#include <xc.h>
#include "badge_user.h"
#include "nyancat.h"
#include "nyancat2.h"
#include "nyancat4.h"
#include "nyancat8.h"
#include "tune_player.h" // Mario theme music

const unsigned char* mario_measures[9] = {
    mario_main0,
    mario_main1,
    mario_main2,
    mario_main3,
    mario_clip0,
    mario_clip1,
    mario_clip2,
    mario_clip3,
    mario_clip4,
};

const uint8_t mario_state_measures[16] = {
    0,4,5,4,5,6,7,6,1,6,7,6,2,8,3,8,
};

const uint8_t mario_state_next[16] = {
    1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,
};

uint32_t play_next_note(uint8_t measure_index, uint8_t note_index, uint8_t *next_note)
{
    uint32_t duration = 0;
    uint8_t  mario_index = 0;
    uint8_t  voice_index = 0;

    *next_note = 0;

    if (measure_index > 8)
    {
        return duration;
    }

    mario_index = note_index * 4;
    if (mario_index < mario_array_limits[measure_index])
    {
        for (voice_index = 0; voice_index < 3; voice_index++)
        {
            sound_set_note(mario_measures[measure_index][mario_index+voice_index],voice_index);
        }

        duration = mario_tempos[mario_measures[measure_index][mario_index+3]];
        *next_note = note_index+1;
    }

    return duration;
}

void user_program_init(void)
{
    K_R1 = 1;
    K_R2 = 1;
    K_R3 = 1;
    K_R4 = 1;
    K_R5 = 1;
}

void user_program_loop(void)
{
    enable_display_scanning(0); //Shut off auto-scanning of character buffer
    uint16_t x = 0;
    uint8_t y = 0;

    // 320 pixels wide line, each 32-bit value in 0xNNRRGGBB format where
    // NN = Not used, RR = 8-bit red, GG = 8-bit green, BB = 8-bit blue
    uint32_t line[320];

    uint16_t i = 0;
    uint32_t pixel_idx = 0;
    uint16_t encoded_run = 0;
    uint32_t pal_entry = 0;
    uint16_t pal_run = 0;
    uint8_t frame_idx = 0;

    // Image scaling control. 1=full scale, 2=half scale, 4=quarter, etc.
    uint8_t multiplier = 1;
    uint8_t multiply_loop=0;

    // Animation speed control
    const uint8_t milliseconds_per_frame = 70;
    uint32_t time_for_next_frame = millis();

    // Audio playback control
    // audio_state is the state machine that sequences measures into a song.
    // Each measure has zero or more 'notes', different from sheet music notes.
    // Each note is a set of three frequencies to be held fof a specific time.
    uint32_t time_for_audio_update = millis();
    uint8_t audio_state = 0;
    uint8_t next_audio_state = 0;
    uint8_t measure_index = 0;
    uint8_t note_index = 0;
    uint8_t next_note = 0;

    while(1) //Loop forever
    {
        // Is it time for the next frame?
        if (millis() >= time_for_next_frame)
        {
            time_for_next_frame += milliseconds_per_frame;

            pixel_idx=0;
            if(frame_idx > 11)
            {
                frame_idx = 0;
            }

            for (y=0;y<240;y+=multiplier)
            {
                x = 0;

                while (x < 320)
                {
                    switch(multiplier)
                    {
                        case 2:
                            encoded_run = cat2_frames[frame_idx][pixel_idx];
                            pal_entry = cat2_palette[(encoded_run>>12)&0xF];
                            break;
                        case 4:
                            encoded_run = cat4_frames[frame_idx][pixel_idx];
                            pal_entry = cat4_palette[(encoded_run>>12)&0xF];
                            break;
                        case 8:
                            encoded_run = cat8_frames[frame_idx][pixel_idx];
                            pal_entry = cat8_palette[(encoded_run>>12)&0xF];
                            break;
                        default:
                            encoded_run = cat1_frames[frame_idx][pixel_idx];
                            pal_entry = cat1_palette[(encoded_run>>12)&0xF];
                    }
                    pal_run = encoded_run&0xFFF;
                    for (i = 0; i < pal_run; i++)
                    {
                        for (multiply_loop=0; multiply_loop<multiplier; multiply_loop++)
                        {
                            line[x+i*multiplier+multiply_loop] = pal_entry;
                        }
                    }
                    x += pal_run*multiplier;
                    pixel_idx++;
                }

                tft_set_write_area(0,y,319,multiplier);
                TFT_24_7789_Write_Command(0x2C);
                for (multiply_loop=0; multiply_loop<multiplier; multiply_loop++)
                {
                    for (i=0;i<320;i++)
                    {
                        pal_entry = line[i];
                        TFT_24_7789_Write_Data3((pal_entry>>16)&0xFF,(pal_entry>>8)&0xFF,(pal_entry>>0)&0xFF);
                    }
                }
            }
            frame_idx++;
        }

        // Is it time for the next audio update?
        if (millis() >= time_for_audio_update)
        {
            if (audio_state != 0xFF)
            {
                measure_index = mario_state_measures[audio_state];
                next_audio_state = mario_state_next[audio_state];

                time_for_audio_update += play_next_note(measure_index,note_index,&next_note);
                if (next_note==0)
                {
                    audio_state = next_audio_state;
                }
                note_index = next_note;
            }
            else
            {
                // Fallback - silence for 10 seconds.
                sound_set_note(0,0);
                sound_set_note(0,1);
                sound_set_note(0,2);
                time_for_audio_update += 10000;
            }
        }

        // Check for specific keys that change our behavior.
        K_R1 = 0;

        if (K_C10==0) // '8' key is down
        {
            multiplier=8;
        }
        else if (K_C2==0) // '4' key is down
        {
            multiplier=4;
        }
        else if (K_C3==0) // '2' key is down
        {
            multiplier=2;
        }
        else if (K_C5==0) // '1' key is down
        {
            multiplier=1;
        }

        K_R1 = 1;
    }
    //Badge will need to be reset to continue
}
