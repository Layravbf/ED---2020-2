#include <iostream>
using namespace std;

class Noh
{
    friend class Filas;

private:
    int dado;
    Noh *proximo;

public:
    Noh(int valor)
    {
        dado = valor;
        proximo = NULL;
    }
};

class Filas
{
private:
    Noh *inicio;
    Noh *fim;
    int tamanho;

public:
    Filas();
    ~Filas();
    void enfileira(int valor);
    int desenfileira();
    int espia();
    bool estaVazia();
};

Filas::Filas()
{
    inicio = NULL;
    fim = NULL;
    tamanho = 0;
}

Filas::~Filas()
{
    while (!estaVazia())
    {
        desenfileira();
    }
}

void Filas::enfileira(int valor)
{
    Noh *novo = new Noh(valor);
    if (tamanho == 0)
    {
        inicio = novo;
        fim = novo;
    }
    else
    {
        fim->proximo = novo;
        fim = novo;
    }
    tamanho++;
}

int Filas::desenfileira()
{
    Noh *aux;
    int removido = inicio->dado;
    aux = inicio;
    inicio = inicio->proximo;
    if (tamanho == 0)
    {
        fim = NULL;
    }
    delete aux;
    tamanho--;
    return removido;
}

int Filas::espia()
{
    return inicio->dado;
}

bool Filas::estaVazia()
{
    if (tamanho == 0)
    {
        return true;
    }
    return false;
}

int main()
{
    Filas fila1, fila2, fila3, aux;
    int tamanho1, tamanho2;
    int valor;

    cin >> tamanho1;
    for (int i = 0; i < tamanho1; i++)
    {
        cin >> valor;
        fila1.enfileira(valor);
    }

    cin >> tamanho2;
    for (int i = 0; i < tamanho2; i++)
    {
        cin >> valor;
        fila2.enfileira(valor);
    }

    while (!fila1.estaVazia() or !fila2.estaVazia())
    {
        if (!fila1.estaVazia())
        {
            if (!fila2.estaVazia())
            {
                if (fila1.espia() > fila2.espia())
                {
                    aux.enfileira(fila2.desenfileira());
                }
                else
                {
                    aux.enfileira(fila1.desenfileira());
                }
            }
            else
            {
                aux.enfileira(fila1.desenfileira());
            }
        }
        else
        {
            aux.enfileira(fila2.desenfileira());
        }

        while (!fila1.estaVazia() and fila1.espia() == aux.espia())
        {
            fila1.desenfileira();
        }

        while (!fila2.estaVazia() and fila2.espia() == aux.espia())
        {
            fila2.desenfileira();
        }

        fila3.enfileira(aux.desenfileira());
    }

    while (!fila3.estaVazia())
    {
        cout << fila3.desenfileira() << " ";
    }

    return 0;
}
