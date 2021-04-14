#include <iostream>
#include <cstdlib>
#include <algorithm>
using namespace std;

class maxHeap
{
private:
	int *dados;
	int tamanho;
	int capacidade;
	int pai(int filho);
	int esquerda(int getPai);
	int direita(int getPai);
	void corrigeDescendo(int pos);
	void corrigeSubindo(int pos);
	void heapify();

public:
	maxHeap(int cap);
	maxHeap(int vet[], int tam);
	~maxHeap();
	void insere(int valor);
	int retiraRaiz();
	void imprime();
	void heapSort();
};

maxHeap::maxHeap(int cap)
{
	capacidade = cap;
	tamanho = 0;
	dados = new int[capacidade];
}

maxHeap::maxHeap(int vet[], int tam)
{
	capacidade = tam + 5;
	tamanho = tam;
	dados = new int[capacidade];

	copy(vet, vet + tam, dados);
	heapify();
}

maxHeap::~maxHeap()
{
	delete[] dados;
}

int maxHeap::pai(int filho)
{
	return (filho - 1) / 2;
}

int maxHeap::esquerda(int pai)
{
	return (2 * pai) + 1;
}

int maxHeap::direita(int pai)
{
	return (2 * pai) + 2;
}

void maxHeap::insere(int valor)
{
	if (tamanho < capacidade)
	{
		dados[tamanho] = valor;
		corrigeSubindo(tamanho);
		tamanho++;
	}
}

void maxHeap::corrigeSubindo(int pos)
{
	int nohPai = pai(pos);

	if (pos != 0)
	{
		if (dados[nohPai] < dados[pos])
		{
			swap(dados[nohPai], dados[pos]);
			corrigeSubindo(nohPai);
		}
	}
}

void maxHeap::corrigeDescendo(int pos)
{
	int filhoEsquerda = esquerda(pos);
	int filhoDireita = direita(pos);
	int maior = pos;

	if (filhoEsquerda < tamanho and dados[filhoEsquerda] > dados[maior])
	{
		maior = filhoEsquerda;
	}
	if (filhoDireita < tamanho and dados[filhoDireita] > dados[maior])
	{
		maior = filhoDireita;
	}
	if (pos != maior)
	{
		swap(dados[maior], dados[pos]);
		corrigeDescendo(maior);
	}
}

int maxHeap::retiraRaiz()
{
	int aux = dados[0];
	swap(dados[0], dados[tamanho - 1]);
	tamanho--;
	if (tamanho > 1)
	{
		corrigeDescendo(0);
	}
	return aux;
}

void maxHeap::heapify()
{
	for (int i = (capacidade / 2); i >= 0; i--)
	{
		corrigeDescendo(i);
	}
}

void maxHeap::heapSort()
{
	while (tamanho > 0)
	{
		cout << retiraRaiz() << " ";
	}
	cout << endl;
}

void maxHeap::imprime()
{
	for (int i = 0; i < tamanho; i++)
	{
		cout << dados[i] << " ";
	}
	cout << endl;
}

int main()
{
	int valor;
	int vetor[15];

	for (int i = 0; i < 15; i++)
	{
		cin >> valor;
		vetor[i] = valor;
	}

	maxHeap *heap = new maxHeap(vetor, 15);
	heap->imprime();
	cout << endl;

	for (int i = 0; i < 5; i++)
	{
		cin >> valor;
		heap->insere(valor);
	}

	heap->imprime();
	cout << endl;
	heap->heapSort();
	heap->imprime();

	delete (heap);

	return 0;
}
