#pragma once

struct AdvGameUtils
{
	static bool HasChangedMuch(const FVector2D& Current, const FVector2D& Previous);

	static int32 GetUUID();
};
