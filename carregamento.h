#ifndef CARREGAMENTO_H
#define CARREGAMENTO_H

#include <stdio.h>
#include "livro.h"
#include "util.h"
#include "usuario.h"
#include "arquivo.h"

// Carrega dados completos de livros e usuários a partir de arquivos binários
// e registra empréstimos pendentes em um arquivo de texto
// pre-condicao: arquivos binários devem existir
// pos-condicao: dados de livros e usuários carregados, e empréstimos pendentes registrados
// Entrada: nomes dos arquivos binários de livros e usuários, e nome do arquivo de texto para empréstimos
// Retorno: nenhum
void carregarDadosCompletos(const char *nome_arquivo_livros, const char *nome_arquivo_usuarios, const char *arquivo_txt);

// Atualiza o estoque de um livro no arquivo binário
// pre-condicao: arquivo binário de livros deve existir
// pos-condicao: estoque do livro atualizado
// Entrada: nome do arquivo binário de livros, código do livro, delta (quantidade a ser adicionada ou subtraída)
void atualizarEstoqueLivro(const char *nome_arquivo_livros, int codigo_livro, int delta);


#endif