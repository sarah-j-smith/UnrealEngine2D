// (c) 2025 Sarah Smith


#include "VerbsUI.h"

#include "AdventurePlayerController.h"
#include "Kismet/GameplayStatics.h"


void UVerbsUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Give->OnClicked.AddDynamic(this, &UVerbsUI::GiveTriggered);
	Open->OnClicked.AddDynamic(this, &UVerbsUI::OpenTriggered);
	Close->OnClicked.AddDynamic(this, &UVerbsUI::CloseTriggered);
	PickUp->OnClicked.AddDynamic(this, &UVerbsUI::PickUpTriggered);
	TalkTo->OnClicked.AddDynamic(this, &UVerbsUI::TalkToTriggered);
	LookAt->OnClicked.AddDynamic(this, &UVerbsUI::LookAtTriggered);
	Use->OnClicked.AddDynamic(this, &UVerbsUI::UseTriggered);
	Push->OnClicked.AddDynamic(this, &UVerbsUI::PushTriggered);
	Pull->OnClicked.AddDynamic(this, &UVerbsUI::PullTriggered);

	Give->OnHovered.AddDynamic(this, &UVerbsUI::VerbHovered);
	Open->OnHovered.AddDynamic(this, &UVerbsUI::VerbHovered);
	Close->OnHovered.AddDynamic(this, &UVerbsUI::VerbHovered);
	PickUp->OnHovered.AddDynamic(this, &UVerbsUI::VerbHovered);
	TalkTo->OnHovered.AddDynamic(this, &UVerbsUI::VerbHovered);
	LookAt->OnHovered.AddDynamic(this, &UVerbsUI::VerbHovered);
	Use->OnHovered.AddDynamic(this, &UVerbsUI::VerbHovered);
	Push->OnHovered.AddDynamic(this, &UVerbsUI::VerbHovered);
	Pull->OnHovered.AddDynamic(this, &UVerbsUI::VerbHovered);

	Give->OnUnhovered.AddDynamic(this, &UVerbsUI::VerbUnhovered);
	Open->OnUnhovered.AddDynamic(this, &UVerbsUI::VerbUnhovered);
	Close->OnUnhovered.AddDynamic(this, &UVerbsUI::VerbUnhovered);
	PickUp->OnUnhovered.AddDynamic(this, &UVerbsUI::VerbUnhovered);
	TalkTo->OnUnhovered.AddDynamic(this, &UVerbsUI::VerbUnhovered);
	LookAt->OnUnhovered.AddDynamic(this, &UVerbsUI::VerbUnhovered);
	Use->OnUnhovered.AddDynamic(this, &UVerbsUI::VerbUnhovered);
	Push->OnUnhovered.AddDynamic(this, &UVerbsUI::VerbUnhovered);
	Pull->OnUnhovered.AddDynamic(this, &UVerbsUI::VerbUnhovered);
	
	AssignNormalStyles();
}

void UVerbsUI::CloseTriggered()
{
	ClearActiveButton();
	SetButtonActive(EVerbType::Close);
}

void UVerbsUI::OpenTriggered()
{
	ClearActiveButton();
	SetButtonActive(EVerbType::Open);
}

void UVerbsUI::GiveTriggered()
{
	ClearActiveButton();
	SetButtonActive(EVerbType::Give);
}

void UVerbsUI::TalkToTriggered()
{
	ClearActiveButton();
	SetButtonActive(EVerbType::TalkTo);
}

void UVerbsUI::LookAtTriggered()
{
	ClearActiveButton();
	SetButtonActive(EVerbType::LookAt);
}

void UVerbsUI::PickUpTriggered()
{
	ClearActiveButton();
	SetButtonActive(EVerbType::PickUp);
}

void UVerbsUI::UseTriggered()
{
	ClearActiveButton();
	SetButtonActive(EVerbType::Use);
}

void UVerbsUI::PushTriggered()
{
	ClearActiveButton();
	SetButtonActive(EVerbType::Push);
}

