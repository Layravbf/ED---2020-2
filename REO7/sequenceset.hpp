/* Sequence Set - Classe Principal
 *
 * by Joukim, Fevereiro de 2021 - Estrutura de Dados (GCC216)
 * Caracteristicas da primeira implementaÃ§Ã£o - fevereiro de 2021:
 * -> cÃ³digo adaptado do cÃ³digo de vetor expansÃ­vel
 * -> uso de constantes (em configuraÃ§Ã£o.hpp) para facilitar ajustes
 * -> se dado pode ser inserido em dois pacotes, escolhe o que gera 
 * -> menos impacto
 */

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdexcept>
#include "pacote.hpp"
#include "bplus.hpp"

using namespace std;

// CabeÃ§alho do Arquivo do Sequence Set
struct cabecalhoArqSS
{
    unsigned capacidadeMaxPacote;
    unsigned capacidadeMinPacote;
    unsigned posicaoMeio;
    unsigned numPacotes;
    int posPrimeiroPacote;
    int proxPosicaoVazia; // para poder reaproveitar pacotes que
                          // fiquem vazios no meio do arquivo
};

class sequenceset
{
    friend class Bplus;

private:
    unsigned numPacotes;
    int posPrimeiroPacote;
    int proxPosicaoVazia;
    Bplus *arvore;
    string nomeArquivo;
    inline bool cabecalhoEhConsistente(const cabecalhoArqSS &umCabecalho);
    void atualizarCabecalhoNoArquivo();
    void gravarPacoteNoArquivo(pacote *umPacote, unsigned posicao);
    void lerPacoteDoArquivo(pacote *umPacote, unsigned posicao);
    unsigned encontrarProxPosDisponivel();
    int encontrarPacoteParaInsercao(pacote *umPacote, dado umDado);
    pacote *dividirPacote(pacote *umPacote, unsigned posNovoPacote);
    dado buscaBinaria(dado vetor[], int inicio, int fim, char *chave);
    void adicionaPacotesAArvore();

public:
    sequenceset(string arquivo);
    ~sequenceset();
    void inserirDado(dado umDado);
    void imprimir();
    void depurar();
    void imprimirArvore();
    dado buscar(char *chave);
    dado buscarSet(char *chave);
    /* 
        void remover(char* chave);
        */
};

sequenceset::sequenceset(string arquivo)
{
    arvore = new Bplus("testearvore.dat");
    nomeArquivo = arquivo;
    ifstream arqEntrada(nomeArquivo);
    cabecalhoArqSS cabecalho;

    // verifica se o arquivo existe, se sim, lÃª o cabeÃ§alho
    // e verifica se os dados sÃ£o consistentes com a configuraÃ§Ã£o
    // do sequence set
    if (arqEntrada)
    {
        arqEntrada.read((char *)&cabecalho, sizeof(cabecalhoArqSS));
        arqEntrada.close();
        if (not cabecalhoEhConsistente(cabecalho))
        {
            throw runtime_error("AplicaÃ§Ã£o usa configuraÃ§Ã£o diferente das usadas no arquivo");
        }
        // atualiza os dados do sequence set de acordo com o cabeÃ§alho do arquivo
        numPacotes = cabecalho.numPacotes;
        posPrimeiroPacote = cabecalho.posPrimeiroPacote;
        proxPosicaoVazia = cabecalho.proxPosicaoVazia;
    }
    // nÃ£o existe o arquivo de entrada ainda, inicializa novo sequence set
    // e salva o cabeÃ§alho no arquivo
    else
    {
        numPacotes = 0;
        posPrimeiroPacote = POS_INVALIDA;
        proxPosicaoVazia = POS_INVALIDA;
        // cria o arquivo
        ofstream ArqSaida(nomeArquivo);
        ArqSaida.close();
        atualizarCabecalhoNoArquivo();
    }
}

sequenceset::~sequenceset()
{
    // apenas atualiza o cabeÃ§alho, para garantir que esteja tudo ok
    atualizarCabecalhoNoArquivo();
}

