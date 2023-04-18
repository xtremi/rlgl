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

    QSurfaceFormat format;
    format.setVersion(4,0);
    format.setSamples(4);
    format.setProfile(QSurfaceFormat::NoProfile);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    setFormat(format);

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

static int paintGLcount = 0;
void rlglQOpenGLWidget::paintGL(){
    std::cout << "paintGL " << paintGLcount++ << std::endl;
    //if(rlglApp->loopIteration()){
        //update();
        //this->context()->makeCurrent(this);

    //}
    QOpenGLWidget::paintGL();

}

static int updateRequestCount = 0;
bool rlglQOpenGLWidget::eventFilter(QObject* o, QEvent *e) {
    std::cout << "rlglQOpenGLWidget::event " << std::endl;

    if (e->type() == QEvent::UpdateRequest){
        std::cout << "UpdateRequest " << updateRequestCount++ << std::endl;
        rlglApp->loopIteration();
        return true;
    }
    return QWidget::eventFilter(o, e);
}


void rlglQOpenGLWidget::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton && rect().contains(event->pos())) {
        setFocus();
        std::cout << "focus on" << std::endl;
    }
}



