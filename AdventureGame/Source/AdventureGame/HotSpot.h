// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"

#include "Engine/StaticMeshActor.h"
#include "VerbInteractions.h"
#include "WalkDirection.h"
#include "Components/SphereComponent.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "PaperSpriteComponent.h"

#include "HotSpot.generated.h"

class UPaperSprite;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HotSpot")
	UPaperSpriteComponent *SpriteComponent;

	//////////////////////////////////
	///
	/// USER INPUT EVENTS
	///

	UFUNCTION(BlueprintCallable, Category = "HotSpot")
	void OnBeginCursorOver(AActor *TouchedActor);

	UFUNCTION(BlueprintCallable, Category = "HotSpot")
	void OnEndCursorOver(AActor *TouchedActor);

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

	/// Is this HotSpot a _Pickup_ - that has a sprite component for displaying
	/// an in-game appearance - or is it just a hotspot with a clickable mesh?
	/// @returns true if this has a valid `USpriteComponent`.
	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	bool IsPickup() const
	{
		return IsValid(SpriteComponent);
	}
	
	/// How far above the player character (closer to the camera) should objects spawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Actions")
	float ZOffsetForSpawn = 5.0;
	
	/// How far above the player character (closer to the camera) should objects spawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Actions")
	FRotator SpawnOrientation = FRotator(0.0, 0.0, -90.0);
	
	/// Spawn an actor of the given class on top of the player character.
	/// Optionally specify a scale and how long it should appear before
	/// being destroyed automatically. Intended for item interactions,
	/// where the actor is destroyed on adding to the inventory.
	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	AActor *SpawnAtPlayerLocation(TSubclassOf<AActor> SpawnClass, float Scale = 1.0f, float Lifetime = 2.0f);
	
	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void Bark(const FText &BarkText);

	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	AAdventurePlayerController* GetAdventureController() const;

	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void AddToInventory(EItemList ItemToAdd);

	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void RemoveFromInventory(EItemList ItemToRemove);

	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void HideSpriteComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void ShowSpriteComponent();

	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void ClearVerb();
};
