#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "fcntl.h"
#include "sys/types.h"
#include "sys/mman.h"
#include "sys/stat.h"

//Error CONSTANTES
#define DEFAULT_ERR_MSG "\nOcorreu um problema: "
#define CLOSE "PROGRAMA ENCERRADO\n"

//Page CONSTANTES
#define PAGE_SIZE 256

//tlb CONSTANTES
#define TLB_ENTRIES 16

//Memory CONSTANTES
#define MEMORY_SIZE 128

//BACKING_STORE CONSTANTES
#define BACKING_STORE_FILE_NAME "BACKING_STORE.bin"

//output file CONSTANTES
#define CORRECT_FILE_NAME "correct.txt" 

FILE *arq_address;
FILE *arq_correct_output;

typedef struct {
   char physical_memory[MEMORY_SIZE];
} physicalMemory;

physicalMemory physical_memory_array[MEMORY_SIZE];


int virtual_address;
int physical_address;
int offset;
int page_number;
int frame_number;
int value;
int memory_index = 0;

int page_and_tlb_path = 0;

FILE* backing_store_file_descriptor;
char* backing_store_data;

int page_fault_counter = 0;

typedef struct {
    int frame;
    int bit;
    int time;
} pageTableStruct;

pageTableStruct page_table[PAGE_SIZE];


//function MODELS DECLARATIONS
int choice_path(char **argv);
int exists_file_path(char *path);
void print_err(char *err);
void read_and_process_address(char *file_name);
void process_page_and_tlb(int choice_path);
void read_backing_store();
void initialize_page_table();
int get_page_number(int virtual_address);
int get_offset(int virtual_address);
int get_frame_number(int page_number);
void prepare_to_write_correct();

int main(int argc, char **argv) {
    char *address_path = argv[1];
    // printf("Values %d", exists_file_path(address_path));
    printf("Dev area operating...\n");

    // int test1 = 10;
    // int test2 = 255;
    // int test3 = test1 & test2;

    // printf("%d", test3);

    if (exists_file_path(address_path) == 0){
        char *ERR = DEFAULT_ERR_MSG"Não foi passado um parametro com o caminho na inicialização do programa";
        print_err(ERR);
        return 1;
    }

    page_and_tlb_path = choice_path(argv);

    printf("%d\n", page_and_tlb_path);
    if (page_and_tlb_path == -1){
        char *ERR = DEFAULT_ERR_MSG"Impossível inicializar o programa com apenas um parametro de algoritmo";
        print_err(ERR);
        return 1;
    }

    if(page_and_tlb_path == -2){
        char *ERR = DEFAULT_ERR_MSG"Impossível inicializar o programa sem parametros de algoritmo";
        print_err(ERR);
        return 1;
    }

    prepare_to_write_correct();

    initialize_page_table();

    read_and_process_address(address_path);
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

void read_and_process_address(char *file_name){
    arq_address = fopen(file_name, "r");
    char line[7];

    if (arq_address == NULL){
        char *ERR = DEFAULT_ERR_MSG"Ao abrir o arquivo. Está vazio ou não existe";
        print_err(ERR);
        exit(1);
    }

    while (!feof(arq_address)){
        fgets(line, sizeof(line), arq_address);  // o 'fgets' lê até 7 caracteres ou até o '\n'
        virtual_address = atoi(line);

        page_number = get_page_number(virtual_address);

        // fprintf(arq_correct_output, "PAGE NUMBER: %d\n", page_number);

        offset = get_offset(virtual_address);

        // fprintf(arq_correct_output, "OFFSET: %d\n", offset);

        // Implementar aqui a chamada da TLB

        // Caso não seja encontrado na TLB, busca na page table

        // Pegando número de frame na page table
        int frame_number_value = get_frame_number(page_number);

        if (frame_number_value == -1){
            //fazendo leitura do backing store e escrevendo valor referente ao page_number ao
            //meu array de char physical_memory
            read_backing_store(page_number);

            if(memory_index == 127){
                memory_index = 0;
            }

            frame_number = memory_index;
            physical_address = (frame_number * 256) + offset;
            memory_index++;
            // Implementar aqui a chamada da atualização da TLB

            page_table[page_number].frame = frame_number;
            page_table[page_number].bit = 1;

            value = physical_memory_array[frame_number].physical_memory[offset];

        }else{
            physical_address = (frame_number_value * 256) + offset;
            printf("%d\n", frame_number_value);
            value = physical_memory_array[frame_number_value].physical_memory[offset];
        }

        fprintf(arq_correct_output, "Virtual Address: %d Physical Address: %d Value: %d\n", virtual_address, physical_address, value);
    }

    fclose(arq_address);
    fclose(arq_correct_output);
}

void read_backing_store(int page_number){
    //Abrindo arquivo
    backing_store_file_descriptor = fopen(BACKING_STORE_FILE_NAME, "rb");

    if (backing_store_file_descriptor == NULL){
        char *ERR = DEFAULT_ERR_MSG"Ao tentar ler o backing store";
        print_err(ERR);
        fclose(backing_store_file_descriptor);
        exit(1);
    }

    //Movendo ponteiro
    fseek(backing_store_file_descriptor, PAGE_SIZE * page_number, SEEK_SET);
    //Lendo dados
    fread(physical_memory_array[memory_index].physical_memory, PAGE_SIZE, 1, backing_store_file_descriptor);
    //Fechando arquivo
    fclose(backing_store_file_descriptor);
    // printf("Page number: %d\n", page_number);
}

int get_page_number(int virtual_address){
    // Deslocando para direita em 8 bits.
    return (virtual_address >> 8);
}

int get_offset(int virtual_address){
    // Fazendo a concatenação da representação de 8 digitos binários com a memória virtual
    return (virtual_address & 255);
}

int get_frame_number(int page_number){
    if (page_table[page_number].frame == -1) {
        page_fault_counter++;
        return -1;
    }

    return page_table[page_number].frame;
}

void prepare_to_write_correct(){
    //Resetando o arquivo a primeira vez que o programa é executado
    arq_correct_output = fopen(CORRECT_FILE_NAME, "w");
    // fprintf(arq_correct_output, "");
    fclose(arq_correct_output);

    //Abrindo arquivo para append no arquivo
    arq_correct_output = fopen(CORRECT_FILE_NAME, "a");
}

void initialize_page_table(){
    for (int i = 0; i < PAGE_SIZE; i++) {
        page_table[i].frame = -1;
        page_table[i].bit = -1;
        page_table[i].time = -1;
    }
}