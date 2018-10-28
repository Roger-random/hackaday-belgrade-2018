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


#define NYANCAT_SEQUENCES 16
#define NYANCAT_MEASURES 9

const uint8_t nyancat_durations[2] = {
    125,
    250,
};

const uint8_t nyancat_measure_limits[NYANCAT_MEASURES] = {
  44,60,56,48,52,56,52,52,52
};

const uint8_t nyancat_measure0[44] = {
    0,41,79,1,
    0,53,81,1,

    0,43,75,0,
    0,43,76,0,
    0,55,76,0,
    0,55,72,0,

    0,40,75,0,
    0,40,74,0,
    0,52,72,1,

    0,45,72,1,
    0,57,74,1,
};

const uint8_t nyancat_measure1[60] = {
    0,38,75,1,
    0,50,76,0,
    0,50,74,0,

    0,43,72,0,
    0,43,74,0,
    0,55,76,0,
    0,55,79,0,

    0,36,81,0,
    0,36,76,0,
    0,48,79,0,
    0,48,74,0,

    0,36,76,0,
    0,36,72,0,
    0,48,74,0,
    0,48,72,0,
};

const uint8_t nyancat_measure2[56] = {
    0,41,76,1,
    0,53,79,1,

    0,43,81,0,
    0,43,76,0,
    0,55,79,0,
    0,55,74,0,

    0,40,76,0,
    0,40,72,0,
    0,52,75,0,
    0,52,76,0,

    0,45,75,0,
    0,45,74,0,
    0,57,72,0,
    0,57,74,0,
};

const uint8_t nyancat_measure3[48] = {
    0,38,75,1,
    0,50,72,0,
    0,50,74,0,

    0,43,76,0,
    0,43,79,0,
    0,55,74,0,
    0,55,76,0,

    0,36,74,0,
    0,36,72,0,
    0,48,74,1,

    0,36,72,1,
    0,48,74,1,
};

const uint8_t nyancat_measure4[52] = {
    0,53,72,1,
    0,57,67,1,

    0,60,72,1,
    0,65,67,0,
    0,65,69,0,

    0,52,72,0,
    0,52,74,0,
    0,55,76,0,
    0,55,72,0,

    0,60,77,0,
    0,60,76,0,
    0,64,77,0,
    0,64,79,0,
};

const uint8_t nyancat_measure5[56] = {
    0,50,72,1,
    0,53,72,1,

    0,57,67,0,
    0,57,69,0,
    0,62,72,0,
    0,62,67,0,

    0,48,77,0,
    0,48,76,0,
    0,52,74,0,
    0,52,72,0,

    0,55,67,0,
    0,55,64,0,
    0,60,65,0,
    0,60,67,0,
};

const uint8_t nyancat_measure6[52] = {
    0,53,72,1,
    0,57,67,0,
    0,57,69,0,

    0,60,72,1,
    0,65,67,0,
    0,65,69,0,

    0,52,72,1,
    0,55,74,0,
    0,55,76,0,

    0,60,72,0,
    0,60,67,0,
    0,64,69,0,
    0,64,67,0,
};

const uint8_t nyancat_measure7[52] = {
    0,50,72,1,
    0,53,72,0,
    0,53,71,0,

    0,57,72,0,
    0,57,67,0,
    0,62,69,0,
    0,62,72,0,

    0,48,77,0,
    0,48,76,0,
    0,52,77,0,
    0,52,79,0,

    0,55,72,1,
    0,60,71,1,
};

const uint8_t nyancat_measure8[52] = {
    0,50,72,1,
    0,53,72,0,
    0,53,71,0,

    0,57,72,0,
    0,57,67,0,
    0,62,69,0,
    0,62,72,0,

    0,48,77,0,
    0,48,76,0,
    0,52,77,0,
    0,52,79,0,

    0,55,72,1,
    0,60,74,1,
};

const uint8_t nyancat_sequence[NYANCAT_SEQUENCES] = {
    0,1,2,3,0,1,2,3,4,5,6,7,4,5,6,8
};

const uint8_t* nyancat_measures[NYANCAT_MEASURES] = {
    nyancat_measure0,
    nyancat_measure1,
    nyancat_measure2,
    nyancat_measure3,
    nyancat_measure4,
    nyancat_measure5,
    nyancat_measure6,
    nyancat_measure7,
    nyancat_measure8,
};

uint32_t play_next_note(uint8_t measure_index, uint8_t note_index, uint8_t *next_note)
{
    uint32_t duration = 0;
    uint8_t  nyancat_index = 0;
    uint8_t  voice_index = 0;

    *next_note = 0;

    if (measure_index > 8)
    {
        return duration;
    }

    nyancat_index = note_index * 4;
    if (nyancat_index < nyancat_measure_limits[measure_index])
    {
        for (voice_index = 0; voice_index < 3; voice_index++)
        {
            sound_set_note(nyancat_measures[measure_index][nyancat_index+voice_index],voice_index);
        }

        duration = nyancat_durations[nyancat_measures[measure_index][nyancat_index+3]];
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
    uint8_t measure_index = 0;
    uint8_t note_index = 0;
    uint8_t next_note = 0;
    uint8_t mute = 0;

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
                measure_index = nyancat_sequence[audio_state];

                time_for_audio_update += play_next_note(measure_index,note_index,&next_note);
                if (next_note==0)
                {
                    audio_state++;
                    if (audio_state >= NYANCAT_SEQUENCES)
                    {
                        audio_state = 0;
                    }
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

        if (K_C10==0) // '8' key uses animation at 1/8th resolution
        {
            multiplier=8;
        }
        else if (K_C2==0) // '4' uses animation at 1/4th resolution
        {
            multiplier=4;
        }
        else if (K_C3==0) // '2' uses animation at 1/2 resolution
        {
            multiplier=2;
        }
        else if (K_C5==0) // '1' uses full resolution animation
        {
            multiplier=1;
        }
        else if (K_C9==0) // '0' immediately mutes music.
        {
            mute = 1;
            sound_set_note(0,0);
            sound_set_note(0,1);
            sound_set_note(0,2);
            audio_state = 0xFF;
        }
        else if (K_C6==0 && mute) // '9' will restart music if muted, otherwise no effect.
        {
            mute = 0;

            time_for_audio_update = millis();
            audio_state = 0;
            note_index = 0;
        }

        K_R1 = 1;
    }
    //Badge will need to be reset to continue
}