void UVerbsUI::PullTriggered()
{
	ClearActiveButton();
	SetButtonActive(EVerbType::Pull);
}

void UVerbsUI::VerbHovered()
{
	SetHoveredVerb(EVerbHoverState::Hovered);
}

void UVerbsUI::VerbUnhovered()
{
	SetHoveredVerb(EVerbHoverState::Unhovered);
}

void UVerbsUI::ClearActiveButton()
{
	switch (CurrentVerb)
	{
	case EVerbType::Close:
		Close->SetStyle(CloseNormalStyle);
		break;
	case EVerbType::Open:
		Open->SetStyle(OpenNormalStyle);
		break;
	case EVerbType::Give:
		Give->SetStyle(GiveNormalStyle);
		break;
	case EVerbType::PickUp:
		PickUp->SetStyle(PickUpNormalStyle);
		break;
	case EVerbType::TalkTo:
		TalkTo->SetStyle(TalkToNormalStyle);
		break;
	case EVerbType::LookAt:
		LookAt->SetStyle(LookAtNormalStyle);
		break;
	case EVerbType::Use:
		Use->SetStyle(UseNormalStyle);
		break;
	case EVerbType::Push:
		Push->SetStyle(PushNormalStyle);
		break;
	case EVerbType::Pull:
		Pull->SetStyle(PullNormalStyle);
		break;
	case EVerbType::WalkTo:
		break;
	default:
		break;
	}
	CurrentVerb = EVerbType::WalkTo;
}

void UVerbsUI::SetButtonActive(EVerbType VerbType)
{
	switch (VerbType)
	{
	case EVerbType::Close:
		Close->SetStyle(CloseSelectedStyle);
		break;
	case EVerbType::Open:
		Open->SetStyle(OpenSelectedStyle);
		break;
	case EVerbType::Give:
		Give->SetStyle(GiveSelectedStyle);
		break;
	case EVerbType::PickUp:
		PickUp->SetStyle(PickUpSelectedStyle);
		break;
	case EVerbType::TalkTo:
		TalkTo->SetStyle(TalkToSelectedStyle);
		break;
	case EVerbType::LookAt:
		LookAt->SetStyle(LookAtSelectedStyle);
		break;
	case EVerbType::Use:
		Use->SetStyle(UseSelectedStyle);
		break;
	case EVerbType::Push:
		Push->SetStyle(PushSelectedStyle);
		break;
	case EVerbType::Pull:
		Pull->SetStyle(PullSelectedStyle);
		break;
	case EVerbType::WalkTo:
		break;
	default:
		break;
	}
	CurrentVerb = VerbType;

	SetActiveVerb();
}

void UVerbsUI::AssignNormalStyles()
{
	CloseNormalStyle = FButtonStyle(Close->GetStyle());
	OpenNormalStyle = FButtonStyle(Open->GetStyle());
	GiveNormalStyle = FButtonStyle(Give->GetStyle());
	LookAtNormalStyle = FButtonStyle(LookAt->GetStyle());
	TalkToNormalStyle = FButtonStyle(TalkTo->GetStyle());
	PickUpNormalStyle = FButtonStyle(PickUp->GetStyle());
	UseNormalStyle = FButtonStyle(Use->GetStyle());
	PushNormalStyle = FButtonStyle(Push->GetStyle());
	PullNormalStyle = FButtonStyle(Pull->GetStyle());
}

void UVerbsUI::SetActiveVerb() const
{
	APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (AAdventurePlayerController *AdventurePlayerController = Cast<AAdventurePlayerController>(PlayerController))
	{
		AdventurePlayerController->AssignVerb(CurrentVerb);
	}
}

void UVerbsUI::SetHoveredVerb(EVerbHoverState IsHovered) const
{
	APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (AAdventurePlayerController *AdventurePlayerController = Cast<AAdventurePlayerController>(PlayerController))
	{
		AdventurePlayerController->HoverVerb(IsHovered);
	}
}

