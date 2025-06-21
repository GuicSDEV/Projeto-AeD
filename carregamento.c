#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include "arquivo.h"
#include "livro.h"
#include "util.h"
#include "usuario.h"
#include "carregamento.h"
/// funcao para processar uma linha de texto e cadastrar um usuário

void processarLinhaUsuario(const char *nome_arquivo_usuarios, char *linha) {
    Usuario usuario;
    char *token;

    // primeiro token: código do usuário
    token = strtok(linha, ";");
    removerEspacos(token);
    usuario.codigo_usuario = atoi(token);

    // segundo token: nome do usuário
    token = strtok(NULL, ";");
    removerEspacos(token);
    strcpy(usuario.nome_usuario, token);

    usuario.prox_registro = -1; // no próximo registro
    // cadastrar o usuário no arquivo binário
    if (buscarUsuarioPorCodigo(nome_arquivo_usuarios, usuario.codigo_usuario)) {
        printf("Erro: Usuário com o código %d já existe.\n", usuario.codigo_usuario);
        return;
    }
    // se o usuário já existe, não o cadastra
    FILE *arquivo = fopen(nome_arquivo_usuarios, "rb+");
    if (!arquivo) {
        // se o arquivo não existe, cria um novo
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
// assumindo que o formato da linha é:
// "CódUsu;CódLivro;DataEmprestimo;DataDevolucao"
// onde DataDevolucao é opcional
// pre-condição: o arquivo de livros deve existir e estar no formato correto
// pos-condição: o empréstimo ou devolução é registrado no arquivo de empréstimos
// Entrada: nome do arquivo de livros, e a linha de texto a ser processada
// Retorno: nenhum
void processarLinhaEmprestimo(const char *nome_arquivo_livros, char *linha) {
    int codigo_usuario;
    int codigo_livro;
    char data_emprestimo[11]; // "dd/mm/aaaa"
    char data_devolucao[11]; // "dd/mm/aaaa" (optional)

    char *token;

    // primeiro token: código do usuário
    token = strtok(linha, ";");
    removerEspacos(token);
    codigo_usuario = atoi(token);

    // Segundo token: código do livro
    token = strtok(NULL, ";");
    removerEspacos(token);
    codigo_livro = atoi(token);

    // terceiro token: data de empréstimo
    token = strtok(NULL, ";");
    removerEspacos(token);
    strcpy(data_emprestimo, token);

    // quarto token: data de devolução 
    token = strtok(NULL, ";");
    if (token) {
        removerEspacos(token);
        strcpy(data_devolucao, token);
    } else {
        strcpy(data_devolucao, ""); // nao há devolução
    }

    // agora vamos registrar o empréstimo ou devolução
    // Primeiro, verificar se o livro existe e se há estoque suficiente
    if (!buscarLivroPorCodigo(nome_arquivo_livros, codigo_livro)) {
        printf("Erro: Livro com código %d não encontrado.\n", codigo_livro);
        return;
    }
    // Se o livro existe, vamos verificar se há estoque
    Livro livro;
    FILE *arquivo_livros = fopen(nome_arquivo_livros, "rb+");
    if (!arquivo_livros) {
        perror("Erro ao abrir o arquivo de livros");
        return;
    }
    Cabecalho cabecalho;
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo_livros);
    int posicao_atual = cabecalho.posicao_inicio_lista;
    while (posicao_atual != -1) {
        fseek(arquivo_livros, posicao_atual, SEEK_SET);
        fread(&livro, sizeof(Livro), 1, arquivo_livros);
        if (livro.codigo == codigo_livro) {
            if (livro.quantidade_estoque <= 0 && strlen(data_devolucao) == 0) {
                printf("Erro: Livro com código %d não está disponível para empréstimo.\n", codigo_livro);
                fclose(arquivo_livros);
                return;
            }
            break; // Livro encontrado
        }
        posicao_atual = livro.prox_registro;
    }
    fclose(arquivo_livros);
    // Agora, se o livro está disponível, vamos registrar o empréstimo ou devolução
    // Se há devolução, vamos registrar a devolução
    // Se não há devolução, vamos registrar o empréstimo


    if (strlen(data_devolucao) > 0) {
        // Registro de devolução
        // Aqui, vamos chamar a função devolverLivro, que deve atualizar o estoque do livro
        // e marcar o empréstimo como devolvido.
        devolverLivro(nome_arquivo_livros, codigo_livro, getNomeUsuarioPorCodigo("usuarios.bin", codigo_usuario));
        // assumindo que devolverLivro atualiza o estoque e marca o empréstimo como devolvido
        // Se devolverLivro foi bem-sucedido, não precisamos fazer mais nada aqui.
        printf("Devolução registrada com sucesso para o usuário %d do livro %d.\n", codigo_usuario, codigo_livro);
    } else {
        // Registro de empréstimo
        // Primeiro, vamos verificar se o usuário existe
        // Se o usuário não existe, não podemos registrar o empréstimo
        if (!buscarUsuarioPorCodigo("usuarios.bin", codigo_usuario)) {
            printf("Erro: Usuário com código %d não encontrado.\n", codigo_usuario);
            return;
        }
        // Se o usuário existe, vamos registrar o empréstimo
        // Vamos pegar o nome do usuário para registrar o empréstimo
        // Aqui, vamos chamar a função emprestarLivro, que deve atualizar o estoque do livro
        // e registrar o empréstimo no arquivo de empréstimos.
        // Primeiro, vamos obter o nome do usuário
        char *userName = getNomeUsuarioPorCodigo("usuarios.bin", codigo_usuario); // Assuming a users.bin file
        if (userName) {
            // Agora, vamos registrar o empréstimo
            // Chama a função emprestarLivro, que deve atualizar o estoque do livro
            // e registrar o empréstimo no arquivo de empréstimos.  
            emprestarLivro(nome_arquivo_livros, codigo_livro, userName);
            free(userName); 
        } else {
            printf("Erro ao carregar empréstimo: Usuário com código %d não encontrado.\n", codigo_usuario);
        }
    }
}

// Função para carregar dados completos de livros e usuários a partir de um arquivo de texto
// pre-condição: o arquivo de texto deve existir e estar no formato correto
// pos-condição: os dados de livros e usuários são carregados, e empréstimos pendentes são registrados
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
        linha[strcspn(linha, "\n")] = '\0'; // Remove newline

        char tipo_registro = linha[0]; // First character indicates type
        char *conteudo_linha = &linha[2]; // Skip type char and semicolon (e.g., "L;")

        removerEspacos(conteudo_linha); // Clean up leading/trailing spaces

        switch (tipo_registro) {
            case 'L': { // Livro
                Livro livro;
                char *token;

                token = strtok(conteudo_linha, ";");
                removerEspacos(token);
                livro.codigo = atoi(token);

                // ... (rest of book parsing, similar to original carregarArquivoDados) ...
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
                processarLinhaEmprestimo(nome_arquivo_livros, conteudo_linha); // Pass book file for stock update
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
    *dest = '\0'; // Termina a string resultante
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