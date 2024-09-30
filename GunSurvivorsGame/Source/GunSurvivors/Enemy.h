// (c) 2024 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "TopDownCharacter.h"

#include "Engine/TimerHandle.h"

#include "Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDiedDelegate);

UCLASS()
class GUNSURVIVORS_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperFlipbookComponent *enemyFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperFlipbook *deadFlipbookAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCapsuleComponent *capsuleComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ATopDownCharacter *playerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanFollow = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopDistance = 20.0f;

	FTimerHandle destroyTimer;

	FEnemyDiedDelegate enemyDiedDelegate;

	AEnemy();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void Die();

	void OnDestroyTimerTimeout();

};
