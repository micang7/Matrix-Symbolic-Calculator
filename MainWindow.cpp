#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "ResultWidget.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_macierzeLayout = static_cast<QHBoxLayout*>(ui->macierze->layout());
    m_zapisaneLayout = static_cast<QVBoxLayout*>(ui->zapisane->layout());
    m_wynikiLayout = static_cast<QVBoxLayout*>(ui->wyniki->layout());

    // moje ustawienia
    ui->zamiana_btn->setFixedSize(50, 25);
    ui->dodawanie_btn->setFixedSize(50, 25);
    ui->odejmowanie_btn->setFixedSize(50, 25);
    ui->mnozenie_btn->setFixedSize(50, 25);

    ui->wyrazenie_lne->setFixedWidth(200);
    ui->evaluate_btn->setFixedSize(22, 22);

    ui->wyniki->setStyleSheet("background-color: rgb(250, 250, 250)");
    m_wynikiLayout->setAlignment(Qt::AlignTop);

    ui->zapisaneContainer->setStyleSheet("background-color: rgb(250, 250, 250)");
    m_zapisaneLayout->addStretch();

    // nowe widgety
    m_matrixWidgetA = new InputMatrixWidget('A', this);
    m_matrixWidgetB = new InputMatrixWidget('B', this);

    m_macierzeLayout->insertWidget(0, m_matrixWidgetA);
    m_macierzeLayout->insertWidget(2, m_matrixWidgetB);

    // signals and slots
    connect(ui->zamiana_btn, &QPushButton::clicked, this, &MainWindow::zamiana_btn_onClick);

    connect(ui->dodawanie_btn, &QPushButton::clicked, this, &MainWindow::dodawanie_btn_onClick);
    connect(ui->odejmowanie_btn, &QPushButton::clicked, this, &MainWindow::odejmowanie_btn_onClick);
    connect(ui->mnozenie_btn, &QPushButton::clicked, this, &MainWindow::mnozenie_btn_onClick);

    connect(ui->evaluate_btn, &QPushButton::clicked, this, &MainWindow::evaluate_btn_onClick);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveMatrix(QString name, int pos, const Matrix& matrix) {
    SavedMatrixWidget* saved = new SavedMatrixWidget(name, matrix, this);

    m_zapisaneLayout->insertWidget(m_zapisaneLayout->count() - 1, saved);

    m_matrixWidgetA->removeComboBoxOption(pos);
    m_matrixWidgetB->removeComboBoxOption(pos);
}

void MainWindow::unsaveMatrix(SavedMatrixWidget* saved, QString name) {
    m_matrixWidgetA->addComboBoxOption(name);
    m_matrixWidgetB->addComboBoxOption(name);
    delete saved;
}

void MainWindow::detBtn_onClick(InputMatrixWidget* matrix) {
    try {
        QHBoxLayout* det_equation = new QHBoxLayout();
        QLabel* det_detLabel = new QLabel("det");
        DisplayedMatrixWidget* det_matrixOperand = new DisplayedMatrixWidget(matrix->getMatrix().toNapisExpand().getStr());
        QLabel* det_equalsLabel = new QLabel("=");

        AbstractSyntaxTree det_resultAst = matrix->getMatrix().determinant();
        Napis det_napis = det_resultAst.toNapis();
        QLabel* det_result = new QLabel(det_napis.getStr());

        det_equation->addWidget(det_detLabel);
        det_equation->addWidget(det_matrixOperand);
        det_equation->addWidget(det_equalsLabel);
        det_equation->addWidget(det_result);

        det_detLabel->setFont(QFont("Cambria Math", 12));
        det_equalsLabel->setFont(QFont("Cambria Math", 12));
        det_result->setFont(QFont("Cambria Math", 12));

        det_equation->setContentsMargins(0, 0, 0, 0);
        det_equation->setSpacing(5);
        det_equation->setAlignment(Qt::AlignLeft);

        ResultWidget* det_wynik = new ResultWidget(
            det_equation,
            std::move(Matrix("{{" + det_napis + "}}")),
            this
        );
        m_wynikiLayout->addWidget(det_wynik);
    }
    catch(std::invalid_argument& e) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowFlags(
            Qt::Dialog |
            Qt::WindowTitleHint |
            Qt::CustomizeWindowHint
            );
        msgBox.setText(e.what());
        msgBox.exec();
    }
}

