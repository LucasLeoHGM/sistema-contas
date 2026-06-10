#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO_DADOS "contas.dat"

// Estrutura com tamanho fixo para o registro do cliente
typedef struct {
    int numero_conta;
    char nome[50];
    double saldo;
    int ativo; // 1 para ativo, 0 para conta encerrada/vazia
} Cliente;

// Prototipos das funcoes
void cadastrar_cliente(FILE *arquivo);
void consultar_cliente(FILE *arquivo);
void atualizar_saldo(FILE *arquivo);
void encerrar_conta(FILE *arquivo);
void listar_clientes(FILE *arquivo);
void restaurar_e_listar(FILE *arquivo);

int main() {
    // Abre o arquivo para leitura e escrita em modo binario (r+b)
    // Se nao existir, cria um novo (w+b)
    FILE *arquivo = fopen(ARQUIVO_DADOS, "rb+");
    if (arquivo == NULL) {
        arquivo = fopen(ARQUIVO_DADOS, "wb+");
        if (arquivo == NULL) {
            printf("Erro ao abrir ou criar o arquivo de dados.\n");
            return 1;
        }
    }

    int opcao;
    do {
        printf("\n--- SISTEMA DE MANUTENCAO DE CONTAS ---\n");
        printf("1. Cadastrar novo cliente em posicao especifica\n");
        printf("2. Consultar cliente pelo numero da conta\n");
        printf("3. Atualizar saldo de um cliente\n");
        printf("4. Encerrar conta (remover cliente)\n");
        printf("5. Listar todos os clientes\n");
        printf("6. Restaurar leitura do inicio (rewind) e listar\n");
        printf("7. Encerrar sistema\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpar o buffer do teclado

        switch (opcao) {
            case 1: cadastrar_cliente(arquivo); break;
            case 2: consultar_cliente(arquivo); break;
            case 3: atualizar_saldo(arquivo); break;
            case 4: encerrar_conta(arquivo); break;
            case 5: listar_clientes(arquivo); break;
            case 6: restaurar_e_listar(arquivo); break;
            case 7: printf("Encerrando o programa...\n"); break;
            default: printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 7);

    fclose(arquivo);
    return 0;
}

// 1. Cadastrar um novo cliente em uma posicao especifica
void cadastrar_cliente(FILE *arquivo) {
    int posicao;
    Cliente novo;

    printf("Digite a posicao (indice iniciando em 0) onde deseja cadastrar: ");
    scanf("%d", &posicao);
    getchar();

    // Move o ponteiro do arquivo para a posicao desejada
    fseek(arquivo, (long)posicao * sizeof(Cliente), SEEK_SET);

    // Verifica se ja existe um cliente ativo nessa posicao
    Cliente existente;
    if (fread(&existente, sizeof(Cliente), 1, arquivo) == 1 && existente.ativo == 1) {
        printf("Aviso: Ja existe um cliente ativo nesta posicao! Operacao cancelada.\n");
        return;
    }

    // Coleta dos dados do novo cliente
    printf("Numero da conta: ");
    scanf("%d", &novo.numero_conta);
    getchar();
    printf("Nome do cliente: ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0'; // Remove o '\n'
    printf("Saldo inicial: ");
    scanf("%lf", &novo.saldo);
    novo.ativo = 1; // Define o registro como ativo

    // Reposiciona para garantir a escrita no local correto apos a leitura de teste
    fseek(arquivo, (long)posicao * sizeof(Cliente), SEEK_SET);
    
    if (fwrite(&novo, sizeof(Cliente), 1, arquivo) == 1) {
        printf("Cliente cadastrado com sucesso na posicao %d!\n", posicao);
    } else {
        printf("Erro ao salvar os dados do cliente.\n");
    }
}

// 2. Consultar um cliente pelo numero da conta
void consultar_cliente(FILE *arquivo) {
    int num_conta;
    Cliente c;
    int encontrado = 0;

    printf("Digite o numero da conta para consulta: ");
    scanf("%d", &num_conta);

    rewind(arquivo); // Volta ao inicio para buscar sequencialmente

    while (fread(&c, sizeof(Cliente), 1, arquivo) == 1) {
        if (c.ativo == 1 && c.numero_conta == num_conta) {
            printf("\n--- Cliente Encontrado ---\n");
            printf("Conta: %d\n", c.numero_conta);
            printf("Nome: %s\n", c.nome);
            printf("Saldo: RS %.2f\n", c.saldo);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Conta numero %d nao foi encontrada ou esta inativa.\n", num_conta);
    }
}

// 3. Atualizar o saldo de um cliente
void atualizar_saldo(FILE *arquivo) {
    int num_conta;
    Cliente c;
    int encontrado = 0;
    double novo_saldo;

    printf("Digite o numero da conta para atualizar o saldo: ");
    scanf("%d", &num_conta);

    rewind(arquivo);

    while (fread(&c, sizeof(Cliente), 1, arquivo) == 1) {
        if (c.ativo == 1 && c.numero_conta == num_conta) {
            encontrado = 1;
            printf("Cliente: %s | Saldo Atual: RS %.2f\n", c.nome, c.saldo);
            printf("Digite o novo saldo: ");
            scanf("%lf", &novo_saldo);

            c.saldo = novo_saldo;

            // Recua o ponteiro do arquivo exatamente 1 registro para sobrescrever
            fseek(arquivo, -(long)sizeof(Cliente), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, arquivo);
            
            printf("Saldo atualizado com sucesso!\n");
            break;
        }
    }

    if (!encontrado) {
        printf("Conta nao encontrada.\n");
    }
}

// 4. Encerra conta (remover cliente de forma logica)
void encerrar_conta(FILE *arquivo) {
    int num_conta;
    Cliente c;
    int encontrado = 0;

    printf("Digite o numero da conta que deseja encerrar: ");
    scanf("%d", &num_conta);

    rewind(arquivo);

    while (fread(&c, sizeof(Cliente), 1, arquivo) == 1) {
        if (c.ativo == 1 && c.numero_conta == num_conta) {
            encontrado = 1;
            c.ativo = 0; // Desativacao logica

            // Recua o ponteiro para atualizar o registro no arquivo
            fseek(arquivo, -(long)sizeof(Cliente), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, arquivo);

            printf("Conta numero %d encerrada com sucesso.\n", num_conta);
            break;
        }
    }

    if (!encontrado) {
        printf("Conta nao encontrada.\n");
    }
}

// 5. Listar todos os clientes da posicao atual do ponteiro em diante
void listar_clientes(FILE *arquivo) {
    Cliente c;
    int existem_clientes = 0;

    // Obtem a posicao atual apenas para exibicao informativa no console
    long posicao_atual = ftell(arquivo) / (long)sizeof(Cliente);
    printf("\n--- Listando a partir da posicao logica: %ld ---\n", posicao_atual);

    while (fread(&c, sizeof(Cliente), 1, arquivo) == 1) {
        if (c.ativo == 1) {
            printf("Conta: %d | Nome: %-20s | Saldo: RS %8.2f\n", c.numero_conta, c.nome, c.saldo);
            existem_clientes = 1;
        }
    }

    if (!existem_clientes) {
        printf("Nenhum cliente ativo encontrado a partir desta posicao.\n");
    }
}

// 6. Restaurar a leitura do arquivo do inicio com rewind() para repetir a listagem
void restaurar_e_listar(FILE *arquivo) {
    printf("\n[Executando rewind()... Ponteiro retornado ao inicio do arquivo]\n");
    rewind(arquivo);
    listar_clientes(arquivo);
}