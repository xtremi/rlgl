#pragma once
#include <QMainWindow>
class QOpenGLWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();


    QOpenGLWidget* glWidget;

private slots:
    void onAddNew();
};
