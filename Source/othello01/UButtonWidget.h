// Fill out your copyright notice in the Description page of Project SettinGridslot.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/GridSlot.h"
#include "GameUserWidget.h"
#include "OPlayerController.h"

#include "UButtonWidget.generated.h"



/**
 * 
 */
UCLASS()
class OTHELLO01_API UUButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
	bool bColor = false;
	int32 CountBlock = 0;

public:
	int8 Slot_Index;//������� 0, black�� 1, white�� 2 , check�� 3 
	bool Slot_Visit_Index;//��ã���� �� ���Ǵ� ����
	AOPlayerController* Othello_Controller;//��Ʈ�ѷ� 
	//��ư ����
	UPROPERTY(meta = (BindWidget))
		UButton* Btn;

	//���� ���� �������� ����
	UGridSlot* Gridslot;
	UGameUserWidget* UGameSceneWidget;

	//�̹��� ����
	UPROPERTY(meta = (BindWidget))
		UImage* IMG_Black;
	UPROPERTY(meta = (BindWidget))
		UImage* IMG_White;
	UPROPERTY(meta = (BindWidget))
		UImage* IMG_BlackCheck;
	UPROPERTY(meta = (BindWidget))
		UImage* IMG_WhitCheck;

	virtual void NativeConstruct() override;
	void SetOwningClass(UGameUserWidget* OwningClass);

	//Set,Get�Լ�
	UFUNCTION()
		void Set_Button();
	UFUNCTION()
		void Set_Black_Button();
	UFUNCTION()
		void Set_White_Button();
	UFUNCTION()
		void Set_Check(bool bColor);
	UFUNCTION()
		void Set_Gridslot(UGridSlot* _Gridslot);
	TPair<int, int> Get_RowColumn();
	UFUNCTION()
		int Get_Black_Score();
	UFUNCTION()
		int Get_White_Score();

	//��ưŬ���̺�Ʈ �Լ�
	UFUNCTION()
		void OnBtn_Click();
};