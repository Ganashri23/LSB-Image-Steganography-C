//Name:Ganashri S
//project Topic:steganography
//Description:Developed an LSB-based Image Steganography application to securely hide secret files within BMP images and extract them when required.
//Implemented bitwise operations, file handling, and command-line arguments for efficient encoding and decoding of hidden data.
//Time:10:57PM
//Date:14/9/2026
#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "common.h"
#include "types.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }


    // No failure return e_success
    return e_success;
}


Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
   char *ext = strchr(argv[2], '.');
   //check in argv[2] .bmp present or not
   if(ext != NULL & strcmp(ext, ".bmp")==0)
   {
        //update into structure
        //encInfo->src_image_fname = argv[2];
        encInfo->src_image_fname = argv[2];
   }
   else
   {
    //if no return e_failure
        return e_failure;
   }

    //check in argv[3] . is present or not
    if(strchr(argv[3], '.')!= NULL)
    {
        //update into structure
        //encInfo->secret_frame = argv[3];
        encInfo->secret_fname = argv[3];
    }
    else
    {
    //if no return e_failure
        return e_failure;
    }

//check argv[4] is NULL or not
if(argv[4] != NULL)
{
    //if not NULL
    //check in argv[4] .bmp is present or not
    char *ext = strchr(argv[4],'.');
    if(ext != NULL & strcmp(ext, ".bmp")==0)
    {
        //update into structure
        //encInfo->stego_image_fname = argv[4];
        encInfo->stego_image_fname = argv[4];
    }
    else
    {    
        //if no return e_failure
        return e_failure;
    }
}
else
{
    //if it is NULL
    //store default name
    //encInfo->stego_image_fname = stego.bmp;
    encInfo->stego_image_fname = "stego.bmp";
}

    //return e_success;
    return e_success;

}

Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) == e_failure)
    {
        //print error message return e_failure
        printf("ERROR: Unable to open files\n");
        return e_failure;
    }

    if(check_capacity(encInfo) == e_failure)
    {
        //print error message return e_failure
        printf("ERROR: Insufficient image capacity\n");
        return e_failure;

    }

    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        //print error message return e_failure
        printf("ERROR: Failed to copy BMP header\n");
        return e_failure;
    }

    if(encode_magic_string(MAGIC_STRING, encInfo) == e_failure)
    {
        //print error message return e_failure
        printf("ERROR: Failed to copy MAGIC string\n");
        return e_failure;
    }
    //get the extension and store into the array and declare
    int size=0;
    size = strlen(encInfo->extn_secret_file);

    if(encode_secret_file_extn_size(size , encInfo) == e_failure)
    {
        //print error message return e_failure
        printf("ERROR: Failed to copy EXTN size\n");
        return e_failure;
    }

    if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_failure)
    {
        //print error message return e_failure
        printf("ERROR: Failed to copy SECRET file\n");
        return e_failure;
    }
    if(encode_secret_file_size(encInfo->size_secret_file,
                           encInfo) == e_failure)
{
    printf("ERROR: Failed to encode file size\n");
    return e_failure;
}

