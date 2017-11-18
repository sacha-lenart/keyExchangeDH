#include "MainWindow.hpp"
#include <iostream>

#include <QFileDialog>
#include <QMessageBox>
#include <string>

MainWindow::MainWindow() : QMainWindow()
{
    // Sessions
    sessionA = new SessionA(); // Alice
    sessionB = new SessionB(); // Bob



    //********************* ALICE *********************

    // information
    info = new QLabel("Select first size of the key\n needed. Then select the\n level of security wanted,\n between 1 and 5.\nOnly the second parameter\n will affect computing time.");

    // key size
    key_size_cb = new QComboBox();
    key_size_cb->addItem("64");
    key_size_cb->addItem("128");
    key_size_cb->addItem("256");
    key_size_cb->addItem("512");
    QObject::connect(key_size_cb, SIGNAL(currentIndexChanged(QString)), this, SLOT(keySizeParameters(QString)));

    // security level
    security_level = new QSlider(Qt::Horizontal);
    security_level->setRange(1, 5);
    security_level->setTickPosition(QSlider::TicksBelow);
    security_level->setTickInterval(1);
    security_level_label = new QLabel("1");
    QObject::connect(security_level, SIGNAL(valueChanged(int)), this, SLOT(changeValue(int)));
    QObject::connect(security_level, SIGNAL(valueChanged(int)), this, SLOT(securityParameters(int)));

    // security level layout
    security_level_layout = new QHBoxLayout();
    security_level_layout->addWidget(security_level);
    security_level_layout->addWidget(security_level_label);

    // start button, Alice system
    start_button_A = new QPushButton("Start");
    start_button_A->setToolTip("Start a new session by generating a new key for the two users");
    start_button_A->setMinimumWidth(80);
    connect(start_button_A, SIGNAL(clicked()), this, SLOT(startSessionA()));

    // stop button, Alice system
    stop_button_A = new QPushButton("Stop");
    stop_button_A->setToolTip("Stop generating key");
    stop_button_A->setMinimumWidth(80);
    QObject::connect(stop_button_A, SIGNAL(clicked()), this, SLOT(stopExportingA()));
    // first, stop button is disabled
    stop_button_A->setFlat(true);
    stop_button_A->setEnabled(false);

    // Alice progress bar
    //prog_bar_A = new QProgressBar();
    //prog_bar_A->hide(); // will be shown when computation start

    // start and stop layout, Alice system
    start_stop_layout_A = new QHBoxLayout();
    start_stop_layout_A->setAlignment(Qt::AlignRight);
    start_stop_layout_A->addWidget(start_button_A);
    start_stop_layout_A->addWidget(stop_button_A);

    // add security level and key size to the form layout
    form = new QFormLayout();
    form->addRow("Key size (bits)", key_size_cb);
    form->addRow("Security level", security_level_layout);

    right_layout_A = new QVBoxLayout();
    right_layout_A->addLayout(form);
    //right_layout_A->addWidget(prog_bar_A);

    // add text and form in horizontal layout
    parameters_layout = new QHBoxLayout();
    parameters_layout->addWidget(info);
    parameters_layout->addLayout(right_layout_A);
    parameters_box = new QGroupBox();
    parameters_box->setLayout(parameters_layout);

    // Alice loads Bob file
    part_file_label_A = new QLabel("Select Bob file");
    file_label_A = new QLabel("file selected : none");
    file_button_A = new QPushButton("Browse file");
    file_button_A->setMaximumWidth(100);
    QObject::connect(file_button_A, SIGNAL(clicked()), this, SLOT(chooseFile()));
    private_key_A = new QTextEdit("Private key : -");
    private_key_A->setReadOnly(true);
    file_layout_A = new QVBoxLayout();
    file_layout_A->addWidget(part_file_label_A);
    file_layout_A->addWidget(file_label_A);
    file_layout_A->addWidget(file_button_A, 0, Qt::AlignRight);
    file_layout_A->addWidget(private_key_A);

    // final vertical layout
    alice_layout = new QVBoxLayout();
    alice_layout->addWidget(parameters_box);
    file_box_A = new QGroupBox();       // allow to disable (impossible with a layout)
    file_box_A->setLayout(file_layout_A);
    file_box_A->setDisabled(true);
    alice_layout->addWidget(file_box_A);
    alice_layout->addLayout(start_stop_layout_A);



    //******************** BOB **********************

    // select file label and button
    file_label_B = new QLabel("selected file : none");
    file_label_B->setMaximumWidth(300);
    file_button_B = new QPushButton("Browse file");
    file_button_B->setToolTip("Select the file containing the generating code");
    file_button_B->setMaximumWidth(100);
    QObject::connect(file_button_B, SIGNAL(clicked()), this, SLOT(chooseFile()));

    // start button, Bob system
    start_button_B = new QPushButton("Start");
    start_button_B->setToolTip("Start a new session by generating a new key for the two users");
    start_button_B->setMaximumWidth(80);
    QObject::connect(start_button_B, SIGNAL(clicked()), this, SLOT(startSessionB()));

    // stop button, Bob system
    stop_button_B = new QPushButton("Stop");
    stop_button_B->setToolTip("Stop generating key");
    stop_button_B->setMaximumWidth(80);
    QObject::connect(stop_button_B, SIGNAL(clicked()), this, SLOT(stopExportingB()));
    // first, stop button is disabled
    stop_button_B->setFlat(true);
    stop_button_B->setEnabled(false);

    // Bob progress bar
    //prog_bar_B = new QProgressBar();
    //prog_bar_B->hide(); // will be shown when computation start

    // start and stop + prog bar layout, Bob system
    progress_layout_B = new QHBoxLayout();
    progress_layout_B->setAlignment(Qt::AlignRight);
    //progress_layout_B->addWidget(prog_bar_B);
    progress_layout_B->addWidget(start_button_B);
    progress_layout_B->addWidget(stop_button_B);

    // private key label
    private_key_B = new QTextEdit("Private key : -");
    private_key_B->setReadOnly(true);

    // select file layout
    bob_layout = new QVBoxLayout();
    bob_layout->addWidget(file_label_B);
    bob_layout->addWidget(file_button_B, 0, Qt::AlignRight);
    bob_layout->addWidget(private_key_B);
    bob_layout->addLayout(progress_layout_B);



    //******************** MAIN ********************

    // A-system
    boxA = new QGroupBox("Alice");
    boxA->setProperty("mandatoryField", true);
    boxA->setLayout(alice_layout);
    boxA->setCheckable(true);
    boxA->setChecked(true);
    QObject::connect(boxA, SIGNAL(clicked(bool)), this, SLOT(setSelParameters(bool)));

    // B-system
    boxB = new QGroupBox("Bob");
    boxB->setProperty("mandatoryField", true);
    boxB->setLayout(bob_layout);
    boxB->setCheckable(true);
    boxB->setChecked(false);
    QObject::connect(boxB, SIGNAL(clicked(bool)), this, SLOT(setSelFile(bool)));

    // quit button
    quit_button = new QPushButton("Quit");
    quit_button->setMaximumWidth(80);
    QObject::connect(quit_button, SIGNAL(clicked()), this, SLOT(quitApp()));

    // main layout
    main_layout = new QVBoxLayout();
    main_layout->addWidget(boxA);
    main_layout->addWidget(boxB);
    main_layout->addWidget(quit_button, 0, Qt::AlignRight); // put button on the right

    // stylesheet
    QFile File(QCoreApplication::applicationDirPath() + "/styleSheet.qss");
    File.open(QFile::ReadOnly);
    qApp->setStyleSheet(QLatin1String(File.readAll()));

    // menu
    this->makeMenu();

    // add main_layout to the QMainWindow
    QWidget *main_widget = new QWidget();
    // first we put the main layout in a widget,
    // because we can't add a layout to a QMainWindow
    main_widget->setLayout(main_layout);
    // then we add the widget to the window
    this->setCentralWidget(main_widget);

    // allows to access the window while computing matrices
    cThread = new ComputationThread(sessionA, sessionB);
    connect(cThread, SIGNAL(fileAExported()), this, SLOT(waitB()));
    connect(cThread, SIGNAL(AImportedBExported()), this, SLOT(endB()));
}

