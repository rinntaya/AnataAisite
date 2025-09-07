#include <AnataAisite.h>
#include <AnataAisite/EntryPoint.h>

#include "EditorLayer.h"

namespace Aisite
{
    class AisiteEditor : public Application
    {
    public:
        AisiteEditor()
            : Application("AnataAisite Editor / 人間は嫌い")
        {
            PushLayer(new EditorLayer());
        }

    };
}

AISITE_APP(::Aisite::AisiteEditor);
