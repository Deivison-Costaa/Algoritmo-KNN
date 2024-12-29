#include <iostream>
#include <math.h>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <random>
#include <fstream>
#include <sstream>

#define TAM_TREINAMENTO 0.8

using namespace std;

class Individuo
{
public:
    double sl, sw, pl, pw;
    string classe;

public:
    Individuo(double sl, double sw, double pl, double pw, string classe)
    {
        this->sl = sl;
        this->sw = sw;
        this->pl = pl;
        this->pw = pw;
        this->classe = classe;
    }

    string getClasse()
    {
        return classe;
    }

    double getSl()
    {
        return sl;
    }

    double getSw()
    {
        return sw;
    }

    double getPl()
    {
        return pl;
    }

    double getPw()
    {
        return pw;
    }

};

double distEuclidiana(Individuo ind1, Individuo ind2)
{
    double result = pow(ind1.getPl() - ind2.getPl(), 2) +
                    pow(ind1.getPw() - ind2.getPw(), 2) +
                    pow(ind1.getSl() - ind2.getSl(), 2) +
                    pow(ind1.getSw() - ind2.getSw(), 2);
    
    return sqrt(result);
}

string classificarAmostra(vector<Individuo> &dataSet, Individuo NovoIndividuo, int k)
{
    if(k %  2 == 0)
    {
        k--;
        if(k <= 0) k = 1;
    }

    int tamVec = dataSet.size();

    set<pair<double, int>> distIndividuos;

    for (int i = 0; i < tamVec; i++)
    {
        double dist = distEuclidiana(dataSet[i], NovoIndividuo);
        distIndividuos.insert(make_pair(dist, i));
    }

    set<pair<double, int> >::iterator it;

    vector<int> contClasses(3);

    int contK = 0;

    for(it = distIndividuos.begin(); it != distIndividuos.end(); it++)
    {
        if (contK == k) break;
        
        string classe = dataSet[it->second].getClasse();

        if(classe == "setosa")
			contClasses[0]++;
		else if(classe == "versicolor")
			contClasses[1]++;
		else
			contClasses[2]++;

		contK++;
    }

    string classificacao;

    if(contClasses[0] >= contClasses[1] && contClasses[0] >= contClasses[2]) classificacao = "setosa";
	else if(contClasses[1] >= contClasses[0] && contClasses[1] >= contClasses[2]) classificacao = "versicolor";
	else classificacao = "virginica";

    return classificacao;  
}

int main() {


    //vetor que armazena dados no CSV
    vector<Individuo> dataSet;

    //leitura dos dados
    ifstream arquivo("iris.csv");
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo" << endl;
        return 1;
    }

    string linha;
    while (getline(arquivo, linha)) {

        stringstream ss(linha);
        string sl, sw, pl, pw, classe;

        // Lê cada atributo separado por vírgula
        //sepal_length,sepal_width,petal_length,petal_width,species

        getline(ss, sl, ',');     // sepal length
        getline(ss, sw, ',');     // sepal width
        getline(ss, pl, ',');     // petal length
        getline(ss, pw, ',');     // petal width
        getline(ss, classe, ','); // specie

        dataSet.push_back(Individuo(stod(sl), stod(sw), stod(pl), stod(pw), classe));
    }
    arquivo.close();

    //Isso aqui faz com que os dados que estão organizados em setosa | versicolor | virginica sejam misturados.
    random_device rd;
    mt19937 g(rd());
    shuffle(dataSet.begin(), dataSet.end(), g);

    //Separar treinamento de teste
    size_t total = dataSet.size();
    size_t tamTreinamento = (size_t)(TAM_TREINAMENTO * total);
    size_t tamTeste = total - tamTreinamento;

    vector<Individuo> treinamento(dataSet.begin(), dataSet.begin() + tamTreinamento);
    vector<Individuo> teste(dataSet.begin() + tamTreinamento, dataSet.end());


    int acertos = 0;
    int K = 3;  // Quantidade de vizinhos

    for (size_t i = 0; i < teste.size(); i++) {

        Individuo amostra = teste[i];

        string classe_obtida = classificarAmostra(treinamento, amostra, K);

        cout << "Amostra " << i + 1 << ":\n";
        cout << "   Classe esperada: " << amostra.classe << "\n";
        cout << "   Classe obtida:   " << classe_obtida << "\n\n";

        if (amostra.classe == classe_obtida) {
            acertos++;
        }
    }


    cout << "Total de amostras de teste: " << tamTeste << endl;
    cout << "Acertos: " << acertos << endl;
    double taxaAcerto = (static_cast<double>(acertos) / tamTeste) * 100.0;
    cout << "Taxa de acerto: " << taxaAcerto << "%\n";

    return 0;
}