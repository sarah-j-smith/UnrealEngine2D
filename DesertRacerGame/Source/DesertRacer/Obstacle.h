// Copyright (c) 2024 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/CapsuleComponent.h"
#include "PaperSpriteComponent.h"
#include "Sound/SoundBase.h"

#include "MainGameMode.h"

#include "Obstacle.generated.h"

UCLASS()
class DESERTRACER_API AObstacle : public AActor
{
	GENERATED_BODY()

	AMainGameMode *mainGameMode;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent *capsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent *obstacleSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase *hitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isFinishLine = false;

	AObstacle();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OverlapBegin(
			UPrimitiveComponent *OverlappedComponent, 
			AActor *OtherActor, UPrimitiveComponent *OtherComponent, 
			int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult);
};
