#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdio.h>
#include "livro.h"

// Estrutura do Cabe�alho do Arquivo Bin�rio
typedef struct Cabecalho {
    int posicao_inicio_lista;          // Posi��o inicial da lista encadeada de livros
    int posicao_primeiro_livre;        // Posi��o do primeiro registro livre no arquivo
    int posicao_lista_livres;          // Posi��o do in�cio da lista de registros removidos (livres)
} Cabecalho;

// Fun��es relacionadas ao gerenciamento do arquivo bin�rio

// Inicializa o arquivo bin�rio
// pre-condicao: nome_arquivo deve ser um nome de arquivo v�lido
// pos-condicao: arquivo bin�rio criado e cabe�alho inicializado, ou arquivo existente aberto para leitura/escrita
// Entrada: nome do arquivo bin�rio
// Retorno: nenhum
void inicializarArquivo(const char *nome_arquivo);

// Carrega dados de um arquivo texto e cadastra no sistema
// pre-condicao: arquivo de texto deve estar no formato correto
// pos-condicao: dados do arquivo texto carregados e cadastrados no sistema
// Entrada: nome do arquivo bin�rio e nome do arquivo texto
// Retorno: nenhum
void carregarArquivoDados(const char *nome_arquivo, const char *arquivo_txt);

// Imprime a lista de registros livres
// pre-condicao: arquivo deve estar aberto para leitura
// pos-condicao: posi��es livres impressas na tela
// Entrada: nome do arquivo bin�rio
// Retorno: nenhum
void imprimirListaLivres(const char *nome_arquivo);
void substituirVirgulaPorPonto(char *str);
void removerEspacos(char *str);

#endif
