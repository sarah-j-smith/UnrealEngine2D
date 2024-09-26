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

	UPROPERTY(EditAnywhere, BlueprintReadonly)
	FTimerHandle deleteTimer;

	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Launch(FVector2D direction, float speed);

	void OnDeleteTimerTimeout();
};
