#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "arquivo.h"
#include <time.h>
#include <string.h>
#include "livro.h"
#include "usuario.h"
#include "carregamento.h"
#include <ctype.h>

// Implementacao das funcoes auxiliares

int calcularTotalLivros(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) return 0;

    Cabecalho cabecalho;
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    int posicao_atual = cabecalho.posicao_inicio_lista;
    int total_livros = 0;
    Livro livro;

    while (posicao_atual != -1) {
        fseek(arquivo, posicao_atual, SEEK_SET);
        fread(&livro, sizeof(Livro), 1, arquivo);
        total_livros++;
        posicao_atual = livro.prox_registro;
    }

    fclose(arquivo);
    return total_livros;
}

int buscarLivroPorCodigo(const char *nome_arquivo, int codigo) {
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) return 0;

    Cabecalho cabecalho;
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    int posicao_atual = cabecalho.posicao_inicio_lista;
    Livro livro;

    while (posicao_atual != -1) {
        fseek(arquivo, posicao_atual, SEEK_SET);
        fread(&livro, sizeof(Livro), 1, arquivo);
        if (livro.codigo == codigo) {
            fclose(arquivo);
            return 1;
        }
        posicao_atual = livro.prox_registro;
    }

    fclose(arquivo);
    return 0;
}

void emprestarLivro(const char *nome_arquivo, int codigo_livro, int codigo_usuario) {
    if (!buscarUsuarioPorCodigo("usuarios.bin", codigo_usuario)) {
         printf("Erro: Usuário com código %d não encontrado.\n", codigo_usuario);
        return;
    }

    FILE *arquivo = fopen(nome_arquivo, "rb+");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo binário");
        return;
    }

    Cabecalho cabecalho;
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    int posicao_atual = cabecalho.posicao_inicio_lista;
    Livro livro;
    int livro_encontrado = 0;

    while (posicao_atual != -1) {
        fseek(arquivo, posicao_atual, SEEK_SET);
        fread(&livro, sizeof(Livro), 1, arquivo);

        if (livro.codigo == codigo_livro) {
            livro_encontrado = 1;
            if (livro.quantidade_estoque <= 0) {
                printf("Não há exemplares disponíveis.\n");
                fclose(arquivo);
                return;
            }
            livro.quantidade_estoque--;

            fseek(arquivo, posicao_atual, SEEK_SET);
            fwrite(&livro, sizeof(Livro), 1, arquivo);

            // Registro do empréstimo em arquivo auxiliar
            FILE *emprestimos = fopen("emprestimos.bin", "ab");
            if (!emprestimos) {
                perror("Erro ao abrir arquivo de empréstimos");
                fclose(arquivo);
                return;
            }

            Emprestimo e;
            e.codigo_livro = codigo_livro;
            e.codigo_usuario = codigo_usuario;
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            snprintf(e.data_emprestimo, sizeof(e.data_emprestimo), "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
            strcpy(e.data_devolucao, "");
            e.devolvido = 0;

            fwrite(&e, sizeof(Emprestimo), 1, emprestimos);
            fclose(emprestimos);

            printf("Empréstimo registrado com sucesso.\n");
            break;
        }
        posicao_atual = livro.prox_registro;
    }

    if (!livro_encontrado) {
        printf("Livro com código %d não encontrado.\n", codigo_livro);
    }

    fclose(arquivo);
}

void devolverLivro(const char *nome_arquivo, int codigo_livro, int codigo_usuario) {
    FILE *emprestimos = fopen("emprestimos.bin", "rb+");
    if (!emprestimos) {
        perror("Erro ao abrir arquivo de empréstimos");
        return;
    }

    Emprestimo e;
    int emprestimo_encontrado = 0;
    while (fread(&e, sizeof(Emprestimo), 1, emprestimos) == 1) {
        if (e.codigo_livro == codigo_livro && e.codigo_usuario == codigo_usuario && e.devolvido == 0) {
            e.devolvido = 1;
            
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            snprintf(e.data_devolucao, sizeof(e.data_devolucao), "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

            fseek(emprestimos, -sizeof(Emprestimo), SEEK_CUR);
            fwrite(&e, sizeof(Emprestimo), 1, emprestimos);
            emprestimo_encontrado = 1;
            break; 
        }
    }
    fclose(emprestimos);

    if (emprestimo_encontrado) {
        atualizarEstoqueLivro(nome_arquivo, codigo_livro, +1); // Reusa a função de `carregamento.c`
        printf("Livro devolvido com sucesso.\n");
    } else {
        printf("Empréstimo não encontrado ou já devolvido para este usuário e livro.\n");
    }
}


void listarEmprestimosPendentes(const char *nome_arquivo_livros, const char *nome_arquivo_usuarios) {
    FILE *emprestimos = fopen("emprestimos.bin", "rb");
    if (!emprestimos) {
        printf("Nenhum empréstimo registrado ainda.\n");
        return;
    }

    printf("\n--- Empréstimos Pendentes ---\n");
    Emprestimo e;
    int found_pending = 0;
    while (fread(&e, sizeof(Emprestimo), 1, emprestimos) == 1) {
        if (e.devolvido == 0) {
            found_pending = 1;
            char* nome_usuario = getNomeUsuarioPorCodigo(nome_arquivo_usuarios, e.codigo_usuario);
            char* titulo_livro = getTituloLivroPorCodigo(nome_arquivo_livros, e.codigo_livro);
            
            printf("Usuário: %d - %s | Livro: %d - %s | Data Empréstimo: %s\n",
                   e.codigo_usuario,
                   (nome_usuario ? nome_usuario : "N/A"),
                   e.codigo_livro,
                   (titulo_livro ? titulo_livro : "N/A"),
                   e.data_emprestimo);
            
            free(nome_usuario);
            free(titulo_livro);
        }
    }

    if (!found_pending) {
        printf("Não há empréstimos pendentes.\n");
    }
    printf("---------------------------\n");

    fclose(emprestimos);
}