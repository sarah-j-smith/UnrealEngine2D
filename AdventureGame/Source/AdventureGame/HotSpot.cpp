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

	WalkToPoint->SetWorldLocation(WalkToPosition);
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
}

void AHotSpot::OnOpen_Implementation()
{
	IVerbInteractions::OnOpen_Implementation();
	UE_LOG(LogAdventureGame, Log, TEXT("On open"));
}

void AHotSpot::OnGive_Implementation()
{
	IVerbInteractions::OnGive_Implementation();
	UE_LOG(LogAdventureGame, Log, TEXT("On give"));
}

void AHotSpot::OnPickUp_Implementation()
{
	IVerbInteractions::OnPickUp_Implementation();
	UE_LOG(LogAdventureGame, Log, TEXT("On Pickup"));
}

void AHotSpot::OnTalkTo_Implementation()
{
	IVerbInteractions::OnTalkTo_Implementation();
	UE_LOG(LogAdventureGame, Log, TEXT("On talk to"));
}

void AHotSpot::OnLookAt_Implementation()
{
	IVerbInteractions::OnLookAt_Implementation();
	UE_LOG(LogAdventureGame, Log, TEXT("On look at"));
}

void AHotSpot::OnPull_Implementation()
{
	IVerbInteractions::OnPull_Implementation();
	UE_LOG(LogAdventureGame, Log, TEXT("On pull"));
}

void AHotSpot::OnPush_Implementation()
{
	IVerbInteractions::OnPush_Implementation();
	UE_LOG(LogAdventureGame, Log, TEXT("On push"));
}

void AHotSpot::OnUse_Implementation()
{
	IVerbInteractions::OnUse_Implementation();
	UE_LOG(LogAdventureGame, Log, TEXT("On use"));
}
