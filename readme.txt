====================================================
  TRABALHO I - ALGORITMOS E ESTRUTURA DE DADOS
====================================================

Universidade: Universidade Estadual do Oeste do Paraná (UNIOESTE) 
Campus:       Foz do Iguaçu 
Curso:        Ciência da Computação 
Professor:    Prof. Rômulo Silva 
Data Entrega: 25 de Junho de 2025 

----------------------------------------------------
          COMPONENTES DO GRUPO
----------------------------------------------------

- Aluno: Guilherme Chagas Silva
- Aluno: Yuri Santos Moller Alves


----------------------------------------------------
        FUNCIONALIDADES IMPLEMENTADAS
----------------------------------------------------

A seguir estão descritas as opções disponíveis no menu do programa, conforme solicitado no guia do trabalho.

1. Cadastrar Livro
   Explicação: Permite adicionar um novo livro ao acervo.  O sistema solicitará todas as informações necessárias, como código (único), título, autor, editora, edição, ano e a quantidade de exemplares disponíveis. 

2. Imprimir Dados do Livro
   Explicação: Exibe todas as informações detalhadas de um livro específico.  Para isso, o usuário deve fornecer o código do livro desejado. 

3. Listar Todos os Livros
   Explicação: Exibe uma lista resumida de todos os livros cadastrados, mostrando o código, título, autor e o número de exemplares disponíveis de cada um. 

4. Busca por Título
   Explicação: Permite ao usuário encontrar um livro buscando pelo seu título exato.  Se encontrado, todas as suas informações são exibidas. 

5. Calcular Total de Livros
   Explicação: Informa a quantidade total de títulos de livros diferentes que estão cadastrados no sistema. 

6. Cadastrar Usuário
   Explicação: Permite registrar um novo usuário no sistema da biblioteca.  É necessário fornecer um código (único) e o nome completo do usuário. 

7. Emprestar Livro
   Explicação: Realiza o registro de um empréstimo.  O usuário deve informar o código do livro e o código do usuário.  O sistema registra a data atual como data do empréstimo e diminui o número de exemplares disponíveis.  Caso não haja exemplares, uma mensagem de aviso será exibida. 

8. Devolver Livro
   Explicação: Realiza o registro da devolução de um livro.  O usuário deve informar o código do livro e o do usuário que o está devolvendo.  O sistema aumenta o número de exemplares disponíveis. 

9. Listar Livros Emprestados
   Explicação: Exibe uma lista de todos os livros que foram emprestados e ainda não foram devolvidos.  A lista informa o código e nome do usuário, o código e título do livro, e a data do empréstimo. 

10. Carregar Arquivo de Dados
    Explicação: Permite cadastrar livros, usuários e empréstimos em lote a partir de um arquivo de texto (.txt).  O programa solicitará o nome do arquivo, que deve seguir a sintaxe especificada no documento do trabalho (campos separados por ";" e linhas iniciadas por L, U ou E).

0. Sair
   Explicação: Encerra a execução do programa.