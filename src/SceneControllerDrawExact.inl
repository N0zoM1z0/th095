#include "FrontEndController.hpp"

namespace th095
{

ChainCallbackResult __fastcall FrontEndControllerView::OnUpdate(void *controller)
{
    return static_cast<FrontEndControllerView *>(controller)->Update();
}

} // namespace th095
