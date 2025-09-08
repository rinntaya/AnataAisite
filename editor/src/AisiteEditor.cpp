#include <AnataAisite.h>
#include <AnataAisite/EntryPoint.h>

#include "EditorLayer.h"

namespace Aisite
{
    class AisiteEditor : public Application
    {
    public:
        AisiteEditor(const ApplicationCommandLineArgs args)
            : Application("AnataAisite Editor / 人間は嫌い", args)
        {
            PushLayer(new EditorLayer());
        }

    };
}

AISITE_APP(::Aisite::AisiteEditor);
