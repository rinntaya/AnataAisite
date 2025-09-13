#include <AnataAisite.h>
#include <AnataAisite/EntryPoint.h>

#include "Sandbox2D.h"


class Sandbox : public Aisite::Application
{
public:
    Sandbox(const Aisite::ApplicationCommandLineArgs args)
        :Application("Sandbox", args)
    {
        PushLayer(new Sandbox2D());
    }
};

AISITE_APP(Sandbox);
