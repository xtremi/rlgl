#include "rlglQOpenGLWidget.h"
#include <iostream>
#include <QOpenGLContext>
#include <QFunctionPointer>

void* getOpenGLProcAddress(const char* functionName)
{
    QOpenGLContext* context = QOpenGLContext::currentContext();
    if (!context)
    {
        return nullptr;
    }

    return reinterpret_cast<void*>(context->getProcAddress(functionName));
}

rlglQOpenGLWidget::rlglQOpenGLWidget() : QOpenGLWidget(nullptr)
{


}

int rlglQOpenGLWidget::initRlglApp(){
    rlglApp = new MyApp("../../data");

    if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(getOpenGLProcAddress)))
    {
        std::cout << "gladLoadGLLoader() - Failed to initialize GLAD" << std::endl;
        return 1;
    }

    if (int err = rlglApp->init(720, 480/*, qtGlContext->getProcAddress*/)) {
        return err;
    }
}


void rlglQOpenGLWidget::paintGL(){
    if(int err = rlglApp->loopIteration()){
        throw("rlglApp->loopIteration() returned error");
    }
}
