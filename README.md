# 📚🧑🏻‍🏫 Sistema de Gerenciamento de Livraria

Este é um sistema em C para gerenciar livros, usuários e empréstimos em uma livraria. Ele permite o cadastro, remoção, busca e listagem de livros, bem como o registro e devolução de empréstimos e o cadastro de usuários. Os dados são persistidos em arquivos binários.

Este projeto foi desenvolvido para a disciplina de Algoritmos e Estruturas de Dados I, ministrada pelo Professor Rômulo César.

## Funcionalidades

O sistema oferece as seguintes opções no menu principal:

1.  **Cadastrar Livro**: Permite inserir informações de um novo livro (código, título, autor, editora, edição, ano de publicação, preço, quantidade em estoque).
2.  **Remover Livro**: Remove um livro do cadastro a partir do seu código, adicionando a posição liberada à lista de registros livres para reutilização.
3.  **Imprimir Dados de um Livro**: Exibe todos os detalhes de um livro específico dado o seu código.
4.  **Listar Todos os Livros**: Apresenta uma lista de todos os livros cadastrados, mostrando o código, título, autor e quantidade em estoque.
5.  **Buscar por Autor**: Lista os títulos de todos os livros de um determinado autor.
6.  **Buscar por Título**: Exibe todas as informações de um livro com base no seu título.
7.  **Calcular Total de Livros**: Informa o número total de livros atualmente cadastrados no sistema.
8.  **Carregar Dados de um Arquivo Texto**: Carrega dados de livros, usuários e empréstimos a partir de um arquivo de texto formatado, incluindo-os no sistema binário.
9.  **Imprimir Lista de Registros Livres**: Mostra as posições no arquivo binário que estão disponíveis para novos registros (resultantes de remoções).
10. **Emprestar Livro**: Registra um empréstimo de livro para um usuário, diminuindo a quantidade em estoque. Se não houver exemplares disponíveis, uma mensagem é exibida.
11. **Devolver Livro**: Registra a devolução de um livro por um usuário, aumentando a quantidade em estoque.
12. **Listar Empréstimos Pendentes**: Lista os códigos de usuário, nomes dos usuários, códigos dos livros, títulos dos livros e datas de empréstimo para todos os livros que ainda não foram devolvidos.
13. **Cadastrar Usuário**: Permite cadastrar um novo usuário com um código único e nome.
14. **Sair**: Encerra o programa.

## Estrutura do Projeto

O projeto é organizado nos seguintes arquivos:

* `main.c`: Contém a função principal (`main`) que gerencia o menu de operações e interage com as demais funcionalidades.
* `livro.h`: Define a estrutura `Livro` e os protótipos das funções relacionadas ao gerenciamento de livros (cadastrar, remover, imprimir, listar, buscar).
* `livro.c`: Contém as implementações das funções declaradas em `livro.h`.
* `arquivo.h`: Define a estrutura `Cabecalho` (para gerenciar o arquivo binário) e os protótipos das funções de manipulação de arquivo (inicialização, carregamento de dados, impressão de lista de livres, utilitários de string).
* `arquivo.c`: Contém as implementações das funções declaradas em `arquivo.h`, incluindo o carregamento de dados de texto e a gestão da lista de registros livres.
* `util.h`: Define as estruturas `Emprestimo` e `Usuario`, além dos protótipos de funções auxiliares como cálculo do total de livros, busca de livro por código, e funções de empréstimo/devolução.
* `util.c`: Contém as implementações das funções auxiliares, incluindo a lógica de empréstimos e devoluções, e o cadastro de usuários.
* `teste.txt`: Um arquivo de texto de exemplo usado para carregar dados em lote, contendo registros de livros (`L;`), usuários (`U;`) e empréstimos (`E;`).

## Compilação e Execução

Para compilar o projeto, você precisará de um compilador C (como GCC). Navegue até a raiz do projeto no terminal e execute:

```bash
gcc main.c livro.c arquivo.c util.c -o livraria