void MainWindow::transposeBtn_onClick(InputMatrixWidget* matrix) {
    try {
        QHBoxLayout* transpose_equation = new QHBoxLayout();
        DisplayedMatrixWidget* transpose_matrixOperand = new DisplayedMatrixWidget(matrix->getMatrix().toNapisExpand().getStr());
        QLabel* transpose_transposeLabel = new QLabel("T");
        QLabel* transpose_equalsLabel = new QLabel("=");

        Matrix transpose_resultMatrix(std::move(matrix->getMatrix().transposed()));
        DisplayedMatrixWidget* transpose_result = new DisplayedMatrixWidget(transpose_resultMatrix.toNapisExpand().getStr());

        transpose_equation->addWidget(transpose_matrixOperand);
        transpose_equation->addWidget(transpose_transposeLabel);
        transpose_equation->addWidget(transpose_equalsLabel);
        transpose_equation->addWidget(transpose_result);

        transpose_transposeLabel->setFont(QFont("Cambria Math", 12));
        transpose_equalsLabel->setFont(QFont("Cambria Math", 12));

        transpose_matrixOperand->adjustSize();
        transpose_transposeLabel->setFixedHeight(transpose_matrixOperand->height() + 15);
        transpose_transposeLabel->setAlignment(Qt::AlignTop);

        transpose_equation->setContentsMargins(0, 0, 0, 0);
        transpose_equation->setSpacing(5);
        transpose_equation->setAlignment(Qt::AlignLeft);

        ResultWidget* transpose_wynik = new ResultWidget(transpose_equation, std::move(transpose_resultMatrix), this);
        m_wynikiLayout->addWidget(transpose_wynik);
    }
    catch(std::invalid_argument& e) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowFlags(
            Qt::Dialog |
            Qt::WindowTitleHint |
            Qt::CustomizeWindowHint
            );
        msgBox.setText(e.what());
        msgBox.exec();
    }
}

void MainWindow::inverseBtn_onClick(InputMatrixWidget* matrix) {
    try {
        QHBoxLayout* inverse_equation = new QHBoxLayout();
        DisplayedMatrixWidget* inverse_matrixOperand = new DisplayedMatrixWidget(matrix->getMatrix().toNapisExpand().getStr());
        QLabel* inverse_inverseLabel = new QLabel("-1");
        QLabel* inverse_equalsLabel = new QLabel("=");

        Matrix inverse_resultMatrix(std::move(matrix->getMatrix().inversed()));
        DisplayedMatrixWidget* inverse_result = new DisplayedMatrixWidget(inverse_resultMatrix.toNapisExpand().getStr());

        inverse_equation->addWidget(inverse_matrixOperand);
        inverse_equation->addWidget(inverse_inverseLabel);
        inverse_equation->addWidget(inverse_equalsLabel);
        inverse_equation->addWidget(inverse_result);

        inverse_inverseLabel->setFont(QFont("Cambria Math", 12));
        inverse_equalsLabel->setFont(QFont("Cambria Math", 12));

        inverse_matrixOperand->adjustSize();
        inverse_inverseLabel->setFixedHeight(inverse_matrixOperand->height() + 15);
        inverse_inverseLabel->setAlignment(Qt::AlignTop);

        inverse_equation->setContentsMargins(0, 0, 0, 0);
        inverse_equation->setSpacing(5);
        inverse_equation->setAlignment(Qt::AlignLeft);

        ResultWidget* inverse_wynik = new ResultWidget(inverse_equation, std::move(inverse_resultMatrix), this);
        m_wynikiLayout->addWidget(inverse_wynik);
    }
    catch(std::invalid_argument& e) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowFlags(
            Qt::Dialog |
            Qt::WindowTitleHint |
            Qt::CustomizeWindowHint
            );
        msgBox.setText(e.what());
        msgBox.exec();
    }
}

