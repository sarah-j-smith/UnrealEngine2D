// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "InteractionType.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "InteractTask.generated.h"

class AAdventurePlayerController;

/// https://unrealcommunity.wiki/creating-asynchronous-blueprint-nodes-ctnmtj0q
/// 
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBarkCompletedDelegate);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSitCompletedDelegate);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FClimbCompletedDelegate);
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
	static UInteractTask* DoInteractTask(const UObject* WorldContextObject, const EInteractionType Interaction);

	void SetWorldContextObject(const UObject* WorldContextObject);

	void SetInteractionType(const EInteractionType InteractionType);
	
	virtual void Activate() override;
private:
	const UObject *WorldContextObject;
	EInteractionType Interaction;
	int32 MyUID;
	
	void Interact();
	void Sit();
	void Climb();

	UFUNCTION()
	void InteractionCompleted(EInteractionType Interaction, int32 UID, bool bSuccess);
	
	AAdventurePlayerController* GetAdventureController() const;
};
