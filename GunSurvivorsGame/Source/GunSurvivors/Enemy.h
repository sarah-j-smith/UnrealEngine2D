// (c) 2024 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "TopDownCharacter.h"

#include "Enemy.generated.h"

UCLASS()
class GUNSURVIVORS_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperFlipbookComponent *enemyFlipbook;

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

	AEnemy();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

};
