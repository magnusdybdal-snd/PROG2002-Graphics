#include "Lab2Application.h"
#include <iostream>

int main(int argc, char* argv[])
{
    Lab2Application application("Lab2 - Simple Framework", "1.0");
    
    // Initialize (this does all the GLFW/GLAD setup)
    if (application.Init() != EXIT_SUCCESS) {
        std::cerr << "Failed to initialize application" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Run your main loop (this is where your current main() code goes)
    return application.Run();
}