if(encode_secret_file_data(encInfo) == e_failure)
{
    printf("ERROR: Failed to encode file data\n");
    return e_failure;
}

    
    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
        {
            printf("ERROR: Failed to copy remaining image data\n");
            return e_failure;
            
        }

        
        return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity =
            get_image_size_for_bmp(encInfo->fptr_src_image);

    encInfo->size_secret_file =
            get_file_size(encInfo->fptr_secret);

    int required_size =
    (
        strlen(MAGIC_STRING) +
        sizeof(int) +
        strlen(encInfo->extn_secret_file) +
        sizeof(int) +
        encInfo->size_secret_file
    ) * 8 + 54;

    printf("Image capacity = %u\n", encInfo->image_capacity);
    printf("Secret file size = %ld\n", encInfo->size_secret_file);
    printf("Extension = %s\n", encInfo->extn_secret_file);
    printf("Required size = %d\n", required_size);

    if(encInfo->image_capacity >= required_size)
    {
        return e_success;
    }

    return e_failure;
}
long get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);

    long size = ftell(fptr);

    rewind(fptr);

    printf("Actual file size = %ld\n", size);

    return size;
}


Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{

    char buffer[54];
    //rewind() both src and dest address
    rewind(fptr_src_image);
    rewind(fptr_dest_image);

    //read the 54 bytes for src
    //fread(buffer, 54, 1, fptr_src_image)
    fread(buffer, 54, 1, fptr_src_image);
    //write the 54 bytes into dest //fwrite()
    fwrite(buffer, 54, 1, fptr_dest_image);

    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    char image_buffer[8];
    //loop for magic string len times
    for(int i=0;magic_string[i] != '\0';i++)
    {
        //read the 8bytes of data from beautiful.bmp and store into image buffer
        fread(image_buffer, 8, 1, encInfo->fptr_src_image);
        //encode_byte_to_lsb(magic_string[i],image_buffer)
        encode_byte_to_lsb(magic_string[i], image_buffer);
        //write the image_buffer 8bytes of data into stego.bmp
        fwrite(image_buffer, 8, 1, encInfo->fptr_stego_image);
    }
        //return e_success
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{

    //loop for 8 times
    for(int i=0;i<8;i++)
    {
        //clear the lsb of image buffer        
        //get 1 bit from msb of data
        //shift the get 1 bit to lsb side
        //set the get bit and image buffer and store into image buffer
        image_buffer[i]=(image_buffer[i] & 0xFE) | ((data >> (7-i)) & 1);
    }
    return e_success;

}

Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
    char image_buffer[32];
    //get the etension from seceret file
    char *ext = strstr(encInfo->secret_fname,".");
    strcpy( encInfo->extn_secret_file,ext);
    
    //get the len of the extension
    int len = strlen(encInfo->extn_secret_file);

    //read the 32 bytes of data from beautiful.bmp store into image buffer
    fread(image_buffer, 32, 1, encInfo->fptr_src_image);

    //encode_int_to_lsb(len_of_extn, image_buffer);
    encode_int_to_lsb(len, image_buffer);

    //write the 32bytes of encoded data into stego.bmp
    fwrite(image_buffer, 32, 1, encInfo->fptr_stego_image);

    //return e_success
    return e_success;
}

Status encode_int_to_lsb(int size, char *image_buffer)
{
    for(int i = 0; i < 32; i++)
    {
        image_buffer[i] = (image_buffer[i] & 0xFE) |
                          ((size >> (31 - i)) & 1);
    }

    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char image_buffer[8];

    for(int i = 0; file_extn[i] != '\0'; i++)
    {
        fread(image_buffer, 8, 1, encInfo->fptr_src_image);

        encode_byte_to_lsb(file_extn[i], image_buffer);

        fwrite(image_buffer, 8, 1, encInfo->fptr_stego_image);
    }

    return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
   
    char ch;

    while (fread(&ch, 1, 1, fptr_src) > 0)
    {
        fwrite(&ch, 1, 1, fptr_dest);
    }

    return e_success;
}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char image_buffer[32];

    fread(image_buffer, 32, 1, encInfo->fptr_src_image);

    encode_int_to_lsb((int)file_size, image_buffer);

    fwrite(image_buffer, 32, 1, encInfo->fptr_stego_image);

    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char image_buffer[8];
    char ch;

    rewind(encInfo->fptr_secret);

    while (fread(&ch, 1, 1, encInfo->fptr_secret) > 0)
    {
        fread(image_buffer, 8, 1, encInfo->fptr_src_image);

        encode_byte_to_lsb(ch, image_buffer);

        fwrite(image_buffer, 8, 1, encInfo->fptr_stego_image);
    }

    return e_success;
}