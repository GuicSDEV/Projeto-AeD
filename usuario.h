#ifndef USUARIO_H
#define USUARIO_H
#define MAX_NOME_USUARIO 100
#define MAX_CODIGO_USUARIO 10 // Assuming user codes can be strings or larger integers if needed




// Struct para representar um usuário
typedef struct Usuario {
    int codigo_usuario; // unico para cada usuário
    char nome_usuario[MAX_NOME_USUARIO];
    int prox_registro; 
} Usuario;

// funcoes para manipulação de usuários
void cadastrarUsuario(const char *nome_arquivo_usuarios, Usuario usuario);
int buscarUsuarioPorCodigo(const char *nome_arquivo_usuarios, int codigo_usuario);

#endif