#pragma once
#include <QMainWindow>

class rlglQOpenGLWidget;

typedef QFunctionPointer (*myLoadFunction)(const char *name);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    void initRlGlApp();
    rlglQOpenGLWidget* glWidget;

private slots:
    void onAddNew();
};
