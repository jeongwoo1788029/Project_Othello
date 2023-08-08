// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Othello_Game_State.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO01_API AOthello_Game_State : public AGameStateBase
{
	GENERATED_BODY()
    //ĭ��, �ð�
    UPROPERTY(Replicated)
        int Count;
    UPROPERTY(Replicated)
        int Time;
    //Į�� ��Ʈ��
    UPROPERTY(Replicated)
        bool ColControl = false;
    //����
    UPROPERTY(Replicated)
        int WhiteScore = 0;
    UPROPERTY(Replicated)
        int BlackScore = 0;
    UPROPERTY(Replicated)
        int Colunm;
    UPROPERTY(Replicated)
        int row;

protected:
    void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:
    //set, get �Լ�
    //ĭ��
    UFUNCTION(NetMulticast, Reliable)
    void setCount(int CountText);
    void setCount_Implementation(int CountText);
    UFUNCTION(Server, Reliable)
    void Server_setCount(int CountText);
    void Server_setCount_Implementation(int CountText);
    UFUNCTION()
    int GetCount();

    //�ð�
    UFUNCTION(NetMulticast, Reliable)
    void setTime(int TimeText);
    void setTime_Implementation(int TimeText);
    UFUNCTION(Server, Reliable)
    void Server_setTime(int TimeText);
    void Server_setTime_Implementation(int TimeText);
    UFUNCTION()
    int GetTime();

    //Į��(��)
    UFUNCTION(NetMulticast, Reliable)
    void SetColor(bool ColorContrl);
    void SetColor_Implementation(bool ColorContrl);
    UFUNCTION(Server, Reliable)
    void Server_SetColor(bool ColorContrl);
    void Server_SetColor_Implementation(bool ColorContrl);
    UFUNCTION()
    bool GetColor();

    //SET����
    UFUNCTION(NetMulticast, Reliable)
    void set_Black_Score(int _blackScore);
    void set_Black_Score_Implementation(int _blackScore);
    UFUNCTION(Server, Reliable)
    void Server_set_Black_Score(int _blackScore);
    void Server_set_Black_Score_Implementation(int _blackScore);
    UFUNCTION()
    int Get_Black_Score();

    UFUNCTION(NetMulticast, Reliable)
    void set_White_Score(int _whiteScore);
    void set_White_Score_Implementation(int _whiteScore);
    UFUNCTION(Server, Reliable)
    void Server_set_White_Score(int _whiteScore);
    void Server_set_White_Score_Implementation(int _whiteScore);
    UFUNCTION()
    int Get_White_Score();
};

