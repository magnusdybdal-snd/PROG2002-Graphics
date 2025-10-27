#include "Lab4Application.h"

int main(int argc, char* argv[])
{

    // Create the Lab2 applicatoin
    Lab4Application application("Lab 4 - Textures", "1.0");

    // Initialize
    if (application.Init() != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    // Run the application
    return application.Run();
}