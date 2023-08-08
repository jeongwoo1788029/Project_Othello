// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "othello01GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO01_API Aothello01GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
    UPROPERTY()
         class AOPlayerController* PlayerController;

    void OpenGameBoard();
   
protected:
    virtual void PostLogin(APlayerController* NewPlayer) override;
    /** Called when the game starts. */
    virtual void BeginPlay() override;
    /** The widget class we will use as our menu when the game starts. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
        TSubclassOf<UUserWidget> StartingWidgetClass;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
        TSubclassOf<UUserWidget> Game_BoardWdiget;
};
