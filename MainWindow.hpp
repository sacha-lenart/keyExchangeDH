#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QFormLayout>
#include <QComboBox>
#include <QGroupBox>
#include <QMainWindow>
#include <QMenuBar>
#include <QTextEdit>

#include "SessionA.hpp"
#include "SessionB.hpp"
#include "ComputationThread.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT // allow the use of personalized slots

    public:
        MainWindow();
        // Qt will automatically delete the widgets

    public slots:
        void chooseFile();
        void changeValue(int val);
        void startSessionA();
        void stopExportingA();
        void waitB();
        void startSessionB();
        void stopExportingB();
        void endB();
        void endA();
        void printHelp();
        void quitApp();
        void keySizeParameters(QString keySize);
        void securityParameters(int level);
        void setSelParameters(bool isChecked);
        void setSelFile(bool isChecked);

    private:
        // widgets
        QLabel *info;
        QLabel *security_level_label;
        QComboBox *key_size_cb;
        QSlider *security_level;
        QLabel *part_file_label_A;
        QLabel *file_label_A;
        QLabel *file_label_B;
        QPushButton *file_button_A;
        QPushButton *file_button_B;
        //QProgressBar *prog_bar_A;
        //QProgressBar *prog_bar_B;
        QPushButton *start_button_A;
        QPushButton *stop_button_A;
        QPushButton *start_button_B;
        QPushButton *stop_button_B;
        QPushButton *quit_button;
        QTextEdit *private_key_B;
        QTextEdit *private_key_A;

        // layouts
        QFormLayout *form;                  // form of system with parameters
        QVBoxLayout *right_layout_A;        // Alice right layout
        QHBoxLayout *security_level_layout; // slider + security_level_label
        QHBoxLayout *parameters_layout;     // system with parameters layout
        QGroupBox *parameters_box;          // allows to disable parameters_layout
        QVBoxLayout *file_layout_A;         // Alice loads Bob file
        QGroupBox *file_box_A;              // allows to disable file_layout_A
        QVBoxLayout *alice_layout;          // contains all Alice part
        QVBoxLayout *bob_layout;            // contains all Bob part
        QGroupBox *boxA;                    // system with parameters selected (Alice)
        QGroupBox *boxB;                    // system with file selected (Bob)
        QVBoxLayout *main_layout;           // contains boxA and boxB
        QHBoxLayout *start_stop_layout_A;   // start and stop buttons + progress bar, Alice
        QHBoxLayout *progress_layout_B;     // start and stop buttons + progress bar, Bob

        // menu
        QMenu *file_menu;
        QMenu *help_menu;
        void makeMenu();

        // other
        SessionA *sessionA;     // Alice's session
        SessionB *sessionB;     // Bob's session

        ComputationThread *cThread; // compute

};

#endif // MAINWINDOW_HPP
