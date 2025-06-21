#ifndef UTIL_H
#define UTIL_H

typedef struct Emprestimo {
    int codigo_livro;
    int codigo_usuario;
    char data_emprestimo[37]; 
    char data_devolucao[37];  
    int devolvido; 
} Emprestimo;


// Proposito: Calcula o número total de livros cadastrados no arquivo.
// pre-condicao: O arquivo de livros existe.
// pos-condicao: Nenhuma.
// Entrada: nome do arquivo de livros.
// Retorno: O número total de livros.
int calcularTotalLivros(const char *nome_arquivo);

// Proposito: Busca por um livro no arquivo a partir do seu código.
// pre-condicao: O arquivo de livros existe.
// pos-condicao: Nenhuma.
// Entrada: nome do arquivo de livros, código do livro a ser buscado.
// Retorno: 1 se o livro for encontrado, 0 caso contrário.
int buscarLivroPorCodigo(const char *nome_arquivo, int codigo);

// Proposito: Registra o empréstimo de um livro para um usuário.
// pre-condicao: O arquivo de livros e o de usuários existem. O livro deve ter exemplares disponíveis.
// pos-condicao: O número de exemplares do livro é decrementado em 1 e um registro de empréstimo é criado.
// Entrada: nome do arquivo de livros, código do livro, código do usuário.
// Retorno: nenhum.
void emprestarLivro(const char *nome_arquivo, int codigo_livro, int codigo_usuario);

// Proposito: Registra a devolução de um livro por um usuário.
// pre-condicao: O arquivo de livros e o de empréstimos existem.
// pos-condicao: O número de exemplares do livro é incrementado em 1 e o registro de empréstimo é marcado como devolvido.
// Entrada: nome do arquivo de livros, código do livro, código do usuário.
// Retorno: nenhum.
void devolverLivro(const char *nome_arquivo, int codigo_livro, int codigo_usuario);

// Proposito: Lista todos os empréstimos que ainda estão em aberto (não devolvidos).
// pre-condicao: Os arquivos de livros, usuários e empréstimos existem.
// pos-condicao: Os dados dos empréstimos pendentes são impressos na tela.
// Entrada: nome do arquivo de livros, nome do arquivo de usuários.
// Retorno: nenhum.
void listarEmprestimosPendentes(const char *nome_arquivo_livros, const char *nome_arquivo_usuarios);

#endif