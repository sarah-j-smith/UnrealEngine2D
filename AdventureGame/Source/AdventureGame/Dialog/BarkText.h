// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"

#include "BarkRequest.h"
#include "Blueprint/UserWidget.h"
#include "../Constants.h"

#include "BarkText.generated.h"

class FBarkRequest;
class UBarkLine;
class UVerticalBox;
class USphereComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FBarkRequestComplete, int32 /* UID */);

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API UBarkText : public UUserWidget
{
	GENERATED_BODY()
public:
		
	//////////////////////////////////
	///
	/// EVENTS
	///

	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativeDestruct() override;

		
	//////////////////////////////////
	///
	/// BARK HANDLING
	///

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBarkLine> BarkLineClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox *BarkContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BarkLineDisplayTime = BARK_LINE_DELAY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxCharactersPerLine = BARK_LINE_WIDTH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor BarkTextColor = FColor::Orange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* BarkPosition;

	FBarkRequestComplete BarkRequestCompleteDelegate;
	
	void AddBarkRequest(const FBarkRequest *BarkRequest);
	
	/// Clear all the lines of text from the widget
	void ClearText();
	
private:
		
	//////////////////////////////////
	///
	/// BARK CONTAINER
	///

	/// Helper method to set a single line of text into the bark container
	void SetText(const FText &NewText);

	void HideContainer();

	void ShowContainer();

	/// Show a warning if we can't get the view target
	bool bWarningShown = false;

	bool IsHidden = false;

	UPROPERTY()
	AActor *ViewTarget = nullptr;
		
	//////////////////////////////////
	///
	/// BARK LINES MANAGEMENT
	///
	
	/// Cheap timer updated by the tick function, since we already need tick
	float BarkLineTimer;

	/// True when the timer is running and barks are displaying
	bool IsBarking = false;
	
	/// Set an array of lines of text into the bark container. First the bark container
	/// is cleared, the timer reset, and the first line pushed into the container.
	/// Then after every timeout another line is added.
	void SetBarkLines(const TArray<FText> &NewBarkLines);

	/// Which line in the BarkLines array is being shown
	int CurrentBarkLine = 0;

	/// A set of lines that is being scrolled into view in the container
	TArray<FText> BarkLines;
	
	void SetBarkLineTimer();

	void ClearBarkLineTimer();

	void DumpBarkText();

		
	//////////////////////////////////
	///
	/// BARK REQUESTS
	///
	
	void LoadNextBarkRequest();

	void AddToLinkedList(const FBarkRequest *BarkRequest);

	FBarkRequest *PopBarkRequest();

	FBarkRequest *RequestQueue;

	const FBarkRequest *CurrentBarkRequest;
	
	int32 CurrentUID = -1;

	bool IsRenderTransitionSet = false;

	bool IsOneLineAtMinimumSet = false;
	
	/// Called when the BarkLineTimer times out. Keeps feeding the current multiline
	/// message <code>BarkLines</code> into the bark container; or if that is complete
	/// then loads a BarkRequest if one is queued.
	void AddQueuedBarkLine();

public:
	/// Remove all queued requests
	void ClearBarkQueue();

};

inline FBarkRequest *UBarkText::PopBarkRequest()
{
	if (RequestQueue)
	{
		FBarkRequest *BarkRequest = RequestQueue;
		RequestQueue = RequestQueue->NextRequest;
		return BarkRequest;
	}
	return nullptr;
}

inline void UBarkText::ClearBarkQueue()
{
	IsBarking = false;
	while (RequestQueue)
	{
		const FBarkRequest *Request = RequestQueue;
		RequestQueue = RequestQueue->NextRequest;
		if (Request == CurrentBarkRequest)
		{
			CurrentBarkRequest = nullptr;
		}
		delete Request;
	}
	if (CurrentBarkRequest != nullptr)
	{
		delete CurrentBarkRequest;
		CurrentBarkRequest = nullptr;
	}
}

inline void UBarkText::AddToLinkedList(const FBarkRequest *BarkRequest)
{
	if (RequestQueue)
	{
		FBarkRequest *Tail = RequestQueue;
		for ( ; Tail->NextRequest; Tail = Tail->NextRequest ) {}
		Tail->NextRequest = const_cast<FBarkRequest*>(BarkRequest);
	}
	else
	{
		RequestQueue = const_cast<FBarkRequest*>(BarkRequest);
	}
}