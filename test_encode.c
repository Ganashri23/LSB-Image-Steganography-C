#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

OperationType check_operation_type(char *argv[]);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("ERROR: Invalid arguments\n");
        return 0;
    }

    OperationType ret;

    ret = check_operation_type(argv);
if (ret == e_encode)
{
    EncodeInfo encInfo;

    printf("Encode Operation\n");

    if (read_and_validate_encode_args(argv, &encInfo) == e_success)
    {
        strcpy(encInfo.extn_secret_file,
               strrchr(encInfo.secret_fname, '.'));

        if (do_encoding(&encInfo) == e_success)
        {
            printf("Encoding Successful\n");
        }
        else
        {
            printf("Encoding Failed\n");
        }
    }
    else
    {
        printf("Argument Validation Failed\n");
    }
}
    else if (ret == e_decode)
{
    DecodeInfo decInfo;

    printf("Decode Operation\n");

    if (read_and_validate_decode_args(argv, &decInfo) == e_success)
    {
        printf("Read and Validate Success\n");

        if (do_decoding(&decInfo) == e_success)
        {
            printf("Decoding Successful\n");
        }
        else
        {
            printf("do_decoding Failed\n");
        }
    }
    else
    {
        printf("Argument Validation Failed\n");
    }
}
else
    {
        printf("Unsupported Operation\n");
    }

    return 0;
}

OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}