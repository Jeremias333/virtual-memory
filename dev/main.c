#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define DEFAULT_ERR_MSG "\nOcorreu um problema: "
#define CLOSE "PROGRAMA ENCERRADO\n"

int choice_path(char **argv);
int exists_file_path(char *path);
void print_err(char *err);

int main(int argc, char **argv) {
    char *address_path = argv[1];
    // printf("Values %d", exists_file_path(address_path));
    printf("Dev area operating...\n");

    if (exists_file_path(address_path) == 0){
        char *ERR = DEFAULT_ERR_MSG"Não foi passado um parametro com o caminho na inicialização do programa";
        print_err(ERR);
        return 1;
    }

    int program_path = choice_path(argv);

    printf("%d\n", program_path);
    if (program_path == -1){
        char *ERR = DEFAULT_ERR_MSG"Impossível inicializar o programa com apenas um parametro de algoritmo";
        print_err(ERR);
        return 1;
    }

    if(program_path == -2){
        char *ERR = DEFAULT_ERR_MSG"Impossível inicializar o programa sem parametros de algoritmo";
        print_err(ERR);
        return 1;
    }
    return 0;
}

int choice_path(char **argv){
    int path = 0;
    /*
        0 fifo and fifo
        1 fifo and lru
        2 lru and fifo
    */

    // printf("Arg 1 - %s\nArg 2 - %s \n", argv[2], argv[3]);

    //Caso o segundo parametro não existe, o 3º parametro é impossivel de existir
    if(argv[2] == NULL){
        path = -2;

    // Os dois valores passados são nulos.
    }else if(argv[2] != NULL && argv[3] == NULL){
        path = -1;
    }else if (strcmp(argv[2], "fifo") == 0 && strcmp(argv[3], "fifo") == 0){
        path = 0;
    }else if(strcmp(argv[2], "fifo") == 0 && strcmp(argv[3], "lru") == 0){
        path = 1;
    }else if(strcmp(argv[2], "lru") == 0 && strcmp(argv[3], "fifo") == 0){
        path = 2;
    }else{
        path = -1;
    }

    return path;
}

int exists_file_path(char *path){
    if (path == NULL){
        return 0;
    }
    return 1;
}

void print_err(char *err){
    printf("%s\n", err);
    printf(CLOSE);
}