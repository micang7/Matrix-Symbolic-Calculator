#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "Matrix.h"

class ResultWidget : public QWidget {
    Q_OBJECT

public:
    ResultWidget(QHBoxLayout* m_equation, Matrix&& resultMatrix, QWidget* parent = nullptr);
    ~ResultWidget();

private:
    QHBoxLayout* m_layout;
    QHBoxLayout* m_equation;
    QVBoxLayout* m_buttonsLayout;
    QPushButton* m_moveToA;
    QPushButton* m_moveToB;
    QPushButton* m_delete;

    Matrix m_resultMatrix;
};

#endif // RESULTWIDGET_H
