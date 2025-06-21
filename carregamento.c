#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "arquivo.h"
#include "livro.h"
#include "util.h"
#include "usuario.h"
#include "carregamento.h"

// Função interna para atualizar o estoque de um livro
void atualizarEstoqueLivro(const char *nome_arquivo_livros, int codigo_livro, int delta) {
    FILE *arquivo = fopen(nome_arquivo_livros, "rb+");
    if (!arquivo) {
        perror("Erro ao abrir arquivo de livros para atualizar estoque");
        return;
    }

    Cabecalho cabecalho;
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);
    int pos_atual = cabecalho.posicao_inicio_lista;
    Livro livro;

    while (pos_atual != -1) {
        fseek(arquivo, pos_atual, SEEK_SET);
        fread(&livro, sizeof(Livro), 1, arquivo);
        if (livro.codigo == codigo_livro) {
            livro.quantidade_estoque += delta;
            fseek(arquivo, pos_atual, SEEK_SET);
            fwrite(&livro, sizeof(Livro), 1, arquivo);
            break;
        }
        pos_atual = livro.prox_registro;
    }
    fclose(arquivo);
}


// Função para processar uma linha de texto e cadastrar um usuário
void processarLinhaUsuario(const char *nome_arquivo_usuarios, char *linha) {
    Usuario usuario;
    char *token;

    token = strtok(linha, ";");
    if (!token) return;
    removerEspacos(token);
    usuario.codigo_usuario = atoi(token);

    token = strtok(NULL, ";");
    if (!token) return;
    removerEspacos(token);
    strncpy(usuario.nome_usuario, token, MAX_NOME_USUARIO -1);
    usuario.nome_usuario[MAX_NOME_USUARIO - 1] = '\0';


    usuario.prox_registro = -1;
    cadastrarUsuario(nome_arquivo_usuarios, usuario);
}

// Função para processar uma linha de texto e registrar um empréstimo ou devolução do arquivo de carga
void processarLinhaEmprestimo(const char *nome_arquivo_livros, char *linha) {
    Emprestimo emp;
    char *token;

    token = strtok(linha, ";");
    removerEspacos(token);
    emp.codigo_usuario = atoi(token);

    token = strtok(NULL, ";");
    removerEspacos(token);
    emp.codigo_livro = atoi(token);

    token = strtok(NULL, ";");
    removerEspacos(token);
    strncpy(emp.data_emprestimo, token, 10);
    emp.data_emprestimo[10] = '\0';

    token = strtok(NULL, ";");
    if (token && strlen(token) > 1) { // Verifica se a data de devolução existe
        removerEspacos(token);
        strncpy(emp.data_devolucao, token, 10);
        emp.data_devolucao[10] = '\0';
        emp.devolvido = 1;
    } else {
        strcpy(emp.data_devolucao, "");
        emp.devolvido = 0;
    }

    // Verifica se livro e usuário existem
    if (!buscarLivroPorCodigo(nome_arquivo_livros, emp.codigo_livro)) {
        printf("Erro de carga: Livro com código %d não encontrado para empréstimo.\n", emp.codigo_livro);
        return;
    }
    if (!buscarUsuarioPorCodigo("usuarios.bin", emp.codigo_usuario)) {
        printf("Erro de carga: Usuário com código %d não encontrado para empréstimo.\n", emp.codigo_usuario);
        return;
    }
    
    // Atualiza o estoque do livro
    // Se o livro não foi devolvido, diminui o estoque.
    // Se foi devolvido (no próprio arquivo de carga), o estoque não muda (saiu e voltou).
    if (emp.devolvido == 0) {
        atualizarEstoqueLivro(nome_arquivo_livros, emp.codigo_livro, -1);
    }

    // Registra o empréstimo no arquivo binário de empréstimos
    FILE *arquivo_emprestimos = fopen("emprestimos.bin", "ab");
    if (!arquivo_emprestimos) {
        perror("Erro ao abrir arquivo de empréstimos para carga");
        return;
    }
    fwrite(&emp, sizeof(Emprestimo), 1, arquivo_emprestimos);
    fclose(arquivo_emprestimos);

    printf("Registro de empréstimo do arquivo de carga processado para usuário %d e livro %d.\n", emp.codigo_usuario, emp.codigo_livro);
}


void carregarDadosCompletos(const char *nome_arquivo_livros, const char *nome_arquivo_usuarios, const char *arquivo_txt) {
    FILE *arquivo_texto = fopen(arquivo_txt, "r");
    if (!arquivo_texto) {
        perror("Erro ao abrir o arquivo de texto para carregamento completo");
        exit(EXIT_FAILURE);
    }

    char linha[512];
    char linha_original[512]; // Buffer para preservar a linha original para strtok
    
    while (fgets(linha, sizeof(linha), arquivo_texto)) {
        linha[strcspn(linha, "\n")] = '\0'; 
        strcpy(linha_original, linha);

        char *conteudo_linha = linha_original;
        char tipo_registro = linha[0]; 
        
        if (strlen(linha) > 2) {
            conteudo_linha = &linha_original[2];
        } else {
            continue;
        }

        switch (tipo_registro) {
            case 'L': { // Livro
                Livro livro;
                char *token;

                token = strtok(conteudo_linha, ";"); removerEspacos(token); livro.codigo = atoi(token);
                token = strtok(NULL, ";"); removerEspacos(token); strncpy(livro.titulo, token, MAX_TITULO - 1);
                token = strtok(NULL, ";"); removerEspacos(token); strncpy(livro.autor, token, MAX_AUTOR - 1);
                token = strtok(NULL, ";"); removerEspacos(token); strncpy(livro.editora, token, MAX_EDITORA - 1);
                token = strtok(NULL, ";"); removerEspacos(token); livro.edicao = atoi(token);
                token = strtok(NULL, ";"); removerEspacos(token); livro.ano_publicacao = atoi(token);
                token = strtok(NULL, ";"); removerEspacos(token); substituirVirgulaPorPonto(token); livro.preco = atof(token);
                token = strtok(NULL, ";"); removerEspacos(token); livro.quantidade_estoque = atoi(token);

                livro.prox_registro = -1;
                cadastrarLivro(nome_arquivo_livros, livro);
                break;
            }
            case 'U': { // Usuário
                processarLinhaUsuario(nome_arquivo_usuarios, conteudo_linha);
                break;
            }
            case 'E': { // Empréstimo
                processarLinhaEmprestimo(nome_arquivo_livros, conteudo_linha);
                break;
            }
            default:
                printf("Tipo de registro desconhecido: %c na linha: %s\n", tipo_registro, linha);
                break;
        }
    }
    fclose(arquivo_texto);
    printf("Dados carregados do arquivo de texto com sucesso.\n");
}

void removerEspacos(char *str) {
    if (str == NULL) return;
    char *i = str;
    char *j = str;
    while (*j != 0) {
        *i = *j++;
        if (!isspace((unsigned char)*i)) {
            i++;
        }
    }
    *i = 0;

    // Remove trailing spaces
    i = str + strlen(str) - 1;
    while(i >= str && isspace((unsigned char)*i)) {
        i--;
    }
    *(i+1) = '\0';
}

void substituirVirgulaPorPonto(char *str) {
    if (str == NULL) return;
    while (*str) {
        if (*str == ',') {
            *str = '.';
        }
        str++;
    }
}