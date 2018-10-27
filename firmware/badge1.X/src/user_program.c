/************************************
 * This is the framework for those
 * who wish to write their own C
 * code for the basic badge
 ************************************/


#include "badge_user.h"
#include "nyancat.h"
#include "nyancat2.h"
#include "nyancat4.h"

void user_program_init(void)
	{
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
        
        const uint8_t multiplier = 2;
        uint8_t multiply_loop=0;
        
        while(1) //Loop forever
        {
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
            
            wait_ms(40); // This number needs tuning. GIF says 70ms between frames but 70 feels slow.
            
            frame_idx++;
        }
        //Badge will need to be reset to continue
	}

