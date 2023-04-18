#include "rlglQOpenGLWidget.h"
#include <iostream>
#include <QOpenGLContext>
#include <QFunctionPointer>
#include <QMouseEvent>

void* getOpenGLProcAddress(const char* functionName)
{
    QOpenGLContext* context = QOpenGLContext::currentContext();
    if (!context)
    {
        return nullptr;
    }

    return reinterpret_cast<void*>(context->getProcAddress(functionName));
}

rlglQOpenGLWidget::rlglQOpenGLWidget() : QOpenGLWidget(nullptr){}


void rlglQOpenGLWidget::initializeGL(){

    if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(getOpenGLProcAddress)))
    {
        throw("gladLoadGLLoader() - Failed to initialize GLAD");
    }
    initRlglApp();
}


int rlglQOpenGLWidget::initRlglApp(){
    setFocus();
    rlglApp = new MyApp("../../../data");

    this->installEventFilter(rlglApp);
    if (int err = rlglApp->init(720, 480/*, qtGlContext->getProcAddress*/)) {
        return err;
    }

}


void rlglQOpenGLWidget::paintGL(){

    if(int err = rlglApp->loopIteration()){
        throw("rlglApp->loopIteration() returned error");
    }
    QOpenGLWidget::paintGL();
}


void rlglQOpenGLWidget::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton && rect().contains(event->pos())) {
        setFocus();
        std::cout << "focus on" << std::endl;
    }
}



