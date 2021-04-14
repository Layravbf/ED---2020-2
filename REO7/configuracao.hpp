/* Sequence Set - Arquivo de Configuração
 * (tipo de elemento armazenado, tamanho do pacote, etc.)
 *
 * by Joukim, Fevereiro de 2021 - Estrutura de Dados (GCC216)
 * Caracteristicas desta implementação - fevereiro de 2021:
 * -> usa chave/valor como dado de exemplo, mas que pode 
 * -> ser trocado com facilidade, usando typedef
 * -> 
 * -> tamanho máximo e mínimo do pacote definido por 
 * -> constantes (CAP_PACOTE e MIN_PACOTE)
 */

using namespace std;
#include <iostream>

struct elemento
{
    char aluno[50];
    unsigned matricula;
    char curso[60];
    char orientador[50];
    bool vagas_reservadas;
    char edital[30];
    char semestre_ingresso[10];
};

typedef elemento dado;

struct elementoB
{
    char aluno[50];
    int pos;
};

typedef elementoB dadoB;

// capacidade máxima e mínima do pacote
const unsigned CAP_PACOTE = 6;
const unsigned MIN_PACOTE = 3;

// posição do elemento do meio do pacote
const unsigned POS_MEIO = 3;

// posição inválida no disco
const int POS_INVALIDA = -1;
