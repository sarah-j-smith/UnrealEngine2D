// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"

#include "Engine/StaticMeshActor.h"
#include "VerbInteractions.h"
#include "WalkDirection.h"
#include "Components/SphereComponent.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "HotSpot.generated.h"

class AAdventurePlayerController;
/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API AHotSpot : public AStaticMeshActor, public IVerbInteractions
{
	GENERATED_BODY()

public:
	AHotSpot();
	
	virtual void BeginPlay() override;

	//////////////////////////////////
	///
	/// HOTSPOT PROPERTIES
	///

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HotSpot")
	USphereComponent* WalkToPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HotSpot")
	FString HotSpotDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HotSpot")
	FVector WalkToPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HotSpot")
	EWalkDirection FacingDirection;

	//////////////////////////////////
	///
	/// USER INPUT EVENTS
	///

	UFUNCTION(BlueprintCallable, Category = "HotSpot")
	void OnBeginCursorOver(UPrimitiveComponent* Component);

	UFUNCTION(BlueprintCallable, Category = "HotSpot")
	void OnEndCursorOver(UPrimitiveComponent* Component);

	//////////////////////////////////
	///
	/// VERB TRIGGER EVENTS
	///

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnClose_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnOpen_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnGive_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnPickUp_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnTalkTo_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnLookAt_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnPull_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnPush_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnUse_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnWalkTo_Implementation() override;

	//////////////////////////////////
	///
	/// PLAYER ACTIONS
	///
	
	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	void Bark(const FText &BarkText);

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	AAdventurePlayerController* GetAdventureController() const;

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	void AddToInventory(EItemList ItemToAdd);

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	void RemoveFromInventory(EItemList ItemToRemove);
};
