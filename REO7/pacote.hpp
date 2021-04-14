/* 
  Trabalho de Estrutura de Dados
  Implementação de Árvore B+ a partir de código de Sequence Set
  Arthur Bonani de Oliveira Paiva
  Glaucio Martins Lima 
  Layra Vilas Boas Ferreira
     Arquivo com a classe que representa o pacote do sequence set
*/

#include <iostream>
#include <string.h>
#include "configuracao.hpp"
using namespace std;

// essa classe também poderia se chamar bloco, pagina, packet ou pedaco
class pacote
{
    friend class sequenceset;
    friend class Bplus;
    // classe não possui métodos públicos
    // objetos da classe são criados e manuseados
    // apenas pela classe sequenceset
private:
    bool eFolha;
    dado elementos[CAP_PACOTE];
    unsigned numElementos;
    pacote *pai;
    pacote *filho[CAP_PACOTE + 1];
    int posProximoPacote;
    pacote(bool folha) : pai(NULL), numElementos(0), posProximoPacote(POS_INVALIDA), eFolha(folha) {}
    bool cheio() { return (numElementos == CAP_PACOTE); }
    void inserir(dado umDado);
    inline bool chaveEhMaiorQueTodos(char *chave);
    inline bool chaveEhMenorQueTodos(char *chave);
    void imprimir();
};

void pacote::inserir(dado umDado)
{
    int posicao = numElementos - 1;
    // Faz a procura pela posição de inserção do elemento de forma decrescente
    while ((posicao >= 0) and strcmp(umDado.aluno, elementos[posicao].aluno) < 0)
    {
        elementos[posicao + 1] = elementos[posicao];
        posicao--;
    }
    elementos[posicao + 1] = umDado;
    numElementos++;
}

void pacote::imprimir()
{
    cout << "[";
    for (unsigned i = 0; i < numElementos; i++)
        cout << "(" << elementos[i].aluno << "/" << elementos[i].matricula << "/" << elementos[i].curso << "/" << elementos[i].orientador << "/" << elementos[i].vagas_reservadas << "/" << elementos[i].edital << "/" << elementos[i].semestre_ingresso << ")";
    cout << "]";
}

bool pacote::chaveEhMaiorQueTodos(char *chave)
{
    return strcmp(elementos[numElementos - 1].aluno, chave) < 0;
}

bool pacote::chaveEhMenorQueTodos(char *chave)
{
    return strcmp(elementos[0].aluno, chave) > 0;
}
