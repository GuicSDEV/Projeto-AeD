#ifndef LIVRO_H
#define LIVRO_H

#include <stdio.h>

// Defini��es para tamanhos m�ximos dos campos de string
#define MAX_TITULO 150
#define MAX_AUTOR 200
#define MAX_EDITORA 50

// Estrutura do Registro de Livro
typedef struct Livro {
    int codigo;                        // C�digo do livro, �nico para cada livro
    char titulo[MAX_TITULO];           // T�tulo do livro
    char autor[MAX_AUTOR];             // Autor do livro
    char editora[MAX_EDITORA];         // Editora do livro
    int edicao;                        // Edi��o do livro
    int ano_publicacao;                // Ano de publica��o do livro
    float preco;                       // Pre�o do livro
    int quantidade_estoque;            // Quantidade em estoque
    int prox_registro;                 // Ponteiro para o pr�ximo registro na lista encadeada
} Livro;

// Fun��es relacionadas aos livros

// Cadastra um livro no arquivo bin�rio
// pre-condicao: arquivo deve estar aberto para leitura e escrita
// pos-condicao: livro cadastrado no arquivo bin�rio
// Entrada: nome do arquivo bin�rio e estrutura Livro contendo as informa��es do livro
// Retorno: nenhum
void cadastrarLivro(const char *nome_arquivo, Livro livro);

// Remove um livro do arquivo bin�rio
// pre-condicao: arquivo deve estar aberto para leitura e escrita
// pos-condicao: livro removido do arquivo bin�rio, e posi��o adicionada � lista de registros livres
// Entrada: nome do arquivo bin�rio e c�digo do livro a ser removido
// Retorno: nenhum
void removerLivro(const char *nome_arquivo, int codigo);

// Imprime os dados de um livro espec�fico
// pre-condicao: arquivo deve estar aberto para leitura
// pos-condicao: dados do livro impressos na tela
// Entrada: nome do arquivo bin�rio e c�digo do livro a ser impresso
// Retorno: nenhum
void imprimirLivro(const char *nome_arquivo, int codigo);

// Lista todos os livros cadastrados no sistema
// pre-condicao: arquivo deve estar aberto para leitura
// pos-condicao: lista de livros impressa na tela
// Entrada: nome do arquivo bin�rio
// Retorno: nenhum
void listarTodosLivros(const char *nome_arquivo);

// Busca livros por autor
// pre-condicao: arquivo deve estar aberto para leitura
// pos-condicao: t�tulos dos livros do autor impressos na tela
// Entrada: nome do arquivo bin�rio e nome do autor a ser buscado
// Retorno: nenhum
void buscarPorAutor(const char *nome_arquivo, const char *autor);

// Busca livros por t�tulo
// pre-condicao: arquivo deve estar aberto para leitura
// pos-condicao: dados do livro com o t�tulo fornecido impressos na tela
// Entrada: nome do arquivo bin�rio e t�tulo do livro a ser buscado
// Retorno: nenhum
void buscarPorTitulo(const char *nome_arquivo, const char *titulo);



// pega o t�tulo de um livro pelo c�digo
// pre-condicao: arquivo deve estar aberto para leitura
// pos-condicao: t�tulo do livro retornado
// Entrada: nome do arquivo bin�rio e c�digo do livro
// Retorno: ponteiro para o t�tulo do livro, ou NULL se n��o encontrado
char* getTituloLivroPorCodigo(const char *nome_arquivo, int codigo);
#endif
