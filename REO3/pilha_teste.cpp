#include <iostream>
using namespace std;

/*struct Pilha{
	int topo;
	int valores[10];
}; */

class pilha{
	private:
		int topo;
		int valores[10];
	public:
		pilha();
		void push(int elemento);
		int pop();
		bool isEmpty();
		bool isFull();
};
	
pilha::pilha(){
	topo = -1;
}

void pilha::push(int elemento){
	topo++;
	valores[topo] = elemento;
}

int pilha::pop(){
    int elem = valores[topo];
    topo--;
    return elem;
}

bool pilha::isEmpty(){
    return (topo == -1);
}

bool pilha::isFull(){
    return (topo == 9);
}

int main(){
	int valor, resto;
	pilha *p = new pilha();
	
   
	cout << "Digite um valor: " << endl;
	cin >> valor;
   
	while (valor != 0){
       resto = valor % 2;
       p->push(resto);
       valor = valor / 2;
	}
   
	while (!p->isEmpty()){
       resto = p->pop();
       cout << resto << endl;
	}

	return 0;
}
