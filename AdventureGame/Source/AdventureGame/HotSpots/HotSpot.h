// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"

#include "Engine/StaticMeshActor.h"
#include "../Gameplay/VerbInteractions.h"
#include "../Enums/WalkDirection.h"
#include "Components/SphereComponent.h"
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

	/// A long description of this hotspot. When the player does a "LookAt" by default
	/// unless overridden in a blueprint, this description will be shown. Should usually
	/// be full sentence case as in "This knife looks sharp."
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HotSpot")
	FText Description;

	/// A short description of this hotspot, suitable for insertion into text templates
	/// with other text around it. Should not be sentence case. Example: "sharp knife".
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HotSpot")
	FText ShortDescription;

	/// Position that the player will walk to when going to this hotspot.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HotSpot")
	FVector WalkToPosition;

	/// Direction the player will face on arrival at the hotspot.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HotSpot")
	EWalkDirection FacingDirection;

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

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnItemUsed_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnItemGiven_Implementation() override;

	//////////////////////////////////
	///
	/// PLAYER ACTIONS
	///

	/// True if this HotSpot is a <b>Pickup</b>, which has a sprite component
	/// for displaying an in-game appearance - or is it just a hotspot with
	/// a clickable mesh?
	/// @returns true if this has a valid `USpriteComponent`.
	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	bool IsPickup() const
	{
		return Pickup;
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
	void Hide();

	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void Show();

	void SetEnableMeshComponent(bool Enabled);
	
protected:
	FString HotSpotType = "HotSpot";
	FString HotSpotName = "HotSpot";
	
	bool HotSpotHidden = false;
	bool Pickup = false;

	/// Internal convenience function. Do not expose to blueprints, they should use
	/// AdvBlueprintFunctionLibrary::Bark() instead.
	void Bark(const FText &Text) const;

	void BarkAndEnd(const FText &Text) const;
};
