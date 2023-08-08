// Fill out your copyright notice in the Description page of Project Settings.

#include "OPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Othello_Game_State.h"
#include "GameUserWidget.h"
#include "GameMenuUserWidget.h"
#include "Components/Button.h"//추가
#include "GameMenuUserWidget.h"

void AOPlayerController::BeginPlay()
{
    Super::BeginPlay();
    //마우스커서 보이게 하기
    SetShowMouseCursor(true);
    //게임과 UI 모두 입력 가능 상태로 전환
    SetInputMode(FInputModeGameAndUI());
}

//화면 체인지

void AOPlayerController::Client_ChangeToGameboard_Implementation(TSubclassOf<UUserWidget> NewWidgetClass)
{   
    if (HasAuthority()) 
    {
         UE_LOG(LogTemp, Warning, TEXT("Server!!"));
    }
    else 
    {
        UE_LOG(LogTemp, Warning, TEXT("Client!!"));
    }

   if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }
    if (NewWidgetClass != nullptr)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport();
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("First %d"), FirstTurn_Checking_Controll);

    if (FirstTurn_Checking_Controll == 0) 
    {
        if (!HasAuthority()) 
        {
            auto OPlayerController = Cast<AOPlayerController>(GetWorld()->GetFirstPlayerController());
            UGameMenuUserWidget* Othello_Manu_Widget = Cast<UGameMenuUserWidget>(OPlayerController->CurrentWidget);
            OPlayerController->Server_Buttons_Handler();
        }
        FirstTurn_Checking_Controll++;
    }
    else if (FirstTurn_Checking_Controll == 1)
    {
        auto OPlayerController = Cast<AOPlayerController>(GetWorld()->GetFirstPlayerController());
        if (!HasAuthority())
        {
            OPlayerController->Server_ChangeToGameboard(NewWidgetClass);
            UGameUserWidget* Othello_Widget = Cast<UGameUserWidget>(OPlayerController->CurrentWidget);
            OPlayerController->Client_Buttons_Handler(Othello_Widget);
        }
        FirstTurn_Checking_Controll++;
    }
    UE_LOG(LogTemp, Warning, TEXT("Second : %d"), FirstTurn_Checking_Controll);
}
void AOPlayerController::ChangeToGameboard_(TSubclassOf<UUserWidget> NewWidgetClass)
{
    if (HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("Server!!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Client!!"));
    }

    if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }
    if (NewWidgetClass != nullptr)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport();
        }
    }
}
bool AOPlayerController::Server_ChangeToGameboard_Validate(TSubclassOf<UUserWidget> NewWidgetClass)
{
    return true;
}
void AOPlayerController::Server_ChangeToGameboard_Implementation(TSubclassOf<UUserWidget> NewWidgetClass)
{
    if (HasAuthority()) 
    {
        UE_LOG(LogTemp, Warning, TEXT("Server"));
    }
    else 
    {
        UE_LOG(LogTemp, Warning, TEXT("Client"));
    }
    if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }
    if (NewWidgetClass != nullptr)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport();
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Server %d"), FirstTurn_Checking_Controll);
    //Client_ChangeToGameboard_Implementation(NewWidgetClass);
    //Multicast_ChangeToGameBoard(NewWidgetClass);
}

void AOPlayerController::Multicast_ChangeToGameBoard_Implementation(TSubclassOf<UUserWidget> NewWidgetClass) 
{
    if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }
    if (NewWidgetClass != nullptr)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport();
        }
    }
}

//시간 초기화 rpc
void AOPlayerController::Multicast_TimeReset_Handler_Implementation(AOPlayerController* PlayerController)
{
    UGameUserWidget* Othello_Widget = Cast<UGameUserWidget>(PlayerController->CurrentWidget);
    if (Othello_Widget) 
    {
        Othello_Widget->Reset_Time();
    }
}
bool AOPlayerController::Server_TimeReset_Handler_Validate() 
{
    return true;
}
void AOPlayerController::Server_TimeReset_Handler_Implementation() 
{
    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; Iterator++) 
    {
        auto OPlayerController = Cast<AOPlayerController>(Iterator->Get());
        OPlayerController->Multicast_TimeReset_Handler(OPlayerController);
    }
}

//점수 rpc
void AOPlayerController::Multicast_Score_Handler_Implementation(AOPlayerController* PlayerController)
{
    UGameUserWidget* Othello_Widget = Cast<UGameUserWidget>(PlayerController->CurrentWidget);
    if (Othello_Widget) 
    {
        Othello_Widget->Set_Score();
        int Black_Score = Cast<AOthello_Game_State>(GetWorld()->GetGameState())->Get_Black_Score();
        int White_Score = Cast<AOthello_Game_State>(GetWorld()->GetGameState())->Get_White_Score();
        Othello_Widget->Set_Score_Texture(Black_Score, White_Score);
    }
}
bool AOPlayerController::Server_Score_Handler_Validate()
{
    return true;
}
void AOPlayerController::Server_Score_Handler_Implementation()
{
    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; Iterator++) 
    {
        auto OPlayerController = Cast<AOPlayerController>(Iterator->Get());
        OPlayerController->Multicast_Score_Handler(OPlayerController);
    }
}

