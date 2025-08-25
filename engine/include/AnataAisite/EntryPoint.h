#pragma once

#include "Application.h"
#include "Log.h"


// ---Entry Point--------------------------
extern Aisite::Application* Aisite::CreateApplication();

int main(int argc, char* argv[])
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    Aisite::Log::Init();
    AT_CORE_WARN("Initlized Log!");
    AT_INFO("Hello from {}!", 1);

    const auto app = Aisite::CreateApplication();
    app->Run();
    delete app;
}
// ----------------------------------------



#define AISITE_APP(appClass) Aisite::Application* Aisite::CreateApplication() { return new appClass(); }