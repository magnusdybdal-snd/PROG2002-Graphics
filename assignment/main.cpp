#include "AssignmentApplication.h"

int main(int argc, char* argv[])
{

    // Create the Lab2 applicatoin
    AssignmentApplication application("Assignment", "1.0");

    // Initialize
    if (application.Init() != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    // Run the application
    return application.Run();
}