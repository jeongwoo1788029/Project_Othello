// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMenuUserWidget.h"
#include "Components/Button.h"//�߰�
#include "Kismet/KismetSystemLibrary.h"//�߰�
#include "Kismet/GameplayStatics.h"//�߰�
#include "Components/EditableTextBox.h"//�߰�
#include "othello01GameModeBase.h"//�߰�
#include "OPlayerController.h"

#include "Othello_Game_State.h"

void UGameMenuUserWidget::NativeConstruct() 
{
	Super::NativeConstruct();

	GameStartButton->OnClicked.AddDynamic(this, &UGameMenuUserWidget::ClickPlayGameStartButton);
}

void UGameMenuUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FString CountBlockInputText = CountBlockTextBox->GetText().ToString();
	FString TimeLimitInputText = TimeLimitTextBox->GetText().ToString();
	//��ư Ȱ��ȭ
	if ((CountBlockInputText.Len() > 0) && (TimeLimitInputText.Len() > 0)) 
	{
		//string -> int ��ȯ
		CountText = FCString::Atoi(*CountBlockInputText);
		TimeText = FCString::Atoi(*TimeLimitInputText);

		if (CountText <= 10 && CountText > 0 
			&& TimeText <= 99 && TimeText >= 0
			&& CountText % 2 == 0) 
		{
			GameStartButton->SetIsEnabled(true);
		}
		else 
		{
			GameStartButton->SetIsEnabled(false);
		}
	}
	else
	{
		GameStartButton->SetIsEnabled(false);
	}
}

void UGameMenuUserWidget::ClickPlayGameStartButton() {
	auto GameState = Cast<AOthello_Game_State>(GetWorld()->GetGameState());
	if (GameState) {
		GameState->setCount(CountText);
		GameState->setTime(TimeText);
		auto GameScene = LoadObject<UClass>(NULL, TEXT("/Game/GameScene.GameScene_C"), NULL, LOAD_None, NULL);
		Cast<AOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->ChangeToGameboard_(GameScene);
		Cast<AOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 1))->Client_ChangeToGameboard(GameScene);
		//Cast<Aothello01GameModeBase>(GetWorld()->GetAuthGameMode())->OpenGameBoard();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Fail"));
	}
}

