// (c) 2024 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"

#include "Components/SphereComponent.h"

#include "PlayerCharacter.h"

#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class CRUSTYPIRATE_API AEnemy : public APaperZDCharacter
{
    GENERATED_BODY()
    
public:
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USphereComponent *playerDetectorSphere;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    APlayerCharacter *followTarget;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float stopDistanceToTarget;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool canMove = true;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool isAlive = true;
    
    AEnemy();
    
    virtual void BeginPlay() override;
    
    virtual void Tick(float DeltaTime) override;
    
    UFUNCTION()
    void DetectorOverlapBegin(
                              UPrimitiveComponent *OverlappedComponent,
                              AActor *OtherActor, UPrimitiveComponent *OtherComponent,
                              int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult);
    
    UFUNCTION()
    void DetectorOverlapEnd(
                            UPrimitiveComponent *OverlappedComponent,
                            AActor *OtherActor, UPrimitiveComponent *OtherComponent,
                            int32 OtherBodyIndex);
};
