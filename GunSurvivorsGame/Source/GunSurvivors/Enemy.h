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

	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
