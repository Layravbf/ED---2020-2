/* 
    salarios.cpp - aplicação para desenvolvimento de atividade
    de Tipo Abstrado de Dados

    Atividade de  GCC216 - Estruturas de Dados, 2020

    Solução de: LAYRA VILAS BOAS FERREIRA
    Enunciado 5: Redimensionar o vetor adicionando três novas posições a cada redimensionamento
*/

/*Foi criado um construtor de cópia para solucionar o erro de segmentação, o qual o destrutor estava sendo chamado automaticamente quando o escopo
do objeto morria, assim não dava pra acessá-lo mais depois.Com esse construtor de cópia não se deseja simplesmente copiar um ponteiro,
mas alocar uma nova área de memória e copiar seu conteúdo.*/

#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

struct funcionario
{
    string nome;
    string cargo;
    float salario;
    funcionario();
};

funcionario::funcionario()
{
    nome.clear();
    cargo.clear();
    salario = 0;
}

class equipe
{
private:
    // *** AVISO: Não altere os atributos privados da classe ***
    funcionario *listaDeFuncionarios;
    int quantidadeDeFuncionarios;
    int capacidadeMaxima;

public:
    equipe(int capacidade);
    equipe(const equipe &outra);
    ~equipe();
    void inserirNovoFuncionario(const funcionario &umFuncionario);
    bool buscarFuncionario(const string &umNome, funcionario &umFuncionario);
    void redimensionarCapacidade();
    friend void simularReajusteSalarial(equipe umaEquipe, float reajuste);
};

equipe::equipe(int capacidade)
{
    quantidadeDeFuncionarios = 0;
    capacidadeMaxima = capacidade;
    listaDeFuncionarios = new funcionario[capacidadeMaxima];
}

equipe::equipe(const equipe &outra) // construtor de cópia
{
	cout<<"copia"<<endl;
    quantidadeDeFuncionarios = outra.quantidadeDeFuncionarios;
    capacidadeMaxima = outra.capacidadeMaxima;
    listaDeFuncionarios = new funcionario[capacidadeMaxima];
	copy(outra.listaDeFuncionarios, outra.listaDeFuncionarios + outra.quantidadeDeFuncionarios, listaDeFuncionarios);
}

equipe::~equipe()
{
	cout << " morri " << endl;
    delete[] listaDeFuncionarios;
}

void equipe::inserirNovoFuncionario(const funcionario &umFuncionario)
{
    if (quantidadeDeFuncionarios == capacidadeMaxima)
    { // vetor cheio, precisa redimensionar
        redimensionarCapacidade();
    }
    listaDeFuncionarios[quantidadeDeFuncionarios] = umFuncionario;
    quantidadeDeFuncionarios++;
}

bool equipe::buscarFuncionario(const string &nome, funcionario &umFuncionario)
{
    int posicaoAtual = 0;

    while ((posicaoAtual < quantidadeDeFuncionarios) and (listaDeFuncionarios[posicaoAtual].nome != nome))
        posicaoAtual++;

    // se posicaoAtual não for menor que quantidade de produtos, é porque não encontrou
    if (posicaoAtual < quantidadeDeFuncionarios)
    {
        umFuncionario = listaDeFuncionarios[posicaoAtual];
        return true;
    }

    return false;
}

