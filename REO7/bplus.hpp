/* 
  Trabalho de Estrutura de Dados
  Implementação de Árvore B+ a partir de código de Sequence Set
  Arthur Bonani de Oliveira Paiva
  Glaucio Martins Lima 
  Layra Vilas Boas Ferreira
     Arquivo com a classe da arvore B+
*/
#include <iostream>
#include <fstream>
#include <string.h>
#include "pacoteB.hpp"
using namespace std;

struct cabecalhoB
{
	unsigned capacidadeMaxPacote;
	unsigned capacidadeMinPacote;
	unsigned posicaoMeio;
	unsigned numNohs;
	int proxPosVazia;
	int raiz;
};
class Bplus
{
private:
	unsigned numNohs;
	int proxPosVazia;
	int raiz;
	string NomeArquivo;

	pacoteB *dividepacoteB(pacoteB *bloco, dadoB &promovido, bool folha);
	void insereEmIntermediarioNaoCheio(pacoteB *bloco, int novo, dadoB promovido);
	void imprimeAux(int posBloco);
	int buscaAux(int bloco, char *chave);
	int insereRecAux(int bloco, dadoB umDado, dadoB &promovido);

	// Funções para lidar com arquivos;

	bool cabecalhoEhConsistente(const cabecalhoB &cabecalhoArq);
	void atualizaCabecalhoArquivo();
	void lerPacoteArq(pacoteB *bloco, const int pos);
	void gravaPacoteArq(pacoteB *bloco, int pos);
	unsigned encontrarProxPosDisponivel();
	int encontrarPacoteParaInsercao(pacoteB *bloco, dadoB umDado);
	int encontrarAux(const int pos, dadoB umDado);
	void imprimirDepuracao(int posBloco, string prefixo, bool temIrmao);

public:
	Bplus(string arquivo);
	~Bplus();
	void depurar();
	void imprimir();
	int busca(char *chave);
	void inserirRecursivamente(dadoB umDado);
	//usando os arquivos
	void inserirDado(dadoB umDado);
};

// FUNÇÕES PARA LIDAR COM ARQUIVO

bool Bplus::cabecalhoEhConsistente(const cabecalhoB &cabecalhoArq)
{
	return (cabecalhoArq.capacidadeMaxPacote == cabecalhoArq.capacidadeMaxPacote) and (cabecalhoArq.capacidadeMinPacote == cabecalhoArq.capacidadeMinPacote) and (cabecalhoArq.posicaoMeio == cabecalhoArq.posicaoMeio);
}

Bplus::Bplus(string arquivo)
{
	NomeArquivo = arquivo;
	ifstream arq(NomeArquivo);
	cabecalhoB cabecalho;

	if (arq)
	{
		arq.read((char *)&cabecalho, sizeof(cabecalhoB));
		arq.close();

		if (!cabecalhoEhConsistente(cabecalho))
			throw runtime_error("Cabeçalho da arvore em arquivo é incosistente com as configurações atuais do programa");

		numNohs = cabecalho.numNohs;
		raiz = cabecalho.raiz;
		proxPosVazia = cabecalho.proxPosVazia;
	}
	else
	{
		numNohs = 0;
		raiz = POS_INVALIDA;
		proxPosVazia = POS_INVALIDA;

		ofstream arq(NomeArquivo);
		arq.close();
		atualizaCabecalhoArquivo();
	}
}

Bplus::~Bplus()
{
	atualizaCabecalhoArquivo();
}

void Bplus::atualizaCabecalhoArquivo()
{
	cabecalhoB cabecalho;
	cabecalho.capacidadeMaxPacote = CAP_PACOTE;
	cabecalho.capacidadeMinPacote = MIN_PACOTE;
	cabecalho.numNohs = numNohs;
	cabecalho.posicaoMeio = POS_MEIO;
	cabecalho.proxPosVazia = proxPosVazia;
	cabecalho.raiz = raiz;

	fstream arq(NomeArquivo, ios::in | ios::out | ios::binary);
	arq.write((const char *)&cabecalho, sizeof(cabecalhoB));
	arq.close();
}

void Bplus::lerPacoteArq(pacoteB *bloco, const int pos)
{
	unsigned posNoArq = sizeof(cabecalhoB) + pos * sizeof(pacoteB);
	fstream arq(NomeArquivo, ios::in | ios::out | ios::binary);
	arq.seekg(posNoArq);
	arq.read((char *)bloco, sizeof(pacoteB));
	arq.close();
}

void Bplus::gravaPacoteArq(pacoteB *bloco, int pos)
{
	unsigned posNoArq = sizeof(cabecalhoB) + pos * sizeof(pacoteB);
	fstream arq(NomeArquivo, ios::in | ios::out | ios::binary);
	arq.seekp(posNoArq);
	arq.write((const char *)bloco, sizeof(pacoteB));
	arq.close();
}

