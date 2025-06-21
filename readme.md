****************************************************
* 🧑🏻‍🏫📚SISTEMA DE GERENCIAMENTO DE BIBLIOTECA EM C   *
****************************************************

Linguagem: C | Licença: MIT

--------------------
SOBRE O PROJETO
--------------------
Este projeto é um sistema de gerenciamento de biblioteca desenvolvido em C puro, como parte do Trabalho I de Algoritmos e Estrutura de Dados da Universidade Estadual do Oeste do Paraná (UNIOESTE).

O principal desafio e diferencial do projeto é a persistência de dados diretamente em arquivos binários, utilizando uma implementação de lista encadeada em disco. Diferente de abordagens que carregam tudo para a RAM, este sistema manipula os registros diretamente no arquivo, tornando-o eficiente em uso de memória e um excelente exercício de manipulação de arquivos de baixo nível.


--------------------
FUNCIONALIDADES
--------------------

O sistema oferece uma interface de linha de comando (CLI) para gerenciar todas as operações da biblioteca:

* Gerenciamento de Livros:
  - Cadastro completo de livros, com informações como título, autor, editora, etc.
  - Busca de livros por código ou título.
  - Listagem de todos os livros com informações essenciais.
  - Cálculo do número total de títulos no acervo.

* Gerenciamento de Usuários:
  - Cadastro de usuários da biblioteca com código e nome.

* Sistema de Empréstimo e Devolução:
  - Registro de empréstimos associando um livro a um usuário, com data automática.
  - Controle de estoque, impedindo o empréstimo de livros sem exemplares.
  - Registro de devoluções, atualizando o estoque e o status do empréstimo.
  - Listagem de todos os empréstimos pendentes.

* Carga de Dados em Lote:
  - Funcionalidade para carregar informações de livros, usuários e empréstimos a partir de um arquivo .txt, permitindo popular o sistema de forma rápida e eficiente.


--------------------
CONCEITOS TÉCNICOS APLICADOS
--------------------

Este projeto foi uma oportunidade para aplicar e demonstrar conhecimento em diversos conceitos fundamentais da ciência da computação:

* Manipulação de Arquivos em Baixo Nível:
  Uso intensivo das funções da stdio.h (fopen, fread, fwrite, fseek) para ler e escrever em arquivos binários.

* Estrutura de Dados em Disco:
  A principal estrutura do sistema é uma lista encadeada implementada diretamente no arquivo binário. Cada registro (struct) no arquivo contém um ponteiro (na verdade, a posição 'long' no arquivo) para o próximo registro, evitando a necessidade de carregar toda a base de dados para a memória RAM.

* Gerenciamento de Espaço Livre (Free List):
  O sistema não apaga fisicamente os registros removidos. Em vez disso, ele gerencia uma lista encadeada de espaços livres. Quando um novo registro é adicionado, o sistema primeiro tenta reutilizar um desses espaços, otimizando o uso do arquivo.

* Modularização em C:
  O código é organizado em módulos com responsabilidades bem definidas (livro, usuario, arquivo, util), utilizando arquivos de cabeçalho (.h) para definir as interfaces públicas de cada módulo.

* Parsing de Arquivos de Texto:
  Implementação de um parser para ler e interpretar o arquivo de carga em lote, utilizando funções como strtok para dividir as strings com base em um delimitador (;).


--------------------
COMO COMPILAR E EXECUTAR
--------------------

Pré-requisitos:
- Um compilador C (como o GCC).

Compilação:
Abra o terminal na pasta raiz do projeto e execute o seguinte comando:

    gcc -o biblioteca main.c livro.c usuario.c util.c arquivo.c carregamento.c -Wextra -Wall

Execução:
Após a compilação, execute o programa com:

    ./biblioteca

O menu interativo da biblioteca será exibido.


--------------------
EXEMPLO DE ARQUIVO DE CARGA (dados.txt)
--------------------

A funcionalidade de carga em lote espera um arquivo .txt com o seguinte formato:

L;7;Memorias Postumas de Bras Cubas;Machado de Assis;Bookman;4;2022;5
L;11;A insustentavel leveza do ser;Milan Kundera;Abril;3;2015;7
L;27;A Hora da Estrela;Clarice Lispector;Abril;5;2007;3
U;11;Joao da Silva
U;7;Maria dos Santos
E;7;27;11/03/2025;25/03/2025
E;11;7;20/06/2025;


