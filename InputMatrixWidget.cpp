#include "InputMatrixWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include "MainWindow.h"

InputMatrixWidget::InputMatrixWidget(char name, QWidget *parent)
    : QWidget(parent), m_size(3), m_rows(0), m_cols(0), m_name(name)
{
    // layouts
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(5);

    m_matrixLayout = new QGridLayout();
    m_matrixLayout->setContentsMargins(0, 0, 0, 0);
    m_matrixLayout->setSpacing(5);

    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->setContentsMargins(0, 0, 0, 0);
    buttonsLayout->setSpacing(10);

    QGridLayout* functionsLayout = new QGridLayout();
    functionsLayout->setContentsMargins(0, 10, 0, 0);
    functionsLayout->setSpacing(7);

    QHBoxLayout* multBtnLayout = new QHBoxLayout();
    multBtnLayout->setContentsMargins(0, 0, 0, 0);
    multBtnLayout->setSpacing(0);

    QHBoxLayout* raiseBtnLayout = new QHBoxLayout();
    raiseBtnLayout->setContentsMargins(0, 0, 0, 0);
    raiseBtnLayout->setSpacing(0);

    QHBoxLayout* defBtnLayout = new QHBoxLayout();
    defBtnLayout->setContentsMargins(0, 0, 0, 0);
    defBtnLayout->setSpacing(0);

    // new widgets
    m_matrixLabel = new QLabel(QString("Matrix ") + m_name, this);

    QLineEdit *lineEdit;
    for (int i = 0; i < m_size; i++) {
        for (int j = 0; j < m_size; j++) {
            lineEdit = new QLineEdit(this);
            lineEdit->setFixedSize(40, 20);
            lineEdit->setAlignment(Qt::AlignCenter);
            lineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            connect(lineEdit, &QLineEdit::editingFinished, this, [lineEdit, this, i, j] {
                updateActualSize(lineEdit, i, j);
            });
            m_matrixLayout->addWidget(lineEdit, i, j);
        }
    }

    QPushButton *clearBtn = new QPushButton("Clear", this);
    QPushButton *increaseBtn = new QPushButton("Add", this);
    QPushButton *decreaseBtn = new QPushButton("Remove", this);

    QPushButton *detBtn = new QPushButton("Determinant", this);
    QPushButton *transposeBtn = new QPushButton("Transpose matrix", this);
    QPushButton *inverseBtn = new QPushButton("Inverse matrix", this);

    QPushButton *multiplyBtn = new QPushButton("Multiply by", this);
    QLineEdit* scalarInput = new QLineEdit();

    QPushButton *raiseBtn = new QPushButton("Raise to the power of", this);
    QLineEdit* exponentInput = new QLineEdit();

    QPushButton *defineAsBtn = new QPushButton("Define as matrix", this);
    m_nameInput = new QComboBox();

    // widget settings
    m_matrixLabel->setAlignment(Qt::AlignCenter);
    m_matrixLabel->setFixedHeight(15);

    clearBtn->setFixedSize(60, 20);
    increaseBtn->setFixedSize(60, 20);
    decreaseBtn->setFixedSize(60, 20);

    detBtn->setFixedSize(170, 25);
    transposeBtn->setFixedSize(170, 25);
    inverseBtn->setFixedSize(170, 25);

    multiplyBtn->setFixedSize(145, 25);
    scalarInput->setFixedSize(25, 25);
    scalarInput->setAlignment(Qt::AlignCenter);

    raiseBtn->setFixedSize(145, 25);
    exponentInput->setFixedSize(25, 25);
    exponentInput->setAlignment(Qt::AlignCenter);

    defineAsBtn->setFixedSize(130, 25);
    m_nameInput->setFixedSize(40, 25);

    for (char c = 'C'; c <= 'Z'; c++)
        m_nameInput->addItem(QString(c));

    m_nameInput->setPlaceholderText("−");

    // add widgets to layouts
    buttonsLayout->addWidget(clearBtn);
    buttonsLayout->addWidget(increaseBtn);
    buttonsLayout->addWidget(decreaseBtn);

    functionsLayout->addWidget(detBtn, 0, 0);
    functionsLayout->addWidget(transposeBtn, 1, 0);
    functionsLayout->addWidget(inverseBtn, 1, 1);

    multBtnLayout->addWidget(multiplyBtn);
    multBtnLayout->addWidget(scalarInput);
    functionsLayout->addLayout(multBtnLayout, 2, 0);

    raiseBtnLayout->addWidget(raiseBtn);
    raiseBtnLayout->addWidget(exponentInput);
    functionsLayout->addLayout(raiseBtnLayout, 2, 1);

    defBtnLayout->addWidget(defineAsBtn);
    defBtnLayout->addWidget(m_nameInput);
    functionsLayout->addLayout(defBtnLayout, 0, 1);

    // join in main layout
    m_layout->addWidget(m_matrixLabel);
    m_layout->addLayout(m_matrixLayout);
    m_layout->addLayout(buttonsLayout);
    m_layout->addLayout(functionsLayout);

    m_layout->setAlignment(m_matrixLabel, Qt::AlignCenter);
    m_layout->setAlignment(m_matrixLayout, Qt::AlignCenter);
    m_layout->setAlignment(buttonsLayout, Qt::AlignCenter);
    m_layout->setAlignment(functionsLayout, Qt::AlignCenter);

    // signals and slots
    connect(clearBtn, &QPushButton::clicked, this, &InputMatrixWidget::clearMatrix);
    connect(increaseBtn, &QPushButton::clicked, this, &InputMatrixWidget::increase);
    connect(decreaseBtn, &QPushButton::clicked, this, &InputMatrixWidget::decrease);

    connect(detBtn, &QPushButton::clicked, parent, [this, parent] {
        if (createActualMatrix())
            qobject_cast<MainWindow*>(parent)->detBtn_onClick(this);
    });

    connect(transposeBtn, &QPushButton::clicked, parent, [this, parent] {
        if (createActualMatrix())
            qobject_cast<MainWindow*>(parent)->transposeBtn_onClick(this);
    });

    connect(inverseBtn, &QPushButton::clicked, parent, [this, parent] {
        if (createActualMatrix())
            qobject_cast<MainWindow*>(parent)->inverseBtn_onClick(this);
    });

    connect(multiplyBtn, &QPushButton::clicked, parent, [this, parent, scalarInput] {
        if (scalarInput->text().isEmpty()) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowFlags(
                Qt::Dialog |
                Qt::WindowTitleHint |
                Qt::CustomizeWindowHint
                );
            msgBox.setText("Enter a scalar.");
            msgBox.exec();
            return;
        }
        try {
            scalarInput->text().toInt();
        }
        catch(std::exception& e) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowFlags(
                Qt::Dialog |
                Qt::WindowTitleHint |
                Qt::CustomizeWindowHint
            );
            msgBox.setText("Scalar has to be a constant.");
            msgBox.exec();
            return;
        }
        if (createActualMatrix())
            qobject_cast<MainWindow*>(parent)->multiplyBtn_onClick(this, scalarInput->text());
    });

    connect(raiseBtn, &QPushButton::clicked, parent, [this, parent, exponentInput] {
        if (exponentInput->text().isEmpty()) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowFlags(
                Qt::Dialog |
                Qt::WindowTitleHint |
                Qt::CustomizeWindowHint
                );
            msgBox.setText("Enter an exponent.");
            msgBox.exec();
            return;
        }
        if (createActualMatrix())
            qobject_cast<MainWindow*>(parent)->raiseBtn_onClick(this, exponentInput->text());
    });

    connect(defineAsBtn, &QPushButton::clicked, parent, [this, parent] {
        if (m_nameInput->count() > 0 && createActualMatrix()) {
            qobject_cast<MainWindow*>(parent)->saveMatrix(
                m_nameInput->currentText(),
                m_nameInput->currentIndex(),
                m_matrix
            );
        }
    });
}