void MainWindow::keySizeParameters(QString keySize)
{
    sessionA->keySizeParameters(keySize);
}

void MainWindow::securityParameters(int level)
{
    sessionA->securityParameters(level);
}

void MainWindow::setSelParameters(bool isChecked)
{
    if(isChecked)
        boxB->setChecked(false);
    else boxB->setChecked(true);
}

void MainWindow::setSelFile(bool isChecked)
{
    if(isChecked)
        boxA->setChecked(false);
    else boxA->setChecked(true);
}

void MainWindow::makeMenu()
{
    // File
    file_menu = menuBar()->addMenu("&File");
    QAction *exit_action = new QAction("&Exit", this);
    file_menu->addAction(exit_action);
    connect(exit_action, SIGNAL(triggered()), this, SLOT(quitApp()));

    // Help
    help_menu = menuBar()->addMenu("&Help");
    QAction *print_help = new QAction("&Print help", this);
    help_menu->addAction(print_help);
    connect(print_help, SIGNAL(triggered()), this, SLOT(printHelp()));
}

void MainWindow::changeValue(int val)
{
    switch (val) {
        case 1:
            security_level_label->setText("1"); break;
        case 2:
            security_level_label->setText("2"); break;
        case 3:
            security_level_label->setText("3"); break;
        case 4:
            security_level_label->setText("4"); break;
        default:
            security_level_label->setText("5");
    }
}

