// (c) 2025 Sarah Smith


#include "AdventureAIController.h"

#include "Navigation/PathFollowingComponent.h"

void AAdventureAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	MoveCompletedDelegate.Broadcast(Result.Code);
}
