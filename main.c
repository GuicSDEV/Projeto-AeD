

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include <ctype.h>

#include "livro.h"
#include "arquivo.h"
#include "util.h"
#include "usuario.h"
#include "carregamento.h"

int main() {
    char nome_arquivo[] = "livraria.bin";
    inicializarArquivo(nome_arquivo);
    char nome_arquivo_usuarios[] = "usuarios.bin"; 
    inicializarArquivo(nome_arquivo_usuarios); 

    int opcao;
    do {
        
        printf("\n----- Menu da Biblioteca -----\n");
        printf("1. Cadastrar Livro\n");
        printf("2. Imprimir Dados do Livro\n");
        printf("3. Listar Todos os Livros\n");
        printf("4. Busca por Título\n");
        printf("5. Calcular Total de Livros\n");
        printf("6. Cadastrar Usuário\n");
        printf("7. Emprestar Livro\n");
        printf("8. Devolver Livro\n");
        printf("9. Listar Livros Emprestados\n");
        printf("10. Carregar Arquivo de Dados\n");
        printf("11. Remover Livro \n");
        printf("12. Imprimir Lista de Livros\n");
        printf("0. Sair\n");
        printf("----------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        int codigo;
        char titulo[MAX_TITULO], autor[MAX_AUTOR];
        Livro livro;
        Usuario usuario;

        switch (opcao) {
            case 1:
                printf("Digite os dados do livro:\n");
                printf("Código: ");
                scanf("%d", &livro.codigo);
                printf("Título: ");
                scanf(" %[^\n]", livro.titulo);
                printf("Autor: ");
                scanf(" %[^\n]", livro.autor);
                printf("Editora: ");
                scanf(" %[^\n]", livro.editora);
                printf("Edição: ");
                scanf("%d", &livro.edicao);
                printf("Ano de Publicação: ");
                scanf("%d", &livro.ano_publicacao);
                printf("Quantidade em Estoque: ");
                scanf("%d", &livro.quantidade_estoque);
                cadastrarLivro(nome_arquivo, livro);
                break;

            case 2:
                printf("Digite o código do livro: ");
                scanf("%d", &codigo);
                imprimirLivro(nome_arquivo, codigo);
                break;

            case 3:
                listarTodosLivros(nome_arquivo);
                break;

            case 4:
                printf("Digite o título do livro: ");
                scanf(" %[^\n]", titulo);
                buscarPorTitulo(nome_arquivo, titulo);
                break;

            case 5:
                printf("Total de livros cadastrados: %d\n", calcularTotalLivros(nome_arquivo));
                break;

            case 6:
                printf("Digite os dados do usuário:\n");
                printf("Código do Usuário: ");
                scanf("%d", &usuario.codigo_usuario);
                printf("Nome do Usuário: ");
                scanf(" %[^\n]", usuario.nome_usuario);
                cadastrarUsuario(nome_arquivo_usuarios, usuario);
                break;
            
            case 7: {
                int codigo_livro_emprestimo, codigo_usuario_emprestimo;
                printf("Digite o código do livro a ser emprestado: ");
                scanf("%d", &codigo_livro_emprestimo);
                printf("Digite o código do usuário: ");
                scanf("%d", &codigo_usuario_emprestimo); // CORRIGIDO: usa %d para ler um inteiro
                emprestarLivro(nome_arquivo, codigo_livro_emprestimo, codigo_usuario_emprestimo);
                break;
            }
            case 8: {
                int codigo_livro_devolucao, codigo_usuario_devolucao;
                printf("Digite o código do livro a ser devolvido: ");
                scanf("%d", &codigo_livro_devolucao);
                printf("Digite o código do usuário: ");
                scanf("%d", &codigo_usuario_devolucao); 
                devolverLivro(nome_arquivo, codigo_livro_devolucao, codigo_usuario_devolucao);
                break;
            }
            case 9:
                printf("Listando empréstimos pendentes...\n");
                listarEmprestimosPendentes(nome_arquivo, nome_arquivo_usuarios);
                break;

            case 10: {
                char arquivo_txt[100];
                printf("Digite o nome do arquivo de texto de carga: ");
                scanf("%99s", arquivo_txt);
                carregarDadosCompletos(nome_arquivo, nome_arquivo_usuarios, arquivo_txt);
                break;
            }

            // Funções extras que não estavam no PDF principal
            case 11:
                printf("Digite o código do livro a ser removido: ");
                scanf("%d", &codigo);
                removerLivro(nome_arquivo, codigo);
                break;

            case 12:
                imprimirListaLivres(nome_arquivo);
                break;
            
            case 0:
                printf("Saindo do programa...\n");
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}