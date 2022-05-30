#include "stdio.h"
#include "string.h"
#include "stdlib.h"

int choice_path(char **argv);

int main(int argc, char **argv) {
    char address_path = argv[0];
    printf("Dev area operating... %s", address_path);

    return 1;
}

int choice_path(char **argv){
    int path = 0;
    /*
        0 fifo and fifo
        1 fifo and lru
        2 lru and fifo
    */

    if(argv[2] == NULL && argv[3] == NULL){
        path = 0;
        // Os dois valores passados são nulos, nesse caso usaremos fifo para paginação e TLB. 
    }else if (argv[2] == "fifo" && argv[3] == "fifo"){
        path = 0;
    }else if(argv[2] == "fifo" && argv[3] == "lru"){
        path = 1;
    }else if(argv[2] == "lru" && argv[3] == "fifo"){
        path = 2;
    }

    return path;
}