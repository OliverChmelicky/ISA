//
// Created by olo on 19.10.19.
//

#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include "parseArgs.h"

int parsePort(int argc, char *argv[]){
    int opt;
    if (argc != 3) {
        return -1;
    }

    while((opt = getopt(argc, argv, ":p:")) != -1)
    {
        switch(opt)
        {
            case 'p':
                char *endPtr;
                long port;
                port = strtol(optarg,&endPtr,10);
                if (*endPtr != '\0'){
                    return -1;
                }
                return port;
            case '?':
                printf("unknown option: %c\n", optopt);
                return -1;
        }
    }

    return -1;
}