#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "livro.h"
#include "arquivo.h"
#include "util.h"

// Implementa��o das fun��es relacionadas aos livros

// Cadastra um livro no arquivo bin�rio
// Insere um novo livro na lista encadeada do arquivo bin�rio, reutilizando posi��es livres se houver
// pre-condicao: arquivo deve estar aberto para leitura e escrita
// pos-condicao: livro cadastrado no arquivo bin�rio
// Entrada: nome do arquivo bin�rio e estrutura Livro contendo as informa��es do livro
// Retorno: nenhum
void cadastrarLivro(const char *nome_arquivo, Livro livro) {
    if (buscarLivroPorCodigo(nome_arquivo, livro.codigo)) {
        printf("Erro: Livro com o c�digo %d j� existe.\n", livro.codigo);
        return;
    }

    FILE *arquivo = fopen(nome_arquivo, "rb+");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo bin�rio");
        exit(EXIT_FAILURE);
    }

    Cabecalho cabecalho;
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    int posicao_livre = cabecalho.posicao_primeiro_livre;
    int nova_posicao;

    if (posicao_livre == -1) {
        fseek(arquivo, 0, SEEK_END);
        nova_posicao = ftell(arquivo);
    } else {
        nova_posicao = posicao_livre;
        fseek(arquivo, posicao_livre, SEEK_SET);

        Livro livro_removido;
        fread(&livro_removido, sizeof(Livro), 1, arquivo);
        cabecalho.posicao_primeiro_livre = livro_removido.prox_registro;
    }

  if (cabecalho.posicao_inicio_lista == -1) {
    cabecalho.posicao_inicio_lista = nova_posicao;
} else {
    int posicao_atual = cabecalho.posicao_inicio_lista;
    Livro livro_atual;

    while (posicao_atual != -1) {
        fseek(arquivo, posicao_atual, SEEK_SET);
        fread(&livro_atual, sizeof(Livro), 1, arquivo);

        if (livro_atual.prox_registro == -1) {
            fseek(arquivo, posicao_atual + offsetof(Livro, prox_registro), SEEK_SET);
            fwrite(&nova_posicao, sizeof(int), 1, arquivo);
            break;
        }
        posicao_atual = livro_atual.prox_registro;
    }
}

livro.prox_registro = -1;  // Novo livro n�o aponta para nenhum outro (�ltimo da lista)

    fseek(arquivo, nova_posicao, SEEK_SET);
    fwrite(&livro, sizeof(Livro), 1, arquivo);

    fseek(arquivo, 0, SEEK_SET);
    fwrite(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    fclose(arquivo);
}
// Remove um livro do arquivo bin�rio
// Marca o livro como removido, adicionando sua posi��o � lista de registros livres
// pre-condicao: arquivo deve estar aberto para leitura e escrita
// pos-condicao: livro removido do arquivo bin�rio e posi��o adicionada � lista de registros livres
// Entrada: nome do arquivo bin�rio e c�digo do livro a ser removido
// Retorno: nenhum
void removerLivro(const char *nome_arquivo, int codigo) {
    FILE *arquivo = fopen(nome_arquivo, "rb+");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo bin�rio");
        exit(EXIT_FAILURE);
    }

    Cabecalho cabecalho;
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    int posicao_atual = cabecalho.posicao_inicio_lista;
    int posicao_anterior = -1;
    Livro livro;

    while (posicao_atual != -1) {
        fseek(arquivo, posicao_atual, SEEK_SET);
        fread(&livro, sizeof(Livro), 1, arquivo);

        if (livro.codigo == codigo) {
            if (posicao_anterior == -1) {
                cabecalho.posicao_inicio_lista = livro.prox_registro;
            } else {
                fseek(arquivo, posicao_anterior + offsetof(Livro, prox_registro), SEEK_SET);
                fwrite(&livro.prox_registro, sizeof(int), 1, arquivo);
            }

            livro.prox_registro = cabecalho.posicao_primeiro_livre;
            fseek(arquivo, posicao_atual, SEEK_SET);
            fwrite(&livro, sizeof(Livro), 1, arquivo);

            cabecalho.posicao_primeiro_livre = posicao_atual;

            fseek(arquivo, 0, SEEK_SET);
            fwrite(&cabecalho, sizeof(Cabecalho), 1, arquivo);

            fclose(arquivo);
            return;
        }

        posicao_anterior = posicao_atual;
        posicao_atual = livro.prox_registro;
    }

    printf("Livro com c�digo %d n�o encontrado.\n", codigo);
    fclose(arquivo);
}

// Imprime os dados de um livro espec�fico
// Encontra e imprime todos os dados de um livro com base no c�digo fornecido
// pre-condicao: arquivo deve estar aberto para leitura
// pos-condicao: dados do livro impressos na tela
// Entrada: nome do arquivo bin�rio e c�digo do livro a ser impresso
// Retorno: nenhum
void imprimirLivro(const char *nome_arquivo, int codigo) {
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo bin�rio");
        exit(EXIT_FAILURE);
    }

    Cabecalho cabecalho;
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    int posicao_atual = cabecalho.posicao_inicio_lista;
    Livro livro;

    while (posicao_atual != -1) {
        fseek(arquivo, posicao_atual, SEEK_SET);
        fread(&livro, sizeof(Livro), 1, arquivo);

        if (livro.codigo == codigo) {
            printf("C�digo: %d\n", livro.codigo);
            printf("T�tulo: %s\n", livro.titulo);
            printf("Autor: %s\n", livro.autor);
            printf("Editora: %s\n", livro.editora);
            printf("Edi��o: %d\n", livro.edicao);
            printf("Ano de Publica��o: %d\n", livro.ano_publicacao);
            printf("Pre�o: %.2f\n", livro.preco);
            printf("Quantidade em Estoque: %d\n", livro.quantidade_estoque);
            fclose(arquivo);
            return;
        }

        posicao_atual = livro.prox_registro;
    }

    printf("Livro com c�digo %d n�o encontrado.\n", codigo);
    fclose(arquivo);
}