void MainWindow::multiplyBtn_onClick(InputMatrixWidget* matrix, QString scalar) {
    try {
        QHBoxLayout* multBy_equation = new QHBoxLayout();
        DisplayedMatrixWidget* multBy_matrixOperand = new DisplayedMatrixWidget(matrix->getMatrix().toNapisExpand().getStr());
        QLabel* multBy_multLabel = new QLabel("× " + scalar);
        QLabel* multBy_equalsLabel = new QLabel("=");

        Napis multBy_result_napis = (scalar == "0" ? "{{0}}" : AbstractSyntaxTree(matrix->getMatrix().toNapis() + "*(" + scalar.toLocal8Bit().data() + ")").toNapis());
        Matrix multBy_resultMatrix;
        if (multBy_result_napis[0] == '-') {
            multBy_result_napis.remove(0, 1);
            multBy_resultMatrix = Matrix(multBy_result_napis);
            multBy_resultMatrix = ~multBy_resultMatrix;
        }
        else multBy_resultMatrix = Matrix(multBy_result_napis);
        DisplayedMatrixWidget* multBy_result = new DisplayedMatrixWidget(multBy_resultMatrix.toNapisExpand().getStr());

        multBy_equation->addWidget(multBy_matrixOperand);
        multBy_equation->addWidget(multBy_multLabel);
        multBy_equation->addWidget(multBy_equalsLabel);
        multBy_equation->addWidget(multBy_result);

        multBy_multLabel->setFont(QFont("Cambria Math", 12));
        multBy_equalsLabel->setFont(QFont("Cambria Math", 12));

        multBy_equation->setContentsMargins(0, 0, 0, 0);
        multBy_equation->setSpacing(5);
        multBy_equation->setAlignment(Qt::AlignLeft);

        ResultWidget* multBy_wynik = new ResultWidget(multBy_equation, std::move(multBy_resultMatrix), this);
        m_wynikiLayout->addWidget(multBy_wynik);
    }
    catch(std::invalid_argument& e) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowFlags(
            Qt::Dialog |
            Qt::WindowTitleHint |
            Qt::CustomizeWindowHint
        );
        msgBox.setText(e.what());
        msgBox.exec();
    }
}

void MainWindow::raiseBtn_onClick(InputMatrixWidget* matrix, QString exponent) {
    try {
        QHBoxLayout* raise_equation = new QHBoxLayout();
        DisplayedMatrixWidget* raise_matrixOperand = new DisplayedMatrixWidget(matrix->getMatrix().toNapisExpand().getStr());
        QLabel* raise_exponentLabel = new QLabel(exponent);
        QLabel* raise_equalsLabel = new QLabel("=");

        Matrix raise_resultMatrix(
            AbstractSyntaxTree(matrix->getMatrix().toNapis() + "^(" + exponent.toLocal8Bit().data() + ")").toNapis()
        );
        DisplayedMatrixWidget* raise_result = new DisplayedMatrixWidget(raise_resultMatrix.toNapisExpand().getStr());

        raise_equation->addWidget(raise_matrixOperand);
        raise_equation->addWidget(raise_exponentLabel);
        raise_equation->addWidget(raise_equalsLabel);
        raise_equation->addWidget(raise_result);

        raise_exponentLabel->setFont(QFont("Cambria Math", 12));
        raise_equalsLabel->setFont(QFont("Cambria Math", 12));

        raise_matrixOperand->adjustSize();
        raise_exponentLabel->setFixedHeight(raise_matrixOperand->height() + 15);
        raise_exponentLabel->setAlignment(Qt::AlignTop);

        raise_equation->setContentsMargins(0, 0, 0, 0);
        raise_equation->setSpacing(5);
        raise_equation->setAlignment(Qt::AlignLeft);

        ResultWidget* raise_wynik = new ResultWidget(raise_equation, std::move(raise_resultMatrix), this);
        m_wynikiLayout->addWidget(raise_wynik);
    }
    catch(std::invalid_argument& e) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowFlags(
            Qt::Dialog |
            Qt::WindowTitleHint |
            Qt::CustomizeWindowHint
        );
        msgBox.setText(e.what());
        msgBox.exec();
    }
}

void MainWindow::moveToA(Matrix* matrix)
{
    m_matrixWidgetA->fillMatrix(matrix);
}

void MainWindow::moveToB(Matrix* matrix)
{
    m_matrixWidgetB->fillMatrix(matrix);
}

void MainWindow::zamiana_btn_onClick() {
    m_macierzeLayout->removeWidget(m_matrixWidgetA);
    m_macierzeLayout->removeWidget(m_matrixWidgetB);

    InputMatrixWidget* tmp = m_matrixWidgetA;
    m_matrixWidgetA = m_matrixWidgetB;
    m_matrixWidgetB = tmp;

    m_matrixWidgetA->setName('A');
    m_matrixWidgetB->setName('B');

    m_macierzeLayout->insertWidget(0, m_matrixWidgetA);
    m_macierzeLayout->insertWidget(2, m_matrixWidgetB);
}

