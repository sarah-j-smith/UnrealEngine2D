// (c) 2024 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"

#include "Engine/TimerHandle.h"

#include "Bullet.generated.h"

UCLASS()
class GUNSURVIVORS_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent *sphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent *bulletSprite;

	UPROPERTY(BlueprintReadWrite)
	FVector2D moveDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float movementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isLaunched;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isDisabled;
	
	UPROPERTY(EditAnywhere, BlueprintReadonly)
	FTimerHandle deleteTimer;

	ABullet();

	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Launch(FVector2D direction, float speed);

	void OnDeleteTimerTimeout();

	UFUNCTION()
	void OverlapBegin(
		UPrimitiveComponent *OverlappedComponent,
		AActor *OtherActor, UPrimitiveComponent *OtherComponent,
		int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult);

	void DisableBullet();
};
