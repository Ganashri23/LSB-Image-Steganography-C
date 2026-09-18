#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "common.h"
#include "types.h"


/* Read and Validate Arguments */

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    char *ext = strstr(argv[2], ".");

    if(ext != NULL && strcmp(ext, ".bmp") == 0)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }

    if(argv[3] != NULL)
    {
        strcpy(decInfo->output_fname, argv[3]);
    }
    else
    {
        strcpy(decInfo->output_fname, "output");
    }

    return e_success;
}

/* Open Files */

Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");

    if(decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        return e_failure;
    }

    return e_success;
}

/* Skip BMP Header */

Status skip_bmp_header(FILE *fptr_stego_image)
{
    fseek(fptr_stego_image, 54, SEEK_SET);

    return e_success;
}

/* Decode Byte */

Status decode_lsb_to_byte(char *data, char *image_buffer)
{
    *data = 0;

    for(int i = 0; i < 8; i++)
    {
        *data = (*data << 1) | (image_buffer[i] & 1);
    }

    return e_success;
}

/* Decode Integer */

Status decode_lsb_to_int(int *size, char *image_buffer)
{
    *size = 0;

    for(int i = 0; i < 32; i++)
    {
        *size = (*size << 1) | (image_buffer[i] & 1);
    }

    return e_success;
}

/* Decode Magic String */

Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
    char image_buffer[8];

    for(int i = 0; i < strlen(MAGIC_STRING); i++)
    {
        fread(image_buffer, 8, 1, decInfo->fptr_stego_image);

        decode_lsb_to_byte(&magic_string[i], image_buffer);
    }

    magic_string[strlen(MAGIC_STRING)] = '\0';

    return e_success;
}

/* Decode Extension Size */

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char image_buffer[32];

    fread(image_buffer, 32, 1, decInfo->fptr_stego_image);

    decode_lsb_to_int((int *)&decInfo->extn_size, image_buffer);

    return e_success;
}

/* Decode Extension */

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char image_buffer[8];

    for(int i = 0; i < decInfo->extn_size; i++)
    {
        fread(image_buffer, 8, 1, decInfo->fptr_stego_image);

        decode_lsb_to_byte(&decInfo->extn_output_file[i],
                           image_buffer);
    }

    decInfo->extn_output_file[decInfo->extn_size] = '\0';

    return e_success;
}

/* Decode Secret File Size */

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char image_buffer[32];

    fread(image_buffer, 32, 1, decInfo->fptr_stego_image);

    decode_lsb_to_int((int *)&decInfo->size_output_file,
                      image_buffer);

    return e_success;
}

/* Decode Secret File Data */

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char image_buffer[8];
    char ch;

    for(int i = 0; i < decInfo->size_output_file; i++)
    {
        fread(image_buffer, 8, 1,
              decInfo->fptr_stego_image);

        decode_lsb_to_byte(&ch, image_buffer);

        fwrite(&ch, 1, 1, decInfo->fptr_output);
    }

    return e_success;
}

/* Main Decoding Function */

Status do_decoding(DecodeInfo *decInfo)
{
    if(open_decode_files(decInfo) == e_failure)
        return e_failure;

    skip_bmp_header(decInfo->fptr_stego_image);

    char magic[10];

    decode_magic_string(magic, decInfo);

    char user_magic[10];

    printf("Enter Magic String : ");
    scanf("%9s", user_magic);

    printf("Decoded Magic String = %s\n", magic);
    printf("User Entered Magic String = %s\n", user_magic);

    if(strcmp(magic, user_magic) != 0)
    {
    printf("Magic String Mismatch\n");
    return e_failure;
    }

    decode_secret_file_extn_size(decInfo);
    printf("Extension Size = %d\n", decInfo->extn_size);

    decode_secret_file_extn(decInfo);
    printf("Extension = %s\n", decInfo->extn_output_file);

    strcat(decInfo->output_fname,
           decInfo->extn_output_file);

    decInfo->fptr_output =fopen(decInfo->output_fname, "w");

    if(decInfo->fptr_output == NULL)
    {
        return e_failure;
    }

    decode_secret_file_size(decInfo);
    printf("Secret File Size = %d\n", decInfo->size_output_file);

    decode_secret_file_data(decInfo);

    fclose(decInfo->fptr_stego_image);
    fclose(decInfo->fptr_output);

    

    return e_success;
}