void MainWindow::dodawanie_btn_onClick() {
    if (m_matrixWidgetA->createActualMatrix() && m_matrixWidgetB->createActualMatrix()) {
        try {
            QHBoxLayout* dodawanie_equation = new QHBoxLayout();
            DisplayedMatrixWidget* dodawanie_matrixOperand1 = new DisplayedMatrixWidget(m_matrixWidgetA->getMatrix().toNapisExpand().getStr());
            QLabel* dodawanie_plusLabel = new QLabel("+");
            DisplayedMatrixWidget* dodawanie_matrixOperand2 = new DisplayedMatrixWidget(m_matrixWidgetB->getMatrix().toNapisExpand().getStr());
            QLabel* dodawanie_equalsLabel = new QLabel("=");

            Matrix dodawanie_resultMatrix(
                AbstractSyntaxTree(
                    m_matrixWidgetA->getMatrix().toNapis() + "+" +
                    m_matrixWidgetB->getMatrix().toNapis()
                ).toNapis()
            );
            DisplayedMatrixWidget* dodawanie_result = new DisplayedMatrixWidget(dodawanie_resultMatrix.toNapisExpand().getStr());

            dodawanie_equation->addWidget(dodawanie_matrixOperand1);
            dodawanie_equation->addWidget(dodawanie_plusLabel);
            dodawanie_equation->addWidget(dodawanie_matrixOperand2);
            dodawanie_equation->addWidget(dodawanie_equalsLabel);
            dodawanie_equation->addWidget(dodawanie_result);

            dodawanie_equation->setContentsMargins(0, 0, 0, 0);
            dodawanie_equation->setSpacing(5);
            dodawanie_equation->setAlignment(Qt::AlignLeft);

            ResultWidget* dodawanie_wynik = new ResultWidget(dodawanie_equation, std::move(dodawanie_resultMatrix), this);
            m_wynikiLayout->addWidget(dodawanie_wynik);
        }
        catch(std::invalid_argument& e) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setWindowFlags(
                Qt::Dialog |
                Qt::WindowTitleHint |
                Qt::CustomizeWindowHint
                );
            msgBox.setText(e.what());
            msgBox.exec();
        }
    }
}

void MainWindow::odejmowanie_btn_onClick() {
    if (m_matrixWidgetA->createActualMatrix() && m_matrixWidgetB->createActualMatrix()) {
        try {
            QHBoxLayout* odejmowanie_equation = new QHBoxLayout();
            DisplayedMatrixWidget* odejmowanie_matrixOperand1 = new DisplayedMatrixWidget(m_matrixWidgetA->getMatrix().toNapisExpand().getStr());
            QLabel* odejmowanie_plusLabel = new QLabel("−");
            DisplayedMatrixWidget* odejmowanie_matrixOperand2 = new DisplayedMatrixWidget(m_matrixWidgetB->getMatrix().toNapisExpand().getStr());
            QLabel* odejmowanie_equalsLabel = new QLabel("=");

            Matrix odejmowanie_resultMatrix(
                AbstractSyntaxTree(
                    m_matrixWidgetA->getMatrix().toNapis() + "-" +
                    m_matrixWidgetB->getMatrix().toNapis()
                    ).toNapis()
                );
            DisplayedMatrixWidget* odejmowanie_result = new DisplayedMatrixWidget(odejmowanie_resultMatrix.toNapisExpand().getStr());

            odejmowanie_equation->addWidget(odejmowanie_matrixOperand1);
            odejmowanie_equation->addWidget(odejmowanie_plusLabel);
            odejmowanie_equation->addWidget(odejmowanie_matrixOperand2);
            odejmowanie_equation->addWidget(odejmowanie_equalsLabel);
            odejmowanie_equation->addWidget(odejmowanie_result);

            odejmowanie_equation->setContentsMargins(0, 0, 0, 0);
            odejmowanie_equation->setSpacing(5);
            odejmowanie_equation->setAlignment(Qt::AlignLeft);

            ResultWidget* odejmowanie_wynik = new ResultWidget(odejmowanie_equation, std::move(odejmowanie_resultMatrix), this);
            m_wynikiLayout->addWidget(odejmowanie_wynik);
        }
        catch(std::invalid_argument& e) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setWindowFlags(
                Qt::Dialog |
                Qt::WindowTitleHint |
                Qt::CustomizeWindowHint
                );
            msgBox.setText(e.what());
            msgBox.exec();
        }
    }
}

