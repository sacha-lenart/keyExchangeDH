#ifndef SESSIONB_HPP
#define SESSIONB_HPP

#include <QWidget>
#include <QUuid>
#include <string>

#include "Matrix.hpp"

class SessionB
{
    public:
        SessionB();

        string exportFile();
        void importFile();

        string getKeyB();
        void setAliceFile(string file_name);
        string getAliceFile();

    private:
        string id;          // session's id, string containing QUuid object
        int n;              // size of A and B matrices
        int k;              // value for Grassmann change of basis
        int key_size;       // size of the key (bits)
        int securityLevel;  // number of grassmann change of basis
        int w_size;
        int mod;            // modulo chosen

        Matrix *UB, *VB, *WB;   // Bob's matrices
        Matrix *Q, *R, *W;      // public matrices

        Matrix *WA;             // matrix sent by Alice in the aliceFile
        string aliceFile;      // name of the file sent by Alice

        Matrix *KB;          // private key
        string keyB;         // string containing the private key
};

#endif // SESSIONB_HPP
