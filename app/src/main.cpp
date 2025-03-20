#include <iostream>
#include <qapplication>
#include <qpushbutton>
#include <string>
#include "ui_main_window.h"
#include "test.h"

// TODO test stuff, delete
std::vector<std::string> v;
// end of test stuff

void inputHandle(std::string inp)
{

}

void tableWidgetUpdate(QTableWidget* tableWidget) 
{
    for (int i = 0; i < v.size(); i++) {
        QTableWidgetItem* twi = new QTableWidgetItem( QString::number(i) );
        tableWidget->setItem(i, 0, twi);
        twi = new QTableWidgetItem(QString::fromStdString(v[i]));
        tableWidget->setItem(i, 1, twi);
    }
    tableWidget->resizeColumnsToContents();

}

int main(int argc, char* argv[]) 
{
    int columnCount = 2;




    QApplication app(argc, argv);

    QMainWindow window;

    Ui::MainWindow ui;
    ui.setupUi(&window);

    QLineEdit* inputField = ui.inputFieldObject;
    QTableWidget* tableWidget = ui.tableWidget;
    QString inputText;

    tableWidget->setWindowTitle("Polynomials");

    QObject::connect(inputField, &QLineEdit::returnPressed, [&inputText, inputField, tableWidget]()
        {
            inputText = inputField->text();
            std::string s = inputText.toStdString();

            v.push_back(s);
            qDebug() << "Input:" << inputText;

            tableWidget->insertRow(tableWidget->rowCount());
            tableWidgetUpdate(tableWidget);

            inputField->clear();
            
        });

    window.show();
    return app.exec();
}
