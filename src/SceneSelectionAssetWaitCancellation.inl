// Shared statements inside the result-queue waits. Target order is Supervisor
// close, controller stop, then the worker exit handshake, after Sleep(10).
// Each break leaves only the wait: the caller still enters publication.
if (SceneSelectionAssetsSupervisorStopRequested() != 0)
{
    break;
}
if (locals.view->flagBits.assetLoadStopRequested != 0)
{
    break;
}
if (SceneSelectionAssetsLoadComplete() != 0)
{
    break;
}
