#include <iostream>
#include "Example1_App.h"


int main(int argc, char* argv[]) {

    std::cout << "started" << std::endl;

    std::string assetDirectory;
    if (argc < 2) {
        std::cout << "ERROR: Asset directory is required as first argument." << std::endl;
        return 0;
    }
    assetDirectory = argv[1];

    glm::ivec2 windowSize(1920/2, 1440/2);

	//rlgl::DemoApp app;
    MyApp app(assetDirectory);
    if (int err = app.init(windowSize.x, windowSize.y)) {
        return err;
    }

    while (!app.windowClosed()){
        if (int err = app.loopIteration()) {
            return err;
        }
    }

	app.cleanUp();

    glfwTerminate();
    return 0;
}

