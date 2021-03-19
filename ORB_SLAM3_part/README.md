# Frame.cc
The current configuration of this file is that: get the yml files from the server but don't take time to store it locally.

To store it locally, you need to uncomment:
line 354: "FILE *fpWrite;"
line 386: "fpWrite = fopen(filepath, "wb");"
line 398: "fwrite(buff, sizeof(char), rec_len, fpWrite);"
line 402: "fclose(fpWrite);"

You can also uncomment the part of storing yml files produced by this program locally:

line 415 - 426 and line 428 - 432

You can also uncomment the part of reading yml files produced by this program locally or from server (in result/):

line 415 - 426 and line 442 - 463
