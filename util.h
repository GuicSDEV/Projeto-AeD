#ifndef UTIL_H
#define UTIL_H
#define MAX_NOME_USUARIO 100

typedef struct Emprestimo {
    int codigo_livro; 
    char nome_usuario[MAX_NOME_USUARIO];  
    char data_emprestimo[16]; // Formato: "DD/MM/AAAA"
    int devolvido; // 0 = não devolvido, 1 = devolvido 
} Emprestimo;

// Fun��es auxiliares

// Calcula o total de livros cadastrados
// pre-condicao: arquivo deve estar aberto para leitura
// pos-condicao: total de livros cadastrado retornado
// Entrada: nome do arquivo bin�rio
// Retorno: n�mero total de livros cadastrados no sistema
int calcularTotalLivros(const char *nome_arquivo);
int buscarLivroPorCodigo(const char *nome_arquivo, int codigo);
void emprestarLivro(const char *nome_arquivo, int codigo_livro, const char *nome_usuario);
void devolverLivro(const char *nome_arquivo, int codigo_livro, const char *nome_usuario);
void listarEmprestimosPendentes(const char *nome_arquivo);

#endif
