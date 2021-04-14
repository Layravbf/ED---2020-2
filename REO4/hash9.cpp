#include <iostream>
using namespace std;

int funcaoHash(int chave)
{
	return chave % 101;
}

struct InfoHash
{
	int chave;
	int valor;
};

class Hash
{
public:
	Hash(int capacidade = 101);
	~Hash();
	void Inserir(int chave);
	void Remover(int chave);
	bool Verificar(int verificaDado);
	void Imprimir();

private:
	int GetTamanho();
	int Buscar(int chave);
	InfoHash *tabela;
	InfoHash dado;
	int _capacidade;
	int tamanho;
};

Hash::Hash(int capacidade)
{
	_capacidade = capacidade;
	tabela = new InfoHash[_capacidade];
	for (int i = 0; i < _capacidade; i++)
	{
		tabela[i].chave = -1;
		tabela[i].valor = -1;
	}
	tamanho = 0;
}

Hash::~Hash()
{
    delete[] tabela;
}

int Hash::Buscar(int chave)
{
	int pos = funcaoHash(chave);
	int posFinal = pos;

	do
	{
		dado = tabela[pos];
		if (dado.valor == -1)
			return -1;
		if (dado.chave == chave)
		{
			return pos;
		}
		pos = (pos + 1) % _capacidade;
	} while (pos != posFinal);

	return -1;
}

void Hash::Inserir(int chave)
{
	int pos = funcaoHash(chave);
	if (tamanho < _capacidade and Buscar(chave) == -1)
	{
		while (tabela[pos].valor != -1)
		{
			pos = (pos + 1) % _capacidade;
		}
		tabela[pos].chave = chave;
		tabela[pos].valor = 1;
		tamanho++;
	}
}

void Hash::Remover(int chave)
{
	int pos = Buscar(chave);
	if (tamanho != 0 and pos != -1)
	{
		tabela[pos].chave = -1;
		tabela[pos].valor = -1;
		tamanho--;
	}
}

bool Hash::Verificar(int verificaDado)
{
	if (verificaDado == Buscar(verificaDado))
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Hash::GetTamanho()
{
	return tamanho;
}

void Hash::Imprimir()
{
	if (tamanho == 0)
	{
		cout << "{}" << endl;
	}
	else
	{
		for (int i = 0; i < _capacidade; i++)
		{
			if (tabela[i].valor == 1)
			{
				cout << tabela[i].chave << " ";
			}
		}
		cout << endl;
	}
}

int main()
{
	Hash tabela;

	int valor;
	for (int i = 0; i < 10; i++)
	{
		cin >> valor;
		tabela.Inserir(valor);
	}

	int removidos;
	for (int i = 0; i < 3; i++)
	{
		cin >> removidos;
		tabela.Remover(removidos);
	}

	bool membro;
	int verificaDado;
	for (int i = 0; i < 2; i++)
	{
		cin >> verificaDado;
		membro = tabela.Verificar(verificaDado);
		if (membro == true)
		{
			cout << "1" << endl;
		}
		else
		{
			cout << "-1" << endl;
		}
	}

	tabela.Imprimir();

	int insere3;
	for (int i = 0; i < 3; i++)
	{
		cin >> insere3;
		tabela.Inserir(insere3);
	}

	tabela.Imprimir();

	return 0;
}
