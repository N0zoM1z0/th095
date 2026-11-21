#include "SceneSelect.hpp"

namespace th095
{

ChainCallbackResult __fastcall SceneSelectControllerView::OnUpdate(void *controller)
{
    return static_cast<SceneSelectControllerView *>(controller)->Update();
}

} // namespace th095
