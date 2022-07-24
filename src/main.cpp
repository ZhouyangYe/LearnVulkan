#include "Core/Application.h"

int main(int argc, char* argv[]) {
    LearnVulkan::Application app;

    try {
        app.Run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
