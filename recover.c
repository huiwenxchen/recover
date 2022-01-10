#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//Define BYTE
typedef uint8_t BYTE;
//set block size 
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    
    //Check for correct argument count
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE");
        return 1;
    }

    //Open file
    FILE *file = fopen(argv[1], "r");

    //Check if file exists
    if (file == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Initialize counter
    int counter = 0;

    FILE *JPEG = NULL;

    // Initialize the length of file name of size 8 
    char filename[8];

    // Initialize buffer of block size
    BYTE buffer[BLOCK_SIZE];

    //while loop to repeat the process until the end of card
    while (fread(buffer, 1, BLOCK_SIZE, file) == BLOCK_SIZE)
    {
        //check if start of new JPEG with 4 signature bytes
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //check if first JPEG
            if (counter == 0)
            {
                //print JPEG file name starting with ### = 000
                sprintf(filename, "%03i.jpg", counter);
                //open new JPEG file
                JPEG = fopen(filename, "w");
                //write into the JPEG file from buffer
                fwrite(buffer, BLOCK_SIZE, 1, JPEG);
            }
            else 
            {
                //close JPEG file
                fclose(JPEG);
                //print new JPEG file name
                sprintf(filename, "%03i.jpg", counter);
                //open JPEG file
                JPEG = fopen(filename, "w");
                //write into JPEF file from buffer
                fwrite(buffer, BLOCK_SIZE, 1, JPEG);
            }
            //increase counter
            counter++;  
        }
        
        else
        {
            //if already found JPEG, counter is not zero
            if (counter != 0)
            {
                //write into JPEG file from buffer
                fwrite(buffer, BLOCK_SIZE, 1, JPEG);
            }
        }
    }
    
    //Close files
    fclose(file);
    fclose(JPEG);
} 
