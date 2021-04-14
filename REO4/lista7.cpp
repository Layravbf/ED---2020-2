#include <iostream>
using namespace std;

class Noh
{
    friend class ListaDuploEncadeada;

private:
    Noh *proximo;
    Noh *anterior;
    int dado;

public:
    Noh(int _dado);
};
Noh::Noh(int _dado = 0)
{
    dado = _dado;
    proximo = NULL;
    anterior = NULL;
}

class ListaDuploEncadeada
{
private:
    int tamanho;
    Noh *primeiro;
    Noh *ultimo;

public:
    ListaDuploEncadeada();
    ~ListaDuploEncadeada();
    void InsereOPrimeiro(int _dado);
    void InsereNoInicio(int _dado);
    void InsereNoFim(int _dado);
    void InsereNaPosicao(int _dado, int _pos);
    void RemoveRepetidos();
    void Imprime();
    void ImpressaoReversa();
};

ListaDuploEncadeada::ListaDuploEncadeada()
{
    primeiro = NULL;
    ultimo = NULL;
    tamanho = 0;
}

ListaDuploEncadeada::~ListaDuploEncadeada()
{
    Noh *atual = primeiro;
    Noh *temp = NULL;

    while (atual->proximo != NULL)
    {
        temp = atual;
        atual = atual->proximo;
        delete temp;
    }
}

void ListaDuploEncadeada::InsereOPrimeiro(int _dado)
{
    Noh *novoNoh = new Noh(_dado);
    primeiro = novoNoh;
    ultimo = novoNoh;
    tamanho++;
}

void ListaDuploEncadeada::InsereNoInicio(int _dado)
{
    if (primeiro == NULL)
    {
        InsereOPrimeiro(_dado);
    }
    else
    {
        Noh *novoNoh = new Noh(_dado);
        novoNoh->proximo = primeiro;
        primeiro->anterior = novoNoh;
        primeiro = novoNoh;
        tamanho++;
    }
}

void ListaDuploEncadeada::InsereNoFim(int _dado)
{
    if (primeiro == NULL)
    {
        InsereOPrimeiro(_dado);
    }
    else
    {
        Noh *novoNoh = new Noh(_dado);
        ultimo->proximo = novoNoh;
        novoNoh->anterior = ultimo;
        ultimo = novoNoh;
        tamanho++;
    }
}

void ListaDuploEncadeada::InsereNaPosicao(int _dado, int _pos)
{
    if (primeiro == NULL)
    {
        InsereOPrimeiro(_dado);
    }
    else if (_pos == 0)
    {
        InsereNoInicio(_dado);
    }
    else if (_pos == tamanho)
    {
        InsereNoFim(_dado);
    }
    else
    {
        Noh *aux = primeiro;
        int i = 0;

        while (i < _pos)
        {
            aux = aux->proximo;
            i++;
        }
        Noh *novoNoh = new Noh(_dado);
        Noh *auxAnterior = aux->anterior;
        auxAnterior->proximo = novoNoh;
        novoNoh->anterior = aux->anterior;
        novoNoh->proximo = aux;
        aux->anterior = novoNoh;
        tamanho++;
    }
}

void ListaDuploEncadeada::RemoveRepetidos()
{
    Noh *compara = primeiro;
    Noh *aux, *aux2, *aux3;

    while (compara->proximo != NULL)
    {
        aux = compara->proximo;
        if (aux->proximo == NULL)
        {
            if (compara->dado == aux->dado)
            {
                aux2 = aux->anterior;
                aux2->proximo = NULL;
                ultimo = aux2;
                tamanho--;
            }
        }
        while (aux->proximo != NULL)
        {
            if (compara->dado == aux->dado)
            {
                aux2 = aux->anterior;
                aux3 = aux->proximo;
                aux2->proximo = aux3;
                aux3->anterior = aux2;
                tamanho--;
                aux = aux2;
            }
            aux = aux->proximo;
            if (aux->proximo == NULL)
            {
                if (compara->dado == aux->dado)
                {
                    aux2 = aux->anterior;
                    aux2->proximo = NULL;
                    ultimo = aux2;
                    tamanho--;
                }
            }
        }
        if (compara->proximo == NULL)
        {
            break;
        }
        else
        {
            compara = compara->proximo;
        }
    }
}

void ListaDuploEncadeada::Imprime()
{
    Noh *nohAtual = primeiro;

    while (nohAtual != NULL)
    {
        cout << nohAtual->dado << " ";
        nohAtual = nohAtual->proximo;
    }
    cout << endl;
}

void ListaDuploEncadeada::ImpressaoReversa()
{
    Noh *nohAtual = ultimo;

    while (nohAtual != NULL)
    {
        cout << nohAtual->dado << " ";
        nohAtual = nohAtual->anterior;
    }
    cout << endl;
}

int main()
{
    ListaDuploEncadeada lista;
    int tam;
    int valor;

    cin >> tam;

    for (int i = 0; i < tam; i++)
    {
        cin >> valor;
        lista.InsereNaPosicao(valor, i);
    }

    lista.RemoveRepetidos();
    lista.Imprime();
    lista.ImpressaoReversa();

    return 0;
}
