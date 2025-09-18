// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"

#include "ConversationData.h"
#include "AdventureGame/Constants.h"
#include "AdventureGame/Enums/DialogState.h"
#include "AdventureGame/Player/AdventurePlayerController.h"
#include "Components/ActorComponent.h"

#include "DialogComponent.generated.h"

class AHotSpotNPC;
class AAdventurePlayerController;
class UBarkText;
class UPromptList;

DECLARE_DYNAMIC_DELEGATE_OneParam(FConversationDataLoad, UDialogComponent *, DialogComponent);
DECLARE_DYNAMIC_DELEGATE_OneParam(FConversationDataSave, UDialogComponent *, DialogComponent);

DECLARE_MULTICAST_DELEGATE(FConversationEnded);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ADVENTUREGAME_API UDialogComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UDialogComponent();
    
    // Copy the data out of the TopicList tables into local ConversationData arrays
    UFUNCTION(BlueprintCallable, Category = "HotSpot")
    void FillConversationData();

    // Mark this prompt has been selected by the player.
    UFUNCTION(BlueprintCallable, Category = "HotSpot")
    void UpdatePromptAtIndex(int32 TopicIndex, int32 PromptIndex);

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;

    FConversationDataLoad ConversationDataLoad;
    FConversationDataSave ConversationDataSave;

    FConversationEnded ConversationEndedEvent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
    EDialogState DialogState = EDialogState::Hidden;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dialog")
    FLinearColor TextColor = G_NPC_Default_Text_Colour;

    /// Conversations loaded from data tables, ready to be barked.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
    TArray<FConversationData> ConversationData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
    int32 TopicIndex = 0;
    
    /// Data tables that contain the dialogue at design time, in the editor.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
    TArray<UDataTable *> TopicList;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompts")
    TArray<FPromptData> PromptsToShow;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompts")
    int CurrentPromptIndex = 0;
    
    /**
     * Get the prompts at the current topic as an array of prompt data
     * from the data tables.
     * @param PromptsToShow Out parameter to be filled with the prompts
     */
    void LoadPrompts(TArray<FPromptData> &PromptsToShow);

    int ConversationCount() const;

    void HandleConversations();

    void StopMonitoringConversations();

    void AssignNewTopic(const UDataTable *NewTopic);
private:
    UFUNCTION()
    void HandlePromptClick(int PromptIndex);

    TArray<int> Stack;

    void DisplayPrompts();
    void ShowPlayerBark();
    void ShowNPCResponse();
    void ShowNextDialogPrompts();
    void InitialiseForBarking();
    void TearDownBarkingSetup();

    UPROPERTY()
    UPromptList *PromptList = nullptr;

    UPROPERTY()
    UBarkText *Bark = nullptr;

    UPROPERTY()
    AAdventurePlayerController *Apc = nullptr;

    UPROPERTY()
    AHotSpotNPC *NPC = nullptr;

    FTimerDelegate BarkTimerDelegate;
    FTimerHandle BarkTimerHandle;
    bool IsBarking = false;

    UFUNCTION()
    void OnBarkTimerTimeout(int32 UID);

    bool PopConversationTopic();

    void PushConversationTopic();

    int32 BarkUID = 0;
};
