#include <iostream>
#include <qapplication>
#include <qpushbutton>
#include <string>
#include <algorithm>
#include <sstream>
#include "ui_main_window.h"
#include "ui_info_widget.h"
#include "test.h"
#include "table.h"
#include "calculator.h"

// TODO test stuff, delete
#include <random>
std::vector<std::string> v;
int zavtraCount = 0;
void foo(QTextEdit* pOutputField) { pOutputField->setHtml(QString::fromStdString(std::string("To a future of grief #") + std::to_string(++zavtraCount))); }
std::string rs(int len = 5) {
    std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    std::string s(len, 'a');
    for (int i = 0; i < len; i++) {
        s[i] = charset[rand() % charset.length()];
    }
    return s;
}
// end of test stuff

std::vector<std::string> polNames;

// to be deleted
std::string despace(const std::string& str) // to be deleted
{ 
    std::string res = "";
    for (auto x : str) {
        if (x != ' ') res += x;
    }
    return res;
}

// to be deleted
std::vector<std::string> tokenize(const std::string& originalExpression) // to be deleted
{
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
            std::string Token = expression.substr(funcStart, funcEnd - funcStart + 1);
            tokens.push_back(Token);
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

void tableWidgetUpdate(QTableWidget* pTableWidget, Aggregator* pAggregator)
{
    std::vector<std::pair< std::string, polynomial>> records = pAggregator->getPolynomials();
    pTableWidget->setRowCount(0);

    for (int i = 0; i < records.size(); i++) {
        std::pair< std::string, polynomial> record = records[i];
        std::ostringstream os;
        os << record.second;
        std::string polyString = os.str();

        pTableWidget->insertRow(pTableWidget->rowCount());
        QTableWidgetItem* twi = new QTableWidgetItem(QString::fromStdString(record.first));
        pTableWidget->setItem(i, 0, twi);
        twi = new QTableWidgetItem(QString::fromStdString(polyString));
        pTableWidget->setItem(i, 1, twi);

    }

    pTableWidget->resizeColumnsToContents();
}

void calculateAction(Aggregator* pAggregator, QTextEdit* pOutputField, std::string polyName, double x = 0.0, double y = 0.0, double z = 0.0, double w = 0.0) {

}

/// @return false if there was an error
/// @return true otherwise
bool inputHandle(std::string inp, Aggregator* pAggregator, QLineEdit* pInputErrorField, QTextEdit* pOutputField, QTableWidget* pTableWidget) // TODO: tell user the index of mistake
{
    std::variant<polynomial, syntax_error, std::string> result = PolynomialCalculator::calculate(inp, pAggregator);
    if (result.index() == 1)
    {
        qDebug() << "Syntax error in polynomial";
        pInputErrorField->setText("Syntax error in polynomial");
        return false;
    }
    else if (result.index() == 2)
    {
        qDebug() << "Runtime error";
        pInputErrorField->setText("Cannot process input");
        return false;
    }
    else 
    {
        polynomial resPol = std::get<polynomial>(result);

        std::ostringstream os;
        os << resPol;
        std::string str;
        str = os.str();
        os.str("");
        os.clear();

        // TODO ��������� �������, ����� ���� ����� ������ ���������� ��� ���������. �� ����� � ˸�� �� ��������
        os << pAggregator->size();
        std::string name = rs();
        pAggregator->addPolynomial(name, resPol);

        pOutputField->setHtml(QString::fromStdString(str) + '\n' + pOutputField->toHtml());
        tableWidgetUpdate(pTableWidget, pAggregator);

        return true;
    }

}

void deleteAction(QTableWidget* pTableWidget, Aggregator* pAggregator, int row) 
{
    if (row >= 0 && row < v.size()) {
        int rowInd = row;


        std::string polyName = (pTableWidget->item(rowInd, 0)->text()).toStdString();
        pAggregator->delPolynomial(polyName);

        pTableWidget->removeRow(row);
        qDebug() << "Row " << rowInd << " deleted";

        tableWidgetUpdate(pTableWidget, pAggregator);
    }
    else {
        if (row != -1) // row == -1 if there is no selection
            throw "something's off with deleting";
    }
}

void clearAction(QTableWidget* pTableWidget, Aggregator* pAggregator)
{
    v.clear();

    auto records = pAggregator->getPolynomials();

    for (auto record : records) {
        pAggregator->delPolynomial(record.first);
    }

    qDebug() << "Cleared";
    pTableWidget->setRowCount(0);
}

void changeTable(Aggregator* pAggregator, std::string text) {
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
        throw "Something went wrong during table changing";
    }
    pAggregator->selectTable(code);
}

