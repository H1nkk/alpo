#include <iostream>
#include <qapplication>
#include <qpushbutton>
#include "test.h"

int main(int argc, char* argv[]) {

    QApplication app(argc, argv);

    QPushButton button("Click");
    button.resize(200, 500);
    button.show();

    return app.exec();
}
