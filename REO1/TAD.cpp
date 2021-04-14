#include <iostream>
#include <string>
using namespace std;

class artista
{
private:
    string nomeArtista;
    string telefone;
    string enderecoStudio;

public:
    int cadastrarArtista(int qtd);
    int buscarDadosArtista(int busca, int qtd);
};

int artista::cadastrarArtista(int qtd)
{
    artista cadastro;
    cout << "CADASTRO DE ARTISTAS" << endl;
    for (int i = 0; i < qtd; i++)
    {
        cout << "Nome do Artista: ";
        cin >> cadastro.nomeArtista;
        cout << "Telefone para contato: ";
        getline(cin, telefone);
        cout << "Endereço do Studio: ";
        getline(cin, enderecoStudio);
    }
}

int artista::buscarDadosArtista(int busca, int qtd)
{
    int i = 0, posicao = -1;
}

class entusiasta
{
private:
    string nomeEntusiasta;
    string nomeObra;
    float valorObra;
    string emailContato;

public:
    int registrarEntusiasta();
};

int main()
{
    int qtdArtistas;
    artista fotografo;
    cout << "Quantos artistas deseja cadastrar? ";
    cin >> qtdArtistas;

    fotografo.cadastrarArtista(qtdArtistas);

    int busca;
    cout << "Qual indice de cadastro de artistas deseja buscar?" << endl;
    cout << "Quantidade de artistas:" << qtdArtistas;
    cin >> busca;

    fotografo.buscarDadosArtista(busca, qtdArtistas);

    return 0;
}