void InputMatrixWidget::clearMatrix() {
    QLineEdit* lineEdit;
    for (int i = 0; i < m_size; i++) {
        for (int j = 0; j < m_size; j++) {
            lineEdit = qobject_cast<QLineEdit*>(m_matrixLayout->itemAtPosition(i, j)->widget());
            lineEdit->clear();
            lineEdit->setPlaceholderText("");
        }
    }
    m_rows = 0;
    m_cols = 0;
    m_matrix = Matrix();
}

void InputMatrixWidget::increase() {
    if (m_size == 10) return;

    int currentSize = m_size;
    QLineEdit *lineEdit;
    for (int i = 0; i < m_size; i++) {
        lineEdit = new QLineEdit(this);
        lineEdit->setFixedSize(40, 20);
        lineEdit->setAlignment(Qt::AlignCenter);
        lineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        connect(lineEdit, &QLineEdit::editingFinished, this, [lineEdit, this, i, currentSize] {
            updateActualSize(lineEdit, i, currentSize);
        });
        m_matrixLayout->addWidget(lineEdit, i, m_size);
    }
    for (int j = 0; j < m_size + 1; j++) {
        lineEdit = new QLineEdit(this);
        lineEdit->setFixedSize(40, 20);
        lineEdit->setAlignment(Qt::AlignCenter);
        lineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        connect(lineEdit, &QLineEdit::editingFinished, this, [lineEdit, this, currentSize, j] {
            updateActualSize(lineEdit, currentSize, j);
        });
        m_matrixLayout->addWidget(lineEdit, m_size, j);
    }
    m_size++;
}

void InputMatrixWidget::decrease() {
    if (m_size == 1) return;

    QLayoutItem *child;
    for (int i = 0; i < m_size; i++) {
        child = m_matrixLayout->itemAtPosition(i, m_size - 1);
        delete child->widget();
    }
    for (int j = 0; j < m_size - 1; j++) {
        child = m_matrixLayout->itemAtPosition(m_size - 1, j);
        delete child->widget();
    }
    m_size--;
    updateActualSize(nullptr, m_size, m_size);
}

