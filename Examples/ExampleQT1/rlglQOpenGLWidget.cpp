#include "rlglQOpenGLWidget.h"
#include <iostream>
#include <QOpenGLContext>
#include <QFunctionPointer>

rlglQOpenGLWidget::rlglQOpenGLWidget() : QOpenGLWidget(nullptr)
{

}




/*void rlglQOpenGLWidget::loadGlad(){

    //QFunctionPointer (QOpenGLContext::*getProcAddress)(const char*) = &QOpenGLContext::getProcAddress;
    //QFunctionPointer (QOpenGLContext::*getProcAddress)(const char*) = &QOpenGLContext::getProcAddress;

    if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(getOpenGLProcAddress)))
    {
        std::cout << "gladLoadGLLoader() - Failed to initialize GLAD" << std::endl;
    }

}*/
