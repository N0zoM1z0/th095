// FUNCTION: TH095 0x0041E750.
// The shallow-local rank mirrors the exact CheckCollision prefix. The inner
// half-size vector is deliberately reused as the maximum bound after minimum
// materialization; keeping these lifetimes separate is target-visible in VC7.1.
#define countTarget iLocal11
#define indexTarget averagedPanLocal12
#define minimumTarget commandCursorLocal02
#define sampleTarget soundIndexLocal01
#define stepTarget restartCommandProcessingLocal05
i32 TH095_PHOTO_LASER_TYPE::CountPhotoTargets(
    Float3 *position, Float3 *size, i32 capture)
{
    Float3 stepTarget;
    i32 indexTarget;
    i32 countTarget;
    Float3 minimumTarget;
    Float3 sampleTarget;

    countTarget = 0;
    indexTarget = 0;
    {
        f32 bufferLocal04 = 6.0f;
        Float3 jLocal00;

        jLocal00 = *size / 2.0f;
        minimumTarget = *position - jLocal00;
        jLocal00 = *position + jLocal00;
        stepTarget.z = 0.0f;
        stepTarget.FromAngleMagnitude(this->angle, 6.0f);
        sampleTarget =
            *reinterpret_cast<Float3 *>(&this->position) + stepTarget;
        stepTarget += stepTarget;

        while (bufferLocal04 + 6.0f < this->length)
        {
            if (sampleTarget.x < minimumTarget.x ||
                sampleTarget.x > jLocal00.x ||
                sampleTarget.y < minimumTarget.y ||
                sampleTarget.y > jLocal00.y)
            {
            }
            else
            {
                countTarget++;
            }
            sampleTarget += stepTarget;
            bufferLocal04 += 12.0f;
            indexTarget++;
        }
    }
    return countTarget;
}
#undef stepTarget
#undef sampleTarget
#undef minimumTarget
#undef indexTarget
#undef countTarget

// Non-trivial Float3 locals must remain independent so VC7 preserves constructor
// timing. These backing buckets restore the target physical order
// difference -> delta -> local without aggregating the objects.
#define nearbyLocal restartCommandProcessingLocal05
#define nearbyDelta averagedPanLocal12
#define nearbyDifference iLocal11
i32 TH095_PHOTO_LASER_TYPE::CountNearbyTargets(
    Float3 *position, f32 radius)
{
    Float3 nearbyLocal;
    Float3 nearbyDifference =
        *position - *reinterpret_cast<Float3 *>(&this->position);
    Float3 nearbyDelta = nearbyDifference;
    Rotate(&nearbyLocal, &nearbyDelta, -this->angle);

    nearbyDelta.x = nearbyLocal.x - radius;
    nearbyDelta.y = nearbyLocal.y - radius;
    nearbyLocal.x += radius;
    nearbyLocal.y += radius;

    if (nearbyDelta.x > this->length ||
        this->width / 2.0f < nearbyDelta.y ||
        nearbyLocal.x < 0.0f ||
        nearbyLocal.y < -this->width / 2.0f)
    {
        return 0;
    }
    return 2;
}
#undef nearbyLocal
#undef nearbyDelta
#undef nearbyDifference
