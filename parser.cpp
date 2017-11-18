#include "parser.hpp"

Matrix parser(string inputFile, Matrix M)
{
    std::ifstream compFormIn(inputFile, std::ios::in);
    Matrix Result("tmp", sqrt(M.getSize()), sqrt(M.getSize()));
    if (compFormIn) {
        int i = 0;
        QJSEngine expression;
        for (int j = 0; j<M.getSize(); j++){
            expression.globalObject().setProperty(QString::number(j), (int)M.getElement(j));
        }
        while(i<1){
            string buffer;
            getline (compFormIn, buffer,'#');
            string ht("#");
            QString expression_string = QString::fromStdString(buffer);
            int my_val=expression.evaluate(expression_string).toNumber();
            Result.setElement(i, my_val);
            i++;
        }
    }
    return Result;
}

int moduloInt(int a, int modulo){
    if(a>=0){
        return a%modulo;
    }
    else{
        return modulo-1-((-a-1)%modulo);
    }
}


void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}
