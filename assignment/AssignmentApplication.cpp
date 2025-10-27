#include "AssignmentApplication.h"

AssignmentApplication::AssignmentApplication(const std::string &name, const std::string &version)
    : GLFWApplication(name, version, 800, 600),
    m_selectedX(0),
    m_selectedY(0)
{

}

AssignmentApplication::~AssignmentApplication()

{

}

unsigned AssignmentApplication::Init()
{
    return 0;
}

unsigned AssignmentApplication::Run()
{
    return 0;
}
