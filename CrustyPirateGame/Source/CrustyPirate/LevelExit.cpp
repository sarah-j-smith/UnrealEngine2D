// (c) 2024 Sarah Smith


#include "LevelExit.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "CrustyPirateGameInstance.h"


// Sets default values
ALevelExit::ALevelExit()
{
	PrimaryActorTick.bCanEverTick = true;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    SetRootComponent(BoxComponent);
    
    DoorFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("DoorFlipbook"));
    DoorFlipbook->SetupAttachment(RootComponent);
    DoorFlipbook->SetPlayRate(0.0f);
    DoorFlipbook->SetLooping(false);
}

// Called when the game starts or when spawned
void ALevelExit::BeginPlay()
{
	Super::BeginPlay();
	
    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALevelExit::OverlapBegin);
    
    DoorFlipbook->SetPlaybackPosition(0.0f, false);
}

// Called every frame
void ALevelExit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelExit::OverlapBegin(UPrimitiveComponent *OverlappedComponent,
                           AActor *OtherActor, UPrimitiveComponent *OtherComponent,
                           int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult)
{
    APlayerCharacter *playerCharacter = Cast<APlayerCharacter>(OtherActor);
    if (playerCharacter && playerCharacter->isAlive)
    {
        if (IsActive)
        {
            IsActive = false;
            
            playerCharacter->Deactivate();
            
            DoorFlipbook->SetPlayRate(1.0f);
            DoorFlipbook->PlayFromStart();
            
            UGameplayStatics::PlaySound2D(GetWorld(), PlayerEnterSound);
            
            GetWorldTimerManager().SetTimer(WaitTimer, this, &ALevelExit::OnWaitTimerTimeout, 1.0f, false, WaitTimeInSeconds);
        }
    }
}

void ALevelExit::OnWaitTimerTimeout()
{
    UGameInstance *gameInstance = GetGameInstance();
    UCrustyPirateGameInstance *MyGameInstance = Cast<UCrustyPirateGameInstance>(gameInstance);
    if (MyGameInstance) {
        MyGameInstance->ChangeLevel(DestinationLevelIndex);
    }
}
