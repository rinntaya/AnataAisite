#pragma once

#include "Application.h"
#include "Debug/Log.h"
#include "Debug/Instrumentor.h"


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

    AT_PROFILE_BEGIN_SESSION("Startup", "TracingProfile-Startup.json");
    const auto app = Aisite::CreateApplication();
    AT_PROFILE_END_SESSION();

    AT_PROFILE_BEGIN_SESSION("Runtime", "TracingProfile-Runtime.json");
    app->Run();
    AT_PROFILE_END_SESSION();

    AT_PROFILE_BEGIN_SESSION("Shutdown", "TracingProfile-Shutdown.json");
    delete app;
    AT_PROFILE_END_SESSION();
}
// ----------------------------------------



#define AISITE_APP(appClass) Aisite::Application* Aisite::CreateApplication() { return new appClass(); }