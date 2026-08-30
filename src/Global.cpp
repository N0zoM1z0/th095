#include "Global.hpp"

namespace th095
{

struct ChainSupervisorView
{
    u8 unknown000[0x664];
    CRITICAL_SECTION criticalSections[7];
    u8 lockCounts[7];

    void StopReplayScan();

    void EnterCriticalSectionWrapper(i32 id)
    {
        EnterCriticalSection(&this->criticalSections[id]);
        this->lockCounts[id]++;
    }

    void LeaveCriticalSectionWrapper(i32 id)
    {
        LeaveCriticalSection(&this->criticalSections[id]);
        this->lockCounts[id]--;
    }
};

extern ChainSupervisorView g_ChainSupervisor;

typedef char ChainSupervisorCriticalSectionsAt664[
    (offsetof(ChainSupervisorView, criticalSections) == 0x664) ? 1 : -1];
typedef char ChainSupervisorLockCountsAt70C[
    (offsetof(ChainSupervisorView, lockCounts) == 0x70c) ? 1 : -1];

struct ChainReleaseSnapshotLocals
{
    ChainElem head;
    ChainElem *cursor;
};

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

    g_ChainSupervisor.EnterCriticalSectionWrapper(0);
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

    g_ChainSupervisor.LeaveCriticalSectionWrapper(0);
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

    g_ChainSupervisor.EnterCriticalSectionWrapper(0);
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

    g_ChainSupervisor.LeaveCriticalSectionWrapper(0);
    return result;
}

i32 Chain::RunCalcChain()
{
    ChainElem *current;
    i32 updatedCount;
    ChainCallbackResult result;

    g_ChainSupervisor.EnterCriticalSectionWrapper(0);

restartFromFirstJob:
    updatedCount = 0;
    current = &this->calcChain;
    while (current != NULL)
    {
        if (current->callback != NULL)
        {
        executeAgain:
            g_ChainSupervisor.LeaveCriticalSectionWrapper(0);
            result = current->callback(current->arg);
            g_ChainSupervisor.EnterCriticalSectionWrapper(0);

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
    g_ChainSupervisor.LeaveCriticalSectionWrapper(0);
    return updatedCount;
}

i32 Chain::RunDrawChain()
{
    ChainElem *current;
    i32 updatedCount;
    ChainCallbackResult result;

    updatedCount = 0;
    current = &this->drawChain;
    g_ChainSupervisor.EnterCriticalSectionWrapper(0);
    while (current != NULL)
    {
        if (current->callback != NULL)
        {
        executeAgain:
            g_ChainSupervisor.LeaveCriticalSectionWrapper(0);
            result = current->callback(current->arg);
            g_ChainSupervisor.EnterCriticalSectionWrapper(0);

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
    g_ChainSupervisor.LeaveCriticalSectionWrapper(0);
    return updatedCount;
}

void Chain::ReleaseSingleChain(ChainElem *root)
{
    ChainReleaseSnapshotLocals snapshot;
    ChainElem *current;
    ChainElem *nextSnapshotEntry;

    snapshot.cursor = new ChainElem();
    snapshot.head.next = snapshot.cursor;

    current = root;
    while (current != NULL)
    {
        snapshot.cursor->releaseTarget = current;
        snapshot.cursor->next = new ChainElem();
        snapshot.cursor = snapshot.cursor->next;
        current = current->next;
    }

    current = &snapshot.head;
    while (current != NULL)
    {
        this->Cut(current->releaseTarget);
        current = current->next;
    }

    snapshot.cursor = snapshot.head.next;
    while (snapshot.cursor != NULL)
    {
        nextSnapshotEntry = snapshot.cursor->next;
        delete snapshot.cursor;
        snapshot.cursor = NULL;
        snapshot.cursor = nextSnapshotEntry;
    }
}

void Chain::Release()
{
    g_ChainSupervisor.StopReplayScan();
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

    g_ChainSupervisor.EnterCriticalSectionWrapper(0);
    this->CutImpl(toRemove);
    g_ChainSupervisor.LeaveCriticalSectionWrapper(0);
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
            g_ChainSupervisor.LeaveCriticalSectionWrapper(0);
            delete toRemove;
            toRemove = NULL;
            g_ChainSupervisor.EnterCriticalSectionWrapper(0);
        }
        else
        {
            if (toRemove->deletedCallback != NULL)
            {
                ChainLifetimeCallback callback = toRemove->deletedCallback;
                toRemove->deletedCallback = NULL;
                g_ChainSupervisor.LeaveCriticalSectionWrapper(0);
                callback(toRemove->arg);
                g_ChainSupervisor.EnterCriticalSectionWrapper(0);
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
