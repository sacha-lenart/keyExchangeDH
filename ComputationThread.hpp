#ifndef COMPUTATIONTHREAD_HPP
#define COMPUTATIONTHREAD_HPP

#include <QThread>
#include <QPushButton>

#include "SessionA.hpp"
#include "SessionB.hpp"

class ComputationThread : public QThread
{
    Q_OBJECT

    public:
        ComputationThread(SessionA* _sessionA, SessionB* _sessionB);
        // overriding the QThread's run() method
        void run();

    public slots :
        void exportFileA();
        void importAExportB();

    signals :
        void fileAExported();
        void AImportedBExported();


    private :
        int state;
        SessionA* sessionA;
        SessionB* sessionB;

};

#endif // COMPUTATIONTHREAD_HPP