void MainWindow::mnozenie_btn_onClick() {
    if (m_matrixWidgetA->createActualMatrix() && m_matrixWidgetB->createActualMatrix()) {
        try {
            QHBoxLayout* mnozenie_equation = new QHBoxLayout();
            DisplayedMatrixWidget* mnozenie_matrixOperand1 = new DisplayedMatrixWidget(m_matrixWidgetA->getMatrix().toNapisExpand().getStr());
            QLabel* mnozenie_plusLabel = new QLabel("*");
            DisplayedMatrixWidget* mnozenie_matrixOperand2 = new DisplayedMatrixWidget(m_matrixWidgetB->getMatrix().toNapisExpand().getStr());
            QLabel* mnozenie_equalsLabel = new QLabel("=");

            Matrix mnozenie_resultMatrix(
                AbstractSyntaxTree(
                    m_matrixWidgetA->getMatrix().toNapis() + "*" +
                    m_matrixWidgetB->getMatrix().toNapis()
                    ).toNapis()
                );
            DisplayedMatrixWidget* mnozenie_result = new DisplayedMatrixWidget(mnozenie_resultMatrix.toNapisExpand().getStr());

            mnozenie_equation->addWidget(mnozenie_matrixOperand1);
            mnozenie_equation->addWidget(mnozenie_plusLabel);
            mnozenie_equation->addWidget(mnozenie_matrixOperand2);
            mnozenie_equation->addWidget(mnozenie_equalsLabel);
            mnozenie_equation->addWidget(mnozenie_result);

            mnozenie_equation->setContentsMargins(0, 0, 0, 0);
            mnozenie_equation->setSpacing(5);
            mnozenie_equation->setAlignment(Qt::AlignLeft);

            ResultWidget* mnozenie_wynik = new ResultWidget(mnozenie_equation, std::move(mnozenie_resultMatrix), this);
            m_wynikiLayout->addWidget(mnozenie_wynik);
        }
        catch(std::invalid_argument& e) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setWindowFlags(
                Qt::Dialog |
                Qt::WindowTitleHint |
                Qt::CustomizeWindowHint
                );
            msgBox.setText(e.what());
            msgBox.exec();
        }
    }
}

void MainWindow::evaluate_btn_onClick()
{
    QString input = ui->wyrazenie_lne->text();
    if (input.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowFlags(
            Qt::Dialog |
            Qt::WindowTitleHint |
            Qt::CustomizeWindowHint
            );
        msgBox.setText("Enter an expression.");
        msgBox.exec();
        return;
    }

    if (input.contains('A')) {
        if (!m_matrixWidgetA->createActualMatrix()) return;
        QString nowy = m_matrixWidgetA->getMatrix().toNapis().getStr();
        input.replace('A', nowy);
    }
    if (input.contains('B')) {
        if (!m_matrixWidgetB->createActualMatrix()) return;
        QString nowy = m_matrixWidgetB->getMatrix().toNapis().getStr();
        input.replace('B', nowy);
    }

    for (char c = 'C'; c <= 'Z'; c++) {
        for (int i = 0; i < m_zapisaneLayout->count() - 1; i++) {
            SavedMatrixWidget* saved = qobject_cast<SavedMatrixWidget*>(m_zapisaneLayout->itemAt(i)->widget());
            if (saved->getName() == c) input.replace(c, saved->getCondensed());
        }
    }

    try {
        QHBoxLayout* expression_equation = new QHBoxLayout();
        QLabel* expression_expressionLabel = new QLabel(input);
        QLabel* expression_equalsLabel = new QLabel("=");

        AbstractSyntaxTree expression_resultAst = AbstractSyntaxTree(input.toLocal8Bit().data());
        Napis expression_napis = expression_resultAst.toNapis();
        QLabel* expression_result = new QLabel(expression_napis.getStr());

        expression_equation->addWidget(expression_expressionLabel);
        expression_equation->addWidget(expression_equalsLabel);
        expression_equation->addWidget(expression_result);

        expression_expressionLabel->setFont(QFont("Cambria Math", 12));
        expression_equalsLabel->setFont(QFont("Cambria Math", 12));
        expression_result->setFont(QFont("Cambria Math", 12));

        expression_equation->setContentsMargins(0, 0, 0, 0);
        expression_equation->setSpacing(5);
        expression_equation->setAlignment(Qt::AlignLeft);

        ResultWidget* expression_wynik = new ResultWidget(
            expression_equation,
            std::move(Matrix(
                ((expression_napis[0] == '{' &&
                  expression_napis[1] == '{' &&
                  expression_napis[expression_napis.getLen()-2] == '}' &&
                  expression_napis[expression_napis.getLen()-1] == '}')
                     ? expression_napis
                     : "{{" + expression_napis + "}}")
            )),
            this
        );
        m_wynikiLayout->addWidget(expression_wynik);
    }
    catch(std::invalid_argument& e) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowFlags(
            Qt::Dialog |
            Qt::WindowTitleHint |
            Qt::CustomizeWindowHint
            );
        msgBox.setText(e.what());
        msgBox.exec();
    }
}
