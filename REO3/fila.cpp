#include <iostream>

using namespace std;

class noh{
	friend class Fila;
	private:
		int dado;
		noh(int valor);
		noh* proximo;		
	
};

noh::noh(int valor){
	dado = valor;
	proximo = NULL;
}

class Fila{
	private:
		noh* inicio;
		noh* final;
		int tamanho;
		
	public:
		void insere(int valor);
		void print();
		int desenfilera();
		int espia();
		bool vazia();
		Fila();
};


Fila::Fila(){
	inicio = NULL;
	final = NULL;
	tamanho = 0;
}

void Fila::insere(int valor){
	noh* novo = new noh(valor);
	
	if(tamanho == 0){
		inicio = novo;
		final = novo;
	}
	else{
		final->proximo = novo;
		final = novo;
	}
	tamanho++;
}

int Fila::desenfilera(){
	
	noh* aux = inicio;
	int valor = aux->dado;
	
	if(inicio == NULL)
		return 0;
	else{
		inicio = inicio->proximo;
		tamanho--;
		delete aux;
		return valor;
	}
	
}

int Fila::espia(){
	return inicio->dado;
}

bool Fila::vazia(){
	if(inicio == NULL)
		return true;
	return false;
}

void Fila::print(){
	noh* aux = inicio;
	for (int i = 0; i < tamanho; i++){
		cout << aux->dado << " ";
		aux = aux->proximo;
	}
}

int main(){
	
	Fila fila1, fila2, fila3;
	int valor, qnt1, qnt2;
	
	cin >> qnt1;
	
	for (int i = 0; i < qnt1; i++){
		cin >> valor;
		fila1.insere(valor);
	}
	
	cin >> qnt2;
	
	for (int i = 0; i < qnt2; i++){
		cin >> valor;
		fila2.insere(valor);
	}
	
	
	bool final = true;
	while(final){
		if(final){
			do{
				if(fila1.espia() > fila2.espia()){
					fila3.insere(fila2.desenfilera());
					fila3.insere(fila1.desenfilera());
				}
				else{
					fila3.insere(fila1.desenfilera());
					fila3.insere(fila2.desenfilera());
				}
			}while(!fila1.vazia() or !fila2.vazia());
			final = false;
		}
	}
	/*
	if(!fila1.vazia()){
		for (int i = 0; i < count1; i++)
			fila3.insere(fila1.desenfilera());
	}
	else if(!fila2.vazia()){
		for (int i = 0; i < count2; i++)
			fila3.insere(fila2.desenfilera());
	}*/
		
	fila3.print();
	
	return 0;
}



/*
7
1 3 5 7 10 10 11
6
2 4 6 8 10 12
*/
