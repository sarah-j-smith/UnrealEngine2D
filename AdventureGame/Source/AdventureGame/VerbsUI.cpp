// (c) 2025 Sarah Smith


#include "VerbsUI.h"


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

void UVerbsUI::ClearActiveButton()
{
	switch (CurrentVerb)
	{
	case EVerbType::Close:
		Close->SetStyle(CloseNormalStyle);
		break;
	case EVerbType::Open:
		Open->SetStyle(OpenSelectedStyle);
		break;
	case EVerbType::Give:
		Close->SetStyle(GiveNormalStyle);
		break;
	case EVerbType::PickUp:
		Close->SetStyle(PickUpNormalStyle);
		break;
	case EVerbType::TalkTo:
		Close->SetStyle(CloseNormalStyle);
		break;
	case EVerbType::LookAt:
		Close->SetStyle(CloseNormalStyle);
		break;
	case EVerbType::Use:
		Close->SetStyle(CloseNormalStyle);
		break;
	case EVerbType::Push:
		Close->SetStyle(CloseNormalStyle);
		break;
	case EVerbType::Pull:
		Close->SetStyle(CloseNormalStyle);
		break;
	case EVerbType::None:
	case EVerbType::WalkTo:
		break;
	}
	CurrentVerb = EVerbType::None;
}

void UVerbsUI::SetButtonActive(EVerbType VerbType)
{
	switch (VerbType)
	{
	case EVerbType::Close:
		CloseNormalStyle = FButtonStyle(Close->GetStyle());
		Close->SetStyle(CloseSelectedStyle);
		break;
	case EVerbType::Open:
		OpenNormalStyle = FButtonStyle(Open->GetStyle());
		Open->SetStyle(OpenSelectedStyle);
		break;
	case EVerbType::Give:
		CloseNormalStyle = FButtonStyle(Close->GetStyle());
		Close->SetStyle(CloseSelectedStyle);
		break;
	case EVerbType::PickUp:
		CloseNormalStyle = FButtonStyle(Close->GetStyle());
		Close->SetStyle(CloseSelectedStyle);
		break;
	case EVerbType::TalkTo:
		CloseNormalStyle = FButtonStyle(Close->GetStyle());
		Close->SetStyle(CloseSelectedStyle);
		break;
	case EVerbType::LookAt:
		CloseNormalStyle = FButtonStyle(Close->GetStyle());
		Close->SetStyle(CloseSelectedStyle);
		break;
	case EVerbType::Use:
		CloseNormalStyle = FButtonStyle(Close->GetStyle());
		Close->SetStyle(CloseSelectedStyle);
		break;
	case EVerbType::Push:
		CloseNormalStyle = FButtonStyle(Close->GetStyle());
		Close->SetStyle(CloseSelectedStyle);
		break;
	case EVerbType::Pull:
		CloseNormalStyle = FButtonStyle(Close->GetStyle());
		Close->SetStyle(CloseSelectedStyle);
		break;
	case EVerbType::None:
	case EVerbType::WalkTo:
		break;
	}
	CurrentVerb = VerbType;
}

