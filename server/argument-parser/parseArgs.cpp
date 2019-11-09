//
// Created by xchmel28 on 19.10.19.
//

#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include "parseArgs.h"

int parsePort(int argc, char *argv[]){
    if (argc == 2 && (0 == strcmp(argv[1], "-h")) ){
    	std::cout << "Usage:  ./isaserver -p <port>\n";
    	exit(0);
    }

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