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
int cont;

class noh {
    friend class avl;
    private:
        dado elemento;
        noh* esq;
        noh* dir;
        unsigned altura;
    public:
        noh(const dado& umDado):
            elemento(umDado),  esq(NULL), dir(NULL), altura(1) { }
        ~noh() { }
        int fatorBalanceamento();
};

int noh::fatorBalanceamento() {
    int hEsq;
    int hDir;

    // cont++;
    // cout << "Nohs acessados em fatorBalan: " << cont << endl;

    if(esq != NULL){
        hEsq = esq->altura;
    }else{
        hEsq = 0;
    }

    if(dir != NULL){
        hDir = dir->altura;
    }else{
        hDir = 0;
    }

    return hEsq - hDir;
}


class avl {
    friend ostream& operator<<(ostream& output, avl& arvore);
    private:
        noh* raiz;
        noh* insereAux(noh* umNoh, const dado& umDado);
        // métodos para manutenção do balanceamento
        noh* rotacaoEsquerda(noh* umNoh);
        noh* rotacaoDireita(noh* umNoh);
        noh* arrumaBalanceamento(noh* umNoh);
        void atualizaAltura(noh* umNoh);
        int informaAltura(noh* umNoh);
        // busca, método iterativo
        noh* buscaAux(tipoChave chave);
        // função auxiliar do destrutor, usa percorrimento pós-ordem
        void destruirRecursivamente(noh* umNoh);
    public:
        avl() { raiz = NULL; cont = 0; }
        ~avl();
        void imprimir();
        // inserção e remoção são recursivos
        void insere(const dado& umDado);
        // inserção e remoção, métodos recursivos
        // busca retorna uma cópia do objeto armazenado
        dado busca(tipoChave chave);
};

// destrutor
avl::~avl() {
    destruirRecursivamente(raiz);
}

// destrutor é recursivo, fazendo percorrimento pós-ordem
void avl::destruirRecursivamente(noh* umNoh) {
    if(umNoh != NULL){
        destruirRecursivamente(umNoh->esq);
        destruirRecursivamente(umNoh->dir);
        delete umNoh;
    }
}

void avl::insere(const dado& umDado) {
    raiz = insereAux(raiz, umDado);
}

// inserção recursiva, devolve nó para atribuição de pai ou raiz
noh* avl::insereAux(noh* umNoh, const dado& umDado) {
    if(umNoh == NULL){
        noh* novoNoh = new noh(umDado);
        return novoNoh;
    }
    else if(umDado.chave < umNoh->elemento.chave){
        umNoh->esq = insereAux(umNoh->esq, umDado);
        cont++;
    }else if(umDado.chave > umNoh->elemento.chave){
        umNoh->dir = insereAux(umNoh->dir, umDado);
        cont++;
    }
    for(int i=0; i<cont; i++){
        cout << "noh acessado" << endl;
    }
    cont = 0;
    return arrumaBalanceamento(umNoh);
}

// checa e arruma, se necessário, o balanceamento em umNoh,
// fazendo as rotações e ajustes necessários
noh* avl::arrumaBalanceamento(noh* umNoh) {
    if(umNoh == NULL){
        return umNoh;
    }

    atualizaAltura(umNoh);
    cont++;
    cout << "nohs acessados Balanceamento: "<< cont << endl;
    cont = 0;
    int fb = umNoh->fatorBalanceamento();
    
    if ((fb >= -1) and (fb <= 1)){ //-1<x<1 ~> nao precisa de balanceamento
        return umNoh;
    }
    if ((fb > 1) and (umNoh->esq->fatorBalanceamento() >= 0)){ //rotacao simples ~> mesmo sinal
        return rotacaoDireita(umNoh);                         //direita ~> positivo
    }
    if ((fb > 1) and (umNoh->esq->fatorBalanceamento() < 0)){ //rotacao dupla ~> sinais contrarios
        umNoh->esq = rotacaoEsquerda(umNoh->esq);                 //fb positivo~>esquerda-direita
        return rotacaoDireita(umNoh);
    }
    if ((fb < -1) and (umNoh->dir->fatorBalanceamento() <= 0)){//rotacao simples ~> mesmo sinal
        return rotacaoEsquerda(umNoh);                     //esquerda ~> negativo
    }
    else if ((fb < -1) and (umNoh->dir->fatorBalanceamento() > 0)){ //rotacao dupla ~> sinais contrarios
        umNoh->dir = rotacaoDireita(umNoh->dir);                //fb negativo ~>direita-esquerda
        return rotacaoEsquerda(umNoh);
    }
    return umNoh;
}

int avl::informaAltura(noh* umNoh){
    if(umNoh == NULL){
        return 0;
    }else{
        // cont++;
        // cout << "Nohs acessados em informaAltura: " << cont << endl;
        return umNoh->altura;
    }
}

void avl::atualizaAltura(noh* umNoh){
    int hEsq = informaAltura(umNoh->esq);
    int hDir = informaAltura(umNoh->dir);

    cont = 0;
    umNoh->altura = 1 + max(hEsq, hDir);
}

// rotação à esquerda na subárvore com raiz em umNoh
// retorna o novo pai da subárvore
noh* avl::rotacaoEsquerda(noh* umNoh) {
    noh* aux = umNoh->dir;
    umNoh->dir = aux->esq;
    
    aux->esq = umNoh;
    cont++;
    cout << "rotacionouPraEsq: " << cont << endl;

    atualizaAltura(umNoh);
    atualizaAltura(aux);
    
    return aux;
}


// rotação à direita na subárvore com raiz em umNoh
// retorna o novo pai da subárvore
noh* avl::rotacaoDireita(noh* umNoh) {
    noh* aux = umNoh->esq;
    umNoh->esq = aux->dir;
    
    aux->dir = umNoh;
    cont++;
    cout << "rotacionouPraDir: " << cont << endl;
    
    atualizaAltura(umNoh);
    atualizaAltura(aux);
    
    return aux;
}


// método de busca auxiliar (retorna o nó), iterativo
noh* avl::buscaAux(tipoChave chave) {
    noh* atual = raiz;
	while(atual != NULL){
        if(atual->elemento.chave == chave){
            for(int i=0; i<cont; i++){
            cout << "noh acessado" << endl;
            }
            cont = 0;
            return atual;
        }else if(atual->elemento.chave > chave){
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

// busca elemento com uma dada chave na árvore e retorna o registro completo
dado avl::busca(tipoChave chave) {
    noh* resultado = buscaAux(chave);
    if (resultado != NULL){
        return resultado->elemento;
    }else
        throw runtime_error("Erro na busca: elemento não encontrado!");
}

int main() {
    avl arvore;
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