//버튼 rpc
void AOPlayerController::Multicast_Button_Handler_Implementation(int x, int y, bool bColor, AOPlayerController* PlayerController)
{
    UGameUserWidget* Othello_Widget = Cast<UGameUserWidget>(PlayerController->CurrentWidget);
    if (Othello_Widget) 
    {
        if (bColor == false) 
        {//흑돌차례라면
            Othello_Widget->Start_Checking_Reverse(x, y, bColor);
            Othello_Widget->Click_Posible(!bColor);
            if (HasAuthority()) 
            {
                //Othello_Widget->Make_Empty_Checking();
                Othello_Widget->Reset_Click_Posible();
            }
            //턴 넘기기
            Cast<AOthello_Game_State>(GetWorld()->GetGameState())->SetColor(true);
        }
        else 
        {//bColor == true 흰돌차례라면
            Othello_Widget->Start_Checking_Reverse(x, y, bColor);
            Othello_Widget->Click_Posible(!bColor);
            if (!HasAuthority()) 
            {
                //Othello_Widget->Make_Empty_Checking();
                Othello_Widget->Reset_Click_Posible();
            }   
            //턴 넘기기
            Cast<AOthello_Game_State>(GetWorld()->GetGameState())->SetColor(false);
        }
    }
}
bool AOPlayerController::Server_Button_Handler_Validate(int x, int y, bool bColor) {
    return true;
}
void AOPlayerController::Server_Button_Handler_Implementation(int x,int y, bool bColor)
{
    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; Iterator++) 
    {
        auto OPlayerController = Cast<AOPlayerController>(Iterator->Get());
        OPlayerController->Multicast_Button_Handler(x, y, bColor, OPlayerController);
    }
}

void AOPlayerController::Client_Buttons_Handler_Implementation(UGameUserWidget* _Othello_Widget) 
{
   _Othello_Widget->Reset_Click_Posible();
}

bool AOPlayerController::Server_Buttons_Handler_Validate() {
    return true;
}
void AOPlayerController::Server_Buttons_Handler_Implementation()
{
    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; Iterator++) 
    {
        if (HasAuthority()) 
        {
            auto OPlayerController = Cast<AOPlayerController>(Iterator->Get());
            UGameMenuUserWidget* Othello_Manu_Widget = Cast<UGameMenuUserWidget>(OPlayerController->CurrentWidget);
            if (Othello_Manu_Widget) 
            {
                Othello_Manu_Widget->LoadingButton->SetVisibility(ESlateVisibility::Hidden);
            }
        }
    }
}

//백업소스

