/* avl - Árvore Binária de Busca com Balanceamento AVL
 *
 * by Joukim, Outubro de 2019 - Estrutura de Dados (GCC216)
 * Caracteristicas de implementação outubro de 2019:
 * -> tratamento de exceção
 * -> usa dado na forma chave/valor
 * -> uso de métodos recursivos
 * -> sem duplo encadeamento
 * -> invés de transplanta, usa removeMenor
 */

#include <iostream>
#include <stdexcept>
#include <algorithm>

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
        // percorrimento em ordem da árvore
        void percorreEmOrdemAux(noh* atual, int nivel);
        void imprimeAutorizadosAux(noh* atual, int impar);
        // funções auxiliares para remoção
        noh* encontraMenor(noh* raizSub);
        noh* removeMenor(noh* raizSub);
        // funções auxiliares para inserção e remoção usando método recursivo
        // retorna o nó para ajustar o pai ou o raiz
        noh* insereAux(noh* umNoh, const dado& umDado);
        noh* removeAux(noh* umNoh, tipoChave chave);
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
        void imprimirDir(const std::string& prefixo, const noh* node);
        void imprimirEsq(const std::string& prefixo, const noh* node, bool temIrmao);
    public:
        avl() { raiz = NULL; }
        ~avl();
        void imprimir();
        // inserção e remoção são recursivos
        void insere(const dado& umDado);
        void remove(tipoChave chave);
        // inserção e remoção, métodos recursivos
        // busca retorna uma cópia do objeto armazenado
        dado busca(tipoChave chave);
        void imprimeAutorizados(int dia);
        bool verificaAutorizado(tipoChave chave, int dia);
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
    }else if(umDado.chave < umNoh->elemento.chave){
        umNoh->esq = insereAux(umNoh->esq, umDado);
    }else if(umDado.chave > umNoh->elemento.chave){
        umNoh->dir = insereAux(umNoh->dir, umDado);
    }
    return arrumaBalanceamento(umNoh);
}

// checa e arruma, se necessário, o balanceamento em umNoh,
// fazendo as rotações e ajustes necessários
noh* avl::arrumaBalanceamento(noh* umNoh) {
    if(umNoh == NULL){
        return umNoh;
    }

    atualizaAltura(umNoh);
    int fb = umNoh->fatorBalanceamento();
    
    if ((fb >= -1) and (fb <= 1)){ //-1<x<1 ~> nao precisa de balanceamento
        return umNoh;
    }
    if ((fb > 1) and (umNoh->esq->fatorBalanceamento() >= 0)){ //rotacao simples ~> mesmo sinal
        return rotacaoDireita(umNoh);                         //direita ~> positivo
    }
    if ((fb > 1) and (umNoh->esq->fatorBalanceamento() < 0)){ //rotacao dupla ~> sinais contrarios
        umNoh->esq = rotacaoEsquerda(umNoh->esq);                  //fb positivo~>esquerda-direita
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
        return umNoh->altura;
    }
}

void avl::atualizaAltura(noh* umNoh){
    int hEsq = informaAltura(umNoh->esq);
    int hDir = informaAltura(umNoh->dir);

    umNoh->altura = 1 + max(hEsq, hDir);
}

// rotação à esquerda na subárvore com raiz em umNoh
// retorna o novo pai da subárvore
noh* avl::rotacaoEsquerda(noh* umNoh) {
    noh* aux = umNoh->dir;
    umNoh->dir = aux->esq;
    
    aux->esq = umNoh;
    
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
    
    atualizaAltura(umNoh);
    atualizaAltura(aux);
    
    return aux;
}


