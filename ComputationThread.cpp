#include "ComputationThread.hpp"

ComputationThread::ComputationThread(SessionA* _sessionA, SessionB* _sessionB)
{
    state = 1;
    sessionA = _sessionA;
    sessionB = _sessionB;
}

void ComputationThread::exportFileA()
{
    state = 1;
    start();
}

void ComputationThread::importAExportB()
{
    state = 2;
    start();
}

void ComputationThread::run()
{
    if (state == 1) {
        sessionA->exportFile();
        emit fileAExported();
    }
    else if (state == 2) {
        sessionB->importFile();
        sessionB->exportFile();
        emit AImportedBExported();
    }
}
