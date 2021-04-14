/* 
  Trabalho de Estrutura de Dados
  Implementação de Árvore B+ a partir de código de Sequence Set
  Arthur Bonani de Oliveira Paiva
  Glaucio Martins Lima 
  Layra Vilas Boas Ferreira
     Main:
        Tem a opção de ler entradas do arquivo "entradas_arvoreB+.txt"
        i: inserir dado (aluno, matricula, curso, orientador, vaga_reservada, edital e semestre de ingresso)  | Ex: i yam 8124 yam yam 1 yam 20208124
        b: buscar dado usando arvore                                                                          | Ex: b yam
        v: buscar dado usando sequenceset                                                                     | Ex: b yam
        p: imprimir sequenceSet e arvore                                                                      | Ex: p
        d: depurar sequenceSet e arvore                                                                       | Ex: d
        k: busca dado na arvore e no sequenceSet e salva no arquivo "saidas.txt"                              | Ex: k yam
*/

#include <iostream>
#include <fstream>
#include "sequenceset.hpp"
#include <chrono>

using namespace std;

int main()
{
    sequenceset meuSeqSet("teste.dat");
    dado umDado;
    char umaChave[50];
    char operacao;

    auto inicio = std::chrono::high_resolution_clock::now();
    auto resultado = std::chrono::high_resolution_clock::now() - inicio;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(resultado).count();
    cout << "ler entradas do aquivo?" << endl;
    char resp;
    cin >> resp;

    if (resp == 's')
    {
        fstream arqInputs("entradas_arvoreB+.txt");
        cout << "Ler quantas entradas?" << endl;
        int resp2, cont = 0;
        cin >> resp2;
        while (arqInputs >> operacao and cont < resp2 and operacao != 's')
        {
            cont++;
            arqInputs >> umDado.aluno >> umDado.matricula >> umDado.curso >> umDado.orientador >> umDado.vagas_reservadas >> umDado.edital >> umDado.semestre_ingresso;
            meuSeqSet.inserirDado(umDado);
            cout << ((umDado.matricula + 1) * 100) / resp2 << "%" << endl;
        }

        cout << "fim arquivo" << endl;
        arqInputs.close();
    }

    ofstream saida("saidas.txt");
    if (!saida)
        cout << "erro ao abrir arquivo" << endl;

    do
    {
        try
        {
            cin >> operacao;
            switch (operacao)
            {
            case 'i': // inserir
                cin >> umDado.aluno >> umDado.matricula >> umDado.curso >> umDado.orientador >> umDado.vagas_reservadas >> umDado.edital >> umDado.semestre_ingresso;
                meuSeqSet.inserirDado(umDado);
                break;
            case 'b': // buscar
                cin >> umaChave;

                inicio = std::chrono::high_resolution_clock::now();
                try
                {
                    umDado = meuSeqSet.buscar(umaChave);
                    cout << "Busca: " << umDado.aluno << "/" << umDado.matricula << "/" << umDado.curso << "/" << umDado.orientador << "/" << umDado.vagas_reservadas << "/" << umDado.edital << "/" << umDado.semestre_ingresso << endl;
                    resultado = std::chrono::high_resolution_clock::now() - inicio;
                    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(resultado).count();
                    cout << "Tempo de busca " << microseconds << endl;
                }
                catch (const runtime_error &error)
                {
                    resultado = std::chrono::high_resolution_clock::now() - inicio;
                    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(resultado).count();
                    cout << "Tempo de busca " << microseconds << endl;
                    cerr << error.what() << endl;
                }
                break;
            case 'v': // buscar
                cin >> umaChave;

                inicio = std::chrono::high_resolution_clock::now();
                try
                {
                    umDado = meuSeqSet.buscarSet(umaChave);
                    cout << "Busca: " << umDado.aluno << "/" << umDado.matricula << "/" << umDado.curso << "/" << umDado.orientador << "/" << umDado.vagas_reservadas << "/" << umDado.edital << "/" << umDado.semestre_ingresso << endl;
                    resultado = std::chrono::high_resolution_clock::now() - inicio;
                    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(resultado).count();
                    cout << "Tempo de busca " << microseconds << endl;
                }
                catch (const runtime_error &error)
                {
                    resultado = std::chrono::high_resolution_clock::now() - inicio;
                    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(resultado).count();
                    cout << "Tempo de busca " << microseconds << endl;
                    cerr << error.what() << endl;
                }
                break;
            case 'p': // mostrar estrutura
                meuSeqSet.imprimir();
                meuSeqSet.imprimirArvore();
                break;
            case 'd': // mostrar estrutura
                meuSeqSet.depurar();

                break;
            case 'k':
                cin >> umaChave;
                cout << umaChave;
                saida << umaChave;

                inicio = std::chrono::high_resolution_clock::now();
                try
                {
                    umDado = meuSeqSet.buscar(umaChave);
                    resultado = std::chrono::high_resolution_clock::now() - inicio;
                    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(resultado).count();
                    cout << " " << microseconds;
                    saida << " " << microseconds;
                }
                catch (const runtime_error &error)
                {
                    resultado = std::chrono::high_resolution_clock::now() - inicio;
                    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(resultado).count();
                    saida << " " << microseconds;
                    cout << " " << microseconds;

                    cerr << error.what() << endl;
                }

                inicio = std::chrono::high_resolution_clock::now();

                try
                {
                    umDado = meuSeqSet.buscarSet(umaChave);

                    resultado = std::chrono::high_resolution_clock::now() - inicio;
                    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(resultado).count();
                    cout << " " << microseconds;
                    saida << " " << microseconds;
                }
                catch (const runtime_error &error)
                {
                    resultado = std::chrono::high_resolution_clock::now() - inicio;
                    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(resultado).count();
                    saida << " " << microseconds;
                    cout << " " << microseconds;
                    saida << " inexistente";
                    cout << " inexistente";

                    cerr << error.what() << endl;
                }
                saida << endl;
                cout << endl;

                break;
            case 's':

                break;
            default:
                cout << "Opção inválida!" << endl;
            }
        }
        catch (runtime_error &e)
        {
            cerr << e.what() << endl;
        }
    } while (operacao != 's');

    saida.close();
    return 0;
}
