

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "livro.h"
#include "arquivo.h"
#include "util.h"

// Cadastra um livro no arquivo binário
void cadastrarLivro(const char *nome_arquivo, Livro livro) {
    if (buscarLivroPorCodigo(nome_arquivo, livro.codigo)) {
        printf("Erro: Livro com o código %d já existe.\n", livro.codigo);
        return;
    }

    FILE *arquivo = fopen(nome_arquivo, "rb+");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo binário");
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

livro.prox_registro = -1;

    fseek(arquivo, nova_posicao, SEEK_SET);
    fwrite(&livro, sizeof(Livro), 1, arquivo);

    fseek(arquivo, 0, SEEK_SET);
    fwrite(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    fclose(arquivo);
}

// Remove um livro do arquivo binário
void removerLivro(const char *nome_arquivo, int codigo) {
    FILE *arquivo = fopen(nome_arquivo, "rb+");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo binário");
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

    printf("Livro com código %d não encontrado.\n", codigo);
    fclose(arquivo);
}

// Imprime os dados de um livro específico
void imprimirLivro(const char *nome_arquivo, int codigo) {
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo binário");
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
            printf("Código: %d\n", livro.codigo);
            printf("Título: %s\n", livro.titulo);
            printf("Autor: %s\n", livro.autor);
            printf("Editora: %s\n", livro.editora);
            printf("Edição: %d\n", livro.edicao);
            printf("Ano de Publicação: %d\n", livro.ano_publicacao);
            printf("Preço: %.2f\n", livro.preco);
            printf("Quantidade em Estoque: %d\n", livro.quantidade_estoque);
            fclose(arquivo);
            return;
        }

        posicao_atual = livro.prox_registro;
    }

    printf("Livro com código %d não encontrado.\n", codigo);
    fclose(arquivo);
}

// Busca livros por autor
void buscarPorAutor(const char *nome_arquivo, const char *autor) {
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo binário");
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
            printf("Título: %s\n", livro.titulo);
            encontrado = 1;
        }

        posicao_atual = livro.prox_registro;
    }

    if (!encontrado) {
        printf("Nenhum livro encontrado para o autor %s.\n", autor);
    }

    fclose(arquivo);
}

// Busca livros por título
void buscarPorTitulo(const char *nome_arquivo, const char *titulo) {
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo binário");
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
            printf("Código: %d\n", livro.codigo);
            printf("Título: %s\n", livro.titulo);
            printf("Autor: %s\n", livro.autor);
            printf("Editora: %s\n", livro.editora);
            printf("Edição: %d\n", livro.edicao);
            printf("Ano de Publicação: %d\n", livro.ano_publicacao);
            printf("Preço: %.2f\n", livro.preco);
            printf("Quantidade em Estoque: %d\n", livro.quantidade_estoque);
            fclose(arquivo);
            return;
        }

        posicao_atual = livro.prox_registro;
    }

    printf("Livro com título '%s' não encontrado.\n", titulo);
    fclose(arquivo);
}

// Obtém o título de um livro pelo código
char* getTituloLivroPorCodigo(const char *nome_arquivo, int codigo) {
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) {
        return NULL;
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

// Lista todos os livros cadastrados no sistema (VERSÃO CORRETA E ÚNICA)
void listarTodosLivros(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo binário");
        return;
    }

    Cabecalho cabecalho;
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    int posicao_atual = cabecalho.posicao_inicio_lista;
    Livro livro;
    int encontrou_livros = 0;

    printf("\n--- Lista de Todos os Livros ---\n");
    while (posicao_atual != -1) {
        fseek(arquivo, posicao_atual, SEEK_SET);
        fread(&livro, sizeof(Livro), 1, arquivo);
        
        // Imprime apenas os campos especificados no PDF
        printf("Código: %d | Título: %s | Autor: %s | Exemplares: %d\n",
               livro.codigo,
               livro.titulo,
               livro.autor,
               livro.quantidade_estoque);

        posicao_atual = livro.prox_registro;
        encontrou_livros = 1;
    }

    if (!encontrou_livros) {
        printf("Nenhum livro cadastrado.\n");
    }
    printf("--------------------------------\n");

    fclose(arquivo);
}