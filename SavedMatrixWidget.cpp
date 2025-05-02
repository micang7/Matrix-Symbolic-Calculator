#include "SavedMatrixWidget.h"

#include "MainWindow.h"

SavedMatrixWidget::SavedMatrixWidget(QString name, const Matrix& matrix, QWidget *parent)
    : QWidget(parent),
    m_name(name),
    m_matrix(matrix),
    m_strCondensed(m_matrix.toNapis().getStr()),
    m_strExpanded(m_matrix.toNapisExpand().getStr()),
    m_show(false)
{
    // layouts
    m_vLayout = new QVBoxLayout(this);
    m_vLayout->setContentsMargins(3, 3, 3, 3);
    m_vLayout->setSpacing(5);

    m_hLayout = new QHBoxLayout();
    m_hLayout->setContentsMargins(0, 0, 0, 0);
    m_hLayout->setSpacing(5);

    // new widgets
    QString title = m_name + " : " + m_strCondensed;
    if (title.length() > 29) title = title.mid(0, 25) + " ...";
    m_matrixTitle = new QLabel(title);

    m_matrixExpanded = new DisplayedMatrixWidget(m_strExpanded);

    m_getExpanded = new QToolButton();
    m_undefine = new QToolButton();

    // settings
    m_matrixTitle->setMaximumWidth(145);

    m_matrixExpanded->setFont(QFont("Lucida Console", 9));
    m_matrixExpanded->setContentsMargins(17, 0, 0, 0);

    m_getExpanded->setText("⏷");
    m_getExpanded->setFixedSize(18, 16);

    m_undefine->setText("×");
    m_undefine->setFixedSize(18, 16);

    // add widgets to layouts
    m_hLayout->addWidget(m_matrixTitle);
    m_hLayout->addWidget(m_getExpanded);
    m_hLayout->addWidget(m_undefine);

    m_hLayout->setAlignment(m_matrixTitle, Qt::AlignLeft);

    m_vLayout->addLayout(m_hLayout);

    // signals and slots
    connect(m_getExpanded, &QToolButton::clicked, this, &SavedMatrixWidget::toggleExpanded);
    connect(m_undefine, &QToolButton::clicked, parent, [this, parent] {
        qobject_cast<MainWindow*>(parent)->unsaveMatrix(this, m_name);
    });
}

SavedMatrixWidget::~SavedMatrixWidget() {
    delete m_matrixTitle;
    delete m_matrixExpanded;
    delete m_getExpanded;
    delete m_undefine;
    delete m_hLayout;
    delete m_vLayout;
}

QString SavedMatrixWidget::getName() {
    return m_name;
}

QString SavedMatrixWidget::getCondensed() {
    return m_strCondensed;
}

void SavedMatrixWidget::toggleExpanded() {
    if (m_show) {
        m_vLayout->removeWidget(m_matrixExpanded);
        m_matrixExpanded->setVisible(false);
        m_show = false;
    }
    else {
        m_vLayout->addWidget(m_matrixExpanded);
        m_vLayout->setAlignment(m_matrixExpanded, Qt::AlignTop);
        m_matrixExpanded->setVisible(true);
        m_show = true;
    }
}
