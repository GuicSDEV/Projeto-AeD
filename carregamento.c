#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include "arquivo.h"
#include "livro.h"
#include "util.h"
#include "usuario.h"
#include "carregamento.h"

// Função para processar uma linha de texto e cadastrar um usuário
void processarLinhaUsuario(const char *nome_arquivo_usuarios, char *linha) {
    Usuario usuario;
    char *token;

    // Primeiro token: código do usuário
    token = strtok(linha, ";");
    removerEspacos(token);
    usuario.codigo_usuario = atoi(token);

    // Segundo token: nome do usuário
    token = strtok(NULL, ";");
    removerEspacos(token);
    strcpy(usuario.nome_usuario, token);

    usuario.prox_registro = -1; // Próximo registro
    // Cadastrar o usuário no arquivo binário
    if (buscarUsuarioPorCodigo(nome_arquivo_usuarios, usuario.codigo_usuario)) {
        printf("Erro: Usuário com o código %d já existe.\n", usuario.codigo_usuario);
        return;
    }
    // Se o usuário já existe, não cadastra novamente
    FILE *arquivo = fopen(nome_arquivo_usuarios, "rb+");
    if (!arquivo) {
        // Se o arquivo não existe, cria um novo
        printf("Arquivo binário de usuários não encontrado. Criando um novo arquivo.\n");
        arquivo = fopen(nome_arquivo_usuarios, "wb+");
        if (!arquivo) {
            perror("Erro ao criar o arquivo binário de usuários");
            exit(EXIT_FAILURE);
        }
        Cabecalho cabecalho = { .posicao_inicio_lista = -1, .posicao_primeiro_livre = -1 };
        fwrite(&cabecalho, sizeof(Cabecalho), 1, arquivo);
    }
    fclose(arquivo);
    
    cadastrarUsuario(nome_arquivo_usuarios, usuario);
}

// Função para processar uma linha de texto e registrar um empréstimo ou devolução
// Formato da linha: "CódUsu;CódLivro;DataEmprestimo;DataDevolucao"
// Onde DataDevolucao é opcional
// Pré-condição: o arquivo de livros deve existir e estar no formato correto
// Pós-condição: o empréstimo ou devolução é registrado no arquivo de empréstimos
// Entrada: nome do arquivo de livros e a linha de texto a ser processada
// Retorno: nenhum
void processarLinhaEmprestimo(const char *nome_arquivo_livros, char *linha) {
    int codigo_usuario;
    int codigo_livro;
    char data_emprestimo[11];
    char data_devolucao[11];

    char *token;

    token = strtok(linha, ";");
    removerEspacos(token);
    codigo_usuario = atoi(token);

    token = strtok(NULL, ";");
    removerEspacos(token);
    codigo_livro = atoi(token);

    token = strtok(NULL, ";");
    removerEspacos(token);
    strcpy(data_emprestimo, token);

    token = strtok(NULL, ";");
    if (token) {
        removerEspacos(token);
        strcpy(data_devolucao, token);
    } else {
        strcpy(data_devolucao, "");
    }

    // Primeiro, verificar se o livro existe e se há estoque suficiente
    if (!buscarLivroPorCodigo(nome_arquivo_livros, codigo_livro)) {
        printf("Erro: Livro com código %d não encontrado.\n", codigo_livro);
        return;
    }

    // Tenta obter o nome do usuário para usar nas funções de empréstimo/devolução
    char *userName = getNomeUsuarioPorCodigo("usuarios.bin", codigo_usuario);
    if (userName == NULL) {
        printf("Erro: Usuário com código %d não encontrado para registrar empréstimo/devolução.\n", codigo_usuario);
        return;
    }

    if (strlen(data_devolucao) > 0) {
        // Registro de devolução
        devolverLivro(nome_arquivo_livros, codigo_livro, userName); 
        printf("Devolução registrada com sucesso para o usuário %s do livro %d.\n", userName, codigo_livro);
    } else {
        // Registro de empréstimo
        // A verificação de estoque já deve acontecer dentro de emprestarLivro
        emprestarLivro(nome_arquivo_livros, codigo_livro, userName); 
        printf("Empréstimo registrado com sucesso para o usuário %s do livro %d.\n", userName, codigo_livro);
    }

    free(userName); // Libera a memória alocada por getNomeUsuarioPorCodigo
}
// Função para carregar dados completos de livros e usuários a partir de um arquivo de texto
// Pré-condição: o arquivo de texto deve existir e estar no formato correto
// Pós-condição: os dados de livros e usuários são carregados, e empréstimos pendentes são registrados
// Entrada: nomes dos arquivos binários de livros e usuários, e nome do arquivo de texto para empréstimos
// Retorno: nenhum
void carregarDadosCompletos(const char *nome_arquivo_livros, const char *nome_arquivo_usuarios, const char *arquivo_txt) {
    FILE *arquivo_texto = fopen(arquivo_txt, "r");
    if (!arquivo_texto) {
        perror("Erro ao abrir o arquivo de texto para carregamento completo");
        exit(EXIT_FAILURE);
    }

    char linha[512];
    while (fgets(linha, sizeof(linha), arquivo_texto)) {
        linha[strcspn(linha, "\n")] = '\0'; // Remove o caractere de nova linha

        char tipo_registro = linha[0]; // Primeiro caractere indica o tipo
        char *conteudo_linha = &linha[2]; // Pula o tipo e o ponto e vírgula (ex: "L;")

        removerEspacos(conteudo_linha); // Remove espaços em branco do início/fim

        switch (tipo_registro) {
            case 'L': { // Livro
                Livro livro;
                char *token;

                token = strtok(conteudo_linha, ";");
                removerEspacos(token);
                livro.codigo = atoi(token);

                token = strtok(NULL, ";");
                removerEspacos(token);
                strcpy(livro.titulo, token);

                token = strtok(NULL, ";");
                removerEspacos(token);
                strcpy(livro.autor, token);

                token = strtok(NULL, ";");
                removerEspacos(token);
                strcpy(livro.editora, token);

                token = strtok(NULL, ";");
                removerEspacos(token);
                livro.edicao = atoi(token);

                token = strtok(NULL, ";");
                removerEspacos(token);
                livro.ano_publicacao = atoi(token);

                token = strtok(NULL, ";");
                removerEspacos(token);
                substituirVirgulaPorPonto(token);
                livro.preco = atof(token);

                token = strtok(NULL, ";");
                removerEspacos(token);
                livro.quantidade_estoque = atoi(token);

                livro.prox_registro = -1;
                cadastrarLivro(nome_arquivo_livros, livro);
                break;
            }
            case 'U': { // Usuário
                processarLinhaUsuario(nome_arquivo_usuarios, conteudo_linha);
                break;
            }
            case 'E': { // Empréstimo
                processarLinhaEmprestimo(nome_arquivo_livros, conteudo_linha); // Passa o arquivo de livros para atualizar o estoque
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

// Função para remover espaços de uma string
void removerEspacos(char *str) {
    char *dest = str;
    while (*str) {
        if (!isspace((unsigned char)*str)) {
            *dest++ = *str;
        }
        str++;
    }
    *dest = '\0'; // Finaliza a string resultante
}

// Função para substituir vírgulas por pontos em uma string
void substituirVirgulaPorPonto(char *str) {
    while (*str) {
        if (*str == ',') {
            *str = '.';
        }
        str++;
    }
}
