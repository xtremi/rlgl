#include <iostream>
#include "MyApp.h"


int main(int argc, char* argv[]) {

    glm::ivec2 windowSize(800, 600);

	//rlgl::DemoApp app;
	MyApp app;
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

