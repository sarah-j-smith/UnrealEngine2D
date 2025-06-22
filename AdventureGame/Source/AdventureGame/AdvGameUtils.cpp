#include "AdvGameUtils.h"

/**
 * Test to see if Current has changed more than DBL_EPSILON compared to Previous in either `X` or `Y`.
 * @param Current `FVector2D` Value to check to see if it differs
 * @param Previous `FVector2D` Value to check against as a baseline
 * @return true if `Current` is significantly different to `Previous`, false otherwise
 */
bool AdvGameUtils::HasChangedMuch(const FVector2D& Current, const FVector2D& Previous)
{
	return (fabs(Previous.X - Current.X) >= std::numeric_limits<double>::epsilon() || fabs(Previous.Y - Current.Y) >= std::numeric_limits<double>::epsilon());
}

int32 AdvGameUtils::GetUUID()
{
	uuid_t UUIDLatent;
	uuid_generate(UUIDLatent);
	return UUIDLatent[0] | UUIDLatent[1] << 8 | UUIDLatent[2] << 16 | UUIDLatent[3] << 24;
}
