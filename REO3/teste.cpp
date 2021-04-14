#include <iostream>
using namespace std;

class MaxHeap {
public:
    MaxHeap(int _capacidade);
    MaxHeap(int _outroVetor[], int _tamanho);
    ~MaxHeap();
    void Inserir(int _valor);
    int RetiraRaiz();
    void HeapSort();
    void Imprimir();
private:
    int GetPai(int _filho);
    int GetFilhoEsquerda(int _pai);
    int GetFilhoDireita(int _pai);
    void CorrigeSubindo(int _pos);
    void CorrigeDescendo(int _pos);
    int capacidade;
    int tamanho;
    int* vetor;
};

MaxHeap::MaxHeap(int _capacidade) {
    capacidade = _capacidade;
    tamanho = 0;
    vetor = new int[capacidade];
}

MaxHeap::MaxHeap(int _outroVetor[], int _tamanho) {
    capacidade = _tamanho + 5;
    tamanho = _tamanho;
    vetor = new int[capacidade];

    for(int i = 0; i < capacidade; i++) {
        vetor[i] = _outroVetor[i];
    }

    for(int i = capacidade / 2; i >= 0; i--) {
        CorrigeDescendo(i);
    }
}

MaxHeap::~MaxHeap() {
    delete [] vetor;
}

int MaxHeap::GetPai(int _filho) {
    return (_filho - 1) / 2;
}

int MaxHeap::GetFilhoEsquerda(int _pai) {
    return 2 * _pai + 1;
}

int MaxHeap::GetFilhoDireita(int _pai) {
    return 2 * _pai + 2;
}

void MaxHeap::Inserir(int _valor) {

    if (tamanho == capacidade) {
        cerr << "Vetor cheio" << endl;
    } else {
        vetor[tamanho] = _valor;
        CorrigeSubindo(tamanho);
        tamanho++;
    }
}

void MaxHeap::CorrigeSubindo(int _pos) {
    int pai = GetPai(_pos);

    if (_pos != 0) {
        if (vetor[pai] < vetor[_pos]) {
            swap(vetor[pai], vetor[_pos]);
            CorrigeSubindo(pai);
        }
    }
}

int MaxHeap::RetiraRaiz() {
    int retorno = vetor[0];
    vetor[0] = vetor[tamanho -1];

    if (tamanho > 1) {
        CorrigeDescendo(0);
    }
    tamanho--;
    return retorno;
}

void MaxHeap::CorrigeDescendo(int _pos) {
    int filhoEsquerda = GetFilhoEsquerda(_pos);
    int filhoDireita = GetFilhoDireita(_pos);
    int maior = _pos;

    if (filhoEsquerda < tamanho and vetor[filhoEsquerda] > vetor[maior]) {
        maior = filhoEsquerda;
    }
    if (filhoDireita < tamanho and vetor[filhoDireita] > vetor[maior]) {
        maior = filhoDireita;
    }
    if (_pos != maior) {
        swap(vetor[maior], vetor[_pos]);
        CorrigeDescendo(maior);
    }
}

void MaxHeap::HeapSort() {
    while (tamanho > 0) {
        cout << RetiraRaiz() << " ";
    }
    cout << endl;
}

void MaxHeap::Imprimir() {
    for (int i = 0; i < tamanho; i++) {
        cout << vetor[i] << " ";
    }
    cout << endl;
}

int main() {
    int valor;
    int vetor[15];
    
    for(int i = 0; i < 15; i++) {
        cin >> valor;
        vetor[i] = valor;
    }

    MaxHeap heap(vetor, 15);
    heap.Imprimir();

    for(int i = 0; i < 5; i++) {
        cin >> valor;
        heap.Inserir(valor);
    }
    heap.Imprimir();

    heap.HeapSort();

    return 0;
}
