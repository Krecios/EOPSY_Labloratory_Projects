#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int ReadWriteCopy(int InputFile, int OutputFile)
{
    size_t CopySize;
    char Buffer[524288]; //512kb Buffer; 524288 = 512 * 1024
    size_t CutOffByte = sizeof(Buffer);
    while((CopySize = read(InputFile, &Buffer, CutOffByte)) > 0)    //reading the content of the file
    {
        if(write(OutputFile, &Buffer, CopySize) < 0)    //checking for errors in write function, while simuntanelously copying the input
        {
            printf("[RW] ERROR WHILE WRITING TO THE OUTPUT FILE! ABORTING!!!\n");
            return 1;
        }
    }
    if(CopySize < 0)    //checking for errors in reading the Input
    {
        printf("[RW] ERROR WHILE READING THE INPUT FILE! ABORTING!!!\n");
        return 1;
    }
}

int MMapCopy(int InputFile, int OutputFile)
{
    struct stat FileStatistic;  //stat returns informations about the file
    void *InputMap, *OutputMap;
    if(fstat(InputFile, &FileStatistic) < 0)    //fstat takes information from the input file
    {
        printf("FileSatistic ON INPUT HAS FAILED! ABORT!!!\n");
        return 1;
    }
    ftruncate(OutputFile, FileStatistic.st_size);   //ftruncate truncates the file according to the size of data form the inputfile, while simuntanelously writing the content of the Input
    if((memcpy(OutputFile, InputFile, FileStatistic.st_size) == NULL) printf("MEMCPY ERROR! ABORT!!!\n"); //memcpy - copies the values of num bytes from the location pointed to by source directly to the memory block pointed to by destination
    if((InputMap = mmap(0, FileStatistic.st_size, PROT_READ, MAP_SHARED, InputFile, 0)) == MAP_FAILED) printf("MMAP INPUT ERROR! ABORT!!!\n");
    if((OutputMap = mmap(0, FileStatistic.st_size, PROT_WRITE, MAP_SHARED, OutputFile, 0)) == MAP_FAILED) printf("MMAP OUTPUT ERROR! ABORT!!!\n");
    //mmap - crreates new mapping in the virtual memory (mapping address, length, protection type, process visibility, file, offset)
}

int main (int argc, char **argv)
{
    char Input;
    int Mode = 0, InputFile = 0, OutputFile = 0;
    printf("TEST\n");
    while ((Input = getopt(argc, argv, "mh")) != -1)
    {
        switch (Input)
        {
            case 'm':
            printf("MODE MMAP SELECTED\n");
            Mode = 1;
            break;
            case 'h':
            printf("MODE HELP SELECTED\n");
            return 0;
            break;
            default:
            printf("INVALID MODE! ABORT!!!\n");
            break;
        }
    }

    if((InputFile = open(argv[optind], O_RDONLY)) < 0)
    {
        printf("CANNOT OPEN THE INPUT FILE! ABORT!!!\n");
        return 1;
    }

    if((OutputFile = open(argv[optind+1], O_RDWR | O_TRUNC | O_CREAT, S_IROTH)) < 0)
    {
        printf("CANNOT OPEN THE OUTPUT FILE! ABORT!!!\n");
        return 1;
    }

    if(!Mode) ReadWriteCopy(InputFile, OutputFile);
    else MMapCopy(InputFile,OutputFile);

    if(close(InputFile))
    {
        printf("CANNOT CLOSE INPUT FILE! ABORT!!!\n");
        return 1;
    }
    if(close(OutputFile))
    {
        printf("CANNOT CLOSE OUTPUT FILE! ABORT!!!\n");
        return 1;
    }

    return 0;
}