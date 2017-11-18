#include "grassmann.hpp"

#include "assert.h"

vector<string> comb (int n, int k){
    std::vector<Loop *> arrs;  // represent the different variables in the for loops
    int r = 0;
    vector<string> result;
    string temp;

    /* Initialisation*/
    arrs.resize(n);
    for(int r=0; r<k; r++){
        arrs[r] = new Loop;
        arrs[r]->end = n-k+r;
    }
    arrs[0]->start = 0-1 ;
    arrs[0]->current = arrs[r]->start;


    while(true){
        if (r >= k-1){
            for(int i = arrs[r]->start+1; i<arrs[r]->end+1; i++){
                arrs[r]->current = i;
                temp = "";
                //afficherCombinaisons();
                for(int j=0; j<k; j++){
                    temp.append( std::to_string(arrs[j]->current) );
                }

                result.push_back(temp);

            }
            r--;
        }
        else{
            if (arrs[r]->current >= arrs[r]->end){
                if(r == 0){
                    return result;
                }
                arrs[r]->current = arrs[r]->start;
                r--;
            }
            else{
                arrs[r]->current++;
                r++;
                arrs[r]->start = arrs[r-1]->current;
                arrs[r]->current = arrs[r]->start;
            }
        }
    }

}


void displayStringVect(vector<string> vect){
    for (unsigned int i = 0; i<vect.size(); i++){
        cout<<endl;
        cout<<vect[i];
    }
    cout<<endl;
}


void display2Dim(vector<vector<long>> vect){
    for (unsigned int i = 0; i<vect.size(); i++){
        cout<<endl;

        for(unsigned int j=0; j<vect[i].size(); j++){
            cout<<vect[i][j]/*<<"   "<<j*/<<"   ";
#ifdef OUTPUT
            computationFormulas<<vect[i][j]<<"#";
#endif

        }
        cout<<endl;
    }
    cout<<endl;
}


//// Make overloading
vector<pairs> multipli2Vects(vector<pairs> row1, vector<pairs> row2, int mod){
    vector<pairs> result;
    for(unsigned int i=0; i<row1.size(); i++){
        for(unsigned int j=0; j<row2.size(); j++){

            row1[i].first = modulo(row1[i].first, mod);

            long val = (row1[i].first*row2[j].first);

            result.push_back(makePairs(val, row1[i].second + row2[j].second));
        }
    }
    for ( vector<pairs>::iterator it = result.begin() ; it != result.end();){
        if(checkRedonduncy(it->second)){
            result.erase(it);
        }
        else{
            ++it;
        }
    }

    return result;
}


void displayPairVector(std::vector<pairs> vector){

    for (const auto& p : vector)
    {
        std::cout << p.first << ", " << p.second << std::endl;
    }
}


bool checkRedonduncy(string str){       /*return true if redondecy found*/
    bool array[10] = {false};


    for (unsigned int i=0; i<str.size(); i++){
        if (array[str[i] - 48]){
            return true;
        }
        array[str[i] - 48] = true;
    }

    return false;
}


int maxIndex(vector<pairs> vector){
    int max = -1;
    for(unsigned int i = 0; i<vector.size(); i++){
        for(unsigned int j = 0; j<vector[i].second.size(); j++){
            int index = stringToInt(vector[i].second, j);
            max = std::max(max, index);

        }
    }
    return max;
}


long stringToInt(string str, int pos){
    char c = str[pos];
    int result = c-'0';
    return result;
}


vector<vector<long>> grassmanCalcul(vector<vector<long>> S, int k, int mod){

    vector<vector<long>> SI;
    int n = S.size();
    vector<pairs> row1;

    vector<pairs> row2;
    vector<string> combinaisons = comb(n, k);
    //displayStringVect(combinaisons);


    for(unsigned int h=0; h<combinaisons.size(); h++){

        vector<pairs> result;
        for (unsigned int a = 0; a<S[0].size(); a++){
            result.push_back(makePairs(S[stringToInt(combinaisons[h], 0)][a],std::to_string(a) ));
        }

        for(unsigned int i = 1; i<combinaisons[h].size(); i++){

            row1 = result;
            result.clear();
            row2.clear();

            for (unsigned int a = 0; a<S[0].size(); a++){
                row2.push_back(makePairs(S[stringToInt(combinaisons[h], i)][a],std::to_string(a) ));
            }

            result = multipli2Vects(row1, row2, mod);

            vector<string> combTemp = comb(maxIndex(result)+1, result[0].second.size());

            int size = result.size();
            for(int j = 0; j<size; ){
                if(checkExactConcordance(result[j].second  , combTemp) == -1){
                    int combinaisonNumber=checkIndexConcordance(result[j].second, combTemp);
                    if(combinaisonNumber!= -1){
                        int sign = parity(result[j].second, combTemp[combinaisonNumber]);

                        result = useSym(result, j,  sign, result[j].first, combTemp[combinaisonNumber], mod);
                        size--;
                    }
                    else{
                        j++;
                    }

                }
                else{
                    j++;
                }
            }
            result = putParenthesis(result);
        }

//        displayPairVector(result);
        vector<long> vectorTemp;                                               ///To be removed
        vectorTemp.clear();
        for(unsigned int k = 0; k<result.size(); k++){
            vectorTemp.push_back(result[k].first);
        }
        SI.push_back(vectorTemp);
        //SI = matrixModulo(SI, 37);
    }
    return SI;
}


int checkExactConcordance(string vector, std::vector<string> combinaisons){
    for(unsigned int i = 0; i<combinaisons.size(); i++){
        if(!vector.compare(combinaisons[i])){
            return i;
        }
    }
    return -1;
}


