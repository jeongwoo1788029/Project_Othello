// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "othello01GameModeBase.h"
#include "Blueprint/UserWidget.h"

#include "GameFramework/PlayerController.h"

#include "GameUserWidget.h"
#include "OPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO01_API AOPlayerController : public APlayerController
{
	GENERATED_BODY()
	int FirstTurn_Checking_Controll = 0;

public:
	virtual void BeginPlay() override;

	UPROPERTY()
		UUserWidget* CurrentWidget;
	//화면 전환
	UFUNCTION(Client, Reliable)
	void Client_ChangeToGameboard(TSubclassOf<UUserWidget> NewWidgetClass);
	void Client_ChangeToGameboard_Implementation(TSubclassOf<UUserWidget> NewWidgetClass);
	UFUNCTION()
	void ChangeToGameboard_(TSubclassOf<UUserWidget> NewWidgetClass);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ChangeToGameboard(TSubclassOf<UUserWidget> NewWidgetClass);
	bool Server_ChangeToGameboard_Validate(TSubclassOf<UUserWidget> NewWidgetClass);
	void Server_ChangeToGameboard_Implementation(TSubclassOf<UUserWidget> NewWidgetClass);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ChangeToGameBoard(TSubclassOf<UUserWidget> NewWidgetClass);
	void Multicast_ChangeToGameBoard_Implementation(TSubclassOf<UUserWidget> NewWidgetClass);

	//타임 리셋
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_TimeReset_Handler(AOPlayerController* PlayerController);
	void Multicast_TimeReset_Handler_Implementation(AOPlayerController* PlayerController);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TimeReset_Handler();
	bool Server_TimeReset_Handler_Validate();
	void Server_TimeReset_Handler_Implementation();

	//점수
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Score_Handler(AOPlayerController* PlayerController);
	void Multicast_Score_Handler_Implementation(AOPlayerController* PlayerController);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Score_Handler();
	bool Server_Score_Handler_Validate();
	void Server_Score_Handler_Implementation();

	//버튼
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Button_Handler(int x, int y, bool bColor, AOPlayerController* PlayerController);
	void Multicast_Button_Handler_Implementation(int x, int y, bool bColor, AOPlayerController* PlayerController);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Button_Handler(int x, int y, bool bColor);
	bool Server_Button_Handler_Validate(int x, int y, bool bColor);
	void Server_Button_Handler_Implementation(int x, int y, bool bColor);

	UFUNCTION(Client, Reliable)
	void Client_Buttons_Handler(UGameUserWidget* _Othello_Widget);
	void Client_Buttons_Handler_Implementation(UGameUserWidget* _Othello_Widget);

	UFUNCTION(Server, Reliable)
	void Server_Buttons_Handler();
	bool Server_Buttons_Handler_Validate();
	void Server_Buttons_Handler_Implementation();

};
