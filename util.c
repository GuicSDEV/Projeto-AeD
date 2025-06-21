#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "arquivo.h"
#include <time.h>
#include <string.h>

// Implementa��o das fun��es auxiliares

// Calcula o total de livros cadastrados
// Conta e retorna o n�mero total de livros cadastrados no arquivo bin�rio
// pre-condicao: arquivo deve estar aberto para leitura
// pos-condicao: total de livros cadastrado retornado
// Entrada: nome do arquivo bin�rio
// Retorno: n�mero total de livros cadastrados no sistema
int calcularTotalLivros(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo bin�rio");
        exit(EXIT_FAILURE);
    }

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
// Fun����o para buscar um livro por c�digo
// Busca um livro pelo c�digo no arquivo bin�rio  
// pre-condicao: arquivo deve estar aberto para leitura
// pos-condicao: retorna 1 se o livro for encontrado, 0 caso contr��rio
// Entrada: nome do arquivo bin�rio e c�digo do livro a ser buscado
// Retorno: 1 se o livro for encontrado, 0 caso contr��rio

int buscarLivroPorCodigo(const char *nome_arquivo, int codigo) {
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
            fclose(arquivo);
            return 1;
        }
        posicao_atual = livro.prox_registro;
    }

    fclose(arquivo);
    return 0;
}
// Fun����o para emprestar um livro
// Registra o empréstimo de um livro, atualizando o estoque e registrando o empréstimo em um arquivo auxiliar
// pre-condicao: arquivo deve estar aberto para leitura e escrita
// pos-condicao: livro emprestado, estoque atualizado e registro de empréstimo criado
// Entrada: nome do arquivo bin�rio, c�digo do livro e nome do usu��rio
// Retorno: nenhum


void emprestarLivro(const char *nome_arquivo, int codigo_livro, const char *nome_usuario) {
    if (!buscarLivroPorCodigo(nome_arquivo, codigo_livro)) {
        printf("Livro com código %d não encontrado.\n", codigo_livro);
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

    while (posicao_atual != -1) {
        fseek(arquivo, posicao_atual, SEEK_SET);
        fread(&livro, sizeof(Livro), 1, arquivo);

        if (livro.codigo == codigo_livro) {
            if (livro.quantidade_estoque <= 0) {
                printf("Não há exemplares disponíveis.\n");
                fclose(arquivo);
                return;
            }
            livro.quantidade_estoque--;

            fseek(arquivo, posicao_atual, SEEK_SET);
            fwrite(&livro, sizeof(Livro), 1, arquivo);
            fclose(arquivo);

            // Registro do empréstimo em arquivo auxiliar
            FILE *emprestimos = fopen("emprestimos.bin", "ab");
            if (!emprestimos) {
                perror("Erro ao abrir arquivo de empréstimos");
                return;
            }

            Emprestimo e;
            e.codigo_livro = codigo_livro;
            strncpy(e.nome_usuario, nome_usuario, MAX_NOME_USUARIO);
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            snprintf(e.data_emprestimo, sizeof(e.data_emprestimo), "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
            e.devolvido = 0;

            fwrite(&e, sizeof(Emprestimo), 1, emprestimos);
            fclose(emprestimos);

            printf("Empréstimo registrado com sucesso.\n");
            return;
        }
        posicao_atual = livro.prox_registro;
    }

    fclose(arquivo);
}
// Fun����o para devolver um livro
// Registra a devolução de um livro, atualizando o estoque e o registro de empréstimo
// pre-condicao: arquivo deve estar aberto para leitura e escrita
// pos-condicao: livro devolvido, estoque atualizado e registro de empréstimo atualizado
// Entrada: nome do arquivo bin�rio, c�digo do livro e nome do usu��rio
// Retorno: nenhum
void devolverLivro(const char *nome_arquivo, int codigo_livro, const char *nome_usuario) {
    FILE *emprestimos = fopen("emprestimos.bin", "rb+");
    if (!emprestimos) {
        perror("Erro ao abrir arquivo de empréstimos");
        return;
    }

    Emprestimo e;
    while (fread(&e, sizeof(Emprestimo), 1, emprestimos) == 1) {
        if (e.codigo_livro == codigo_livro && strcmp(e.nome_usuario, nome_usuario) == 0 && e.devolvido == 0) {
            e.devolvido = 1;
            fseek(emprestimos, -sizeof(Emprestimo), SEEK_CUR);
            fwrite(&e, sizeof(Emprestimo), 1, emprestimos);
            fclose(emprestimos);

            FILE *arquivo = fopen(nome_arquivo, "rb+");
            if (!arquivo) {
                perror("Erro ao abrir o arquivo binário");
                return;
            }

            Cabecalho cabecalho;
            fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

            int posicao_atual = cabecalho.posicao_inicio_lista;
            Livro livro;

            while (posicao_atual != -1) {
                fseek(arquivo, posicao_atual, SEEK_SET);
                fread(&livro, sizeof(Livro), 1, arquivo);

                if (livro.codigo == codigo_livro) {
                    livro.quantidade_estoque++;
                    fseek(arquivo, posicao_atual, SEEK_SET);
                    fwrite(&livro, sizeof(Livro), 1, arquivo);
                    fclose(arquivo);

                    printf("Livro devolvido com sucesso.\n");
                    return;
                }
                posicao_atual = livro.prox_registro;
            }
            fclose(arquivo);
            return;
        }
    }

    fclose(emprestimos);
    printf("Empréstimo não encontrado ou já devolvido.\n");
}
// Função para listar empréstimos pendentes
// Lista todos os empréstimos pendentes, mostrando o nome do usuário, código do livro, título e data do empréstimo
// pre-condicao: arquivo de empréstimos deve estar aberto para leitura
// pos-condicao: lista de empréstimos pendentes impressa na tela
// Entrada: nome do arquivo binário de livros (para obter títulos)
// Retorno: nenhum
void listarEmprestimosPendentes(const char *nome_arquivo) { // nome_arquivo is livraria.bin
    FILE *emprestimos = fopen("emprestimos.bin", "rb");
    if (!emprestimos) {
        printf("Nenhum empréstimo registrado ainda.\n");
        return;
    }

    printf("Empréstimos Pendentes:\n");
    Emprestimo e;
    int found_pending = 0;
    while (fread(&e, sizeof(Emprestimo), 1, emprestimos) == 1) {
        if (e.devolvido == 0) {
            found_pending = 1;
            char* titulo_livro = getTituloLivroPorCodigo(nome_arquivo, e.codigo_livro);
            // Assuming nome_usuario is already available in the Emprestimo struct
            printf("Usuário: %s | Código Livro: %d | Título: %s | Data Empréstimo: %s\n",
                   e.nome_usuario, e.codigo_livro, (titulo_livro ? titulo_livro : "N/A"), e.data_emprestimo);
            free(titulo_livro); // Free allocated memory for title
        }
    }

    if (!found_pending) {
        printf("Não há empréstimos pendentes.\n");
    }

    fclose(emprestimos);
}
