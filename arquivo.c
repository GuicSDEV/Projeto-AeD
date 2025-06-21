#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"
#include "util.h"
#include <ctype.h>

// Implementa��o das fun��es relacionadas ao arquivo bin�rio

// Inicializa o arquivo bin�rio
// Escreve o cabe�alho inicial no arquivo caso ele n�o exista
// pre-condicao: nome_arquivo deve ser um nome de arquivo v�lido
// pos-condicao: arquivo bin�rio criado e cabe�alho inicializado, ou arquivo existente aberto para leitura/escrita
// Entrada: nome do arquivo bin�rio
// Retorno: nenhum
void inicializarArquivo(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "rb+");
    if (!arquivo) {
        printf("Arquivo bin�rio n�o encontrado. Criando um novo arquivo.\n");
        arquivo = fopen(nome_arquivo, "wb+");
        if (!arquivo) {
            perror("Erro ao criar o arquivo bin�rio");
            exit(EXIT_FAILURE);
        }

        Cabecalho cabecalho = { -1, -1, -1 };
        fwrite(&cabecalho, sizeof(Cabecalho), 1, arquivo);
    } else {
        printf("Arquivo bin�rio existente encontrado.\n");
    }
    fclose(arquivo);
}


