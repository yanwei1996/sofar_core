
//gcc .\test.c ..\sdk_print.c -I../ -lpthread

#include <stdio.h>
#include <unistd.h>
#include "sdk_print_error.h"
#include "sdk_print_debug.h"

int main(int argc, char* argv[])
{
    char str1[8]="123";
    char str2[8]="abc";
    while(1)
    {
       DEBUG("str1=%s\n", str1);
       ERROR("str2=%s\n", str2);
       sleep(1);
    }
    
    return 0;
}

