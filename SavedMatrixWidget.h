#ifndef SAVEDMATRIXWIDGET_H
#define SAVEDMATRIXWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QToolButton>
#include "Matrix.h"
#include "DisplayedMatrixWidget.h"

class SavedMatrixWidget : public QWidget {
    Q_OBJECT

public:
    SavedMatrixWidget(QString name, const Matrix& matrix, QWidget *parent = nullptr);
    ~SavedMatrixWidget();

    QString getName();
    QString getCondensed();

private slots:
    void toggleExpanded();

private:
    QHBoxLayout* m_hLayout;
    QVBoxLayout* m_vLayout;
    QLabel* m_matrixTitle;
    DisplayedMatrixWidget* m_matrixExpanded;
    QToolButton* m_getExpanded;
    QToolButton* m_undefine;

    QString m_name;
    Matrix m_matrix;
    QString m_strCondensed;
    QString m_strExpanded;
    bool m_show;
};

#endif // SAVEDMATRIXWIDGET_H