// Lista todos os livros cadastrados no sistema
// Percorre a lista encadeada de livros no arquivo e imprime os dados principais de cada um
// pre-condicao: arquivo deve estar aberto para leitura
// pos-condicao: lista de livros impressa na tela
// Entrada: nome do arquivo bin�rio
// Retorno: nenhum
void listarTodosLivros(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo bin�rio");
        exit(EXIT_FAILURE);
    }

    Cabecalho cabecalho;
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    int posicao_atual = cabecalho.posicao_inicio_lista;
    Livro livro;

    while (posicao_atual != -1) {
        fseek(arquivo, posicao_atual, SEEK_SET);
        fread(&livro, sizeof(Livro), 1, arquivo);

        printf("C�digo: %d\n", livro.codigo);
        printf("T�tulo: %s\n", livro.titulo);
        printf("Autor: %s\n", livro.autor);
        printf("Quantidade em Estoque: %d\n", livro.quantidade_estoque);
        printf("prox_registro: %d\n\n", livro.prox_registro);  // Debug: Imprime o prox_registro

        posicao_atual = livro.prox_registro;
    }

    fclose(arquivo);
}




// Busca livros por autor
// Percorre a lista de livros no arquivo e imprime os t�tulos de todos os livros de um autor espec�fico
// pre-condicao: arquivo deve estar aberto para leitura
// pos-condicao: t�tulos dos livros do autor impressos na tela
// Entrada: nome do arquivo bin�rio e nome do autor a ser buscado
// Retorno: nenhum
void buscarPorAutor(const char *nome_arquivo, const char *autor) {
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo bin�rio");
        exit(EXIT_FAILURE);
    }

    Cabecalho cabecalho;
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    int posicao_atual = cabecalho.posicao_inicio_lista;
    Livro livro;
    int encontrado = 0;

    while (posicao_atual != -1) {
        fseek(arquivo, posicao_atual, SEEK_SET);
        fread(&livro, sizeof(Livro), 1, arquivo);

        if (strcmp(livro.autor, autor) == 0) {
            printf("T�tulo: %s\n", livro.titulo);
            encontrado = 1;
        }

        posicao_atual = livro.prox_registro;
    }

    if (!encontrado) {
        printf("Nenhum livro encontrado para o autor %s.\n", autor);
    }

    fclose(arquivo);
}

// Busca livros por t�tulo
// Encontra e imprime os dados completos de um livro com base no t�tulo fornecido
// pre-condicao: arquivo deve estar aberto para leitura
// pos-condicao: dados do livro com o t�tulo fornecido impressos na tela
// Entrada: nome do arquivo bin�rio e t�tulo do livro a ser buscado
// Retorno: nenhum
void buscarPorTitulo(const char *nome_arquivo, const char *titulo) {
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo bin�rio");
        exit(EXIT_FAILURE);
    }

    Cabecalho cabecalho;
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    int posicao_atual = cabecalho.posicao_inicio_lista;
    Livro livro;

    while (posicao_atual != -1) {
        fseek(arquivo, posicao_atual, SEEK_SET);
        fread(&livro, sizeof(Livro), 1, arquivo);

        if (strcmp(livro.titulo, titulo) == 0) {
            printf("C�digo: %d\n", livro.codigo);
            printf("T�tulo: %s\n", livro.titulo);
            printf("Autor: %s\n", livro.autor);
            printf("Editora: %s\n", livro.editora);
            printf("Edi��o: %d\n", livro.edicao);
            printf("Ano de Publica��o: %d\n", livro.ano_publicacao);
            printf("Pre�o: %.2f\n", livro.preco);
            printf("Quantidade em Estoque: %d\n", livro.quantidade_estoque);
            fclose(arquivo);
            return;
        }

        posicao_atual = livro.prox_registro;
    }

    printf("Livro com t�tulo '%s' n�o encontrado.\n", titulo);
    fclose(arquivo);
}
// Obt�m o t�tulo de um livro pelo c�digo
// Busca um livro pelo c�digo e retorna seu t�tulo
// pre-condicao: arquivo deve estar aberto para leitura
// pos-condicao: t�tulo do livro retornado ou NULL se n��o encontrado
// Entrada: nome do arquivo bin�rio e c�digo do livro
// Retorno: ponteiro para o t�tulo do livro, ou NULL se n��o encontrado
char* getTituloLivroPorCodigo(const char *nome_arquivo, int codigo) {
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) {
        return NULL; // Or handle error appropriately
    }

    Cabecalho cabecalho;
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    int posicao_atual = cabecalho.posicao_inicio_lista;
    Livro livro;

    while (posicao_atual != -1) {
        fseek(arquivo, posicao_atual, SEEK_SET);
        fread(&livro, sizeof(Livro), 1, arquivo);

        if (livro.codigo == codigo) {
            fclose(arquivo);
            char* titulo = malloc(sizeof(char) * MAX_TITULO);
            if (titulo) strcpy(titulo, livro.titulo);
            return titulo;
        }
        posicao_atual = livro.prox_registro;
    }

    fclose(arquivo);
    return NULL;
}