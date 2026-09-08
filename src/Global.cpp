#include "Global.hpp"
#include "Main.hpp"

namespace th095
{

// TH095's static initializer/destructor wrappers at 0x00493F30/0x00494210
// construct and destroy the object at 0x004BE3C8. TH08 corroborates the same
// Global.cpp ownership. DIFFABLE_STATIC preserves that real production
// storage while leaving address-bound comparison builds free to externalize it.
DIFFABLE_STATIC(Chain, g_Chain);

// The target keeps the two eight-byte RNG states consecutively at
// 0x004BE208/0x004BE210.  They are process-lifetime objects owned by the same
// global translation unit as their implementations and static initialization.
DIFFABLE_STATIC(Rng, g_Rng);
DIFFABLE_STATIC(Rng, g_Rng2);

// The log buffer begins at 0x004C2420; its cursor and message-box flag are
// members at the end of that same object, not independent proxy globals.
DIFFABLE_STATIC(GameErrorContext, g_GameErrorContext);

// TH095 keeps the persistent input snapshots in the Global family.  The menu
// pair at 0x004BE21C/0x004BE21E and the frame-history counters are distinct
// storage; TH08 independently corroborates Global.cpp ownership for the same
// input-state family.
DIFFABLE_STATIC(u16, g_ResultMenuInput);
DIFFABLE_STATIC(u16, g_PressedButtons);
DIFFABLE_STATIC(u16, g_CurFrameInput);
DIFFABLE_STATIC(u16, g_LastFrameInput);
DIFFABLE_STATIC(u16, g_NumOfFramesInputsWereHeld);
DIFFABLE_STATIC(u16, g_IsEighthFrameOfHeldInput);
DIFFABLE_STATIC(u32, g_PhotoScreenFadeColor);

ChainElem::ChainElem()
{
    this->prev = NULL;
    this->next = NULL;
    this->callback = NULL;
    this->releaseTarget = this;
    this->addedCallback = NULL;
    this->deletedCallback = NULL;
    this->priority = 0;
    this->isHeapAllocated = false;
}

ChainElem::~ChainElem()
{
    if (this->deletedCallback != NULL)
    {
        this->deletedCallback(this->arg);
    }

    this->prev = NULL;
    this->next = NULL;
    this->callback = NULL;
    this->addedCallback = NULL;
    this->deletedCallback = NULL;
}

Chain::Chain()
{
}

Chain::~Chain()
{
}

i32 Chain::AddToCalcChain(ChainElem *elem, i32 priority)
{
    ChainElem *current = &this->calcChain;
    i32 result = 0;

    if (elem->addedCallback != NULL)
    {
        result = elem->addedCallback(elem->arg);
        elem->addedCallback = NULL;
    }

    g_Supervisor.EnterCriticalSectionWrapper(0);
    g_Supervisor.criticalSectionLockCounts[0]++;
    elem->priority = priority;
    while (current->next != NULL)
    {
        if (current->priority > priority)
        {
            break;
        }
        current = current->next;
    }

    if (current->priority > priority)
    {
        elem->next = current;
        elem->prev = current->prev;
        if (elem->prev != NULL)
        {
            elem->prev->next = elem;
        }
        current->prev = elem;
    }
    else
    {
        elem->next = NULL;
        elem->prev = current;
        current->next = elem;
    }

    g_Supervisor.LeaveCriticalSectionWrapper(0);
    g_Supervisor.criticalSectionLockCounts[0]--;
    return result;
}

i32 Chain::AddToDrawChain(ChainElem *elem, i32 priority)
{
    ChainElem *current = &this->drawChain;
    i32 result = 0;

    if (elem->addedCallback != NULL)
    {
        result = elem->addedCallback(elem->arg);
        elem->addedCallback = NULL;
    }

    g_Supervisor.EnterCriticalSectionWrapper(0);
    g_Supervisor.criticalSectionLockCounts[0]++;
    elem->priority = priority;
    while (current->next != NULL)
    {
        if (current->priority > priority)
        {
            break;
        }
        current = current->next;
    }

    if (current->priority > priority)
    {
        elem->next = current;
        elem->prev = current->prev;
        if (elem->prev != NULL)
        {
            elem->prev->next = elem;
        }
        current->prev = elem;
    }
    else
    {
        elem->next = NULL;
        elem->prev = current;
        current->next = elem;
    }

    g_Supervisor.LeaveCriticalSectionWrapper(0);
    g_Supervisor.criticalSectionLockCounts[0]--;
    return result;
}

i32 Chain::RunCalcChain()
{
    ChainElem *current;
    i32 updatedCount;
    ChainCallbackResult result;

    g_Supervisor.EnterCriticalSectionWrapper(0);
    g_Supervisor.criticalSectionLockCounts[0]++;

restartFromFirstJob:
    updatedCount = 0;
    current = &this->calcChain;
    while (current != NULL)
    {
        if (current->callback != NULL)
        {
        executeAgain:
            g_Supervisor.LeaveCriticalSectionWrapper(0);
            g_Supervisor.criticalSectionLockCounts[0]--;
            result = current->callback(current->arg);
            g_Supervisor.EnterCriticalSectionWrapper(0);
            g_Supervisor.criticalSectionLockCounts[0]++;

            switch (result)
            {
            case CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB:
            {
                ChainElem *tmp1 = current;
                current = current->next;
                this->CutImpl(tmp1);
                updatedCount++;
                continue;
            }

            case CHAIN_CALLBACK_RESULT_EXECUTE_AGAIN:
                goto executeAgain;

            case CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS:
                updatedCount = 0;
                goto loopExit;

            case CHAIN_CALLBACK_RESULT_BREAK:
                updatedCount = 1;
                goto loopExit;

            case CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR:
                updatedCount = -1;
                goto loopExit;

            case CHAIN_CALLBACK_RESULT_RESTART_FROM_FIRST_JOB:
                goto restartFromFirstJob;

            default:
                break;
            }
            updatedCount++;
        }
        current = current->next;
    }

loopExit:
    g_Supervisor.LeaveCriticalSectionWrapper(0);
    g_Supervisor.criticalSectionLockCounts[0]--;
    return updatedCount;
}

i32 Chain::RunDrawChain()
{
    ChainElem *current;
    i32 updatedCount;
    ChainCallbackResult result;

    updatedCount = 0;
    current = &this->drawChain;
    g_Supervisor.EnterCriticalSectionWrapper(0);
    g_Supervisor.criticalSectionLockCounts[0]++;
    while (current != NULL)
    {
        if (current->callback != NULL)
        {
        executeAgain:
            g_Supervisor.LeaveCriticalSectionWrapper(0);
            g_Supervisor.criticalSectionLockCounts[0]--;
            result = current->callback(current->arg);
            g_Supervisor.EnterCriticalSectionWrapper(0);
            g_Supervisor.criticalSectionLockCounts[0]++;

            switch (result)
            {
            case CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB:
            {
                ChainElem *tmp1 = current;
                current = current->next;
                this->CutImpl(tmp1);
                updatedCount++;
                continue;
            }

            case CHAIN_CALLBACK_RESULT_EXECUTE_AGAIN:
                goto executeAgain;

            case CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS:
                updatedCount = 0;
                goto loopExit;

            case CHAIN_CALLBACK_RESULT_BREAK:
                updatedCount = 1;
                goto loopExit;

            case CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR:
                updatedCount = -1;
                goto loopExit;

            default:
                break;
            }
            updatedCount++;
        }
        current = current->next;
    }

loopExit:
    g_Supervisor.LeaveCriticalSectionWrapper(0);
    g_Supervisor.criticalSectionLockCounts[0]--;
    return updatedCount;
}

// TH08 proves that the snapshot head and cursor are separate locals. Stock
// VC7.1 places the real cursor in the target's shallow third pointer bucket
// when backed by jLocal00; keeping it separate also makes EH unwind destroy the
// ChainElem head directly instead of synthesizing an aggregate destructor.
#define releaseSnapshotCursor jLocal00
void Chain::ReleaseSingleChain(ChainElem *root)
{
    ChainElem releaseSnapshotHead;
    ChainElem *current;
    ChainElem *releaseSnapshotCursor;
    ChainElem *nextSnapshotEntry;

    releaseSnapshotCursor = new ChainElem();
    releaseSnapshotHead.next = releaseSnapshotCursor;

    current = root;
    while (current != NULL)
    {
        releaseSnapshotCursor->releaseTarget = current;
        releaseSnapshotCursor->next = new ChainElem();
        releaseSnapshotCursor = releaseSnapshotCursor->next;
        current = current->next;
    }

    current = &releaseSnapshotHead;
    while (current != NULL)
    {
        this->Cut(current->releaseTarget);
        current = current->next;
    }

    releaseSnapshotCursor = releaseSnapshotHead.next;
    while (releaseSnapshotCursor != NULL)
    {
        nextSnapshotEntry = releaseSnapshotCursor->next;
        delete releaseSnapshotCursor;
        releaseSnapshotCursor = NULL;
        releaseSnapshotCursor = nextSnapshotEntry;
    }
}
#undef releaseSnapshotCursor

void Chain::Release()
{
    g_Supervisor.StopReplayScan();
    this->ReleaseSingleChain(&this->calcChain);
    this->ReleaseSingleChain(&this->drawChain);
}

ChainElem *Chain::CreateElem(ChainCallback callback)
{
    ChainElem *elem = new ChainElem();

    elem->SetCallback(callback);
    elem->isHeapAllocated = true;
    return elem;
}

void Chain::Cut(ChainElem *toRemove)
{
    if (toRemove == NULL)
    {
        return;
    }

    g_Supervisor.EnterCriticalSectionWrapper(0);
    g_Supervisor.criticalSectionLockCounts[0]++;
    this->CutImpl(toRemove);
    g_Supervisor.LeaveCriticalSectionWrapper(0);
    g_Supervisor.criticalSectionLockCounts[0]--;
}

void Chain::CutImpl(ChainElem *toRemove)
{
    BOOL isDrawChain;
    ChainElem *tmp;

    isDrawChain = FALSE;
    if (toRemove == NULL)
    {
        return;
    }

    tmp = &this->calcChain;
    while (tmp != NULL)
    {
        if (tmp == toRemove)
        {
            goto destroyElem;
        }
        tmp = tmp->next;
    }

    isDrawChain = TRUE;
    tmp = &this->drawChain;
    while (tmp != NULL)
    {
        if (tmp == toRemove)
        {
            goto destroyElem;
        }
        tmp = tmp->next;
    }
    return;

destroyElem:
    if (toRemove->prev != NULL)
    {
        toRemove->callback = NULL;
        toRemove->prev->next = toRemove->next;
        if (toRemove->next != NULL)
        {
            toRemove->next->prev = toRemove->prev;
        }
        toRemove->prev = NULL;
        toRemove->next = NULL;

        if (toRemove->isHeapAllocated)
        {
            g_Supervisor.LeaveCriticalSectionWrapper(0);
            g_Supervisor.criticalSectionLockCounts[0]--;
            delete toRemove;
            toRemove = NULL;
            g_Supervisor.EnterCriticalSectionWrapper(0);
            g_Supervisor.criticalSectionLockCounts[0]++;
        }
        else
        {
            if (toRemove->deletedCallback != NULL)
            {
                ChainLifetimeCallback callback = toRemove->deletedCallback;
                toRemove->deletedCallback = NULL;
                g_Supervisor.LeaveCriticalSectionWrapper(0);
                g_Supervisor.criticalSectionLockCounts[0]--;
                callback(toRemove->arg);
                g_Supervisor.EnterCriticalSectionWrapper(0);
                g_Supervisor.criticalSectionLockCounts[0]++;
            }
        }
    }
}

ZunMemory::~ZunMemory()
{
    if (this->bRegistryInUse)
    {
        for (i32 index = 0; index < ARRAY_SIZE_SIGNED(this->registry); index++)
        {
            if (this->registry[index] != NULL)
            {
                free(this->registry[index]);
            }
        }
    }
}

} // namespace th095
