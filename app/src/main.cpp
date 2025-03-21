#include <iostream>
#include <qapplication>
#include <qpushbutton>
#include <string>
#include <algorithm>
#include "ui_main_window.h"
#include "test.h"
#include "table.h"

// TODO test stuff, delete
std::vector<std::string> v;
int zavtraCount = 0;
void foo(QTextEdit* outputField) { outputField->setHtml(QString::fromStdString(std::string("To a future of grief #") + std::to_string(++zavtraCount))); }
// end of test stuff

std::vector<std::string> polNames;

std::string despace(const std::string& str) {
    std::string res = "";
    for (auto x : str) {
        if (x != ' ') res += x;
    }
    return res;
}

std::vector<std::string> tokenize(const std::string& originalExpression) {
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;
    std::string expression = despace(originalExpression);

    while (end < expression.length()) 
    {

        if (expression[end] == '+' || expression[end] == '-' || expression[end] == '*' || expression[end] == '/')
        {
            tokens.push_back(std::string(1, expression[end]));
            end++;
            continue;
        }

        if (expression[end] == '(') 
        {
            size_t funcStart = end;
            if (!(end == 0 || expression[end] == '+' || expression[end] == '-' || expression[end] == '*' || expression[end] == '/')) {
                tokens.pop_back();
                while (funcStart > 0 && (isalnum(expression[funcStart - 1]) || expression[funcStart - 1] == '_'))
                {
                    funcStart--;
                }
            }

            size_t funcEnd = expression.find(')', end);
            if (funcEnd == std::string::npos)
            {
                throw "Mismatch of opening and closing parenthesis";
            }
            std::string token = expression.substr(funcStart, funcEnd - funcStart + 1);
            tokens.push_back(token);
            end = funcEnd + 1;
            continue;
        }

        start = end;
        while (end < expression.length() && expression[end] != '+' && expression[end] != '-' && expression[end] != '*' && expression[end] != '/' && expression[end] != '(') 
        {
            end++;
        }
        if (start != end) {
            tokens.push_back(expression.substr(start, end - start));
        }
    }


    return tokens;
}

void inputHandle(std::string inp)
{
    size_t equalPos = inp.find("=");

    if (equalPos == std::string::npos) 
    {
        throw "There must be a \"=\" sign in the expression";
    }

    if (equalPos != inp.find_last_of('=')) { // TODO need to think about operator==
        throw "There must be only one \"=\" sign in the expression";
    }

    std::string polyName = inp.substr(0, equalPos);

    polyName = despace(polyName);

    // polyName check for not being a polynomial itself
    polynomial tmp;
    auto x = tmp.from_string(polyName);
    if (x.index() == 0) 
    {
        throw "Polynomial name should not be valid polynomial";
    }
    
    if (polyName == "" || polyName.find(' ') != std::string::npos)
    {
        throw "Enter a valid polynom name";
    }

    std::string polys = inp.substr(equalPos + 1);

    polys = despace(polys);

    std::vector<std::string> tokens = tokenize(polys);
    qDebug() << "Tokens:";
    for (auto x : tokens)
        qDebug() << x;

    for (auto token : tokens) {
    }


    polNames.push_back(polyName); // в самом конце!!
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

void changeTable(Aggregator* aggregator, std::string text) {
    std::string code;
    if (text == "Linear array table") 
    {
        code = "liar";
    }
    else if (text == "Linear list table")
    {
        code = "lili";
    }
    else if (text == "Ordered array table")
    {
        code = "ordr";
    }
    else if (text == "Tree table")
    {
        code = "tree";
    }
    else if (text == "Open address hash table")
    {
        code = "opha";
    }
    else if (text == "Separate chaining hash table")
    {
        code = "seha";
    } 
    else {
        throw "WHAT";
    }
    aggregator->selectTable(code);
}

int main(int argc, char* argv[]) 
{
    QApplication app(argc, argv);

    QMainWindow window;

    Ui::MainWindow ui;
    ui.setupUi(&window);

    QLineEdit* inputField = ui.inputFieldObject;
    QTableWidget* tableWidget = ui.tableWidget;
    QPushButton* deleteButton = ui.deleteButton;
    QPushButton* clearButton = ui.clearButton;
    QTextEdit* outputField = ui.outputField;
    QComboBox* comboBox = ui.comboBox;
    QString inputText;

    Aggregator* aggregator = new Aggregator();

    tableWidget->setWindowTitle("Polynomials");

    QObject::connect(inputField, &QLineEdit::returnPressed, [&inputText, inputField, tableWidget, outputField]()
        {
            try {
                inputText = inputField->text();
                std::string s = inputText.toUtf8().constData();

                v.push_back(s);
                qDebug() << "Input:" << inputText;

                inputHandle(s);

                tableWidget->insertRow(tableWidget->rowCount());
                tableWidgetUpdate(tableWidget);

                inputField->clear();

                foo(outputField);
            }
            catch (std::string x) {
                qDebug() << "Error: " << x;
            }
            catch (const char* c) {
                qDebug() << "Error: " << c;
            }
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

    QComboBox::connect(comboBox, &QComboBox::currentTextChanged, [&](const QString& text) {
        std::string s = text.toUtf8().constData();
        changeTable(aggregator, s);
        qDebug() << "Table changed to" << text;
        });

    window.show();
    return app.exec();
}
