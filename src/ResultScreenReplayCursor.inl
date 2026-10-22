// Included inside namespace th095 at the original ResultScreen emission site.
// Move requires a positive count and a reachable enabled entry in the movement
// direction. SetCount(0) is allowed for inactive menus; calling Move then is
// not defined by the target. Push/Pop clear disabled entries and saturate depth
// exactly as the target does; Pop requires an earlier initialized save slot.
i32 ResultScreenReplayCursor::Move(i32 amount)
{
    i32 i;

tryAgain:
    this->current += amount;
    while (this->current >= this->count)
    {
        if (this->wraps != 0)
        {
            this->current -= this->count;
        }
        else
        {
            this->current = this->count - 1;
        }
    }
    while (this->current < 0)
    {
        if (this->wraps != 0)
        {
            this->current += this->count;
        }
        else
        {
            this->current = 0;
        }
    }
    for (i = 0; i < this->disabledEntryCount; i++)
    {
        if (this->disabledEntries[i] == this->current)
        {
            goto tryAgain;
        }
    }
    return this->current;
}

void ResultScreenReplayCursor::Push()
{
    this->savedCurrent[this->saveDepth] = this->current;
    this->savedCount[this->saveDepth] = this->count;
    this->saveDepth++;
    if (this->saveDepth >= 16)
    {
        this->saveDepth = 15;
    }
    this->disabledEntryCount = 0;
}

void ResultScreenReplayCursor::Pop()
{
    this->saveDepth--;
    if (this->saveDepth < 0)
    {
        this->saveDepth = 0;
    }
    this->current = this->savedCurrent[this->saveDepth];
    this->count = this->savedCount[this->saveDepth];
    this->disabledEntryCount = 0;
}
