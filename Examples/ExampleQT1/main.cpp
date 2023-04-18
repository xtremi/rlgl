#include <iostream>
#include <QApplication>
#include "glad/gl.h"
#include "mainwindow.h"
//#include <QOpenGLContext>



int main(int argc, char* argv[]) {

    std::string assetDirectory;
    if (argc < 2) {
        std::cout << "ERROR: Asset directory is required as first argument." << std::endl;
        return 0;
    }
    assetDirectory = argv[1];

    QApplication qapp(argc, argv);
    QCoreApplication::setApplicationName("Example 3 - Qt");
    QCoreApplication::setOrganizationName("rlgl");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    
    MainWindow mainWindow;
    mainWindow.resize(QSize(720, 480));
    mainWindow.show();
    return qapp.exec();
}

