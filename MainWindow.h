#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "InputMatrixWidget.h"
#include "SavedMatrixWidget.h"
#include "Matrix.h"

#include <QHBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void saveMatrix(QString name, int pos, const Matrix& matrix);
    void unsaveMatrix(SavedMatrixWidget* saved, QString name);

    void detBtn_onClick(InputMatrixWidget* matrix);
    void transposeBtn_onClick(InputMatrixWidget* matrix);
    void inverseBtn_onClick(InputMatrixWidget* matrix);
    void multiplyBtn_onClick(InputMatrixWidget* matrix, QString scalar);
    void raiseBtn_onClick(InputMatrixWidget* matrix, QString exponent);

    void moveToA(Matrix* matrix);
    void moveToB(Matrix* matrix);

private slots:
    void zamiana_btn_onClick();

    void dodawanie_btn_onClick();
    void odejmowanie_btn_onClick();
    void mnozenie_btn_onClick();

    void evaluate_btn_onClick();

private:
    Ui::MainWindow *ui;

    QHBoxLayout* m_macierzeLayout;
    QVBoxLayout* m_zapisaneLayout;
    QVBoxLayout* m_wynikiLayout;
    InputMatrixWidget* m_matrixWidgetA;
    InputMatrixWidget* m_matrixWidgetB;
};
#endif // MAINWINDOW_H
