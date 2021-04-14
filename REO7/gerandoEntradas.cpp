/* 
  Trabalho de Estrutura de Dados
  Implementação de Árvore B+ a partir de código de Sequence Set
  Arthur Bonani de Oliveira Paiva
  Glaucio Martins Lima 
  Layra Vilas Boas Ferreira
     Arquivo com o objetivo de gerar inserções
*/

#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    ofstream arq("entradas_arvoreB+.txt");

    if (!arq)
        cout << "Erro ao abrir arq";

    char letras[3] = {'a', 'a', 'a'};
    int cont = 0;
    for (int i = 0; i < 26; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            for (int x = 0; x < 13; x++)
            {
                arq << "i " << letras[0] << letras[1] << letras[2] << " " << cont << " " << letras[0] << letras[1] << letras[2] << " " << letras[0] << letras[1] << letras[2] << " " << (cont % 2 == 0) << " " << letras[0] << letras[1] << letras[2] << " "
                    << "2020" + to_string(cont) << endl;
                letras[2]++;
                cont++;
            }

            letras[1]++;
            letras[2] = 'a';
        }
        letras[0]++;
        letras[1] = 'a';
    }
    arq << "s" << endl;
    arq << "d";
    arq.close();
    cout << cont;
    return 0;
}