void MainWindow::chooseFile()
{
    QString file_name;
    // 1st case : Bob is loading Alice file
    if(boxB->isChecked()) {
        file_name = QFileDialog::getOpenFileName(this, "Open file", QString(), "Alice (*.alice)");
    }
    // 2nd case : Alice is loading Bob file
    else {
        file_name = QFileDialog::getOpenFileName(this, "Open file", QString(), "Bob (*.bob)");
    }

    if(file_name == "") {
        file_name = "none";
    }

    QMessageBox::information(this, "File", "You selected :\n" + file_name);
    QFileInfo file_info(file_name);
    QString file_name_short = file_info.fileName();
    QString text = "selected file : ";

    // 1st case : Bob is loading Alice file
    if(boxB->isChecked()) {
        file_label_B->setText(text + file_name_short);
        sessionB->setAliceFile(file_name_short.toStdString());
    }
    // 2nd case : Alice is loading Bob file
    else {
        file_label_A->setText(text + file_name_short);
        sessionA->setBobFile(file_name_short.toStdString());
    }
}

void MainWindow::startSessionA()
{
    if (parameters_box->isEnabled()) {
        // prog_bar->show();
        start_button_A->setFlat(true);
        start_button_A->setDisabled(true);
        stop_button_A->setFlat(false);
        stop_button_A->setDisabled(false);
        boxB->setEnabled(false);

        cThread->exportFileA();
    }
    else {
        if (sessionA->getBobFile() == "none") {
            QMessageBox::warning(this, "Error", "Please select a generating file");
        }
        else {
            start_button_A->setFlat(true);
            start_button_A->setDisabled(true);
            stop_button_A->setFlat(false);
            stop_button_A->setDisabled(false);

            bool ok = sessionA->importFile();

            if (!ok) {
                QMessageBox::warning(this, "Error", "The file selected doesn't correspond to the session");
                start_button_A->setFlat(false);
                start_button_A->setDisabled(false);
                stop_button_A->setFlat(true);
                stop_button_A->setDisabled(true);
            }
            else {
                endA();
            }
         }
    }
}

void MainWindow::waitB()
{
    // prog_bar_A->hide();
    start_button_A->setFlat(false);
    start_button_A->setEnabled(true);
    stop_button_A->setFlat(true);
    stop_button_A->setEnabled(false);
    parameters_box->setDisabled(true);
    file_box_A->setDisabled(false);
    boxB->setEnabled(true);
}

void MainWindow::stopExportingA()
{
    // prog_bar_A->hide();
    start_button_A->setFlat(false);
    start_button_A->setEnabled(true);
    stop_button_A->setFlat(true);
    stop_button_A->setEnabled(false);
    boxB->setEnabled(true);
    //cThread->wait();
}

void MainWindow::startSessionB()
{
    if (sessionB->getAliceFile() == "none") {
        QMessageBox::warning(this, "Error", "Please select a generating file");
    }
    else {
        start_button_B->setFlat(true);
        start_button_B->setEnabled(false);
        stop_button_B->setFlat(false);
        stop_button_B->setEnabled(true);

        cThread->importAExportB();
    }
}

void MainWindow::endB()
{
    // prog_bar_B->hide();
    start_button_B->setFlat(false);
    start_button_B->setEnabled(true);
    stop_button_B->setFlat(true);
    stop_button_B->setEnabled(false);

    QString key_str = QString::fromStdString(sessionB->getKeyB());
    private_key_B->setText("Private Key : " + key_str);
}

void MainWindow::stopExportingB()
{
    // prog_bar_B->hide();
    start_button_B->setFlat(false);
    start_button_B->setEnabled(true);
    stop_button_B->setFlat(true);
    stop_button_B->setEnabled(false);
}

void MainWindow::endA()
{
    start_button_A->setFlat(false);
    start_button_A->setDisabled(false);
    stop_button_A->setFlat(true);
    stop_button_A->setDisabled(true);

    QString key_str = QString::fromStdString(sessionA->getKeyA());
    private_key_A->setText("Private Key : " + key_str);
}


void MainWindow::printHelp()
{
    QString s1 = "Welcome to help page.\n";
    QString s2 = "First, you need to generate with Alice matrices.\n";
    QString s3 = "In the system A, you must choose the size of the key and the\n level of security needed.\n";
    QString s4 = "Then, select with Bob the file generated by Alice\n containing datas to generate the key.\n";
    QString s5 = "Once is done, press start button to begin computation.\n";
    QString s6 = "With Alice, import the file generated by Bob.\n";
    QString s7 = "You can exit this program by clicking on quit button.\n";
    QString help_text = s1+s2+s3+s4+s5+s6+s7;
    QMessageBox::information(this, "Help", help_text);
}

void MainWindow::quitApp()
{
    if(start_button_A->isFlat()) {
        int ans = QMessageBox::question(this, "Warning", "Do you really want to quit this program?", QMessageBox::Yes | QMessageBox::No);
        if (ans == QMessageBox::Yes) qApp->quit();
    }
    else
        qApp->quit();
}










