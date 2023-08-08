// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO01_API UGameMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

	int CountText = 0;
	int TimeText = 0;
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
		class UButton* GameStartButton;
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* CountBlockTextBox;
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* TimeLimitTextBox;
	UPROPERTY(meta = (BindWidget))
		class UButton* LoadingButton;

private:
	UFUNCTION()
		void ClickPlayGameStartButton();
};
