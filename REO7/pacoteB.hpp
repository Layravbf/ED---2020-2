/* 
  Trabalho de Estrutura de Dados
  Implementação de Árvore B+ a partir de código de Sequence Set
  Arthur Bonani de Oliveira Paiva
  Glaucio Martins Lima 
  Layra Vilas Boas Ferreira
     Arquivo com a classe que representa o pacote da arvore B+
*/

#include <iostream>
#include <string.h>
using namespace std;

// essa classe também poderia se chamar bloco, pagina, packet ou pedaco
class pacoteB
{
    friend class sequenceset;
    friend class Bplus;
    // classe não possui métodos públicos
    // objetos da classe são criados e manuseados
    // apenas pela classe sequenceset
private:
    bool eFolha;
    dadoB elementos[CAP_PACOTE]; // ELE TEM QUE GUARDAR A POSICAO NO ARQUIVO (INDICE) E A CHAVE PRA BUSCAR
    unsigned numElementos;
    int pai;
    int filho[CAP_PACOTE + 1];
    int filhosSet[CAP_PACOTE + 1];
    pacoteB(bool folha) : pai(POS_INVALIDA), numElementos(0), eFolha(folha)
    {
        filhosSet[0] = POS_INVALIDA;
        for (size_t i = 0; i < CAP_PACOTE + 1; i++)
        {
            filho[i] = 0;
        }
    }
    bool cheio() { return (numElementos == CAP_PACOTE); }
    void inserir(dadoB umDado);
    inline bool chaveEhMaiorQueTodos(char *chave);
    inline bool chaveEhMenorQueTodos(char *chave);
    void imprimir();
    int busca(char *chave);
};

void pacoteB::inserir(dadoB umDado)
{
    if (filhosSet[0] == POS_INVALIDA)
    {
        filhosSet[0] = umDado.pos;
    }
    else
    {
        int posicao = numElementos - 1;
        // Faz a procura pela posição de inserção do elemento de forma decrescente
        while ((posicao >= 0) and strcmp(umDado.aluno, elementos[posicao].aluno) < 0)
        {
            elementos[posicao + 1] = elementos[posicao];
            filhosSet[posicao + 3] = filhosSet[posicao + 1];
            posicao--;
        }

        elementos[posicao + 1] = umDado;
        filhosSet[posicao + 2] = umDado.pos;
        numElementos++;
    }
}

void pacoteB::imprimir()
{

    cout << "[";
    for (unsigned i = 0; i < numElementos; i++)
        cout << "(" << elementos[i].aluno << "/" << elementos[i].pos << ")";
    cout << "]";
}

bool pacoteB::chaveEhMaiorQueTodos(char *chave)
{
    return strcmp(elementos[numElementos - 1].aluno, chave) < 0;
}

bool pacoteB::chaveEhMenorQueTodos(char *chave)
{
    return strcmp(elementos[0].aluno, chave) > 0;
}

int pacoteB::busca(char *chave)
{

    int i = 0;
    while (i < numElementos and strcmp(elementos[i].aluno, chave) <= 0)
        i++;

    return filhosSet[i];
}