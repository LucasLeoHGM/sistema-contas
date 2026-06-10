# Sistema de Manutenção de Contas

## Sobre o Projeto

Este projeto foi desenvolvido como atividade da disciplina **PIF (Programação Imperativa Fundamental)**, com o objetivo de praticar a manipulação de arquivos binários em linguagem C utilizando registros de tamanho fixo.

A aplicação implementa um sistema simples de manutenção de contas bancárias, permitindo cadastrar, consultar, atualizar e remover clientes armazenados em um arquivo binário.

Durante o desenvolvimento foram utilizados conceitos importantes da linguagem C, como:

* Estruturas (`struct`)
* Arquivos binários
* Registros de tamanho fixo
* Leitura e escrita de arquivos
* Acesso direto a registros utilizando `fseek()`
* Leitura de registros com `fread()`
* Escrita de registros com `fwrite()`
* Reposicionamento do ponteiro com `rewind()`

---

## Funcionalidades

1. Cadastrar um novo cliente em uma posição específica.
2. Consultar um cliente pelo número da conta.
3. Atualizar o saldo de um cliente.
4. Encerrar uma conta (remoção lógica).
5. Listar todos os clientes ativos.
6. Restaurar a leitura do arquivo utilizando `rewind()` e repetir a listagem.
7. Encerrar o sistema.

---

## Estrutura do Registro

```c
typedef struct {
    int numero_conta;
    char nome[50];
    double saldo;
    int ativo;
} Cliente;
```

---

## Arquivo de Dados

Os registros são armazenados no arquivo:

```text
contas.dat
```

O arquivo utiliza registros de tamanho fixo para permitir acesso direto às posições desejadas através da função `fseek()`.

---

## Como Compilar

```bash
gcc main.c -o sistema_contas
```

## Como Executar

### Linux/macOS

```bash
./sistema_contas
```

### Windows

```bash
sistema_contas.exe
```

---

## Objetivo Acadêmico

Esta atividade foi proposta na disciplina **PIF (Programação Imperativa Fundamental)** com o objetivo de demonstrar o uso de arquivos binários e das funções de manipulação de arquivos da biblioteca padrão da linguagem C, especialmente:

* `fseek()`
* `fread()`
* `fwrite()`
* `rewind()`


