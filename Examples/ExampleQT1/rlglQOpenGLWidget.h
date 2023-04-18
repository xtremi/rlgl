#ifndef RLGLQOPENGLWIDGET_H
#define RLGLQOPENGLWIDGET_H
//#include "glad/gl.h"
#include "ExampleQT1_App.h"
#include <QOpenGLWidget>


class rlglQOpenGLWidget : public QOpenGLWidget
{
public:
    rlglQOpenGLWidget();

    int initRlglApp();

    void paintGL() override;

    MyApp* rlglApp = nullptr;
};

#endif // RLGLQOPENGLWIDGET_H
