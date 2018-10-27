/************************************
 * This is the framework for those
 * who wish to write their own C
 * code for the basic badge
 ************************************/


#include "badge_user.h"
#include "nyancat.h"

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

        while(1) //Loop forever
        {
            pixel_idx=0;
            if(frame_idx > 11)
            {
                frame_idx = 0;
            }
            
            for (y=0;y<240;y++)
            {
                x = 0;

                while (x < 320)
                {
                    encoded_run = cat_frames[frame_idx][pixel_idx];
                    pal_entry = cat_palette[(encoded_run>>12)&0xF];
                    pal_run = encoded_run&0xFFF;
                    for (i = 0; i < pal_run; i++)
                    {
                        line[x+i] = pal_entry;
                    }
                    x += pal_run;
                    pixel_idx++;
                }

                tft_set_write_area(0,y,319,1);
                TFT_24_7789_Write_Command(0x2C);
                for (i=0;i<320;i++)
                {
                    pal_entry = line[i];
                    TFT_24_7789_Write_Data3((pal_entry>>16)&0xFF,(pal_entry>>8)&0xFF,(pal_entry>>0)&0xFF);
                }
            }
            
            wait_ms(40); // This number needs tuning. GIF says 70ms between frames but 70 feels slow.
            
            frame_idx++;
        }
        //Badge will need to be reset to continue
	}