unsigned Bplus::encontrarProxPosDisponivel()
{
	if (proxPosVazia == POS_INVALIDA)
		return numNohs;
	else
	{
		throw runtime_error("Não era para proxPosicaoVazia ser diferente de POS_INVALIDA");
		return proxPosVazia;
	}
}

int Bplus::encontrarPacoteParaInsercao(pacoteB *bloco, dadoB umDado)
{
	if (raiz == POS_INVALIDA)
	{

		raiz = encontrarProxPosDisponivel();
		numNohs = 1;
		atualizaCabecalhoArquivo();
		return raiz;
	}
	else
	{

		int pos = encontrarAux(raiz, umDado);
		lerPacoteArq(bloco, pos);
		return pos;
	}
}

int Bplus::encontrarAux(const int pos, dadoB umDado)
{
	pacoteB *aux = new pacoteB(false);
	lerPacoteArq(aux, pos);
	if (aux->eFolha)
		return pos;
	else
	{
		if (aux->chaveEhMenorQueTodos(umDado.aluno))
			return encontrarAux(aux->filho[0], umDado);
		else if (aux->chaveEhMaiorQueTodos(umDado.aluno))
			return encontrarAux(aux->filho[aux->numElementos], umDado);
		else
		{
			int i = 0;
			while (i < aux->numElementos + 1 and strcmp(aux->elementos[i].aluno, umDado.aluno) < 0)
				i++;
			return encontrarAux(aux->filho[i], umDado);
		}
	}
}

void Bplus::depurar()
{
	cout << "-*- Dados da Arvore B+ -*-" << endl
		 << "Numero de pacotes: " << numNohs << endl
		 << "posicao do primeiro pacote: " << raiz << endl
		 << "Proximo posicao vazia (-1 se nao tiver remoção): " << proxPosVazia << endl;
	if (numNohs > 0)
	{
		cout << "Dados dos pacotes no formato: "
			 << "{posicao do pacote}(Numero de elementos/filhos)[elementos]"
			 << endl;
		imprimirDepuracao(raiz, "", false);
	}
	cout << endl
		 << "-*- Fim dos Dados -*-" << endl;
}

void Bplus::imprimirDepuracao(int posBloco, string prefixo, bool temIrmao)
{
	pacoteB *bloco = new pacoteB(false);
	lerPacoteArq(bloco, posBloco);
	cout << prefixo << "|---{" << posBloco << "}"
		 << "(" << bloco->numElementos;

	if (!bloco->eFolha)
	{
		cout << "/ filhos: " << bloco->filho[0];
		for (int i = 1; i < bloco->numElementos + 1; i++)
			cout << "," << bloco->filho[i];
	}

	string novoprefixo = temIrmao ? "|   " : "    ";

	cout << ") Elementos: ";
	bloco->imprimir();
	cout << endl;
	if (!bloco->eFolha)
	{
		for (int i = 0; i <= bloco->numElementos; i++)
			imprimirDepuracao(bloco->filho[i], prefixo + novoprefixo, i != bloco->numElementos);
	}
}

// FUNÇÕES DE ARVORE B

pacoteB *Bplus::dividepacoteB(pacoteB *bloco, dadoB &promovido, bool folha)
{

	promovido = bloco->elementos[POS_MEIO];
	pacoteB *novo = new pacoteB(bloco->eFolha);

	novo->numElementos = CAP_PACOTE / 2 - 1;
	bloco->numElementos = CAP_PACOTE / 2;

	for (int i = 0; i < CAP_PACOTE / 2 + 1; i++)
		novo->filhosSet[i] = bloco->filhosSet[POS_MEIO + i];

	if (!bloco->eFolha)
	{
		for (int i = 0; i < CAP_PACOTE / 2 + 1; i++)
			novo->elementos[i] = bloco->elementos[CAP_PACOTE / 2 + i + 1];

		for (int i = 0; i < CAP_PACOTE / 2 + 1; i++)
		{
			novo->filho[i] = bloco->filho[CAP_PACOTE / 2 + i + 1];
		}
	}
	else
	{
		for (int i = 0; i <= CAP_PACOTE / 2 + 1; i++)
			novo->elementos[i] = bloco->elementos[POS_MEIO + i];
	}

	return novo;
}

void Bplus::insereEmIntermediarioNaoCheio(pacoteB *bloco, int novo, dadoB promovido)
{
	int pos = int(bloco->numElementos) - 1;

	while (pos >= 0 and strcmp(bloco->elementos[pos].aluno, promovido.aluno) > 0)
	{
		bloco->elementos[pos + 1] = bloco->elementos[pos];
		bloco->filho[pos + 2] = bloco->filho[pos + 1];
		pos--;
	}

	bloco->elementos[pos + 1] = promovido;
	bloco->filho[pos + 2] = novo;
	bloco->numElementos++;
}

