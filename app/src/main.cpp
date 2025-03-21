#include <iostream>
#include <qapplication>
#include <qpushbutton>
#include <string>
#include "ui_main_window.h"
#include "test.h"
#include "table.h"

// TODO test stuff, delete
std::vector<std::string> v;
int zavtraCount = 0;
void foo(QTextEdit* outputField) { outputField->setHtml(QString::fromStdString(std::string("To a future of grief #") + std::to_string(++zavtraCount))); }
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

void deleteAction(QTableWidget* tableWidget, int row) 
{
   
    if (row >= 0 && row < v.size()) {
        int rowInd = row;
        tableWidget->removeRow(row);
        qDebug() << "Row " << rowInd << " deleted";
        v.erase(std::next(v.begin(), row));
    }
    else {
        if (row != -1) // no selection
            throw "something's off with deleting";
    }
}

void clearAction(QTableWidget* tableWidget) 
{
    v.clear();
    qDebug() << "Cleared";
    tableWidget->setRowCount(0);
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
    QPushButton* deleteButton = ui.deleteButton;
    QPushButton* clearButton = ui.clearButton;
    QTextEdit* outputField = ui.outputField;
    QString inputText;

    tableWidget->setWindowTitle("Polynomials");

    QObject::connect(inputField, &QLineEdit::returnPressed, [&inputText, inputField, tableWidget, outputField]()
        {
            inputText = inputField->text();
            std::string s = inputText.toStdString();

            v.push_back(s);
            qDebug() << "Input:" << inputText;

            inputHandle(s);

            tableWidget->insertRow(tableWidget->rowCount());
            tableWidgetUpdate(tableWidget);

            inputField->clear();
            
            foo(outputField);
        });

    QObject::connect(tableWidget, &QTableWidget::customContextMenuRequested, [&](const QPoint &pos) {

        QTableWidgetItem* item = tableWidget->itemAt(pos);
        if (item) {
            QMenu menu;

            QAction* buttonDeleteAction = menu.addAction("Delete");

            QObject::connect(buttonDeleteAction, &QAction::triggered, [tableWidget, item]()
                {
                    int row = item->row();
                    deleteAction(tableWidget, row);

                    tableWidget->clearSelection();
                    tableWidgetUpdate(tableWidget);
                });

            menu.exec(tableWidget->viewport()->mapToGlobal(pos));
        }
        });

    QPushButton::connect(deleteButton, &QPushButton::clicked, [tableWidget]()
        {
            int row = tableWidget->selectionModel()->currentIndex().row();
            deleteAction(tableWidget, row);

            tableWidget->clearSelection();
            tableWidgetUpdate(tableWidget);
        });

    QPushButton::connect(clearButton, &QPushButton::clicked, [tableWidget]() {
        clearAction(tableWidget);
        });

    window.show();
    return app.exec();
}
