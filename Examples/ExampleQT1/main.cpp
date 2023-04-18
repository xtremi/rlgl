#include <iostream>
#include <QApplication>
#include "ExampleQT1_App.h"
#include "mainwindow.h"
#include <QOpenGLContext>
#include "rlglQOpenGLWidget.h"

void* getOpenGLProcAddress(const char* functionName)
{
    QOpenGLContext* context = QOpenGLContext::currentContext();
    if (!context)
    {
        return nullptr;
    }

    return reinterpret_cast<void*>(context->getProcAddress(functionName));
}

int main(int argc, char* argv[]) {

    std::string assetDirectory;
    if (argc < 2) {
        std::cout << "ERROR: Asset directory is required as first argument." << std::endl;
        return 0;
    }
    assetDirectory = argv[1];

    glm::ivec2 windowSize(1920/2, 1440/2);


    QApplication qapp(argc, argv);
    QCoreApplication::setApplicationName("Qt Hello GL 2 Example");
    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    
    MainWindow mainWindow;
    mainWindow.resize(QSize(windowSize.x, windowSize.y));
    mainWindow.show();

    if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(getOpenGLProcAddress)))
    {
        std::cout << "gladLoadGLLoader() - Failed to initialize GLAD" << std::endl;
        return 1;
    }


    MyApp app(assetDirectory);
    if (int err = app.init(windowSize.x, windowSize.y/*, qtGlContext->getProcAddress*/)) {
        return err;
    }
    while (true){
        if (int err = app.loopIteration()) {
            return err;
        }
        QCoreApplication::processEvents();
        mainWindow.glWidget->update();
    }


    return qapp.exec();




	app.cleanUp();
    glfwTerminate();
    return 0;
}

