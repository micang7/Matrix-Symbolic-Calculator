#include "ResultWidget.h"

#include "MainWindow.h"

ResultWidget::ResultWidget(QHBoxLayout* equation, Matrix&& resultMatrix, QWidget* parent)
    : QWidget(parent), m_equation(equation), m_resultMatrix(std::move(resultMatrix))
{
    // layouts
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(15);

    m_buttonsLayout = new QVBoxLayout();
    m_buttonsLayout->setContentsMargins(0, 0, 0, 0);
    m_buttonsLayout->setSpacing(5);
    m_buttonsLayout->setAlignment(Qt::AlignTop);

    // widgets
    m_moveToA = new QPushButton("Move to A");
    m_moveToB = new QPushButton("Move to B");
    m_delete = new QPushButton("Delete");

    // settings
    m_moveToA->setFixedSize(80, 25);
    m_moveToB->setFixedSize(80, 25);
    m_delete->setFixedSize(80, 25);

    // add widgets to layouts
    m_buttonsLayout->addWidget(m_moveToA);
    m_buttonsLayout->addWidget(m_moveToB);
    m_buttonsLayout->addWidget(m_delete);

    m_layout->addLayout(m_equation);
    m_layout->addLayout(m_buttonsLayout);

    m_layout->setAlignment(m_equation, Qt::AlignLeft);

    // signals and slots
    connect(m_moveToA, &QPushButton::clicked, parent, [this, parent] {
        qobject_cast<MainWindow*>(parent)->moveToA(&m_resultMatrix);
    });

    connect(m_moveToB, &QPushButton::clicked, parent, [this, parent] {
        qobject_cast<MainWindow*>(parent)->moveToB(&m_resultMatrix);
    });

    connect(m_delete, &QPushButton::clicked, this, [this] {
        delete this;
    });
}

ResultWidget::~ResultWidget() {
    delete m_buttonsLayout;
    delete m_moveToA;
    delete m_moveToB;
    delete m_delete;
    delete m_equation;
    delete m_layout;
}
