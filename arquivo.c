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

// Carrega dados de um arquivo texto e cadastra no sistema
// L� um arquivo texto com dados de livros e cadastra cada livro no sistema, evitando duplicatas
// pre-condicao: arquivo de texto deve estar no formato correto
// pos-condicao: dados do arquivo texto carregados e cadastrados no sistema
// Entrada: nome do arquivo bin�rio e nome do arquivo texto
// Retorno: nenhum
void carregarArquivoDados(const char *nome_arquivo, const char *arquivo_txt) {
    FILE *arquivo = fopen(arquivo_txt, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de texto");
        exit(EXIT_FAILURE);
    }

    char linha[512];
    while (fgets(linha, sizeof(linha), arquivo)) {
        // Remove a nova linha (\n) no final da string
        linha[strcspn(linha, "\n")] = '\0';

        Livro livro;
        char *token;

        // Tratamento da linha para remover espa�os em branco
        removerEspacos(linha);

        // Primeiro token: c�digo do livro
        token = strtok(linha, ";");
        removerEspacos(token);
        livro.codigo = atoi(token);

        // Verifica se o livro j� existe pelo c�digo
        if (buscarLivroPorCodigo(nome_arquivo, livro.codigo)) {
            printf("Livro com c�digo %d j� existe.\n", livro.codigo);
            continue;
        }

        // Segundo token: t�tulo
        token = strtok(NULL, ";");
        removerEspacos(token);
        strcpy(livro.titulo, token);

        // Terceiro token: autor
        token = strtok(NULL, ";");
        removerEspacos(token);
        strcpy(livro.autor, token);

        // Quarto token: editora
        token = strtok(NULL, ";");
        removerEspacos(token);
        strcpy(livro.editora, token);

        // Quinto token: edi��o
        token = strtok(NULL, ";");
        removerEspacos(token);
        livro.edicao = atoi(token);

        // Sexto token: ano de publica��o
        token = strtok(NULL, ";");
        removerEspacos(token);
        livro.ano_publicacao = atoi(token);

        // S�timo token: pre�o
        token = strtok(NULL, ";");
        removerEspacos(token);
        substituirVirgulaPorPonto(token);
        livro.preco = atof(token);

        // Oitavo token: quantidade em estoque
        token = strtok(NULL, ";");
        removerEspacos(token);
        livro.quantidade_estoque = atoi(token);

        // Inicializando o campo prox_registro
        livro.prox_registro = -1;

        // Cadastrando o livro
        cadastrarLivro(nome_arquivo, livro);
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
