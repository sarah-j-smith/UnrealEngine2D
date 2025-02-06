// (c) 2024 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"

#include "CollectableItem.generated.h"


UENUM(BlueprintType)
enum class CollectableType : uint8 {
    Diamond,
    HealthPotion,
    DoubleJumpUpgrade
};


UCLASS()
class CRUSTYPIRATE_API ACollectableItem : public AActor
{
	GENERATED_BODY()
    
public:	
	// Sets default values for this actor's properties
	ACollectableItem();
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UCapsuleComponent *CapsuleComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPaperFlipbookComponent *ItemFlipbook;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    CollectableType ItemType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OverlapBegin(UPrimitiveComponent *OverlappedComponent,
                               AActor *OtherActor, UPrimitiveComponent *OtherComponent,
                               int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult);
};
