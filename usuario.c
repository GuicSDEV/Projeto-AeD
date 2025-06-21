
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stddef.h> 

#include "usuario.h"
#include "util.h"   
#include "arquivo.h" 
#include "livro.h"   

// Funcao para chechar se um usuário existe no arquivo binário
int buscarUsuarioPorCodigo(const char *nome_arquivo_usuarios, int codigo_usuario) {
    FILE *arquivo = fopen(nome_arquivo_usuarios, "rb");
    if (!arquivo) {
        return 0; // se o arquivo não existe, o usuário não pode ser encontrado
        perror("Erro ao abrir o arquivo binário de usuários");
        exit(EXIT_FAILURE);
    }

    Cabecalho cabecalho; // assumindo que o cabeçalho contém a posição inicial da lista de usuários
    // Lê o cabeçalho do arquivo
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    int posicao_atual = cabecalho.posicao_inicio_lista; // assumindo que a posição inicial da lista de usuários é armazenada no cabeçalho
    Usuario usuario;
    // Percorre a lista encadeada de usuários
    // enquanto a posição atual não for -1 (indicando o final da lista)
    while (posicao_atual != -1) {
        fseek(arquivo, posicao_atual, SEEK_SET);
        fread(&usuario, sizeof(Usuario), 1, arquivo);
        if (usuario.codigo_usuario == codigo_usuario) {
            fclose(arquivo);
            return 1; // nao encontrado
        }
        posicao_atual = usuario.prox_registro;
    }

    fclose(arquivo);
    return 0; // usuario não encontrado
}

// Funcao para cadastrar um usuário no arquivo binário
// Verifica se o usuário já existe antes de cadastrar
// Se o usuário já existe, exibe uma mensagem de erro
void cadastrarUsuario(const char *nome_arquivo_usuarios, Usuario usuario) {
    if (buscarUsuarioPorCodigo(nome_arquivo_usuarios, usuario.codigo_usuario)) {
        printf("Erro: Usuário com o código %d já existe.\n", usuario.codigo_usuario);
        return;
    }

    FILE *arquivo = fopen(nome_arquivo_usuarios, "rb+");
    if (!arquivo) {
        // se o arquivo não existe, cria um novo
        printf("Arquivo binário de usuários não encontrado. Criando um novo arquivo.\n");
         // Cria o arquivo binário e inicializa o cabeçalho
         // O cabeçalho contém posições iniciais para a lista de usuários e registros livres
        arquivo = fopen(nome_arquivo_usuarios, "wb+");
        if (!arquivo) {
            perror("Erro ao criar o arquivo binário de usuários");
            exit(EXIT_FAILURE);
        }
        Cabecalho cabecalho_inicial = {-1, -1, -1};
        fwrite(&cabecalho_inicial, sizeof(Cabecalho), 1, arquivo);
    }

    Cabecalho cabecalho;
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    int nova_posicao;
    if (cabecalho.posicao_primeiro_livre == -1) {
        fseek(arquivo, 0, SEEK_END);
        nova_posicao = ftell(arquivo);
    } else {
        nova_posicao = cabecalho.posicao_primeiro_livre;
        fseek(arquivo, cabecalho.posicao_primeiro_livre, SEEK_SET);
        Usuario usuario_removido;
        fread(&usuario_removido, sizeof(Usuario), 1, arquivo);
        cabecalho.posicao_primeiro_livre = usuario_removido.prox_registro;
    }

    if (cabecalho.posicao_inicio_lista == -1) {
        cabecalho.posicao_inicio_lista = nova_posicao;
    } else {
        int posicao_atual = cabecalho.posicao_inicio_lista;
        Usuario usuario_atual;
        while (posicao_atual != -1) {
            fseek(arquivo, posicao_atual, SEEK_SET);
            fread(&usuario_atual, sizeof(Usuario), 1, arquivo);
            if (usuario_atual.prox_registro == -1) {
                fseek(arquivo, posicao_atual + offsetof(Usuario, prox_registro), SEEK_SET);
                fwrite(&nova_posicao, sizeof(int), 1, arquivo);
                break;
            }
            posicao_atual = usuario_atual.prox_registro;
        }
    }
    usuario.prox_registro = -1;

    fseek(arquivo, nova_posicao, SEEK_SET);
    fwrite(&usuario, sizeof(Usuario), 1, arquivo);

    fseek(arquivo, 0, SEEK_SET);
    fwrite(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    fclose(arquivo);
    printf("Usuário %s cadastrado com sucesso (Código: %d).\n", usuario.nome_usuario, usuario.codigo_usuario);
}

// funcao para obter o nome de um usuário pelo código
char* getNomeUsuarioPorCodigo(const char *nome_arquivo_usuarios, int codigo_usuario) {
    FILE *arquivo = fopen(nome_arquivo_usuarios, "rb");
    if (!arquivo) {
        return NULL;
    }

    Cabecalho cabecalho;
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    int posicao_atual = cabecalho.posicao_inicio_lista;
    Usuario usuario;

    while (posicao_atual != -1) {
        fseek(arquivo, posicao_atual, SEEK_SET);
        fread(&usuario, sizeof(Usuario), 1, arquivo);
        if (usuario.codigo_usuario == codigo_usuario) {
            fclose(arquivo);
            char* nome = malloc(sizeof(char) * MAX_NOME_USUARIO);
            if (nome) strcpy(nome, usuario.nome_usuario);
            return nome;
        }
        posicao_atual = usuario.prox_registro;
    }
    fclose(arquivo);
    return NULL;
}
