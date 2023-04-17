#include <iostream>
#include <QApplication>
#include "mainwindow.h"
#include "ExampleQT1_App.h"


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


    MyApp app(assetDirectory);
    if (int err = app.init(windowSize.x, windowSize.y)) {
        return err;
    }
    while (!app.windowClosed()){
        if (int err = app.loopIteration()) {
            return err;
        }
    }


    return qapp.exec();




	app.cleanUp();
    glfwTerminate();
    return 0;
}

