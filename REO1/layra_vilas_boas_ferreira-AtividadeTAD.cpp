#include <iostream>
#include <string>
using namespace std;

/*
 * Nome: Layra Vilas Boas Ferreira
 * Turma: 10A
 * Matrícula: 202010142
 * Enunciado 03
 * Alterações quanto a implementação: adicionei um método para a verificação da busca.
 * Valores definidos pelos os usuários e assim o tamanho será alocado.
 * Variável posicao = -1 --> utilizada para pegar a posicao do artista buscado, caso nao ache retorna -1.
 * Função verificarBusca --> se encontrou o nome do artista, será passado posicao com o valor de i, assim será impresso seus dados,
 * se posicao ser retornada como -1, será impresso "Artista não cadastrado".
 */

struct artista
{
    string nomeArtista;
    string telefone;
    string enderecoStudio;
};

struct entusiasta
{
    string nomeEntusiasta;
    string nomeObra;
    float valorObra;
    string emailContato;
};

class curador
{
private:
    artista *cadastroArtista;
    entusiasta *registroEntusiasta;
    int totalArtistas;
    int totalEntusiastas;

public:
    curador(int quantidadeArtista, int quantidadeEntusiasta);
    ~curador();
    void cadastrarArtista();
    int buscarDadosArtista(string busca, int posicao);
    void registrarEntusiastas();
    void verificarBusca(int posicao);
};

curador::curador(int quantidadeArtista, int quantidadeEntusiasta)
{
    totalArtistas = quantidadeArtista;
    totalEntusiastas = quantidadeEntusiasta;
    cadastroArtista = new artista[totalArtistas];
    registroEntusiasta = new entusiasta[totalEntusiastas];
};

curador::~curador()
{
    delete[] cadastroArtista;
    delete[] registroEntusiasta;
    totalArtistas = 0;
    totalEntusiastas = 0;
};

void curador::cadastrarArtista()
{
    for (int i = 0; i < totalArtistas; i++)
    {
        cout << "CADASTRO DE ARTISTAS" << endl;
        cout << "Nome do Artista: ";
        getline(cin, cadastroArtista[i].nomeArtista);
        cin.ignore(0, '\n');
        cout << "Telefone: ";
        getline(cin, cadastroArtista[i].telefone);
        cin.ignore(0, '\n');
        cout << "Endereco do Studio: ";
        getline(cin, cadastroArtista[i].enderecoStudio);
        cout << "Cadastro realizado com sucesso!" << endl;
    }
}

void curador::registrarEntusiastas()
{
    for (int i = 0; i < totalEntusiastas; i++)
    {
        cout << "CADASTRO DE ENTUSIASTAS" << endl;
        cout << "Nome do Entusiasta: ";
        getline(cin, registroEntusiasta[i].nomeEntusiasta);
        cin.ignore(0, '\n');
        cout << "Nome da obra: ";
        getline(cin, registroEntusiasta[i].nomeObra);
        cin.ignore(0, '\n');
        cout << "Valor da obra: ";
        cin >> registroEntusiasta[i].valorObra;
        cout << "Email de contato: ";
        cin >> registroEntusiasta[i].emailContato;
        cin.ignore(1, '\n');
        cout << "Registro realizado com sucesso!" << endl;
    }
}

int curador::buscarDadosArtista(string busca, int posicao)
{
    for (int i = 0; i < totalArtistas; i++)
    {
        if (busca == cadastroArtista[i].nomeArtista)
        {
            posicao = i;
        }
    }
    return posicao;
}

void curador::verificarBusca(int posicao)
{
    if (posicao != -1)
    {
        cout << "Nome: " << cadastroArtista[posicao].nomeArtista << endl;
        cout << "Telefone: " << cadastroArtista[posicao].telefone << endl;
        cout << "Endereco do Studio: " << cadastroArtista[posicao].enderecoStudio << endl;
    }
    else
    {
        cout << "Artista nao cadastrado!" << endl;
    }
}

int main()
{
    int quantidadeArtista, quantidadeEntusiasta;
    cout << "Quantos artistas deseja cadastrar?" << endl;
    cin >> quantidadeArtista;
    cout << "Quantos entusiastas deseja registrar?" << endl;
    cin >> quantidadeEntusiasta;
    cin.ignore(1, '\n');

    curador *ptr = new curador(quantidadeArtista, quantidadeEntusiasta);
    ptr->cadastrarArtista();
    ptr->registrarEntusiastas();

    int posicao = -1;
    string busca;
    if(quantidadeArtista > 0)
    {
		cout << "Qual o nome do artista que deseja consultar os dados?" << endl;
		getline(cin, busca);
		posicao = ptr->buscarDadosArtista(busca, posicao);
		ptr->verificarBusca(posicao);
	}

    ptr->~curador();
    return 0;
}
