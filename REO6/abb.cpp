#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <ctime>

using namespace std;

struct dado {
    unsigned chave;
    string nome;
    unsigned long long int cpf;
};

ostream& operator<<(ostream& saida, const dado& e) {
    saida << "Nome: " << e.nome << " | CPF: "  << e.cpf <<  " | Cod: " << e.chave ;
    return saida;
}

istream& operator>>(istream& entrada, dado& e) {
    entrada >> e.chave >> e.nome >> e.cpf;
    return entrada;
}

typedef unsigned tipoChave; // tipo da chave usada na comparação

class noh {
    friend class ABB;
    private:
        dado elemento;
        noh* esq;
        noh* dir;
        noh* pai;
    public:
        noh(const dado& umDado):
            elemento(umDado),  esq(NULL), dir(NULL), pai(NULL) { }
        ~noh() { }
};

class ABB {
    friend ostream& operator<<(ostream& output, ABB& arvore);
    private:
        noh* raiz;
        int cont;
        noh* insereRecursivo(noh* umNoh, const dado& umDado);
        // busca, método iterativo
        noh* buscaAux(tipoChave chave);
    public:
        ABB();
        ~ABB();
        void insere(const dado& umDado);
        void imprimir();
        // inserção e remoção são recursivos
        // inserção e remoção, métodos recursivos
        // busca retorna uma cópia do objeto armazenado
        dado busca(tipoChave chave);
};

ABB::ABB() {
  raiz = NULL;
  cont = 0;
}

ABB::~ABB(){
  delete raiz;
}

void ABB::insere(const dado& umDado) {
    raiz = insereRecursivo(raiz, umDado);
}

noh* ABB::insereRecursivo(noh* umNoh, const dado& umDado) {
    if(umNoh == NULL){
        noh* novoNoh = new noh(umDado);
        return novoNoh;
    }
    else if(umDado.chave < umNoh->elemento.chave){
        umNoh->esq = insereRecursivo(umNoh->esq, umDado);
        cont++;
        umNoh->esq->pai = umNoh;
    }else{
        umNoh->dir = insereRecursivo(umNoh->dir, umDado);
        cont++;
        umNoh->dir->pai = umNoh;
    }
    for(int i=0; i<cont; i++){
        cout << "noh acessado" << endl;
    }
    cont = 0;
    return umNoh;
}

noh* ABB::buscaAux(tipoChave chave){
    noh* atual = raiz;
    while(atual != NULL){
        if(atual->elemento.chave == chave){
            for(int i=0; i<cont; i++){
            cout << "noh acessado" << endl;
            }
            cont = 0;
            return atual;
        }
        else if(atual->elemento.chave > chave){
            cont++;
            atual = atual->esq;
        }else{
            cont++;
            atual = atual->dir;
        }
    }
    if(atual == NULL){
        for(int i=0; i<cont; i++){
            cout << "noh acessado" << endl;
        }
        cont = 0;
    }
    return atual;
}

dado ABB::busca(tipoChave chave){
    noh* resultado = buscaAux(chave);
    if(resultado == NULL){
        throw runtime_error("Erro na busca: elemento não encontrado!");
    }else{
        return resultado->elemento;
    }
}

int main() {
    ABB arvore;
    tipoChave chave;
    dado umDado;
    int dia = 0;
    clock_t t;

    char operacao;
    t = clock();

    do {
        try {
            cin >> operacao;
            switch (operacao) {
                case 'i': // Inserir recursivamente
                    // objeto recebe id, nome, quantidade, valor
                    cin >> umDado;
                    arvore.insere(umDado);
                    break;
                case 'b': // Buscar
                    cin >> chave; // ler a chave
                    umDado = arvore.busca(chave); // escrever o valor
                    cout << umDado << endl;
                    break;
                case 'f': // Finalizar execução
                    break;
                default:
                    cout << "Comando invalido!\n";
            }
        } catch (runtime_error& e) {
            cout << e.what() << endl;
        }
    } while (operacao != 'f');

    t = clock() - t;

    cout << "Tempo de execucao: " << (t)/((CLOCKS_PER_SEC/1000)) << endl;

    return 0;
}