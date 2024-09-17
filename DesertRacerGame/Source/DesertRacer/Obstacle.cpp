// Copyright (c) 2024 Sarah Smith


#include "Obstacle.h"

#include "PlayerCharacter.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(capsuleComponent);

	obstacleSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ObstacleSprite"));
	obstacleSprite->SetupAttachment(RootComponent);
}

void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	
	capsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::OverlapBegin);

	AGameModeBase *aGameMode = UGameplayStatics::GetGameMode(GetWorld());
	mainGameMode = Cast<AMainGameMode>(aGameMode);
}

void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AObstacle::OverlapBegin(
					UPrimitiveComponent *OverlappedComponent, 
					AActor *OtherActor, UPrimitiveComponent *OtherComponent, 
					int32 OtherBodyIndex, bool FromSweep, 
					const FHitResult &SweepResult)
{
	APlayerCharacter *PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (PlayerCharacter) {
		if (PlayerCharacter->canMove) {
			PlayerCharacter->canMove = false;
			mainGameMode->ResetGameLevel(false);
		}
	}
}
