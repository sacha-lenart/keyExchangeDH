#include <fstream>

#include "SessionB.hpp"
#include "parser.hpp"
#include "grassmann.hpp"

SessionB::SessionB()
{
    aliceFile = "none";
}

string SessionB::getKeyB()
{
    return keyB;
}

void SessionB::setAliceFile(string file_name)
{
    aliceFile = file_name;
}

string SessionB::getAliceFile()
{
    return aliceFile;
}


void SessionB::importFile()
{
    std::ifstream alice_file(aliceFile, ios::in);

    if (alice_file)
    {
        // main parameters
        char tmp;

        alice_file >>n>>tmp>>k>>tmp>>key_size>>tmp>>mod>>tmp>>w_size>>tmp>>securityLevel;

        // matrices
        // Q
        string Q_line;
        getline(alice_file, Q_line); // line break, ignored
        getline(alice_file, Q_line);
        Q = new Matrix("Q", n, n);
        Q->lineToMatrix(Q_line);
        // R
        string R_line;
        getline(alice_file, R_line);
        R = new Matrix("R", n, n);
        R->lineToMatrix(R_line);
        // W
        string W_line;
        getline(alice_file, W_line);
        W = new Matrix("W", w_size, w_size);
        W->lineToMatrix(W_line);
        // WA
        string WA_line;
        getline(alice_file, WA_line);
        WA = new Matrix("WA", w_size, w_size);
        WA->lineToMatrix(WA_line);

        // close file
        alice_file.close();
    }
    else
    {
        cerr << "Error : impossible to open file.\n";
    }
}

string SessionB::exportFile()
{
    // Bob chooses 2 polynomials from Q and R
    Matrix FBQ("FB(Q)", n, n);
    FBQ = Q->randomPolynomial(10, mod);
    Matrix GBR("GB(R)", n, n);
    GBR = R->randomPolynomial(10, mod);

    // change of basis in Grassmann algebra
    // FB(Q)
    Matrix FBQ_grassmann("FBQ_g", w_size, w_size);
    //FBQ_grassmann = parser("10-9.txt", FBQ);
    int i;
    FBQ_grassmann = vectToMat(grassmanCalcul(matToVect(FBQ), k, mod));
    FBQ_grassmann.modulo(mod);
    for(i=1; i<securityLevel; i++){
        FBQ_grassmann = vectToMat(grassmanCalcul(matToVect(FBQ_grassmann), k, mod));
        FBQ_grassmann.modulo(mod);
    }
    // GB(R)
    Matrix GBR_grassmann("GBR_g", w_size, w_size);
    //GBR_grassmann = parser("10-9.txt", GBR);
    GBR_grassmann = vectToMat(grassmanCalcul(matToVect(GBR), k, mod));
    GBR_grassmann.modulo(mod);
    for(i=1; i<securityLevel; i++){
        GBR_grassmann = vectToMat(grassmanCalcul(matToVect(GBR_grassmann), k, mod));
        GBR_grassmann.modulo(mod);
    }

    // Bob chooses 2 new polynomials
    // UA
    UB = new Matrix("UB", w_size, w_size);
    *UB = FBQ_grassmann.randomPolynomial(10, mod);
    // VA
    VB = new Matrix("VB", w_size, w_size);
    *VB = GBR_grassmann.randomPolynomial(10, mod);

    // Bob computes his final matrix
    WB = new Matrix("WB", w_size, w_size);
    *WB = UB->mult(*W, mod);
    *WB = WB->mult(*VB, mod);

    // he puts this matrix in a file for Alice
    string output_file = aliceFile;
    output_file.erase (output_file.end()-5, output_file.end()); // remove ".alice"
    output_file += "bob";
    std::ofstream bob_file(output_file, ios::trunc);
    if(bob_file) {
        // parameters for Alice
        bob_file <<n<<"/"<<k<<"/"<<key_size<<"/"<<mod<<"/"<<w_size<<'\n';
        bob_file << WB->toLine();
        bob_file.close();
    }
    else {
        cerr << "Error : impossible to open file.\n";
    }

    // Bob computes the common private key
    KB = new Matrix("KB", w_size, w_size);
    *KB = UB->mult(*WA, mod);
    *KB = KB->mult(*VB, mod);

    cout << *KB;

    keyB = "";
    for(int i=0; i<key_size; i++) {
        keyB += to_string(KB->getElement(i));
        keyB += " ";
    }

    return output_file;
}
