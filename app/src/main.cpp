#include <iostream>
#include <qapplication>
#include <qpushbutton>
#include <string>
#include <algorithm>
#include <sstream>
#include <format>
#include "ui_main_window.h"
#include "ui_info_widget.h"
#include "test.h"
#include "table.h"
#include "calculator.h"

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

void calculateAction(Aggregator* pAggregator, QTextEdit* pOutputField, std::string polyName, double w = 0.0, double x = 0.0, double y = 0.0, double z = 0.0) {
    double result = (pAggregator->findPolynomial(polyName).value()).evaluate(w, x, y, z);

    std::string outputLine = polyName + std::format("({}, {}, {}, {})", w, x, y, z) + " = " + std::to_string(result);

    pOutputField->setHtml(QString::fromStdString(outputLine) + '\n' + pOutputField->toHtml());
}

/// @return false if there was an error
/// @return true otherwise
bool inputHandle(std::string inp, Aggregator* pAggregator, QLineEdit* pInputErrorField, QTextEdit* pOutputField, QTableWidget* pTableWidget) // TODO: tell user the index of mistake
{
    std::variant<polynomial, syntax_error, std::string> result = PolynomialCalculator::calculate(inp, pAggregator);
    if (result.index() == 1)
    {
        qDebug() << "Syntax error in polynomial";
        pInputErrorField->setText(QString::fromStdString( (std::string)"Syntax error in polynomial at index " + std::to_string(std::get<syntax_error>(result).pos) + (std::string)": " + std::get<syntax_error>(result).message) );
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

        pOutputField->setHtml(QString::fromStdString(str) + '\n' + pOutputField->toHtml());
        tableWidgetUpdate(pTableWidget, pAggregator);

        return true;
    }

}

void deleteAction(QTableWidget* pTableWidget, Aggregator* pAggregator, int row) 
{
    if (row >= 0 && row < pAggregator->size()) {
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
    auto records = pAggregator->getPolynomials();

    for (auto record : records) {
        pAggregator->delPolynomial(record.first);
    }

    qDebug() << "Cleared";
    pTableWidget->setRowCount(0);
}

void changeTable(QTableWidget* pTableWidget, Aggregator* pAggregator, std::string text) {
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
    tableWidgetUpdate(pTableWidget, pAggregator);
}

int main(int argc, char* argv[]) 
{
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
    pValidator->setLocale(QLocale::C);

    pXContainer->setValidator(pValidator);
    pYContainer->setValidator(pValidator);
    pZContainer->setValidator(pValidator);
    pWContainer->setValidator(pValidator);

    QObject::connect(pInputField, &QLineEdit::returnPressed, [&]()
        {
            try {
                inputText = pInputField->text();
                std::string s = inputText.toUtf8().constData();

                qDebug() << "Input:" << inputText;

                if (inputHandle(s, pAggregator, pInputErrorField, pOutputField, pTableWidget)) { // TODO may be "else" is needed
                    pInputField->clear();
                    pInputErrorField->clear(); // clear error field if everything is fine
                }
            }
            catch (std::string x) {
                qDebug() << "Error: " << x;
                pInputErrorField->setText(QString::fromStdString(x));
            }
            catch (const char* c) {
                qDebug() << "Error: " << c;
                pInputErrorField->setText(QString::fromStdString(c));
            }
        });

    QObject::connect(pTableWidget, &QTableWidget::customContextMenuRequested, [&](const QPoint &pos) {

        QTableWidgetItem* pItem = pTableWidget->itemAt(pos);
        if (pItem) {
            QMenu menu;

            QAction* buttonCalculateAction = menu.addAction("Calculate");
            QAction* buttonDeleteAction = menu.addAction("Delete");

            QObject::connect(buttonDeleteAction, &QAction::triggered, [pTableWidget, pItem, pAggregator]()
                {
                    int row = pItem->row();
                    deleteAction(pTableWidget, pAggregator, row);

                    pTableWidget->clearSelection();
                    tableWidgetUpdate(pTableWidget, pAggregator);
                });

            QObject::connect(buttonCalculateAction, &QAction::triggered, [&]() {
                double w = pWContainer->text().toDouble();
                double x = pXContainer->text().toDouble();
                double y = pYContainer->text().toDouble();
                double z = pZContainer->text().toDouble();

                try
                {
                    int row = pTableWidget->selectionModel()->currentIndex().row();

                    std::string polyName = pTableWidget->item(row, 0)->text().toStdString();

                    calculateAction(pAggregator, pOutputField, polyName, w, x, y, z);
                }
                catch (char* s)
                {
                    pInputErrorField->setText("Select a polynomial to evaluate");
                }
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
            changeTable(pTableWidget, pAggregator, s);
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
            double w = pWContainer->text().toDouble();
            double x = pXContainer->text().toDouble();
            double y = pYContainer->text().toDouble();
            double z = pZContainer->text().toDouble();
   
            int row = pTableWidget->selectionModel()->currentIndex().row();
            if (row == -1) {
                pInputErrorField->setText("Select a polynomial to evaluate");
                return;
            }
            std::string polyName = pTableWidget->item(row, 0)->text().toStdString();

            calculateAction(pAggregator, pOutputField, polyName, w, x, y, z);
            pInputErrorField->clear();
        });

    QLineEdit::connect(pWContainer, &QLineEdit::textChanged, [&](const QString& text) {
        QString txt = text;
        if (txt.contains(',')) {
            pWContainer->setText(txt.replace(',', '.'));
        }
        });

    QLineEdit::connect(pXContainer, &QLineEdit::textChanged, [&](const QString& text) {
        QString txt = text;
        if (txt.contains(',')) {
            pXContainer->setText(txt.replace(',', '.'));
        }
        });

    QLineEdit::connect(pYContainer, &QLineEdit::textChanged, [&](const QString& text) {
        QString txt = text;
        if (txt.contains(',')) {
            pYContainer->setText(txt.replace(',', '.'));
        }
        });

    QLineEdit::connect(pZContainer, &QLineEdit::textChanged, [&](const QString& text) {
        QString txt = text;
        if (txt.contains(',')) {
            pZContainer->setText(txt.replace(',', '.'));
        }
        });

    window.show();
    return app.exec();
}
