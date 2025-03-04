#include <iostream>
#include <qapplication>
#include <qpushbutton>
#include "ui_main_window.h"
#include "test.h"

int main(int argc, char* argv[]) {

    QApplication app(argc, argv);

    QMainWindow window;

    Ui::MainWindow ui;
    ui.setupUi(&window);

    window.show();
    return app.exec();
}
