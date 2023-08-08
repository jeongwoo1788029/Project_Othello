// Copyright Epic Games, Inc. All Rights Reserved.


#include "othello01GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "OPlayerController.h"
#include "Kismet/GameplayStatics.h"


void Aothello01GameModeBase::PostLogin(APlayerController* NewPlayer) {
    Super::PostLogin(NewPlayer);
    PlayerController = Cast<AOPlayerController>(NewPlayer);
    PlayerController->Client_ChangeToGameboard(StartingWidgetClass);
}

void Aothello01GameModeBase::BeginPlay()
{
    Super::BeginPlay();
}

void Aothello01GameModeBase::OpenGameBoard() {
    UE_LOG(LogTemp, Warning, TEXT("Game Start!!"));
    //화면 체인지
    auto GameScene = LoadObject<UClass>(NULL, TEXT("/Game/GameScene.GameScene_C"), NULL, LOAD_None, NULL);
    PlayerController->ChangeToGameboard_(GameScene);
    PlayerController->Client_ChangeToGameboard(GameScene);
}