#ifndef INPUTMATRIXWIDGET_H
#define INPUTMATRIXWIDGET_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include "Matrix.h"
#include "SavedMatrixWidget.h"

class InputMatrixWidget : public QWidget {
    Q_OBJECT

public:
    InputMatrixWidget(char name, QWidget *parent = nullptr);

    char getName() const;
    void setName(char name);

    int getRows() const;
    int getCols() const;

    QString getTextAtPosition(int row, int col) const;

    void addComboBoxOption(QString name);
    void removeComboBoxOption(int pos);

    Matrix getMatrix();

    bool createActualMatrix();

    void fillMatrix(Matrix* matrix);

private slots:
    void clearMatrix();
    void increase();
    void decrease();

    void updateActualSize(const QLineEdit* lineEdit, int row, int col);

private:
    QLabel* m_matrixLabel;
    QVBoxLayout* m_layout;
    QGridLayout *m_matrixLayout;
    QComboBox* m_nameInput;

    int m_size;
    int m_rows;
    int m_cols;
    char m_name;

    Matrix m_matrix;
};

#endif // INPUTMATRIXWIDGET_H