////화면 체인지
//void AOPlayerController::Client_ChangeToGameboard_Implementation(TSubclassOf<UUserWidget> NewWidgetClass)
//{
//    if (HasAuthority())
//    {
//        UE_LOG(LogTemp, Warning, TEXT("Server"));
//    }
//    else
//    {
//        UE_LOG(LogTemp, Warning, TEXT("Client!!"));
//    }
//
//    if (CurrentWidget != nullptr)
//    {
//        CurrentWidget->RemoveFromViewport();
//        CurrentWidget = nullptr;
//    }
//    if (NewWidgetClass != nullptr)
//    {
//        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
//        if (CurrentWidget != nullptr)
//        {
//            CurrentWidget->AddToViewport();
//        }
//    }
//
//    if (FirstTurn_Checking_Controll == 0)
//    {
//        FirstTurn_Checking_Controll++;
//        if (!HasAuthority())
//        {
//            for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; Iterator++)
//            {
//                auto OPlayerController = Cast<AOPlayerController>(Iterator->Get());
//                UGameMenuUserWidget* Othello_Manu_Widget = Cast<UGameMenuUserWidget>(OPlayerController->CurrentWidget);
//                OPlayerController->Server_Buttons_Handler();
//            }
//        }
//    }
//    else if (FirstTurn_Checking_Controll == 1)
//    {
//        for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; Iterator++)
//        {
//            auto OPlayerController = Cast<AOPlayerController>(Iterator->Get());
//            if (!HasAuthority())
//            {
//                UGameUserWidget* Othello_Widget = Cast<UGameUserWidget>(OPlayerController->CurrentWidget);
//                OPlayerController->Client_Buttons_Handler(Othello_Widget);
//            }
//            else
//            {
//                UGameMenuUserWidget* Othello_Manu_Widget = Cast<UGameMenuUserWidget>(OPlayerController->CurrentWidget);
//            }
//        }
//        FirstTurn_Checking_Controll++;
//    }
//}
//bool AOPlayerController::Server_ChangeToGameboard_Validate(TSubclassOf<UUserWidget> NewWidgetClass)
//{
//    return true;
//}
//void AOPlayerController::Server_ChangeToGameboard_Implementation(TSubclassOf<UUserWidget> NewWidgetClass)
//{
//    if (HasAuthority())
//    {
//        UE_LOG(LogTemp, Warning, TEXT("Server"));
//    }
//    else
//    {
//        UE_LOG(LogTemp, Warning, TEXT("Client"));
//    }
//
//    if (CurrentWidget != nullptr)
//    {
//        CurrentWidget->RemoveFromViewport();
//        CurrentWidget = nullptr;
//    }
//    if (NewWidgetClass != nullptr)
//    {
//        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
//        if (CurrentWidget != nullptr)
//        {
//            CurrentWidget->AddToViewport();
//        }
//    }
//}
//
////시간 초기화 rpc
//void AOPlayerController::Multicast_TimeReset_Handler_Implementation(AOPlayerController* PlayerController)
//{
//    UGameUserWidget* Othello_Widget = Cast<UGameUserWidget>(PlayerController->CurrentWidget);
//    if (Othello_Widget)
//    {
//        Othello_Widget->Reset_Time();
//    }
//}
//bool AOPlayerController::Server_TimeReset_Handler_Validate()
//{
//    return true;
//}
//void AOPlayerController::Server_TimeReset_Handler_Implementation()
//{
//    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; Iterator++)
//    {
//        auto OPlayerController = Cast<AOPlayerController>(Iterator->Get());
//        OPlayerController->Multicast_TimeReset_Handler(OPlayerController);
//    }
//}
//
////점수 rpc
//void AOPlayerController::Multicast_Score_Handler_Implementation(AOPlayerController* PlayerController)
//{
//    UGameUserWidget* Othello_Widget = Cast<UGameUserWidget>(PlayerController->CurrentWidget);
//    if (Othello_Widget)
//    {
//        Othello_Widget->Set_Score();
//        int Black_Score = Cast<AOthello_Game_State>(GetWorld()->GetGameState())->Get_Black_Score();
//        int White_Score = Cast<AOthello_Game_State>(GetWorld()->GetGameState())->Get_White_Score();
//        Othello_Widget->Set_Score_Texture(Black_Score, White_Score);
//    }
//}
//bool AOPlayerController::Server_Score_Handler_Validate()
//{
//    return true;
//}
//void AOPlayerController::Server_Score_Handler_Implementation()
//{
//    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; Iterator++)
//    {
//        auto OPlayerController = Cast<AOPlayerController>(Iterator->Get());
//        OPlayerController->Multicast_Score_Handler(OPlayerController);
//    }
//}
//
////버튼 rpc
//void AOPlayerController::Multicast_Button_Handler_Implementation(int x, int y, bool bColor, AOPlayerController* PlayerController)
//{
//    UGameUserWidget* Othello_Widget = Cast<UGameUserWidget>(PlayerController->CurrentWidget);
//    if (Othello_Widget)
//    {
//        if (bColor == false)
//        {//흑돌차례라면
//            Othello_Widget->Start_Checking_Reverse(x, y, bColor);
//            Othello_Widget->Click_Posible(!bColor);
//            if (HasAuthority()) {
//                Othello_Widget->Make_Empty_Checking();
//            }
//
//            Cast<AOthello_Game_State>(GetWorld()->GetGameState())->SetColor(true);
//        }
//        else
//        {//bColor == true 흰돌차례라면
//            Othello_Widget->Start_Checking_Reverse(x, y, bColor);
//            Othello_Widget->Click_Posible(!bColor);
//            if (!HasAuthority())
//            {
//                Othello_Widget->Make_Empty_Checking();
//            }
//            //턴 넘기기
//            Cast<AOthello_Game_State>(GetWorld()->GetGameState())->SetColor(false);
//        }
//    }
//}
//bool AOPlayerController::Server_Button_Handler_Validate(int x, int y, bool bColor) {
//    return true;
//}
//void AOPlayerController::Server_Button_Handler_Implementation(int x, int y, bool bColor)
//{
//    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; Iterator++)
//    {
//        auto OPlayerController = Cast<AOPlayerController>(Iterator->Get());
//        OPlayerController->Multicast_Button_Handler(x, y, bColor, OPlayerController);
//    }
//}
//
//void AOPlayerController::Client_Buttons_Handler_Implementation(UGameUserWidget* _Othello_Widget)
//{
//    _Othello_Widget->Make_Empty_Checking();
//}
//
//bool AOPlayerController::Server_Buttons_Handler_Validate() {
//    return true;
//}
//void AOPlayerController::Server_Buttons_Handler_Implementation()
//{
//    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; Iterator++) {
//        if (HasAuthority()) {
//            auto OPlayerController = Cast<AOPlayerController>(Iterator->Get());
//            UGameMenuUserWidget* Othello_Manu_Widget = Cast<UGameMenuUserWidget>(OPlayerController->CurrentWidget);
//            if (Othello_Manu_Widget) {
//                Othello_Manu_Widget->LoadingButton->SetVisibility(ESlateVisibility::Hidden);
//            }
//        }
//    }
//}