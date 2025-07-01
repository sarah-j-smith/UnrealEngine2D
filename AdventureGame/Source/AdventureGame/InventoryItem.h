// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "VerbInteractions.h"

#include "Paper2D/Classes/PaperSprite.h"

#include "InventoryItem.generated.h"

class AAdventurePlayerController;

/**
 * The `Item` in our inventory.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class ADVENTUREGAME_API UInventoryItem: public UObject, public IVerbInteractions
{
	GENERATED_UCLASS_BODY()
public:
	//////////////////////////////////
	///
	/// ITEM HANDLING
	///

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemHandling")
	FString Description = "A nameless thing";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemHandling")
	EItemList ItemKind = EItemList::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemHandling")
	TObjectPtr<UPaperSprite> Thumbnail;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemHandling")
	EItemList InteractableItem = EItemList::None;
	
	virtual void OnItemCombineSuccess();

	virtual void OnItemCombineFailure();

	//////////////////////////////////
	///
	/// VERB INTERACTION
	///
	
	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnClose_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnOpen_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnGive_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnPickUp_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnTalkTo_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnLookAt_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnPull_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnPush_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnUse_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnWalkTo_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	virtual void OnItemUsed_Implementation(EItemList ItemUsed) override;

	//////////////////////////////////
	///
	/// BARK TEXT
	///
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerbInteractions")
	FText CloseDefaultText = FText::FromString("Can't close it");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerbInteractions")
	FText OpenDefaultText = FText::FromString("Can't open it");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerbInteractions")
	FText GiveDefaultText = FText::FromString("Can't give it");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerbInteractions")
	FText PickUpDefaultText = FText::FromString("Can't pick it up");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerbInteractions")
	FText TalkToDefaultText = FText::FromString("It's not listening");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerbInteractions")
	FText LookAtDefaultText = FText::FromString("Doesn't look like much");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerbInteractions")
	FText PullDefaultText = FText::FromString("Can't pull it");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerbInteractions")
	FText PushDefaultText = FText::FromString("Can't push it");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerbInteractions")
	FText UseDefaultText = FText::FromString("Can't use it");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerbInteractions")
	FText WalkToDefaultText = FText::FromString("What?");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerbInteractions")
	FText ItemUsedDefaultText = FText::FromString("That won't work...");

	UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
	void Bark(const FText &BarkText);

	
	//////////////////////////////////
	///
	/// BARK TEXT
	///
		
	void SetAdventureController(AAdventurePlayerController *AdventurePlayerController)
	{
		this->AdventurePlayerController = AdventurePlayerController;
	}
	
private:
	TObjectPtr<AAdventurePlayerController> AdventurePlayerController; 
};
