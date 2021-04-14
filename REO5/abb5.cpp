#include <iostream>
using namespace std;

class Noh {
friend class ABB;
public:
  Noh(int dado);
private:
  Noh* esquerda;
  Noh* direita;
  Noh* pai;
  int valor;
};

Noh::Noh(int dado) {
  valor = dado;
  direita = NULL;
  esquerda = NULL;
  pai = NULL;
}

class ABB {
public:
  ABB();
  ~ABB();
  void Inserir(int _dado);
  void Imprime();
private:
  int ContarNohsAux(Noh *_noh);
  Noh* raiz;
  int nohs;
};

ABB::ABB() {
  raiz = NULL;
  nohs = 0;
}

ABB::~ABB(){
  delete raiz;
}

void ABB::Inserir(int _dado) {
  Noh* novoNoh = new Noh(_dado);

  if (raiz == NULL) {
    raiz = novoNoh;
  }else {
    Noh* percorredor = raiz;
    bool inserido = false;

    while (!inserido) {
      if (_dado < percorredor->valor) {
        if (percorredor->esquerda == NULL) {
          percorredor->esquerda = novoNoh;
          novoNoh->pai = percorredor;
          inserido = true;
        }else {
          percorredor = percorredor->esquerda;
        }
      }else {
        if (percorredor->direita == NULL) {
          percorredor->direita = novoNoh;
          novoNoh->pai = percorredor;
          inserido = true;
        }else {
          percorredor = percorredor->direita;
        }
      }
    }
  }
}

int ABB::ContarNohsAux(Noh* _noh) {
  if (_noh == NULL) {
    return 0;
  }else{
    return 1 + ContarNohsAux(_noh->esquerda) + ContarNohsAux(_noh->direita);
  }
}


void ABB::Imprime() {
  cout << ContarNohsAux(raiz) << endl;
  cout << ContarNohsAux(raiz->esquerda) - ContarNohsAux(raiz->direita) << endl;
}

int main () {
  ABB arvore;
  int chave;
	
  cin >> chave;
  do{
    arvore.Inserir(chave);
    cin >> chave;
  }while(chave != -1);
	
	
  arvore.Imprime();

  return 0;
}

