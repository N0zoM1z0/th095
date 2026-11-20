f32 PhotoGameUpdateView::AngleFromPoint(Float3 *position)
{
    f32 xDelta = this->playerPosition.x - position->x;
    f32 yDelta = this->playerPosition.y - position->y;

    if (yDelta == 0.0f && xDelta == 0.0f)
    {
        return 1.5707964f;
    }
    return atan2f(yDelta, xDelta);
}

i32 PhotoGameUpdateView::CheckBulletCollision(Float3 *position, Float3 *size)
{
    Float3 boundsMin;
    Float3 boundsMax;

    boundsMin.x = position->x - size->x / 2.0f;
    boundsMin.y = position->y - size->y / 2.0f;
    boundsMax.x = size->x / 2.0f + position->x;
    boundsMax.y = size->y / 2.0f + position->y;

    if (this->hurtboxBoundsMin.x > boundsMax.x ||
        this->hurtboxBoundsMin.y > boundsMax.y ||
        this->hurtboxBoundsMax.x < boundsMin.x ||
        this->hurtboxBoundsMax.y < boundsMin.y)
    {
        return 0;
    }
    if (this->mode == PHOTO_PLAYER_MODE_DEATH_TRANSITION)
    {
        return 0;
    }
    if (this->mode == PHOTO_PLAYER_MODE_PHOTO_LIMIT_TRANSITION)
    {
        return 0;
    }
    this->Die();
    return 1;
}

u32 PhotoGameUpdateView::CalcLaserHitbox(
    Float3 *origin, f32 angle, f32 width, f32 length)
{
    Float3 incomingMin;
    Float3 incomingMax;

    incomingMin = this->playerPosition - *origin;
    Rotate(&incomingMax, &incomingMin, -angle);
    incomingMin = incomingMax - this->hurtboxHalfSize;
    incomingMax = incomingMax + this->hurtboxHalfSize;

    if (incomingMin.x > length ||
        width / 2.0f < incomingMin.y ||
        incomingMax.x < 0.0f ||
        incomingMax.y < -width / 2.0f)
    {
        return 0;
    }
    if (this->mode == PHOTO_PLAYER_MODE_DEATH_TRANSITION)
    {
        return 0;
    }
    if (this->mode == PHOTO_PLAYER_MODE_PHOTO_LIMIT_TRANSITION)
    {
        return 0;
    }
    this->Die();
    return 1;
}
