/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.1
        Device            :  PIC24FJ256GA705
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB 	          :  MPLAB X v5.50
 */

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
 */

/**
  Section: Included Files
 */
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/sd_spi/sd_spi.h"
#include "mcc_generated_files/fatfs/ff.h"
#include "mcc_generated_files/fatfs/diskio.h"
#include "mcc_generated_files/clock.h"

#define FILE_NAME "test.txt"
#define READ_BUFF_LEN 32

/*
                         Main application
 */

typedef enum
{
    CHECK_MEDIA,
    MOUNT,
    CREATE_FILE,
    WRITE_STRING_DATA,
    READ_DATA,
    CLOSE_FILE,
    IDLE
} WRITE_PROCESS;

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    //variables declaration
    FATFS file_system;
    FIL file;
    UINT no_bytes_written, no_bytes_read;
    //buffer with the string that will be written 
    BYTE write_buffer[] = "1 2 3 4 5";
    //
    BYTE read_buffer[READ_BUFF_LEN];
    WRITE_PROCESS current_state = CHECK_MEDIA;

    puts("UART test\r\n");

    while (1) {
        switch (current_state) {
            case CHECK_MEDIA:
            {
                if (SD_SPI_IsMediaPresent()) {
                    puts("Media present\n");
                    current_state = MOUNT;
                }
                else {
                    puts("Media not present\n");
                    current_state = IDLE;
                }
                break;
            }

            case MOUNT:
            {
                if (f_mount(&file_system, "0:", 1) == FR_OK) {
                    puts("Mounts ok\n");
                    current_state = CREATE_FILE;
                }
                else {
                    puts("Problem at mounting\n");
                    current_state = IDLE;
                }
                break;
            }

            case CREATE_FILE:
            {
                if (f_open(&file, FILE_NAME, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) {
                    puts("File created ok\n");
                    current_state = WRITE_STRING_DATA;
                }
                else {
                    puts("Couldn't create the file\n");
                    current_state = IDLE;
                }
                break;
            }

            case WRITE_STRING_DATA:
            {
                //first write a string
                if (f_write(&file, write_buffer, sizeof (write_buffer), &no_bytes_written) == FR_OK) {
                    puts("Data written successfully\r\n");
                    f_close(&file);
                    current_state = READ_DATA;
                }
                else {
                    puts("Couldn't write data\r\n");
                    current_state = IDLE;
                }
                break;
            }

            case READ_DATA:
            {
                if (f_open(&file, FILE_NAME, FA_READ | FA_WRITE) == FR_OK) {
                    FRESULT read_res = f_read(&file, read_buffer, sizeof (read_buffer), &no_bytes_read);
                    if (read_res == FR_OK) {
                        printf("Number of bytes written %d\n", no_bytes_written);
                        printf("Number of bytes read %d\n", no_bytes_read);
                        printf("File content is: %s\n", read_buffer);

                        current_state = CLOSE_FILE;
                    }
                    else {
                        puts("Could not read from the file\r\n");
                        current_state = IDLE;
                    }
                }
                else {
                    puts("Could not open the file\r\n");
                    current_state = IDLE;
                }
                break;
            }

            case CLOSE_FILE:
            {
                puts("Close the file and unmount the drive\n");

                f_close(&file);

                f_unmount("0:");

                current_state = IDLE;
            }

            case IDLE:
            {
                Nop();
            }
        }
    }

    return 1;
}
/**
 End of File
 */

