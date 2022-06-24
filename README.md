# virtual-memory

### Este é um programa que simula uma memória virtual.

## Para utiliza-lo é necessário ter uma máquina UNIX (linux ou mac) e os seguintes arquivos:

### - Arquivo com os endereços virtuais: address.txt
### - Arquivo que simula a memória secundária (BACKING_STORE): BACKING STORE.bin


## Como executar:

### - Existem 3 parametros extremamente necessários para execução. O primeiro é o arquivo de endereços virtuais (nome.extenção), o segundo é o algoritmo de substituição da page table e o terceiro algoritmo de substituição da TLB, podendo variar entre lru e fifo.

### - Exemplo: ./vm address.txt fifo fifo

### - Utilizar o seguinte comando para limpar o arquivo executável gerado: make clean

### - Utilizar o seguinte comando para gerar um novo arquivo executável: make

### - A execução dele não funciona no make por conta dos parametros que devem ser passados, utilize o exemplo mostrado.

