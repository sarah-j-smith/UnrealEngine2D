#include "InteractionType.h"

FString InteractionGetDescriptiveString(const EInteractionType InteractionType)
{
	switch (InteractionType) {
	case EInteractionType::Interact:
		return "Interact";
		break;
	case EInteractionType::Sit:
		return "Sit";
		break;
	case EInteractionType::Climb:
		return "Climb";
		break;
	default:
		return "None";
		break;	
	}
}
