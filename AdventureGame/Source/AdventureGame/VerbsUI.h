// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "Constants.h"

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "VerbType.h"
#include "Styling/SlateTypes.h"

#include "VerbsUI.generated.h"

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API UVerbsUI : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeOnInitialized() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton *Give;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton *Open;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton *Close;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton *PickUp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton *LookAt;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton *TalkTo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton *Use;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton *Push;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton *Pull;

	////////// Button styles ///////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button Controls")
	FButtonStyle CloseNormalStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button Controls")
	FButtonStyle CloseSelectedStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button Controls")
	FButtonStyle OpenNormalStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button Controls")
	FButtonStyle OpenSelectedStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button Controls")
	FButtonStyle GiveNormalStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button Controls")
	FButtonStyle GiveSelectedStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button Controls")
	FButtonStyle PickUpNormalStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button Controls")
	FButtonStyle PickUpSelectedStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button Controls")
	FButtonStyle TalkToNormalStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button Controls")
	FButtonStyle TalkToSelectedStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button Controls")
	FButtonStyle LookAtNormalStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button Controls")
	FButtonStyle LookAtSelectedStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button Controls")
	FButtonStyle UseNormalStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button Controls")
	FButtonStyle UseSelectedStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button Controls")
	FButtonStyle PushNormalStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button Controls")
	FButtonStyle PushSelectedStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button Controls")
	FButtonStyle PullNormalStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button Controls")
	FButtonStyle PullSelectedStyle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Button Controls")
	EVerbType CurrentVerb;

	////// 
	/// Button handling
	///

	UFUNCTION() void CloseTriggered();
	UFUNCTION() void OpenTriggered();
	UFUNCTION() void GiveTriggered();
	UFUNCTION() void TalkToTriggered();
	UFUNCTION() void LookAtTriggered();
	UFUNCTION() void PickUpTriggered();
	UFUNCTION() void UseTriggered();
	UFUNCTION() void PushTriggered();
	UFUNCTION() void PullTriggered();

	UFUNCTION() void VerbHovered();
	UFUNCTION() void VerbUnhovered();
	
	void ClearActiveButton();
	
private:
	void SetButtonActive(EVerbType VerbType);

	/**
	 * Called during initialization to pick up the normal, unactivated style of
	 * the buttons from the values specified in the UI builder.
	 */
	void AssignNormalStyles();

	void SetActiveVerb() const;

	void SetHoveredVerb(EVerbHoverState IsHovered) const;
};
