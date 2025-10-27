#include "Lab3Application.h"

int main(int argc, char* argv[])
{

    // Create the Lab2 applicatoin
    Lab3Application application("Lab 3 - Chessboard", "1.0");

    // Initialize
    if (application.Init() != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    // Run the application
    return application.Run();
}