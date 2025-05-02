#ifndef DISPLAYEDMATRIXWIDGET_H
#define DISPLAYEDMATRIXWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include "Matrix.h"

class DisplayedMatrixWidget : public QWidget {
    Q_OBJECT

public:
    DisplayedMatrixWidget(QString matrix, QWidget* parent = nullptr);
    ~DisplayedMatrixWidget();

private:
    QHBoxLayout* m_layout;
    QLabel* m_rightP;
    QLabel* m_leftP;
    QLabel* m_matrix;

    QString m_str;
};

#endif // DISPLAYEDMATRIXWIDGET_H
