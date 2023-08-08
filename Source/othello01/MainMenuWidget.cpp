// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct() 
{
	HostGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::CreateServerHandler);
	JoinGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::JoinServerHandler);
	QuitGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);
}

void UMainMenuWidget::CreateServerHandler() 
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Minimal_Default"), true, ((FString)(L"Listen")));
}

void UMainMenuWidget::JoinServerHandler() 
{
	FString IPaddress = IPaddressTextBox->GetText().ToString();
	UGameplayStatics::OpenLevel(GetWorld(), FName(IPaddress));
}

void UMainMenuWidget::QuitGame() 
{
	UKismetSystemLibrary::QuitGame(this, 0, EQuitPreference::Quit, false);
}