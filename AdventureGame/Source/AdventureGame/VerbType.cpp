#include "VerbType.h"

FString VerbGetDescriptiveString(EVerbType Verb)
{
	switch (Verb)
	{
	case EVerbType::Close:
		return FString("Close");
	case EVerbType::Open:
		return FString("Open");
	case EVerbType::Give:
		return FString("Give");
	case EVerbType::PickUp:
		return FString("Pick up");
	case EVerbType::TalkTo:
		return FString("Talk to");
	case EVerbType::LookAt:
		return FString("Look at");
	case EVerbType::Use:
		return FString("Use");
	case EVerbType::Push:
		return FString("Push");
	case EVerbType::Pull:
		return FString("Pull");
	case EVerbType::WalkTo:
		return FString("Walk to");
	default:
		return FString("None");
	}
}