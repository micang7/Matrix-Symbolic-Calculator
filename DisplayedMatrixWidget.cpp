#include "DisplayedMatrixWidget.h"

#include <QPainter>
#include <QPixmap>

DisplayedMatrixWidget::DisplayedMatrixWidget(QString matrix, QWidget* parent)
    : QWidget(parent), m_str(matrix)
{
    // layout
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    m_layout->setAlignment(Qt::AlignLeft);

    // new widgets
    m_leftP = new QLabel();
    m_rightP = new QLabel();
    m_matrix = new QLabel(m_str);

    // settings
    m_matrix->setFont(QFont("Lucida Console", 10));
    m_matrix->adjustSize();

    int w = 10;
    int h = m_matrix->height();
    m_leftP->setFixedSize(w, h);
    m_rightP->setFixedSize(w, h);

    QPixmap pixmap(m_leftP->size());
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    QPen pen(Qt::black, 1);
    painter.setPen(pen);

    painter.drawLine(2, 0, 2, h-2);
    painter.drawLine(2, 0, w-4, 0);
    painter.drawLine(2, h-2, w-4, h-2);
    m_leftP->setPixmap(pixmap);

    painter.eraseRect(0, 0, w, h);

    painter.drawLine(w-4, 0, w-4, h-2);
    painter.drawLine(2, 0, w-4, 0);
    painter.drawLine(2, h-2, w-4, h-2);
    m_rightP->setPixmap(pixmap);

    // add widgets to layouts
    m_layout->addWidget(m_leftP);
    m_layout->addWidget(m_matrix);
    m_layout->addWidget(m_rightP);
}

DisplayedMatrixWidget::~DisplayedMatrixWidget() {
    delete m_layout;
    delete m_rightP;
    delete m_leftP;
    delete m_matrix;
}
