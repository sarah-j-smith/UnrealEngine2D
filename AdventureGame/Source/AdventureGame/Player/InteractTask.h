// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "../Enums/InteractionType.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "InteractTask.generated.h"

class AAdventurePlayerController;

/// https://unrealcommunity.wiki/creating-asynchronous-blueprint-nodes-ctnmtj0q

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractOutputPin);

/**
 * Manage player asynchronous actions.
 */
UCLASS()
class ADVENTUREGAME_API UInteractTask : public UBlueprintAsyncActionBase
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FInteractOutputPin TaskCompleted;

	UPROPERTY(BlueprintAssignable)
	FInteractOutputPin TaskInterrupted;
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Flow Control")
	static UInteractTask* DoInteractTask(const UObject* WorldContextObject, const EInteractionType Interaction, const EInteractTimeDirection TimeDirection);

	void SetWorldContextObject(const UObject* WorldContextObject);

	void SetInteractionType(const EInteractionType InteractionType);

	void SetTimeDirection(const EInteractTimeDirection TimeDirection)
	{
		this->TimeDirection = TimeDirection;
	}

	virtual void Activate() override;
private:
	const UObject *WorldContextObject;
	EInteractionType Interaction;
	EInteractTimeDirection TimeDirection;
	int32 MyUID;
	
	void Interact();
	void Sit();
	void Climb();
	void TurnLeft();
	void TurnRight();

	UFUNCTION()
	void InteractionCompleted(EInteractionType Interaction, int32 UID, bool bSuccess);
	
	AAdventurePlayerController* GetAdventureController() const;
};