void InputMatrixWidget::updateActualSize(const QLineEdit* lineEdit, int row, int col) {
    if (!lineEdit || lineEdit->text().isEmpty()) {
        m_rows = 0;
        m_cols = 0;
        QString text;
        for (int i = 0; i < m_size; i++) {
            for (int j = 0; j < m_size; j++) {
                text = qobject_cast<QLineEdit*>(m_matrixLayout->itemAtPosition(i, j)->widget())->text();
                if (!text.isEmpty()) {
                    if (i > m_rows - 1) m_rows = i + 1;
                    if (j > m_cols - 1) m_cols = j + 1;
                }
            }
        }
    }
    else {
        if (row > m_rows - 1) m_rows = row + 1;
        if (col > m_cols - 1) m_cols = col + 1;
    }

    // dla estetyki ustaw placeholdery
    QLineEdit* element;
    for (int i = 0; i < m_size; i++) {
        for (int j = 0; j < m_size; j++) {
            element = qobject_cast<QLineEdit*>(m_matrixLayout->itemAtPosition(i, j)->widget());
            element->setPlaceholderText("");
        }
    }
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            element = qobject_cast<QLineEdit*>(m_matrixLayout->itemAtPosition(i, j)->widget());
            if (element->text().isEmpty())
                element->setPlaceholderText("0");
        }
    }
}

char InputMatrixWidget::getName() const
{
    return m_name;
}

void InputMatrixWidget::setName(char name)
{
    m_name = name;
    m_matrixLabel->setText(QString("Matrix ") + m_name);
}

int InputMatrixWidget::getRows() const
{
    return m_rows;
}

int InputMatrixWidget::getCols() const
{
    return m_cols;
}

QString InputMatrixWidget::getTextAtPosition(int row, int col) const
{
    return qobject_cast<QLineEdit*>(m_matrixLayout->itemAtPosition(row, col)->widget())->text();
}

void InputMatrixWidget::addComboBoxOption(QString name) {
    int i = 0;
    int maxCount = m_nameInput->count();
    while (i < maxCount && name > m_nameInput->itemText(i)) i++;
    m_nameInput->insertItem(i, name);
    m_nameInput->setCurrentIndex(0);
}

void InputMatrixWidget::removeComboBoxOption(int pos) {
    m_nameInput->removeItem(pos);
}

Matrix InputMatrixWidget::getMatrix() {
    return m_matrix;
}

bool InputMatrixWidget::createActualMatrix()
{
    if (m_rows == 0) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowFlags(
            Qt::Dialog |
            Qt::WindowTitleHint |
            Qt::CustomizeWindowHint
            );
        msgBox.setText(QString("Matrix ") + m_name + " is empty.\nDefine at least one its element.");
        msgBox.exec();
        return false;
    }

    QString text;
    QString wyrazenie = "{";
    for (int i = 0; i < m_rows - 1; i++) {
        wyrazenie += "{";
        for (int j = 0; j < m_cols - 1; j++) {
            text = getTextAtPosition(i, j);
            wyrazenie += text.isEmpty() ? "0" : text;
            wyrazenie += ",";
        }
        text = getTextAtPosition(i, m_cols - 1);
        wyrazenie += text.isEmpty() ? "0" : text;
        wyrazenie += "},";
    }
    wyrazenie += "{";
    for (int j = 0; j < m_cols - 1; j++) {
        text = getTextAtPosition(m_rows - 1, j);
        wyrazenie += text.isEmpty() ? "0" : text;
        wyrazenie += ",";
    }
    text = getTextAtPosition(m_rows - 1, m_cols - 1);
    wyrazenie += text.isEmpty() ? "0" : text;
    wyrazenie += "}}";

    try {
        m_matrix = Matrix(wyrazenie.toLocal8Bit().data());
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
        return false;
    }
    return true;
}

void InputMatrixWidget::fillMatrix(Matrix* matrix)
{
    while (m_matrixLayout->itemAt(0)) delete m_matrixLayout->itemAt(0)->widget();

    m_rows = matrix->getRows();
    m_cols = matrix->getCols();
    m_size = (m_rows >= m_cols ? m_rows : m_cols);

    QLineEdit *lineEdit;
    for (int i = 0; i < m_size; i++) {
        for (int j = 0; j < m_size; j++) {
            lineEdit = new QLineEdit(matrix->getElement(i, j).toNapis().getStr(), this);
            lineEdit->setFixedSize(40, 20);
            lineEdit->setAlignment(Qt::AlignCenter);
            lineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            connect(lineEdit, &QLineEdit::editingFinished, this, [lineEdit, this, i, j] {
                updateActualSize(lineEdit, i, j);
            });
            m_matrixLayout->addWidget(lineEdit, i, j);
        }
    }
    createActualMatrix();
}
