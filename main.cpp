#include "main.hpp"

#define  OUTPUT 1

#ifdef OUTPUT
std::ofstream computationFormulas("computationFormulas.txt", std::ios::out | std::ios::trunc);
#endif

char display[] = {'1','2','3','4','5','6'};

int main(int argc, char *argv[]) {

    srand(time(NULL));

    // start application
    QApplication app(argc, argv);

    // main window
    MainWindow main_wind;

    // display main window
    main_wind.show();

    // end application
    return app.exec();

}
