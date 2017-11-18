#ifndef SESSIONA_HPP
#define SESSIONA_HPP

#include <QUuid>
#include <string>

#include "Matrix.hpp"
#include "parser.hpp"

class SessionA
{

    public:
        SessionA();
        void exportFile();
        bool importFile();

        void securityParameters(int security_level);
        void keySizeParameters(QString size);

        string getKeyA();
        string getId();
        void setBobFile(string file_name);
        string getBobFile();

    private:
        string id;          // session's id, string containing QUuid object
        int n;              // size of A and B matrices
        int k;              // value for Grassmann change of basis
        int key_size;       // size of the key (bytes)
        int securityLevel;  // number of grassmann change of basis
        int w_size;
        int mod;            // modulo chosen

        Matrix *Q, *R, *W;  // public matrices
        Matrix *UA, *VA, *WA; // Alice's matrices

        Matrix *WB;           // matrix sent by Bob in the bobFile
        string bobFile;      // name of the file sent by Bob

        Matrix *KA;           // private key
        string keyA;          // string containing the private key

};

#endif // SESSIONA_HPP
