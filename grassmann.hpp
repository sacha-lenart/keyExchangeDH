#ifndef GRASSMANN_HPP
#define GRASSMANN_HPP

#include <fstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <alloca.h>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <math.h>
#include <QString>
#include <QObject>
#include <QJSEngine>
#include <regex>

#include "Matrix.hpp"

class Loop {
    public:
        long start;
        long end;
        long current;
};

class pairs{
    public:
        long first;
        std::string second;
};

#define LETTER  5563
#define NUMBER  5564
#define COORD   5565

using namespace std;
vector<string> comb(int n, int k);
void displayStringVect(vector<string> vect);
void display2Dim(vector<vector<long>> vect);
void calcul();
vector<pairs> multipli2Vects(vector<pairs> row1, vector<pairs> row2);
void displayPairVector(std::vector<pairs> vector);
bool checkRedonduncy(string str);
int maxIndex(vector<pairs> vector);
long stringToInt(string str, int pos);
vector<vector<long>> grassmanCalcul(vector<vector<long>> S, int k, int mod);
int checkExactConcordance(string vector, std::vector<string> combinaisons);
int checkIndexConcordance(string vector, std::vector<string> combinaisons);
int parity(string combinaison, string vector);
vector<pairs> useSym(std::vector<pairs> vector, int pos, int sign, long value, string comb, int mod);
//vector<pairs> useSym(std::vector<pairs> vector, int pos, int sign, std::string value, string comb);
pairs makePairs(long val, string vector);
vector<vector<long> >  matrixModulo(vector<vector<long> > matrix, int modulo);
vector<vector<long> > newMatrix(vector<vector<long>> in);
Matrix vectToMat(vector<vector<long>> in);
vector <vector<long>> matToVect(Matrix in);
vector<vector<string> > fillMatrix(int n, int MODE);
vector<vector<string> > setParenthesis(vector<vector<string>> vect);
vector<vector<vector<string>>>  splitVector(vector<vector<string>> vect, int number);
vector<pairs> putParenthesis(vector<pairs> in);
long modulo(long in, int modulo);

#endif // GRASSMANN_HPP
