// (c) 2025 Sarah Smith


#include "AdventureAIController.h"

#include "../AdventureGame.h"
#include "Navigation/PathFollowingComponent.h"

void AAdventureAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("AAdventureAIController::OnPossess"));
}

void AAdventureAIController::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("AAdventureAIController::OnConstruction"));
}

void AAdventureAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	MoveCompletedDelegate.Broadcast(Result.Code);
}
