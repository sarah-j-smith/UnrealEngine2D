// (c) 2025 Sarah Smith


#include "HotSpot.h"

#include "AdventureCharacter.h"
#include "AdventureGame.h"
#include "AdventurePlayerController.h"
#include "Kismet/GameplayStatics.h"

AHotSpot::AHotSpot()
	: AStaticMeshActor()
{
	// This is not a "real" sphere - its not a mesh, its just a collision volume defined by dimensions
	WalkToPoint = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDetectorSphere"));
	WalkToPoint->SetupAttachment(RootComponent);
	WalkToPoint->SetSphereRadius(4.0f);

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetupAttachment(RootComponent);
	SpriteComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));
	SpriteComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 5.0f));
	SpriteComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	SpriteComponent->SetVisibility(false);
}

void AHotSpot::BeginPlay()
{
	FString HotSpotType = IsPickup() ? "PickUp" : "HotSpot";
	FString HotSpotName = HotSpotDescription.IsEmpty() ? GetClass()->GetName() : HotSpotDescription;
	UStaticMeshComponent* StaticMeshComponent = GetStaticMeshComponent();
	if (StaticMeshComponent && StaticMeshComponent->GetStaticMesh())
	{
		UE_LOG(LogAdventureGame, Verbose, TEXT("%s %s - BeginPlay()- static mesh is valid."),
			*HotSpotType, *HotSpotName);
		// StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
		StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		StaticMeshComponent->SetGenerateOverlapEvents(true);

		Super::OnBeginCursorOver.AddDynamic(this, &AHotSpot::OnBeginCursorOver);
		Super::OnEndCursorOver.AddDynamic(this, &AHotSpot::OnEndCursorOver);
	}
	else
	{
		UE_LOG(LogAdventureGame, Warning, TEXT("%s %s - BeginPlay()- static mesh NOT valid"), *HotSpotType, *HotSpotName);
	}
	Super::BeginPlay();

	APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	WalkToPosition = WalkToPoint->GetComponentLocation();
	WalkToPosition.Z = PlayerPawn->GetActorLocation().Z;
}

void AHotSpot::OnBeginCursorOver(AActor *TouchedActor)
{
	GEngine->AddOnScreenDebugMessage(1, 3.0, FColor::White, TEXT("HotSpot::OnBeginCursorOver()"),
								 false, FVector2D(2.0, 2.0));

	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("OnBeginCursorOver"));
	APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AAdventurePlayerController *AdventurePlayerController = Cast<AAdventurePlayerController>(PlayerController);
	if (IsValid(AdventurePlayerController))
	{
		AdventurePlayerController->MouseEnterHotSpot(this);
	}
}

void AHotSpot::OnEndCursorOver(AActor *TouchedActor)
{
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("OnEndCursorOver"));
	APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AAdventurePlayerController *AdventurePlayerController = Cast<AAdventurePlayerController>(PlayerController);
	if (IsValid(AdventurePlayerController))
	{
		AdventurePlayerController->MouseLeaveHotSpot();
	}
}

void AHotSpot::OnClose_Implementation()
{
	IVerbInteractions::OnClose_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On close"));
	Bark(FText::FromString("I can't close that."));
}

void AHotSpot::OnOpen_Implementation()
{
	IVerbInteractions::OnOpen_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On open"));
	Bark(FText::FromString("I can't open that."));
}

void AHotSpot::OnGive_Implementation()
{
	IVerbInteractions::OnGive_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On give"));
	Bark(FText::FromString("No-one to give to."));
}

void AHotSpot::OnPickUp_Implementation()
{
	IVerbInteractions::OnPickUp_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On Pickup"));
	Bark(FText::FromString("It's not possible to pick it up."));
}

void AHotSpot::OnTalkTo_Implementation()
{
	IVerbInteractions::OnTalkTo_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On talk to"));
	Bark(FText::FromString("Not very talkative."));
}

void AHotSpot::OnLookAt_Implementation()
{
	IVerbInteractions::OnLookAt_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On look at"));
	Bark(FText::FromString("Doesn't look like much at all."));
}

void AHotSpot::OnPull_Implementation()
{
	IVerbInteractions::OnPull_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On pull"));
	Bark(FText::FromString("Nope, can't pull that."));
}

void AHotSpot::OnPush_Implementation()
{
	IVerbInteractions::OnPush_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On push"));
	Bark(FText::FromString("Pushing that won't work."));
}

void AHotSpot::OnUse_Implementation()
{
	IVerbInteractions::OnUse_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On use from AHotSpot default implement."));
	Bark(FText::FromString("Can't use that."));
}

void AHotSpot::OnWalkTo_Implementation()
{
	IVerbInteractions::OnWalkTo_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On walk to"));
	AAdventurePlayerController *PC = GetAdventureController();
	if (PC->IsAlreadyAtHotspotClicked())
	{
		PC->PlayerBark(FText::FromString("I'm here, what should I do?"));
	}
	else
	{
		PC->PlayerBark(FText::FromString("Arrived here. What now?"));;
	}
}

AActor *AHotSpot::SpawnAtPlayerLocation(TSubclassOf<AActor> SpawnClass, float Scale, float Lifetime)
{
	APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	FVector PlayerLocation = PlayerPawn->GetTransform().GetLocation();
	FVector SpawnLocation(PlayerLocation.X, PlayerLocation.Y, PlayerLocation.Z + ZOffsetForSpawn);
	check(SpawnClass)
	AActor* Item = GetWorld()->SpawnActor<AActor>(
		SpawnClass,
		SpawnLocation,
		SpawnOrientation);
	Item->SetLifeSpan(Lifetime);
	Item->SetActorScale3D(FVector(Scale, Scale, Scale));

	return Item;
}

void AHotSpot::Bark(const FText &BarkText)
{
	if (AAdventurePlayerController *AdventurePlayerController = GetAdventureController())
	{
		AdventurePlayerController->PlayerBark(BarkText);
	}
}

AAdventurePlayerController* AHotSpot::GetAdventureController() const
{
	APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AAdventurePlayerController *AdventurePlayerController = Cast<AAdventurePlayerController>(PlayerController);
	return AdventurePlayerController;
}

void AHotSpot::AddToInventory(EItemList ItemToAdd)
{
	GetAdventureController()->OnItemAddToInventory(ItemToAdd);
}

void AHotSpot::RemoveFromInventory(EItemList ItemToRemove)
{
	GetAdventureController()->OnItemRemoveFromInventory(ItemToRemove);
}

void AHotSpot::HideSpriteComponent()
{
	SpriteComponent->SetVisibility(false);
}

void AHotSpot::ShowSpriteComponent()
{
	SpriteComponent->SetVisibility(true);
}

void AHotSpot::ClearVerb()
{
	GetAdventureController()->InterruptCurrentAction();
}
