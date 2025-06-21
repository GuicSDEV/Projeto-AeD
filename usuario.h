#ifndef USUARIO_H
#define USUARIO_H

#define MAX_NOME_USUARIO 51 
#define MAX_CODIGO_USUARIO 10 

typedef struct Usuario {
    int codigo_usuario; 
    char nome_usuario[MAX_NOME_USUARIO];
    int prox_registro;
} Usuario;


// Proposito: Cadastra um novo usuário no arquivo binário.
// pre-condicao: O arquivo de usuários já foi inicializado.
// pos-condicao: O usuário é adicionado ao final da lista no arquivo ou em um espaço livre.
// Entrada: nome do arquivo de usuários, struct Usuario com os dados.
// Retorno: nenhum.
void cadastrarUsuario(const char *nome_arquivo_usuarios, Usuario usuario);

// Proposito: Busca por um usuário no arquivo a partir do seu código.
// pre-condicao: O arquivo de usuários existe.
// pos-condicao: Nenhuma alteração no arquivo.
// Entrada: nome do arquivo de usuários, código do usuário a ser buscado.
// Retorno: 1 se o usuário for encontrado, 0 caso contrário.
int buscarUsuarioPorCodigo(const char *nome_arquivo_usuarios, int codigo_usuario);

// Proposito: Retorna o nome de um usuário a partir do seu código.
// pre-condicao: O arquivo de usuários existe.
// pos-condicao: Uma nova string com o nome do usuário é alocada na memória.
// Entrada: nome do arquivo de usuários, código do usuário.
// Retorno: ponteiro para a string com o nome do usuário (deve ser liberado com free()) ou NULL se não encontrado.
char* getNomeUsuarioPorCodigo(const char *nome_arquivo_usuarios, int codigo_usuario);

#endif