int main(int argc, char* argv[]) 
{
    srand(time(NULL));

    QApplication app(argc, argv);

    QMainWindow window;
    QMainWindow infoWindow;

    Ui::MainWindow ui;
    Ui::infoWidget infoUi;
    ui.setupUi(&window);
    infoUi.setupUi(&infoWindow);

    
    QPushButton* pDeleteButton = ui.deleteButton;
    QPushButton* pClearButton = ui.clearButton;
    QPushButton* pHelpButton = ui.helpButton;
    QPushButton* pCalculateButton = ui.calculateButton;
    QLineEdit* pInputField = ui.inputField;
    QLineEdit* pXContainer = ui.xLineEdit;
    QLineEdit* pYContainer = ui.yLineEdit;
    QLineEdit* pZContainer = ui.zLineEdit;
    QLineEdit* pWContainer = ui.wLineEdit;
    QLineEdit* pInputErrorField = ui.inputError;
    QComboBox* pComboBox = ui.comboBox;
    QTextEdit* pOutputField = ui.outputField;
    QTableWidget* pTableWidget = ui.tableWidget;

    QDoubleValidator* pValidator = new QDoubleValidator(&window);
    QString inputText;

    Aggregator* pAggregator = new Aggregator();

    infoWindow.setWindowTitle("Help");
    pValidator->setNotation(QDoubleValidator::StandardNotation);

    pXContainer->setValidator(pValidator);
    pYContainer->setValidator(pValidator);
    pZContainer->setValidator(pValidator);
    pWContainer->setValidator(pValidator);

    QObject::connect(pInputField, &QLineEdit::returnPressed, [&]()
        {
            try {
                inputText = pInputField->text();
                std::string s = inputText.toUtf8().constData();

                v.push_back(s);
                qDebug() << "Input:" << inputText;

                if (inputHandle(s, pAggregator, pInputErrorField, pOutputField, pTableWidget)) {
                    /*
                    input processing here
                    */

                    pInputField->clear();
                    pInputErrorField->clear(); // clear error field if everything is fine
                }
                else {

                }

            }
            catch (std::string x) {
                qDebug() << "Error: " << x;
            }
            catch (const char* c) {
                qDebug() << "Error: " << c;
            }
        });

    QObject::connect(pTableWidget, &QTableWidget::customContextMenuRequested, [&](const QPoint &pos) {

        QTableWidgetItem* pItem = pTableWidget->itemAt(pos);
        if (pItem) {
            QMenu menu;

            QAction* buttonDeleteAction = menu.addAction("Delete");

            QObject::connect(buttonDeleteAction, &QAction::triggered, [pTableWidget, pItem, pAggregator]()
                {
                    int row = pItem->row();
                    deleteAction(pTableWidget, pAggregator, row);

                    pTableWidget->clearSelection();
                    tableWidgetUpdate(pTableWidget, pAggregator);
                });

            menu.exec(pTableWidget->viewport()->mapToGlobal(pos));
        }
        });

    QPushButton::connect(pDeleteButton, &QPushButton::clicked, [pTableWidget, pAggregator]()
        {
            int row = pTableWidget->selectionModel()->currentIndex().row();
            deleteAction(pTableWidget, pAggregator, row);

            pTableWidget->clearSelection();
            tableWidgetUpdate(pTableWidget, pAggregator);
        });

    QPushButton::connect(pClearButton, &QPushButton::clicked, [pTableWidget, pAggregator]()
        {
            clearAction(pTableWidget, pAggregator);
        });

    QComboBox::connect(pComboBox, &QComboBox::currentTextChanged, [&](const QString& text) 
        {
            std::string s = text.toUtf8().constData();
            changeTable(pAggregator, s);
            qDebug() << "Table changed to" << text;
        });

    QComboBox::connect(pHelpButton, &QPushButton::clicked, [&]()
        {
            if (infoWindow.isVisible())
                infoWindow.hide();
            else
                infoWindow.show();
        });

    QPushButton::connect(pCalculateButton, &QPushButton::clicked, [&]() 
        {
            double x = pXContainer->text().toDouble();
            double y = pYContainer->text().toDouble();
            double z = pZContainer->text().toDouble();
            double w = pWContainer->text().toDouble();
            int row = pTableWidget->selectionModel()->currentIndex().row();
            std::string polyName = pTableWidget->item(row, 0)->text().toStdString();

            calculateAction(pAggregator, pOutputField, polyName, x, y, z, w);

            /* I dont know if this is useful
            pXContainer->clear();
            pYContainer->clear();
            pZContainer->clear();
            pWContainer->clear();
            */
        });

    window.show();
    return app.exec();
}
