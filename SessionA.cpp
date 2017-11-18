#include <fstream>

#include "SessionA.hpp"
#include "grassmann.hpp"

SessionA::SessionA()
{
    n = 10;
    k = 9;
    key_size = 8; // min : 8 bytes = 64 bits
    mod = 257;
    bobFile = "none";
    securityLevel = 1;
}


void SessionA::securityParameters(int security_level)
{
    securityLevel = security_level;
}

void SessionA::keySizeParameters(QString size)
{
    key_size = size.toInt()/8; // convert in bytes
}

string SessionA::getKeyA()
{
    return keyA;
}

string SessionA::getId()
{
    return id;
}

void SessionA::setBobFile(string file_name)
{
    bobFile = file_name;
}

string SessionA::getBobFile()
{
    return bobFile;
}

int fact(int n) {
    return n > 1 ? n * fact(n-1) : 1;
}


void SessionA::exportFile()
{
    // select public matrices
    Q = new Matrix("Q", n, n);
    Q->random(mod, 0, 0);
    R = new Matrix("R", n, n);
    R->random(mod, 0, 0);
    w_size = fact(n)/(fact(k)*fact(n-k));
    W = new Matrix("W", w_size, w_size);
    W->random(mod, 0, 0);

    // Alice chooses 2 polynomials from Q and R
    Matrix FAQ("FA(Q)", n, n);
    FAQ = Q->randomPolynomial(10, mod);
    Matrix GAR("GA(R)", n, n);
    GAR = R->randomPolynomial(10, mod);

    // change of basis in Grassmann algebra
    // F(Q)
    Matrix FAQ_grassmann("FAQ_g", w_size, w_size);
    //FAQ_grassmann = parser("10-9.txt", FAQ); // parse version
    int i;
    FAQ_grassmann = vectToMat(grassmanCalcul(matToVect(FAQ), k, mod));
    FAQ_grassmann.modulo(mod);
    for(i=1; i<securityLevel; i++) {
        FAQ_grassmann = vectToMat(grassmanCalcul(matToVect(FAQ_grassmann), k, mod));
        FAQ_grassmann.modulo(mod);
    }

    // G(R)
    Matrix GAR_grassmann("GAR_g", w_size, w_size);
    //GAR_grassmann = parser("10-9.txt", GAR); // parse version
    GAR_grassmann = vectToMat(grassmanCalcul(matToVect(GAR), k, mod));
    GAR_grassmann.modulo(mod);
    for(i=1; i<securityLevel; i++) {
        GAR_grassmann = vectToMat(grassmanCalcul(matToVect(GAR_grassmann), k, mod));
        GAR_grassmann.modulo(mod);
    }

    // Alice chooses 2 new polynomials
    // UA
    UA = new Matrix("UA", w_size, w_size);
    *UA = FAQ_grassmann.randomPolynomial(10, mod);
    // VA
    VA = new Matrix("VA", w_size, w_size);
    *VA = GAR_grassmann.randomPolynomial(10, mod);

    // Alice computes her final matrix
    WA = new Matrix("WA", w_size, w_size);
    *WA = UA->mult(*W, mod);
    *WA = WA->mult(*VA, mod);

    // id for this new session
    QUuid uuid = QUuid::createUuid();
    id = uuid.toString().toStdString(); // convert in QString and then in string
    id.erase(remove(id.begin(), id.end(), '-'), id.end()); // remove '-'
    id.erase(0, 1);             // remove '{'
    id.erase(id.size() - 1);    // remove '}'
    std::ofstream alice_file(id + ".alice", ios::trunc);

    // Alice puts all parameters needed in a file
    if(alice_file) {
        alice_file <<n<<"/"<<k<<"/"<<key_size<<"/"<<mod<<"/"<<w_size<<"/"<<securityLevel<<'\n';
        alice_file << Q->toLine() <<'\n';
        alice_file << R->toLine() <<'\n';
        alice_file << W->toLine() <<'\n';
        alice_file << WA->toLine() <<'\n';
        alice_file.close();
    }
    else {
        cerr << "Error : impossible to open file.\n";
    }
}


bool SessionA::importFile()
{
    std::ifstream bob_file(bobFile, ios::in);
    string bob_id = bobFile;
    bob_id.erase(bob_id.end()-4, bob_id.end()); // remove ".bob"

    if (bob_file)
    {
        // checking the Bob file is from the right session
        bobFile.erase();
        if (bob_id != id) {
            return false;
        }

        // parameters
        char tmp;
        bob_file >>n>>tmp>>k>>tmp>>key_size>>tmp>>mod>>tmp>>w_size;

        // recover Bob's final matrix (WB)
        string line;
        getline(bob_file, line); // line break, ignored
        getline(bob_file, line);
        WB = new Matrix("WB", w_size, w_size);
        WB->lineToMatrix(line);

        // close file
        bob_file.close();
    }
    else
    {
        cerr << "Error : impossible to open file.\n";
        return false;
    }

    KA = new Matrix("KA", w_size, w_size);
    *KA = UA->mult(*WB, mod);
    *KA = KA->mult(*VA, mod);

    keyA = "";
    for(int i=0; i<key_size; i++) {
        keyA += to_string(KA->getElement(i));
        keyA += " ";
    }

    return true;
}


















