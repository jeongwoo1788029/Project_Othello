// Fill out your copyright notice in the Description page of Project Settings.

#include "UButtonWidget.h"
#include "Kismet/GameplayStatics.h"
#include "othello01GameModeBase.h"
#include "OPlayerController.h"
#include "Othello_Game_State.h"

void UUButtonWidget::NativeConstruct() {
	Super::NativeConstruct();

	//����, ĭ ���� �ʱ�ȭ �� UGameSceneWidget ĳ���� 
	bColor = Cast<AOthello_Game_State>(GetWorld()->GetGameState())->GetColor();
	CountBlock = Cast<AOthello_Game_State>(GetWorld()->GetGameState())->GetCount();														
	//�̹��� �ʱ�ȭ(Collapsed)
	IMG_Black->SetVisibility(ESlateVisibility::Collapsed);
	IMG_White->SetVisibility(ESlateVisibility::Collapsed);
	IMG_BlackCheck->SetVisibility(ESlateVisibility::Collapsed);
	//��Ʈ�ѷ� ĳ����
	Othello_Controller = Cast<AOPlayerController>(GetWorld()->GetFirstPlayerController());
}
void UUButtonWidget::SetOwningClass(UGameUserWidget* OwningClass)
{
	UGameSceneWidget = OwningClass;
}

void UUButtonWidget::Set_Button() 
{
	//�̹��� �ʱ�ȭ
	Btn->SetVisibility(ESlateVisibility::HitTestInvisible);
	IMG_WhitCheck->SetVisibility(ESlateVisibility::Hidden);
	//���� �ε��� �ʱ�ȭ
	Slot_Index = 0;
	Slot_Visit_Index = false;
	//��ư Ŭ�� �̺�Ʈ
	Btn->OnClicked.AddDynamic(this, &UUButtonWidget::OnBtn_Click);
}
void UUButtonWidget::Set_Black_Button() 
{
	//�̹��� ����
	IMG_WhitCheck->SetVisibility(ESlateVisibility::Hidden);
	IMG_Black->SetVisibility(ESlateVisibility::Visible);
	//���� �ε��� ����
	Slot_Index = 1;
	Slot_Visit_Index = false;
	//��ư ����
	Btn->SetVisibility(ESlateVisibility::HitTestInvisible);
}
void UUButtonWidget::Set_White_Button() 
{
	//�̹��� ����
	IMG_White->SetVisibility(ESlateVisibility::Visible);
	IMG_WhitCheck->SetVisibility(ESlateVisibility::Hidden);
	//���� �ε��� ����
	Slot_Index = 2;
	Slot_Visit_Index = false;
	//��ư ����
	Btn->SetVisibility(ESlateVisibility::HitTestInvisible);
}
void UUButtonWidget::Set_Check(bool _bColor) 
{
	//��ư ����
	Btn->SetVisibility(ESlateVisibility::Visible);
	//�̹��� ����
	IMG_White->SetVisibility(ESlateVisibility::Hidden);
	IMG_Black->SetVisibility(ESlateVisibility::Hidden);
	if (_bColor == false) {//������ üũ
		IMG_WhitCheck->SetVisibility(ESlateVisibility::Hidden);
		IMG_BlackCheck->SetVisibility(ESlateVisibility::Visible);
	}
	else {//�Ͼ�� üũ
		IMG_BlackCheck->SetVisibility(ESlateVisibility::Hidden);
		IMG_WhitCheck->SetVisibility(ESlateVisibility::Visible);
	}
	//���� �ε��� ����
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
	//Ÿ�� ����
	Othello_Controller->Server_TimeReset_Handler();
	//�� ��ư�� ��ǥ ����
	UGameSceneWidget->Set_RowColumn(this);
	//üũ ����
	UGameSceneWidget->Reset_Click_Posible();
	//��ư ����
	Btn->SetVisibility(ESlateVisibility::HitTestInvisible);
	//���� �� ���� �������� false == black, true == white
	bColor = Cast<AOthello_Game_State>(GetWorld()->GetGameState())->GetColor();
	//��ư �ڵ鷯
	Othello_Controller->Server_Button_Handler(Gridslot->Column, Gridslot->Row, bColor);
	if (bColor == false) 
	{
		//�ε��� ����
		Slot_Index = 1;
		//�̹��� ����
		IMG_White->SetVisibility(ESlateVisibility::Hidden);
		IMG_WhitCheck->SetVisibility(ESlateVisibility::Hidden);
		IMG_BlackCheck->SetVisibility(ESlateVisibility::Hidden);
		IMG_Black->SetVisibility(ESlateVisibility::Visible);
	}
	else //bColor == true
	{
		//�ε��� ����
		Slot_Index = 2;
		//�̹��� ����
		IMG_Black->SetVisibility(ESlateVisibility::Hidden);
		IMG_WhitCheck->SetVisibility(ESlateVisibility::Hidden);
		IMG_BlackCheck->SetVisibility(ESlateVisibility::Hidden);
		IMG_White->SetVisibility(ESlateVisibility::Visible);
	}
	//���� ���� 
	Othello_Controller->Server_Score_Handler();
}