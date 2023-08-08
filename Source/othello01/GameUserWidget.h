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
	
	//�� �޾� �� ���� �� ���� �� ����(���ʺ��� �ð��������)
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

	TArray<UUButtonWidget*> ButtonArray;//��ư ��� �迭
	TPair<int, int> RowColumn;//��,�� 

public:
	virtual void NativeConstruct() override;
	UFUNCTION()//��� ���� �Լ�
	void Set_RowColumn(UUButtonWidget* btn);

	UFUNCTION()//������ �ִ� �Լ�
	void Start_Checking_Reverse(int x, int y, bool color);
	UFUNCTION()//������ �� �ִ��� �˻� �� �ִ� �Լ�
	bool IsCheck(int x, int y, int dir, bool check);
	UFUNCTION()//Check �Լ��� ������ ���� ������ �ٲ��ִ� �Լ�
	void Change(bool color);

	UFUNCTION()//���������� �ڸ� �˻� �� ǥ�� �� �ִ� �Լ�
	void Click_Posible(bool color);
	UFUNCTION()//���������� �ڸ� ǥ�õȰ� �����ִ� �Լ�
	void Reset_Click_Posible();

	//�ð� �Լ�
	UFUNCTION()
	void Go_Time();
	UFUNCTION()
	void Reset_Time();

	//���� ���� �Լ�
	UFUNCTION()
	void Set_Score();
	UFUNCTION()
	void Set_Score_Texture(int Black, int White);

	//���� ���� �Լ�
	UFUNCTION()
	void Check_Game0ver();
};