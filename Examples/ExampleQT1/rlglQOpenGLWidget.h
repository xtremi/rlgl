#ifndef RLGLQOPENGLWIDGET_H
#define RLGLQOPENGLWIDGET_H
//#include "glad/gl.h"
#include "ExampleQT1_App.h"
#include <QOpenGLWidget>

class QMouseEvent;

class rlglQOpenGLWidget : public QOpenGLWidget
{
public:
    rlglQOpenGLWidget();

    int initRlglApp();

    void paintGL() override;
    void initializeGL() override;

    MyApp* rlglApp = nullptr;

protected:
    void mousePressEvent(QMouseEvent* event) override;


};

#endif // RLGLQOPENGLWIDGET_H