void Bplus::inserirRecursivamente(dadoB umDado)
{
	if (raiz == POS_INVALIDA)
	{
		pacoteB *aux = new pacoteB(true);
		int pos = encontrarProxPosDisponivel();
		gravaPacoteArq(aux, pos);
		raiz = pos;
		numNohs++;
	}

	dadoB promovido;
	int posnovo = insereRecAux(raiz, umDado, promovido);
	if (posnovo != POS_INVALIDA)
	{
		int antigaRaiz = raiz;
		pacoteB *aux = new pacoteB(false);
		aux->elementos[0] = promovido;
		aux->numElementos = 1;
		aux->filho[0] = antigaRaiz;
		aux->filho[1] = posnovo;
		unsigned novaraiz = encontrarProxPosDisponivel();
		raiz = novaraiz;
		gravaPacoteArq(aux, novaraiz);
		numNohs++;
	}
	atualizaCabecalhoArquivo();
}

int Bplus::insereRecAux(int posbloco, dadoB umDado, dadoB &promovido)
{
	pacoteB *bloco = new pacoteB(false);
	lerPacoteArq(bloco, posbloco);
	if (bloco->eFolha)
	{
		if (!bloco->cheio())
		{
			bloco->inserir(umDado);
			gravaPacoteArq(bloco, posbloco);
			return POS_INVALIDA;
		}
		else
		{
			pacoteB *novo = dividepacoteB(bloco, promovido, bloco->eFolha);
			if (strcmp(umDado.aluno, bloco->elementos[CAP_PACOTE / 2].aluno) <= 0)
			{
				bloco->inserir(umDado);
			}
			else
			{

				novo->inserir(umDado);
			}
			int posnovo = encontrarProxPosDisponivel();
			gravaPacoteArq(bloco, posbloco);
			gravaPacoteArq(novo, posnovo);
			numNohs++;
			return posnovo;
		}
	}
	else
	{
		int i = bloco->numElementos - 1;
		while (i >= 0 and strcmp(bloco->elementos[i].aluno, umDado.aluno) > 0)
			i--;

		int posAux = insereRecAux(bloco->filho[i + 1], umDado, promovido);
		if (posAux != POS_INVALIDA)
		{
			if (!bloco->cheio())
			{
				insereEmIntermediarioNaoCheio(bloco, posAux, promovido);
				gravaPacoteArq(bloco, posbloco);
				return POS_INVALIDA;
			}
			else
			{
				dadoB promovidoFilho = promovido;
				pacoteB *novo = dividepacoteB(bloco, promovido, bloco->eFolha);
				int posnovo = encontrarProxPosDisponivel();
				gravaPacoteArq(bloco, posbloco);
				gravaPacoteArq(novo, posnovo);
				numNohs++;

				return posnovo;
			}
		}
		return POS_INVALIDA;
	}
}

void Bplus::imprimir()
{
	if (raiz != POS_INVALIDA)
		imprimeAux(raiz);
	else
		cout << "Arvore vazia";
	cout << endl;
}

void Bplus::imprimeAux(int posBloco)
{
	pacoteB *bloco = new pacoteB(false);
	lerPacoteArq(bloco, posBloco);
	bloco->imprimir();
	if (!bloco->eFolha)
		for (int i = 0; i <= bloco->numElementos; i++)
			imprimeAux(bloco->filho[i]);
	else
		cout << " foia ";
}

int Bplus::busca(char *chave)
{
	return buscaAux(raiz, chave);
}

int Bplus::buscaAux(int posBloco, char *umDado)
{
	if (raiz == POS_INVALIDA)
		return POS_INVALIDA;

	pacoteB *bloco = new pacoteB(false);
	lerPacoteArq(bloco, posBloco);
	if (bloco)
	{
		if (bloco->eFolha)
		{
			int buscado = bloco->busca(umDado);
			if (buscado == POS_INVALIDA)
			{
				throw runtime_error("Busca: elemento não encontrado.");
				return POS_INVALIDA;
			}
			else
				return buscado;
		}
		else
		{
			if (bloco->chaveEhMenorQueTodos(umDado))
			{
				return buscaAux(bloco->filho[0], umDado);
			}
			else if (bloco->chaveEhMaiorQueTodos(umDado))
				return buscaAux(bloco->filho[bloco->numElementos], umDado);
			else
			{
				int i = 0;
				while (i < bloco->numElementos + 1 and strcmp(bloco->elementos[i].aluno, umDado) < 0)
					i++;

				return buscaAux(bloco->filho[i], umDado);
			}
		}
	}
	else
		return POS_INVALIDA;
}