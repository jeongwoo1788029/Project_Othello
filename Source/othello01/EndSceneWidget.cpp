// Fill out your copyright notice in the Description page of Project Settings.


#include "EndSceneWidget.h"
#include "Kismet/KismetSystemLibrary.h"

void UEndSceneWidget::NativeConstruct() {
	QuitButton->OnClicked.AddDynamic(this, &UEndSceneWidget::Click_QuitButton);
}

void UEndSceneWidget::Click_QuitButton() {
	UKismetSystemLibrary::QuitGame(this, 0, EQuitPreference::Quit, false);
}