// Fill out your copyright notice in the Description page of Project Settings.


#include "Othello_Game_State.h"
#include "OPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void AOthello_Game_State::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AOthello_Game_State, Count);
    DOREPLIFETIME(AOthello_Game_State, Time);
    DOREPLIFETIME(AOthello_Game_State, ColControl);
    DOREPLIFETIME(AOthello_Game_State, WhiteScore);
    DOREPLIFETIME(AOthello_Game_State, BlackScore);
}

//Count
void AOthello_Game_State::setCount_Implementation(int CountText) {
    Count = CountText;
    Server_setCount(CountText);
}
void AOthello_Game_State::Server_setCount_Implementation(int CountText) {
    Count = CountText;
}
int AOthello_Game_State::GetCount() {
    return Count;
}

//Time
void AOthello_Game_State::setTime_Implementation(int TimeText) {
    Time = TimeText;
    Server_setTime(TimeText);
}
void AOthello_Game_State::Server_setTime_Implementation(int TimeText) {
    Time = TimeText;
}
int AOthello_Game_State::GetTime() {
    return Time;
}

//Color
void AOthello_Game_State::SetColor_Implementation(bool ColorContrl) {
    ColControl = ColorContrl;
    Server_SetColor_Implementation(ColorContrl);
}
void AOthello_Game_State::Server_SetColor_Implementation(bool ColorContrl) {
    ColControl = ColorContrl;
}
bool AOthello_Game_State::GetColor() {
    return ColControl;
}

//BlackColor
void AOthello_Game_State::set_Black_Score_Implementation(int _blackScore) {
    BlackScore = _blackScore;
    Server_set_Black_Score(_blackScore);
}
void AOthello_Game_State::Server_set_Black_Score_Implementation(int _blackScore) {
    BlackScore = _blackScore;
}
int AOthello_Game_State::Get_Black_Score() {
    return BlackScore;
}

//WhiteColor
void AOthello_Game_State::set_White_Score_Implementation(int _whiteScore) {
    WhiteScore = _whiteScore;
    Server_set_White_Score_Implementation(_whiteScore);
}
void AOthello_Game_State::Server_set_White_Score_Implementation(int _whiteScore) {
    WhiteScore = _whiteScore;
}
int AOthello_Game_State::Get_White_Score() {
    return WhiteScore;
}