bool sequenceset::cabecalhoEhConsistente(const cabecalhoArqSS &umCabecalho)
{
    return ((umCabecalho.capacidadeMaxPacote == CAP_PACOTE) and (umCabecalho.capacidadeMinPacote == MIN_PACOTE) and (umCabecalho.posicaoMeio == POS_MEIO));
}

void sequenceset::atualizarCabecalhoNoArquivo()
{
    cabecalhoArqSS cabecalho;
    cabecalho.capacidadeMaxPacote = CAP_PACOTE;
    cabecalho.capacidadeMinPacote = MIN_PACOTE;
    cabecalho.posicaoMeio = POS_MEIO;
    cabecalho.numPacotes = numPacotes;
    cabecalho.posPrimeiroPacote = posPrimeiroPacote;
    cabecalho.proxPosicaoVazia = proxPosicaoVazia;
    // precisa ser fstream para nÃ£o apagar o arquivo jÃ¡ existente
    fstream arqSaida(nomeArquivo, ios::in | ios::out | ios::binary);
    arqSaida.write((const char *)&cabecalho, sizeof(cabecalhoArqSS));
    arqSaida.close();
}

void sequenceset::lerPacoteDoArquivo(pacote *umPacote, unsigned posicao)
{
    // pula o cabeÃ§alho do arquivo e o nÃºmero de pÃ¡ginas anteriores
    unsigned posArq = sizeof(cabecalhoArqSS) + posicao * sizeof(pacote);
    fstream arqEntrada(nomeArquivo, ios::in | ios::out | ios::binary);
    arqEntrada.seekg(posArq);
    arqEntrada.read((char *)umPacote, sizeof(pacote));
    arqEntrada.close();
}

void sequenceset::gravarPacoteNoArquivo(pacote *umPacote, unsigned posicao)
{
    // pula o cabeÃ§alho do arquivo e o nÃºmero de pÃ¡ginas anteriores
    unsigned posArq = sizeof(cabecalhoArqSS) + posicao * sizeof(pacote);
    // precisa ser fstream para nÃ£o apagar o arquivo jÃ¡ existente
    fstream arqSaida(nomeArquivo, ios::in | ios::out | ios::binary);
    arqSaida.seekp(posArq);
    arqSaida.write((const char *)umPacote, sizeof(pacote));
    arqSaida.close();
}

unsigned sequenceset::encontrarProxPosDisponivel()
{
    // se nÃ£o tem pacote vazio no meio do arquivo
    // entÃ£o a prÃ³xima posiÃ§Ã£o disponÃ­vel serÃ¡ uma posiÃ§Ã£o a mais
    // que a Ãºltima do arquivo (numPacotes-1)
    if (proxPosicaoVazia == POS_INVALIDA)
    {
        return numPacotes;
    }
    else
    {
        // este trecho cÃ³digo sÃ³ Ã© Ãºtil com remoÃ§Ã£o
        // como ainda nÃ£o temos remoÃ§Ã£o, entÃ£o gera exceÃ§Ã£o
        throw runtime_error("NÃ£o era para proxPosicaoVazia ser diferente de POS_INVALIDA");

        // reaproveitar primeira posiÃ§Ã£o vazia
        unsigned posicao = proxPosicaoVazia;
        pacote *pacoteInvalido = new pacote(true);
        lerPacoteDoArquivo(pacoteInvalido, proxPosicaoVazia);
        proxPosicaoVazia = pacoteInvalido->posProximoPacote;
        return posicao;
    }
}