void equipe::redimensionarCapacidade()
{
	funcionario *listaAux = new funcionario [capacidadeMaxima];
    copy(listaDeFuncionarios, listaDeFuncionarios+quantidadeDeFuncionarios, listaAux);
    delete[] listaDeFuncionarios;
    
    int capacidadeNova;
    if(capacidadeMaxima % 2 == 0){
		capacidadeNova = capacidadeMaxima/2;
	}else{
		capacidadeNova = (capacidadeMaxima/2)+1;
	}
    capacidadeMaxima += capacidadeNova;
    
    listaDeFuncionarios = new funcionario[(int)capacidadeMaxima];
    copy(listaAux, listaAux+quantidadeDeFuncionarios, listaDeFuncionarios);
    delete[] listaAux;
    cout << "Aumentou com sucesso o tamanho do vetor para: " << capacidadeMaxima << endl;
	
	
    /*funcionario *listaDeFuncionariosAtualizada = new funcionario[capacidadeMaxima];
    copy(listaDeFuncionarios, listaDeFuncionarios + quantidadeDeFuncionarios, listaDeFuncionariosAtualizada);//copia dados da listaDeFuncionarios para a nova lista
    delete[] listaDeFuncionarios; //deleta os dados que agora já foi feito cópia, então podem ser apagados
    
    cout << "Capacidade máxima inicial da equipe: " << capacidadeMaxima << endl; 
    int novaCapacidade = capacidadeMaxima + 3;
    
    listaDeFuncionarios = new funcionario[novaCapacidade]; //aloca com a novaCapacidade agora redimensionada.Memória será desalocada quando chamado a função novamente ou com o destrutor ao acabar o programa
    copy(listaDeFuncionariosAtualizada, listaDeFuncionariosAtualizada + quantidadeDeFuncionarios, listaDeFuncionarios);// copia dados da nova lista para a listaDeFuncionarios
    delete[] listaDeFuncionariosAtualizada; // dados ja copiados, pode-se desalocar a memória
    
    capacidadeMaxima = novaCapacidade;
    cout << "Aumentou com sucesso o tamanho do vetor para: " << capacidadeMaxima << endl;*/
}

void simularReajusteSalarial(equipe umaEquipe, float reajuste)
{
    // *** AVISO: Não altere esta função ***
    float valorTotalSemReajuste = 0;
    float valorTotalComReajuste = 0;
    float novoSalario;
    for (int i = 0; i < umaEquipe.quantidadeDeFuncionarios; i++)
    {
        valorTotalSemReajuste += umaEquipe.listaDeFuncionarios[i].salario;
        novoSalario = umaEquipe.listaDeFuncionarios[i].salario * (1 + reajuste / 100);
        valorTotalComReajuste += novoSalario;
        cout << umaEquipe.listaDeFuncionarios[i].nome << " "
             << umaEquipe.listaDeFuncionarios[i].salario << " "
             << novoSalario << endl;
    }
    cout << valorTotalSemReajuste << " " << valorTotalComReajuste << endl;
    cout << endl;
}

int main()
{
    // *** AVISO: Não altere a função principal, o main() ***
    int capacidadeInicial;

    cout << "Salarios - Aplicativo para Calcular Reajustes\n"
         << "Entre com capacidade máxima inicial da equipe: ";
    cin >> capacidadeInicial;

    equipe minhaEquipe(capacidadeInicial);

    cout << "Salários - Escolha a Opção:\n"
         << "i - inserir novo funcionário\n"
         << "b - buscar por dados de um funcionário a partir do nome\n"
         << "r - simular reajuste salarial de todos os funcionários\n"
         << "s - para sair do programa" << endl;

    char opcaoDoMenu;
    funcionario umFuncionario;
    string nome;
    float reajuste;

    cin >> opcaoDoMenu;

    while (opcaoDoMenu != 's')
    {
        switch (opcaoDoMenu)
        {
        case 'i':
            cout << "Entre com dados do funcionário (nome, cargo e salário):" << endl;
            cin >> umFuncionario.nome >> umFuncionario.cargo >> umFuncionario.salario;
            minhaEquipe.inserirNovoFuncionario(umFuncionario);
            break;

        case 'b':
            cout << "Entre com nome do funcionário para busca: ";
            cin >> nome;

            if (minhaEquipe.buscarFuncionario(nome, umFuncionario))
                cout << umFuncionario.nome << " "
                     << umFuncionario.cargo << " " << umFuncionario.salario << endl
                     << endl;
            else
                cout << "Funcionário não encontrado!" << endl
                     << endl;
            break;

        case 'r':
            cout << "Entre com valor do reajuste (em porcentagem): ";
            cin >> reajuste;
            simularReajusteSalarial(minhaEquipe, reajuste);
            break;

        case 's': // não faz nada, espera retorno ao while para sair
            break;

        default:
            cout << "Opção inválida!" << endl;
            break;
        }
        cout << "Salários - Escolha a Opção:\n"
             << "i - inserir novo funcionário\n"
             << "b - buscar por dados de um funcionário a partir do nome\n"
             << "r - simular reajuste salarial de todos os funcionários\n"
             << "s - para sair do programa" << endl;
        cin >> opcaoDoMenu;
    }

    return 0;
}
