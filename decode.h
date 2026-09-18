#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types



#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 20

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *stego_image_fname; //bmp file name
    FILE *fptr_stego_image; //bmp file address

    /* Output File Info */
    char output_fname[50];
    FILE *fptr_output;
    int extn_size;
    char extn_output_file[MAX_FILE_SUFFIX];
    int size_output_file;

  

} DecodeInfo;


/* Decoding function prototype */


/* Read and validate decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *decInfo);

/* Skip bmp image header */
Status skip_bmp_header(FILE *fptr_stego_image);

/* get Magic String */
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo);

/* read the magic string the user and compare both if matched continue else stop */

/* decode secret file extenstion size */
Status decode_secret_file_extn_size( DecodeInfo *decInfo);

/* decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* decode a byte into LSB of image data array */
Status decode_lsb_to_byte(char *data, char *image_buffer);

//char decode_lsb_to_byte(char *image_buffer);

/* decode a lsb into int of image data array */
Status decode_lsb_to_int(int *size, char *image_buffer);
#endif