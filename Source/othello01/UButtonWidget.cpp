// Fill out your copyright notice in the Description page of Project Settings.

#include "UButtonWidget.h"
#include "Kismet/GameplayStatics.h"
#include "othello01GameModeBase.h"
#include "OPlayerController.h"
#include "Othello_Game_State.h"

void UUButtonWidget::NativeConstruct() {
	Super::NativeConstruct();

	//색깔, 칸 개수 초기화 및 UGameSceneWidget 캐스팅 
	bColor = Cast<AOthello_Game_State>(GetWorld()->GetGameState())->GetColor();
	CountBlock = Cast<AOthello_Game_State>(GetWorld()->GetGameState())->GetCount();														
	//이미지 초기화(Collapsed)
	IMG_Black->SetVisibility(ESlateVisibility::Collapsed);
	IMG_White->SetVisibility(ESlateVisibility::Collapsed);
	IMG_BlackCheck->SetVisibility(ESlateVisibility::Collapsed);
	//컨트롤러 캐스팅
	Othello_Controller = Cast<AOPlayerController>(GetWorld()->GetFirstPlayerController());
}
void UUButtonWidget::SetOwningClass(UGameUserWidget* OwningClass)
{
	UGameSceneWidget = OwningClass;
}

void UUButtonWidget::Set_Button() 
{
	//이미지 초기화
	Btn->SetVisibility(ESlateVisibility::HitTestInvisible);
	IMG_WhitCheck->SetVisibility(ESlateVisibility::Hidden);
	//슬롯 인덱스 초기화
	Slot_Index = 0;
	Slot_Visit_Index = false;
	//버튼 클릭 이벤트
	Btn->OnClicked.AddDynamic(this, &UUButtonWidget::OnBtn_Click);
}
void UUButtonWidget::Set_Black_Button() 
{
	//이미지 설정
	IMG_WhitCheck->SetVisibility(ESlateVisibility::Hidden);
	IMG_Black->SetVisibility(ESlateVisibility::Visible);
	//슬롯 인덱스 설정
	Slot_Index = 1;
	Slot_Visit_Index = false;
	//버튼 설정
	Btn->SetVisibility(ESlateVisibility::HitTestInvisible);
}
void UUButtonWidget::Set_White_Button() 
{
	//이미지 설정
	IMG_White->SetVisibility(ESlateVisibility::Visible);
	IMG_WhitCheck->SetVisibility(ESlateVisibility::Hidden);
	//슬롯 인덱스 설정
	Slot_Index = 2;
	Slot_Visit_Index = false;
	//버튼 설정
	Btn->SetVisibility(ESlateVisibility::HitTestInvisible);
}
void UUButtonWidget::Set_Check(bool _bColor) 
{
	//버튼 설정
	Btn->SetVisibility(ESlateVisibility::Visible);
	//이미지 설정
	IMG_White->SetVisibility(ESlateVisibility::Hidden);
	IMG_Black->SetVisibility(ESlateVisibility::Hidden);
	if (_bColor == false) {//검은색 체크
		IMG_WhitCheck->SetVisibility(ESlateVisibility::Hidden);
		IMG_BlackCheck->SetVisibility(ESlateVisibility::Visible);
	}
	else {//하얀색 체크
		IMG_BlackCheck->SetVisibility(ESlateVisibility::Hidden);
		IMG_WhitCheck->SetVisibility(ESlateVisibility::Visible);
	}
	//슬롯 인덱스 설정
	Slot_Index = 3;
	Slot_Visit_Index = false;
}

void UUButtonWidget::Set_Gridslot(UGridSlot* _Gridslot)
{
	Gridslot = _Gridslot;
}
TPair<int, int> UUButtonWidget::Get_RowColumn()
{
	return TPair<int, int>(Gridslot->Column, Gridslot->Row);
}

int UUButtonWidget::Get_Black_Score()
{
	return Cast<AOthello_Game_State>(GetWorld()->GetGameState())->Get_Black_Score();
}
int UUButtonWidget::Get_White_Score()
{
	return Cast<AOthello_Game_State>(GetWorld()->GetGameState())->Get_White_Score();
}

void UUButtonWidget::OnBtn_Click() {
	//타임 리셋
	Othello_Controller->Server_TimeReset_Handler();
	//이 버튼의 좌표 설정
	UGameSceneWidget->Set_RowColumn(this);
	//체크 리셋
	UGameSceneWidget->Reset_Click_Posible();
	//버튼 설정
	Btn->SetVisibility(ESlateVisibility::HitTestInvisible);
	//현재 색 정보 가져오기 false == black, true == white
	bColor = Cast<AOthello_Game_State>(GetWorld()->GetGameState())->GetColor();
	//버튼 핸들러
	Othello_Controller->Server_Button_Handler(Gridslot->Column, Gridslot->Row, bColor);
	if (bColor == false) 
	{
		//인덱스 설정
		Slot_Index = 1;
		//이미지 설정
		IMG_White->SetVisibility(ESlateVisibility::Hidden);
		IMG_WhitCheck->SetVisibility(ESlateVisibility::Hidden);
		IMG_BlackCheck->SetVisibility(ESlateVisibility::Hidden);
		IMG_Black->SetVisibility(ESlateVisibility::Visible);
	}
	else //bColor == true
	{
		//인덱스 설정
		Slot_Index = 2;
		//이미지 설정
		IMG_Black->SetVisibility(ESlateVisibility::Hidden);
		IMG_WhitCheck->SetVisibility(ESlateVisibility::Hidden);
		IMG_BlackCheck->SetVisibility(ESlateVisibility::Hidden);
		IMG_White->SetVisibility(ESlateVisibility::Visible);
	}
	//점수 설정 
	Othello_Controller->Server_Score_Handler();
}