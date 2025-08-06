// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "ConversationData.h"
#include "DialogInteractable.h"
#include "AdventureGame/Constants.h"
#include "AdventureGame/HotSpots/HotSpot.h"
#include "HotSpotNPC.generated.h"

class UPaperFlipbook;
class UPaperFlipbookComponent;

DECLARE_DYNAMIC_DELEGATE_OneParam(FConversationDataLoad, AHotSpotNPC *, NPC);
DECLARE_DYNAMIC_DELEGATE_OneParam(FConversationDataSave, AHotSpotNPC *, NPC);

/**
 * An Non-player Character who just stays in the one position, and uses
 * an animation - a FlipBook.
 */
UCLASS()
class ADVENTUREGAME_API AHotSpotNPC : public AHotSpot, public IDialogInteractable
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AHotSpotNPC();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    FConversationDataLoad ConversationDataLoad;
    FConversationDataSave ConversationDataSave;

    virtual void OnConverseWith_Implementation() override;

    // Copy the data out of the TopicList tables into local ConversationData arrays
    UFUNCTION(BlueprintCallable, Category = "HotSpot")
    void FillConversationData();

    UFUNCTION(BlueprintCallable, Category = "HotSpot")
    void UpdatePromptAtIndex(int32 TopicIndex, int32 PromptIndex);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    UPaperFlipbookComponent *FlipbookComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "NPC")
    UPaperFlipbook *DefaultFlipbook;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dialog")
    FLinearColor TextColor = G_NPC_Default_Text_Colour;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
    TArray<FConversationData> ConversationData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
    TArray<UDataTable *> TopicList;

private:
    
};
