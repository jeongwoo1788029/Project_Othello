// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "GameUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO01_API UGameUserWidget : public UUserWidget
{
	GENERATED_BODY()

	int32 CountBlock;
	int TimeLimit;	
	int time;
	int Click_Number;
	bool bExist_Check;
	
	//왼 왼아 아 오아 오 오위 위 왼위(왼쪽부터 시계방향으로)
	int dx[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
	int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

public:
	UPROPERTY(meta = (BindWidget))
		class UGridPanel* BackgroundGridPanel;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUButtonWidget> ButtononWidgetRef;
	UPROPERTY(EditAnywhere)
		TArray<class UTexture2D*> TNumber_Texture_Array;
	UPROPERTY(meta = (BindWidget))
		UImage* WhiteDigitTen;
	UPROPERTY(meta = (BindWidget))
		UImage* WhiteDigitOne;
	UPROPERTY(meta = (BindWidget))
		UImage* BlackDigitTen;
	UPROPERTY(meta = (BindWidget))
		UImage* BlackDigitOne;
	UPROPERTY(meta = (BindWidget))
		UImage* TimeDigitTen;
	UPROPERTY(meta = (BindWidget))
		UImage* TimeDigitOne;

	TArray<UUButtonWidget*> ButtonArray;//버튼 담는 배열
	TPair<int, int> RowColumn;//행,열 

public:
	virtual void NativeConstruct() override;
	UFUNCTION()//행렬 설정 함수
	void Set_RowColumn(UUButtonWidget* btn);

	UFUNCTION()//뒤집어 주는 함수
	void Start_Checking_Reverse(int x, int y, bool color);
	UFUNCTION()//뒤집을 수 있는지 검사 해 주는 함수
	bool IsCheck(int x, int y, int dir, bool check);
	UFUNCTION()//Check 함수가 끝나면 돌의 색깔을 바꿔주는 함수
	void Change(bool color);

	UFUNCTION()//뒤집어지는 자리 검사 및 표시 해 주는 함수
	void Click_Posible(bool color);
	UFUNCTION()//뒤집어지는 자리 표시된거 없애주는 함수
	void Reset_Click_Posible();

	//시간 함수
	UFUNCTION()
	void Go_Time();
	UFUNCTION()
	void Reset_Time();

	//점수 설정 함수
	UFUNCTION()
	void Set_Score();
	UFUNCTION()
	void Set_Score_Texture(int Black, int White);

	//게임 오버 함수
	UFUNCTION()
	void Check_Game0ver();
};