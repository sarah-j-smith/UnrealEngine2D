// (c) 2025 Sarah Smith


#include "AdventureAIController.h"

#include "AdventureGame.h"
#include "Navigation/PathFollowingComponent.h"

void AAdventureAIController::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UE_LOG(LogAdventureGame, Warning, TEXT("AAdventureAIController::OnConstruction"));
}

void AAdventureAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	MoveCompletedDelegate.Broadcast(Result.Code);
}
