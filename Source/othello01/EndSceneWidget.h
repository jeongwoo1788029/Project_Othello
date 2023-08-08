// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "EndSceneWidget.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO01_API UEndSceneWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void Click_QuitButton();
};
