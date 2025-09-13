#include <AnataAisite.h>
#include <AnataAisite/EntryPoint.h>

#include "EditorLayer.h"

namespace Aisite
{
    class AisiteEditor : public Application
    {
    public:
        AisiteEditor(const ApplicationSpecification& spec)
            : Application(spec)
        {
            PushLayer(new EditorLayer());
        }

    };
}

AISITE_APP("AnataAisite Editor / 人間は嫌い", ::Aisite::AisiteEditor);
