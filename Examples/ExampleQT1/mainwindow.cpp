#include <glad/gl.h>
#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include "rlglQOpenGLWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>

MainWindow::MainWindow()
{
    QMenuBar *menuBar = new QMenuBar;
    QMenu *menuWindow = menuBar->addMenu(tr("&Window"));
    QAction *addNew = new QAction(menuWindow);
    addNew->setText(tr("Add new"));
    menuWindow->addAction(addNew);
    connect(addNew, &QAction::triggered, this, &MainWindow::onAddNew);
    setMenuBar(menuBar);

    glWidget = new rlglQOpenGLWidget();

    QSlider *slider1 = new QSlider(Qt::Vertical);
    QSlider *slider2 = new QSlider(Qt::Vertical);
    slider1->setRange(0, 360 * 16);
    slider2->setRange(0, 360 * 16);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *container = new QHBoxLayout;
    container->addWidget(glWidget);
    container->addWidget(slider1);
    container->addWidget(slider2);

    QWidget *w = new QWidget;
    w->setLayout(container);
    mainLayout->addWidget(w);
    setLayout(mainLayout);

    setCentralWidget(w);

    setWindowTitle(tr("Hello GL"));

    glWidget->makeCurrent();
}


void MainWindow::onAddNew()
{
    QMessageBox::information(nullptr, tr("Infobox"),
        tr("Custom message, hello!"));
}