void sequenceset::inserirDado(dado umDado)
{
    unsigned posicao;
    pacote *pacoteDestino = new pacote(true);
    posicao = encontrarPacoteParaInsercao(pacoteDestino, umDado);
    // pacote estÃ¡ cheio, precisa dividir
    if (pacoteDestino->cheio())
    {
        unsigned posicaoNovoPacote = encontrarProxPosDisponivel();
        pacote *novoPacote = dividirPacote(pacoteDestino, posicaoNovoPacote);
        if (strcmp(umDado.aluno, novoPacote->elementos[0].aluno) > 0)
            novoPacote->inserir(umDado);
        else
            pacoteDestino->inserir(umDado);

        dadoB pacoteAux;

        if (arvore->busca(pacoteDestino->elementos[0].aluno) == POS_INVALIDA)
        {
            strcpy(pacoteAux.aluno, pacoteDestino->elementos[0].aluno);
            pacoteAux.pos = posicao;
            arvore->inserirRecursivamente(pacoteAux);
        }

        strcpy(pacoteAux.aluno, novoPacote->elementos[0].aluno);
        pacoteAux.pos = posicaoNovoPacote;
        arvore->inserirRecursivamente(pacoteAux);

        gravarPacoteNoArquivo(pacoteDestino, posicao);
        gravarPacoteNoArquivo(novoPacote, posicaoNovoPacote);
        delete novoPacote;
        numPacotes++;
        atualizarCabecalhoNoArquivo();
    }
    else
    {
        pacoteDestino->inserir(umDado);
        gravarPacoteNoArquivo(pacoteDestino, posicao);
    }
    delete pacoteDestino;
}

// o mÃ©todo encontrarPacoteParaInsercao retorna dois elementos:
// um pacote, jÃ¡ carregado do disco, em que serÃ¡ feita a inserÃ§Ã£o
// e a posiÃ§Ã£o relativa desse pacote no disco, para facilitar
// a gravaÃ§Ã£o posterior, apÃ³s alteraÃ§Ãµes
// o mÃ©todo recebe um pacote recÃ©m-criado e o dado para busca
int sequenceset::encontrarPacoteParaInsercao(pacote *umPacote, dado umDado)
{
    // caso vetor expansÃ­vel esteja vazio, criar primeiro pacote
    if (posPrimeiroPacote == POS_INVALIDA)
    {
        posPrimeiroPacote = encontrarProxPosDisponivel();
        numPacotes = 1;
        atualizarCabecalhoNoArquivo();
        return posPrimeiroPacote;
    }
    // sequence set nÃ£o estÃ¡ vazio: procura o pacote para inserir o elemento
    else
    {
        lerPacoteDoArquivo(umPacote, posPrimeiroPacote);
        int posicao = posPrimeiroPacote;

        // este laÃ§o vai lendo pacotes do disco, enquanto a chave
        // for maior que os valores do pacote atual
        while ((umPacote->posProximoPacote != POS_INVALIDA) and (umPacote->chaveEhMaiorQueTodos(umDado.aluno)))
        {
            posicao = umPacote->posProximoPacote;
            lerPacoteDoArquivo(umPacote, posicao);
        }
        return posicao;
    }
}

pacote *sequenceset::dividirPacote(pacote *umPacote, unsigned posNovoPacote)
{
    pacote *novo = new pacote(true);
    // copia metade superior dos dados do pacote atual para o novo
    for (unsigned i = 0; i <= CAP_PACOTE / 2; i++)
    {
        novo->elementos[i] = umPacote->elementos[i + CAP_PACOTE / 2];
    }

    novo->posProximoPacote = umPacote->posProximoPacote;
    umPacote->posProximoPacote = posNovoPacote;
    novo->numElementos = CAP_PACOTE - CAP_PACOTE / 2;
    umPacote->numElementos = CAP_PACOTE / 2;
    return novo;
}

dado sequenceset::buscar(char *chave)
{

    if (posPrimeiroPacote == POS_INVALIDA)
    {

        throw runtime_error("Busca: Sequence Set vazio.");
    }
    // sequence set nÃ£o estÃ¡ vazio:
    // procura o pacote que poderia conter o elemento
    else
    {
        pacote *umPacote = new pacote(true);
        lerPacoteDoArquivo(umPacote, arvore->busca(chave));

        dado retorno = buscaBinaria(umPacote->elementos, 0, umPacote->numElementos - 1, chave);

        return retorno;
    }
}

