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
	int8 Slot_Index;//빈공간은 0, black은 1, white은 2 , check는 3 
	bool Slot_Visit_Index;//길찾기할 때 사용되는 변수
	AOPlayerController* Othello_Controller;//컨트롤러 
	//버튼 위젯
	UPROPERTY(meta = (BindWidget))
		UButton* Btn;

	//슬롯 정보 가져오는 변수
	UGridSlot* Gridslot;
	UGameUserWidget* UGameSceneWidget;

	//이미지 위젯
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

	//Set,Get함수
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

	//버튼클릭이벤트 함수
	UFUNCTION()
		void OnBtn_Click();
};