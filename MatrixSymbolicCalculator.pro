QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DisplayedMatrixWidget.cpp \
    InputMatrixWidget.cpp \
    MainWindow.cpp \
    ResultWidget.cpp \
    SavedMatrixWidget.cpp \
    main.cpp \
    AbstractSyntaxTree.cpp \
    BinaryOperation.cpp \
    Constant.cpp \
    Matrix.cpp \
    Napis.cpp \
    Operation.cpp \
    Parser.cpp \
    Stos.cpp \
    TreeNode.cpp \
    UnaryOperation.cpp \
    Variable.cpp

HEADERS += \
    DisplayedMatrixWidget.h \
    InputMatrixWidget.h \
    MainWindow.h \
    AbstractSyntaxTree.h \
    BinaryOperation.h \
    Constant.h \
    Matrix.h \
    Napis.h \
    Operation.h \
    Parser.h \
    ResultWidget.h \
    SavedMatrixWidget.h \
    Stos.h \
    TreeNode.h \
    UnaryOperation.h \
    Variable.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    icon.ico