int checkIndexConcordance(string vector, std::vector<string> combinaisons){
    string sortedVector = vector;
    std::sort(sortedVector.begin(), sortedVector.end());
    for(unsigned int i=0; i<combinaisons.size(); i++){
        string sortedComb = combinaisons[i];
        std::sort(sortedComb.begin(), sortedComb.end());
        if(!sortedVector.compare(sortedComb)){
            return i;
        }
    }
    return -1;
}


int parity(string combinaison, string vector){
    int power=0;
    std::vector<long> alreadyRead;
    unsigned int i = 0;

    while(alreadyRead.size() < vector.size()){
        int j = i;
        int compteur = 0;
        do{

            compteur++;
            int k=0;
            alreadyRead.push_back(stringToInt(combinaison, j));
            while(combinaison[k] != vector[j]){
                k++;
            }
            j = k;


        }while(combinaison[i] != vector[j]);
        if (std::find(alreadyRead.begin(), alreadyRead.end(), stringToInt(combinaison, j)) == alreadyRead.end() ){
            compteur++;
            alreadyRead.push_back(stringToInt(combinaison, j));
        }
        i++;
        for(unsigned int l=0; l<alreadyRead.size(); l++){
        }
        while( ( std::find(alreadyRead.begin(), alreadyRead.end(), stringToInt(combinaison, i)) != alreadyRead.end() ) && ( i<combinaison.size()-1 ) ){
            i++;
        }

        power += compteur-1;
    }
    return pow(-1, power);
}


vector<pairs> useSym(std::vector<pairs> vector, int pos, int sign, long value, string comb, int mod){
    int i = 0;
    while(vector[i].second.compare(comb)!=0){
        i++;
    }
    vector[i].first = modulo(vector[i].first + sign*value, mod);
    std::vector<pairs>::iterator it = vector.begin()+pos;
    vector.erase(it);
    return vector;
}


pairs makePairs(long val, string vector){
    pairs newPair;
    newPair.first = val;
    newPair.second = vector;
    return newPair;
}


vector<vector<long> > matrixModulo(vector<vector<long> > matrix, int modulo){
    for(unsigned int i=0; i<matrix.size(); i++){
        for(unsigned int j=0; j<matrix[0].size(); j++){
            if(matrix[i][j]>=0){
                matrix[i][j] = matrix[i][j]%modulo;
            }
            else{
                matrix[i][j] =(modulo +  (matrix[i][j]%modulo)) %modulo;
            }
        }
    }
    return matrix;
}


vector<vector<long> > newMatrix(vector<vector<long>> in){
    vector<vector<long> > out(in[0].size(), vector<long>(in.size()));

    for(unsigned int i=0; i<in.size(); i++){
        for(unsigned int j=0; j<in[0].size(); j++){
            out[j][i] = in[i][j];
        }
    }
    return out;
}


Matrix vectToMat(vector<vector<long>> in){
    Matrix out("t22", in.size(), in[0].size());
    for(unsigned int i=0; i<in.size(); i++){
        for(unsigned int j=0; j<in[0].size(); j++){
            out.setElement(j,i,in[j][i]);
        }
    }
    return out;
}


vector<vector<long>> matToVect(Matrix in){
    vector<vector<long> > out(in.getN(), vector<long>(in.getM()));
    for(int i=0; i<in.getN(); i++){
        for(int j=0; j<in.getM(); j++){
            out[i][j] = in.getElement(i,j);
        }
    }
    return out;
}


vector<vector<string>> fillMatrix(int n, int MODE){

    char c= 97;
    int a=0;
    vector<vector<string> > result(n, vector<string>(n));
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            switch(MODE){
            case LETTER:
                result[i][j].push_back(c++);
                break;
            case NUMBER:
                result[i][j] = std::to_string(a++);
                break;
            case COORD:
                result[i][j] = std::to_string(i)+","+std::to_string(j);
                break;

            }
        }
    }
    return result;
}


vector<vector<string> > setParenthesis(vector<vector<string>> vect){
    for (unsigned int i = 0; i<vect.size(); i++){
        for(unsigned int j=0; j<vect[i].size(); j++){
            string pre = "(";
            string suf = ")";
            vect[i][j] = pre+vect[i][j]+suf;
        }
    }
    return vect;
}


vector<vector<vector<string>>> splitVector(vector<vector<string>> vect, int number){
    vector<vector<vector<string>>> result;
    number /=2 ;
    std::size_t const newSize = vect.size() / number;

    for (int i = 0; i< number; i++){
        vector< vector<string> >::iterator a = vect.begin()+i*newSize;
        vector< vector<string> >::iterator b = a+newSize;
        vector<vector<string>> split_temp1(a, b);
        vector<vector<string>> split_temp2(a, b);
        for(unsigned int j = 0; j<newSize; j++){
            vector<string>::iterator c = split_temp1[j].begin();
            vector<string>::iterator d = c+newSize;
            vector<string> split_temp3(c, d);
            split_temp1[j]= split_temp3;
        }
        result.push_back(split_temp1);

        for(unsigned int j = 0; j<newSize; j++){
            vector<string>::iterator c = split_temp2[j].begin()+newSize-1;
            vector<string>::iterator d = c+newSize;
            vector<string> split_temp3(c, d);
            split_temp2[j]= split_temp3;
        }
        result.push_back(split_temp2);
    }

    return result;
}


vector<pairs> putParenthesis(vector<pairs> in){
    return in;
}

long modulo(long in, int modulo){

            if(in>=0){
                in = in%modulo;
            }
            else{
                in =(modulo +  (in%modulo)) %modulo;
            }


    return in;
}