//funcao de busca binaria recursiva
dado sequenceset::buscaBinaria(dado vetor[], int inicio, int fim, char *chave)
{
    int meio = (inicio + fim) / 2;

    if (inicio <= fim)
    {
        if (strcmp(chave, vetor[meio].aluno) > 0)
            return buscaBinaria(vetor, meio + 1, fim, chave);
        else if (strcmp(chave, vetor[meio].aluno) < 0)
            return buscaBinaria(vetor, inicio, meio - 1, chave);
        else
            return vetor[meio];
    }
    else
    { // inicio == fim, ou seja, nÃ£o hÃ¡ mais o que buscar
        // retornamos -1 para indicar posiÃ§Ã£o nÃ£o encontrada

        throw runtime_error("Busca: elemento nÃ£o na binaria encontrado.");
    }
}

void sequenceset::imprimir()
{
    if (numPacotes > 0)
    {
        pacote *auxiliar = new pacote(true);
        lerPacoteDoArquivo(auxiliar, posPrimeiroPacote);
        int posAtual = posPrimeiroPacote;
        while (auxiliar->posProximoPacote != POS_INVALIDA)
        {
            cout << "(" << posAtual << ") ";
            auxiliar->imprimir();
            cout << " ->";
            posAtual = auxiliar->posProximoPacote;
            lerPacoteDoArquivo(auxiliar, auxiliar->posProximoPacote);
        }
        cout << "(" << posAtual << ") ";
        auxiliar->imprimir();
        delete auxiliar;
    }
    cout << endl;
}

void sequenceset::depurar()
{
    cout << "-*- Dados do Sequence Set -*-" << endl
         << "NÃºmero de pacotes: " << numPacotes << endl
         << "PosiÃ§Ã£o do primeiro pacote: " << posPrimeiroPacote << endl
         << "PrÃ³xima posiÃ§Ã£o vazia (-1 se nÃ£o tiver remoÃ§Ã£o): " << proxPosicaoVazia << endl;
    if (numPacotes > 0)
    {
        cout << "Dados dos pacotes no formato: "
             << "{posiÃ§Ã£o do pacote}(nÃºmero de elementos/prÃ³ximo pacote)[elementos]"
             << endl;
        pacote *auxiliar = new pacote(true);
        unsigned posicao = posPrimeiroPacote;
        lerPacoteDoArquivo(auxiliar, posPrimeiroPacote);

        while (auxiliar->posProximoPacote != POS_INVALIDA)
        {
            cout << "{" << posicao << "}"
                 << "(" << auxiliar->numElementos << "/"
                 << auxiliar->posProximoPacote << ")";
            auxiliar->imprimir();
            cout << "->";
            posicao = auxiliar->posProximoPacote;
            lerPacoteDoArquivo(auxiliar, auxiliar->posProximoPacote);
        }
        cout << "{" << posicao << "}"
             << "(" << auxiliar->numElementos << "/"
             << auxiliar->posProximoPacote << ")";
        auxiliar->imprimir();
        delete auxiliar;
    }
    cout << endl
         << "-*- Fim dos Dados do Sequence Set -*-" << endl;
    arvore->depurar();
}

void sequenceset::imprimirArvore()
{
    arvore->imprimir();
}

dado sequenceset::buscarSet(char *chave)
{

    if (posPrimeiroPacote == POS_INVALIDA)
    {
        throw runtime_error("Busca: Sequence Set vazio.");
    }
    // sequence set não está vazio:
    // procura o pacote que poderia conter o elemento
    else
    {
        pacote *umPacote = new pacote(false);
        lerPacoteDoArquivo(umPacote, posPrimeiroPacote);

        // este laço vai lendo pacotes do disco, enquanto a chave
        // for maior que os valores do pacote atual
        while ((umPacote->posProximoPacote != POS_INVALIDA) and (umPacote->chaveEhMaiorQueTodos(chave)))
        {
            lerPacoteDoArquivo(umPacote, umPacote->posProximoPacote);
        }
        // ou o dado está no pacote que saiu do while
        // ou não existe no sequence set - precisa agora buscar o elemento no
        // vetor de elementos

        dado retorno = buscaBinaria(umPacote->elementos, 0, umPacote->numElementos - 1, chave);

        return retorno;
    }
}