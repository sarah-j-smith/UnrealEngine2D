// Copyright (c) 2024 Sarah Smith


#include "Obstacle.h"

#include "PlayerCharacter.h"

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
	
	UE_LOG(LogTemp, Warning, TEXT("binding capsule collision handler"));
	capsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::OverlapBegin);
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
	UE_LOG(LogTemp, Warning, TEXT("overlap occurred"));
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, TEXT("Overlapped"));
	APlayerCharacter *PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (PlayerCharacter) {
		UE_LOG(LogTemp, Warning, TEXT("overlap on player"));
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, TEXT("Overlapped with player character"));
	}
}