// método de busca auxiliar (retorna o nó), iterativo
noh* avl::buscaAux(tipoChave chave) {
    noh* atual = raiz;
	while(atual != NULL){
        if(atual->elemento.chave == chave){
            return atual;
        }else if(atual->elemento.chave > chave){
            atual = atual->esq;
        }else{
            atual = atual->dir;
        }
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

// nó mínimo (sucessor) de subárvore com raiz em raizSub (folha mais à esquerda)
noh* avl::encontraMenor(noh* raizSub) {
    if (raizSub->esq != NULL){
        raizSub = encontraMenor(raizSub->esq);
    }
    return raizSub;
}

// procedimento auxiliar para remover o sucessor substituíndo-o pelo
// seu filho à direita
noh* avl::removeMenor(noh* raizSub) {
    if(raizSub->esq == NULL){
        return raizSub->dir;
    }else{
        raizSub->esq = removeMenor(raizSub->esq);
        return arrumaBalanceamento(raizSub);
    }
}

// remoção recursiva
void avl::remove(tipoChave chave) {
    raiz = removeAux(raiz, chave);
}

noh* avl::removeAux(noh* umNoh, tipoChave chave) {
    if(umNoh == NULL){
        throw runtime_error("Erro na busca: elemento não encontrado!");
    }

    noh* novaRaiz;
    novaRaiz = umNoh;

    if(chave < (umNoh->elemento.chave)){
        umNoh->esq = removeAux(umNoh->esq, chave);
    }else if (chave > (umNoh->elemento.chave)){
        umNoh->dir = removeAux(umNoh->dir, chave);
    }else if(umNoh->esq == NULL){
        novaRaiz = umNoh->dir;
    }else if(umNoh->dir == NULL){
        novaRaiz = umNoh->esq;
    }else {
        novaRaiz = encontraMenor(umNoh->dir);
        novaRaiz->dir = removeMenor(umNoh->dir);
        novaRaiz->esq = umNoh->esq;
        delete(umNoh);
    }
    return arrumaBalanceamento(novaRaiz);
}

// utiliza o nó atual e seu nível na árvore (para facilitar visualização)
void avl::imprimeAutorizadosAux(noh* atual, int impar) {
    if(atual != NULL){
        if(impar == 0){
            if(atual->elemento.cpf % 2 == 0){
                imprimeAutorizadosAux(atual->esq, impar);
                cout << "Nome: " << atual->elemento.nome << " | CPF: " << atual->elemento.cpf << " | Cod: " << atual->elemento.chave << endl;
                imprimeAutorizadosAux(atual->dir, impar);
            }
        }else if(impar != 0){
            if (atual->elemento.cpf % 2 != 0){
                imprimeAutorizadosAux(atual->esq, impar);
                cout << "Nome: " << atual->elemento.nome << " | CPF: " << atual->elemento.cpf << " | Cod: " << atual->elemento.chave << endl;
                imprimeAutorizadosAux(atual->dir, impar);
            }
            imprimeAutorizadosAux(atual->esq, impar);
            imprimeAutorizadosAux(atual->dir, impar);
        }
    }
}

// utiliza o nó atual e seu nível na árvore (para facilitar visualização)
void avl::imprimeAutorizados(int dia) {
    imprimeAutorizadosAux(this->raiz, dia % 2);
}

// utiliza o nó atual e seu nível na árvore (para facilitar visualização)
bool avl::verificaAutorizado(tipoChave chave, int dia) {
    noh* aux = buscaAux(chave);
    if (aux == NULL){
        throw runtime_error("Erro na verificacao: chave inexistente!");
    }else{
        if(chave % 2 == 0 and dia % 2 == 0){
            return true;
        }else if(chave % 2 != 0 and dia % 2 != 0){
            return true;
        } 
    }
    return false;
}

// utiliza o nó atual e seu nível na árvore (para facilitar visualização)
void avl::percorreEmOrdemAux(noh* atual, int nivel) {
    if(atual != NULL){
        percorreEmOrdemAux(atual->esq, nivel+1);
        
        percorreEmOrdemAux(atual->dir, nivel+1);
    }
}

ostream& operator<<(ostream& output, avl& arvore) {
    // arvore.percorreEmOrdemAux(arvore.raiz,0);
    arvore.imprimir();
    return output;
}

// imprime formatado seguindo o padrao tree as subarvores direitas de uma avl
void avl::imprimirDir(const std::string& prefixo, const noh* node)
{
    if( node != nullptr )
    {
        std::cout << prefixo
                  << "└d─"
                  << "(" << node->elemento.chave << "," << node->elemento.nome << ")"
                  << std::endl;

        // Repassa o prefixo para manter o historico de como deve ser a formatacao e chama no filho direito e esquerdo
        imprimirEsq( prefixo + "    " , node->esq , node->dir==nullptr );
        imprimirDir( prefixo + "    " , node->dir );
    }
}

// imprime formatado seguindo o padrao tree as subarvores direitas de uma avl
void avl::imprimirEsq(const std::string& prefixo, const noh* node, bool temIrmao)
{
    if( node != nullptr )
    {
        std::cout << prefixo ;

        // A impressao da arvore esquerda depende da indicacao se existe o irmao a direita
        if (temIrmao)
            std::cout << "└e─" ;
        else
            std::cout << "├e─";

        std::cout << "(" << node->elemento.chave << "," << node->elemento.nome << ")"
                  << std::endl;

        // Repassa o prefixo para manter o historico de como deve ser a formatacao e chama no filho direito e esquerdo
        imprimirEsq( prefixo + "│   " , node->esq, node->dir==nullptr );
        imprimirDir( prefixo + "│   " , node->dir );
    }
}

// imprime formatado seguindo o padrao tree uma avl
void avl::imprimir()
{
    if( this->raiz != nullptr )
    {
        std::cout << "(" << this->raiz->elemento.chave << "," << this->raiz->elemento.nome << ")" << std::endl;
        // apos imprimir a raiz, chama os respectivos metodos de impressao nas subarvore esquerda e direita
        // a chamada para a impressao da subarvore esquerda depende da existencia da subarvore direita
        imprimirEsq( " " , this->raiz->esq, this->raiz->dir==nullptr );
        imprimirDir( " " , this->raiz->dir );
    } else
        std::cout << "*arvore vazia*" << std::endl;
}

int main() {
    avl arvore;
    tipoChave chave;
    dado umDado;
    int dia = 0;

    char operacao;

    do {
        try {
            cin >> operacao;
            switch (operacao) {
                case 'i': // Inserir recursivamente
                    // objeto recebe id, nome, quantidade, valor
                    cin >> umDado;
                    arvore.insere(umDado);
                    break;
                case 'r': // Remover recursivamente
                    cin >> chave;
                    arvore.remove(chave);
                    break;
                case 'b': // Buscar
                    cin >> chave; // ler a chave
                    umDado = arvore.busca(chave); // escrever o valor
                    cout << umDado << endl;
                    break;
                case 'e': // Escrever arvore no formato tree
                    cout << arvore ;
                    break;
                case 'l': // lista os autorizados 
                    cin >> dia;
                    arvore.imprimeAutorizados(dia);
                    break;     
                case 'v': // Verifica se funcionario esta autorizado
                    cin >> chave >> dia;
                    if ( arvore.verificaAutorizado(chave, dia) )
                        cout << "Autorizado." << endl;
                    else
                        cout << "Não autorizado." << endl;
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

    return 0;
}