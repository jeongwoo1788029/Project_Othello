// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"

#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO01_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
		UButton* HostGameButton;
	UPROPERTY(meta = (BindWidget))
		UButton* JoinGameButton;
	UPROPERTY(meta = (BindWidget))
		UButton* QuitGameButton;
	UPROPERTY(meta = (BindWidget))
		UEditableTextBox* IPaddressTextBox;
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void CreateServerHandler();
	UFUNCTION()
		void JoinServerHandler();
	UFUNCTION()
		void QuitGame();
};
