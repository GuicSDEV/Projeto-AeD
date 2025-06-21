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


// Imprime a lista de registros livres
// Exibe as posi��es dos registros que foram removidos e est�o dispon�veis para reutiliza��o
// pre-condicao: arquivo deve estar aberto para leitura
// pos-condicao: posi��es livres impressas na tela
// Entrada: nome do arquivo bin�rio
// Retorno: nenhum
void imprimirListaLivres(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo bin�rio");
        exit(EXIT_FAILURE);
    }

    Cabecalho cabecalho;
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    int posicao_livre = cabecalho.posicao_primeiro_livre;
    Livro livro;

    printf("Lista de registros livres:\n");
    while (posicao_livre != -1) {
        fseek(arquivo, posicao_livre, SEEK_SET);
        fread(&livro, sizeof(Livro), 1, arquivo);
        printf("Posi��o Livre: %d\n", posicao_livre);
        posicao_livre = livro.prox_registro;
    }

    fclose(arquivo);
}
