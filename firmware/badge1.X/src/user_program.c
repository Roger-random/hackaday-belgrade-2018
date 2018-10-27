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
        /* Now do something interesting */
        enable_display_scanning(0); //Shut off auto-scanning of character buffer
        uint16_t x = 0;
        uint8_t y = 0;

        uint32_t line[320];
        
        uint16_t i = 0;
        uint32_t pixel_idx = 0;
        uint16_t encoded_run = 0;
        uint32_t pal_entry = 0;
        uint16_t pal_run = 0;
        uint8_t frame_idx = 0;
        
        uint8_t multiplier = 1;
        uint8_t multiply_loop=0;
        
        const uint8_t milliseconds_per_frame = 70;
        uint32_t time_for_next_frame = millis();
        
        while(1) //Loop forever
        {
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

