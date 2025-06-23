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
}

void AHotSpot::BeginPlay()
{
	UStaticMeshComponent* StaticMeshComponent = GetStaticMeshComponent();
	if (StaticMeshComponent && StaticMeshComponent->GetStaticMesh())
	{
		UE_LOG(LogAdventureGame, Verbose, TEXT("HotSpot::BeginPlay()- static mesh is valid"));
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		StaticMeshComponent->SetGenerateOverlapEvents(true);
	}
	else
	{
		UE_LOG(LogAdventureGame, Warning, TEXT("HotSpot::BeginPlay()- static mesh NOT valid"));
	}
	Super::BeginPlay();

	APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	WalkToPosition = WalkToPoint->GetComponentLocation();
	WalkToPosition.Z = PlayerPawn->GetActorLocation().Z;
}

void AHotSpot::OnBeginCursorOver(UPrimitiveComponent *Component)
{
	UE_LOG(LogAdventureGame, Log, TEXT("OnBeginCursorOver"));
	APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AAdventurePlayerController *AdventurePlayerController = Cast<AAdventurePlayerController>(PlayerController);
	if (IsValid(AdventurePlayerController))
	{
		AdventurePlayerController->MouseEnterHotSpot(this);
	}
}

void AHotSpot::OnEndCursorOver(UPrimitiveComponent *Component)
{
	UE_LOG(LogAdventureGame, Log, TEXT("OnEndCursorOver"));
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
	UE_LOG(LogAdventureGame, Log, TEXT("On close"));
	Bark(FText::FromString("I can't close that."));
}

void AHotSpot::OnOpen_Implementation()
{
	IVerbInteractions::OnOpen_Implementation();
	UE_LOG(LogAdventureGame, Log, TEXT("On open"));
	Bark(FText::FromString("I can't open that."));
}

void AHotSpot::OnGive_Implementation()
{
	IVerbInteractions::OnGive_Implementation();
	UE_LOG(LogAdventureGame, Log, TEXT("On give"));
	Bark(FText::FromString("No-one to give to."));
}

void AHotSpot::OnPickUp_Implementation()
{
	IVerbInteractions::OnPickUp_Implementation();
	UE_LOG(LogAdventureGame, Log, TEXT("On Pickup"));
	Bark(FText::FromString("It's not possible to pick it up."));
}

void AHotSpot::OnTalkTo_Implementation()
{
	IVerbInteractions::OnTalkTo_Implementation();
	UE_LOG(LogAdventureGame, Log, TEXT("On talk to"));
	Bark(FText::FromString("Not very talkative."));
}

void AHotSpot::OnLookAt_Implementation()
{
	IVerbInteractions::OnLookAt_Implementation();
	UE_LOG(LogAdventureGame, Log, TEXT("On look at"));
	Bark(FText::FromString("Doesn't look like much at all."));
}

void AHotSpot::OnPull_Implementation()
{
	IVerbInteractions::OnPull_Implementation();
	UE_LOG(LogAdventureGame, Log, TEXT("On pull"));
	Bark(FText::FromString("Nope, can't pull that."));
}

void AHotSpot::OnPush_Implementation()
{
	IVerbInteractions::OnPush_Implementation();
	UE_LOG(LogAdventureGame, Log, TEXT("On push"));
	Bark(FText::FromString("Pushing that won't work."));
}

void AHotSpot::OnUse_Implementation()
{
	IVerbInteractions::OnUse_Implementation();
	UE_LOG(LogAdventureGame, Log, TEXT("On use"));
	Bark(FText::FromString("Can't use that."));
}

void AHotSpot::OnWalkTo_Implementation()
{
	IVerbInteractions::OnWalkTo_Implementation();
	UE_LOG(LogAdventureGame, Log, TEXT("On walk to"));
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

void AHotSpot::Bark(const FText &BarkText)
{
	APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AAdventurePlayerController *AdventurePlayerController = Cast<AAdventurePlayerController>(PlayerController);
	if (IsValid(AdventurePlayerController))
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
