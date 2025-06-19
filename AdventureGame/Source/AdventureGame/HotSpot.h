// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"

#include "Engine/StaticMeshActor.h"
#include "VerbInteractions.h"
#include "WalkDirection.h"
#include "Components/SphereComponent.h"

#include "HotSpot.generated.h"

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HotSpot")
	USphereComponent* WalkToPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HotSpot")
	FString HotSpotDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HotSpot")
	FVector WalkToPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HotSpot")
	EWalkDirection FacingDirection;

	UFUNCTION(BlueprintCallable, Category = "HotSpot")
	void OnBeginCursorOver(UPrimitiveComponent* Component);

	UFUNCTION(BlueprintCallable, Category = "HotSpot")
	void OnEndCursorOver(UPrimitiveComponent* Component);

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
private:
	void Bark(const FText &BarkText);
};
