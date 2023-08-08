// Fill out your copyright notice in the Description page of Project Settings.

#include "GameUserWidget.h"

#include "Components/GridPanel.h"
#include "Components/Button.h"
#include "Components/Image.h"

#include "Kismet/GameplayStatics.h"
#include "othello01GameModeBase.h"
#include "OPlayerController.h"

#include "UButtonWidget.h"

#include "Othello_Game_State.h"
#include "Net/UnrealNetwork.h"

void UGameUserWidget::NativeConstruct() 
{
	Super::NativeConstruct();

	//ĭ ����, �ð� �ʱ�ȭ
	CountBlock = Cast<AOthello_Game_State>(GetWorld()->GetGameState())->GetCount();
	TimeLimit = Cast<AOthello_Game_State>(GetWorld()->GetGameState())->GetTime();

	for (int i = 0; i < CountBlock; i++) 
	{
		BackgroundGridPanel->SetColumnFill(i, 1);
		BackgroundGridPanel->SetRowFill(i, 1);
	}

	if (ButtononWidgetRef) 
	{
		//�� ����
		for (int i = 0; i < CountBlock; i++) 
		{
			for (int j = 0; j < CountBlock; j++) 
			{
				UUserWidget* Userwidget = CreateWidget(this, ButtononWidgetRef);
				UUButtonWidget* button = Cast<UUButtonWidget>(Userwidget);
				button->SetOwningClass(this);
				button->Set_Gridslot(BackgroundGridPanel->AddChildToGrid(button, i, j));	
				button->Set_Button();
				ButtonArray.Add(button);
			}
		}
		//��� �� 4�� ����
		ButtonArray[CountBlock * (CountBlock / 2 - 1) + (CountBlock / 2 - 1)]->Set_White_Button();
		ButtonArray[CountBlock * (CountBlock / 2) + (CountBlock / 2)]->Set_White_Button();
		ButtonArray[CountBlock * (CountBlock / 2 - 1) + (CountBlock / 2)]->Set_Black_Button();
		ButtonArray[CountBlock * (CountBlock / 2) + (CountBlock / 2 - 1)]->Set_Black_Button();
	}

	FTimerHandle TimerHandle;
	Reset_Time();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UGameUserWidget::Go_Time, 1.f, true, 0.0);

	Click_Posible(false);//ó�� Black ������ ����
}
void UGameUserWidget::Set_RowColumn(UUButtonWidget* btn)
{
	RowColumn = btn->Get_RowColumn();
}

void UGameUserWidget::Start_Checking_Reverse(int x, int y, bool color) 
{
	for (int i = 0; i < 8; i++) 
	{
		for (int k = 0; k < CountBlock * CountBlock; k++) 
		{
			ButtonArray[k]->Slot_Visit_Index = false;
		}
		if (IsCheck(x, y, i, color)) 
		{
			Change(color);
		}
	}
}
bool UGameUserWidget::IsCheck(int x, int y, int dir, bool color) 
{
	bool flag = false;
	int nx = x;
	int ny = y;
	ButtonArray[CountBlock * ny + nx]->Slot_Visit_Index = true;
	//black
	if (color == false)
	{
		while (1) 
		{
			nx += dx[dir];
			ny += dy[dir];
			if (nx < 0 || nx >= CountBlock || ny < 0 || ny >= CountBlock) break;
			if (ButtonArray[CountBlock * ny + nx]->Slot_Visit_Index || ButtonArray[CountBlock * ny + nx]->Slot_Index == 0) break;
			if (ButtonArray[CountBlock * ny + nx]->Slot_Index == 1) 
			{
				flag = true;
				break;
			}
			ButtonArray[CountBlock * ny + nx]->Slot_Visit_Index = true;
		}
	}
	//white
	else
	{
		while (1) 
		{
			nx += dx[dir];
			ny += dy[dir];
			if (nx < 0 || nx >= CountBlock || ny < 0 || ny >= CountBlock) break;
			if (ButtonArray[CountBlock * ny + nx]->Slot_Visit_Index || ButtonArray[CountBlock * ny + nx]->Slot_Index == 0) break;
			if (ButtonArray[CountBlock * ny + nx]->Slot_Index == 2) 
			{
				flag = true;
				break;
			}
			ButtonArray[CountBlock * ny + nx]->Slot_Visit_Index = true;
		}
	}
	return flag;
}
void UGameUserWidget::Change(bool color) 
{
	for (int i = 0; i < CountBlock; i++) 
	{
		for (int j = 0; j < CountBlock; j++) 
		{
			if ((color == false) && (ButtonArray[CountBlock * i + j]->Slot_Visit_Index == true)) //������ ��
			{
				ButtonArray[CountBlock * i + j]->IMG_White->SetVisibility(ESlateVisibility::Hidden);
				ButtonArray[CountBlock * i + j]->IMG_WhitCheck->SetVisibility(ESlateVisibility::Hidden);
				ButtonArray[CountBlock * i + j]->IMG_BlackCheck->SetVisibility(ESlateVisibility::Hidden);

				ButtonArray[CountBlock * i + j]->IMG_Black->SetVisibility(ESlateVisibility::Visible);
				ButtonArray[CountBlock * i + j]->Slot_Index = 1;
			}
			else if ((color == true) && (ButtonArray[CountBlock * i + j]->Slot_Visit_Index == true)) //��� ��
			{
				ButtonArray[CountBlock * i + j]->IMG_Black->SetVisibility(ESlateVisibility::Hidden);
				ButtonArray[CountBlock * i + j]->IMG_WhitCheck->SetVisibility(ESlateVisibility::Hidden);
				ButtonArray[CountBlock * i + j]->IMG_BlackCheck->SetVisibility(ESlateVisibility::Hidden);

				ButtonArray[CountBlock * i + j]->IMG_White->SetVisibility(ESlateVisibility::Visible);
				ButtonArray[CountBlock * i + j]->Slot_Index = 2;
			}
		}
	}
}
void UGameUserWidget::Click_Posible(bool color) 
{
	bool CheckButton = false;
	for (int i = 0; i < CountBlock * CountBlock; i++) 
	{
		for (int dir = 0; dir < 8; dir++) 
		{
			int nX = i % CountBlock;
			int nY = i / CountBlock;
			bool bPossible = false;

			while (ButtonArray[i]->Slot_Index == 0)
			{
				nX += dx[dir];
				nY += dy[dir];

				if (nX < 0 || nX > CountBlock - 1 || nY < 0 || nY > CountBlock - 1)
					break;
				if (ButtonArray[nX + (nY * CountBlock)]->Slot_Index == 0 || ButtonArray[nX + (nY * CountBlock)]->Slot_Index == 3)
					break;
				if ((color == false && ButtonArray[CountBlock * nY + nX]->Slot_Index == 1) ||
					(color == true && ButtonArray[CountBlock * nY + nX]->Slot_Index == 2))
				{
					if (bPossible)
					{
						ButtonArray[i]->Slot_Index = 3;
						ButtonArray[i]->Set_Check(color);
						ButtonArray[i]->SetVisibility(ESlateVisibility::Visible);//�߰�
					}
					break;
				}
				bPossible = true;
			}
		}
	}
	for (int i = 0; i < CountBlock * CountBlock; i++) 
	{
		if (ButtonArray[i]->Slot_Index == 3)
		{
			bExist_Check = true;
			break;
		}
	}
	if (color == false && bExist_Check == false && Click_Number == 0) 
	{
		Cast<AOthello_Game_State>(GetWorld()->GetGameState())->SetColor(true);
		Click_Posible(true);
		Click_Number++;
	}
	else if (color == true && bExist_Check == false && Click_Number == 0) 
	{
		Cast<AOthello_Game_State>(GetWorld()->GetGameState())->SetColor(false);
		Click_Posible(false);
		Click_Number++;
	}
	if (bExist_Check == false && Click_Number > 0) 
	{
		//���� ����
		auto EndScene = LoadObject<UClass>(NULL, TEXT("/Game/EndScene.EndScene_C"), NULL, LOAD_None, NULL);
		Cast<AOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->Client_ChangeToGameboard(EndScene);
		Cast<AOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->Server_ChangeToGameboard(EndScene);
		//Cast<AOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->ChangeToGameboard_(EndScene);
		/*Cast<AOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->ChangeToGameboard_(EndScene);
		Cast<AOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 1))->Client_ChangeToGameboard(EndScene);*/
	}
	Click_Number = 0;
}
void UGameUserWidget::Reset_Click_Posible() 
{
	for (int i = 0; i < CountBlock * CountBlock; i++) 
	{
		if (ButtonArray[i]->Slot_Index == 3)
		{
			ButtonArray[i]->Slot_Index = 0;
			ButtonArray[i]->IMG_BlackCheck->SetVisibility(ESlateVisibility::Hidden);
			ButtonArray[i]->IMG_WhitCheck->SetVisibility(ESlateVisibility::Hidden);
			ButtonArray[i]->IMG_White->SetVisibility(ESlateVisibility::Hidden);
			ButtonArray[i]->IMG_Black->SetVisibility(ESlateVisibility::Hidden);

			ButtonArray[i]->SetVisibility(ESlateVisibility::HitTestInvisible);//(�߰�) ���� �Ѿ���� ��� �� Ŭ���� ���� �˴ϴ�.
		}
	}
}

void UGameUserWidget::Go_Time()
{
	Check_Game0ver();

	if (time != 0)
	{
		time -= 1;//1�ʾ� �귯����.
		
		int TimeTen = time / 10;
		int TimeOne = time % 10;

		TimeDigitTen->SetBrushFromTexture(TNumber_Texture_Array[TimeTen]);
		TimeDigitTen->SetBrushSize(FVector2D(225.0f, 225.0f));
		TimeDigitOne->SetBrushFromTexture(TNumber_Texture_Array[TimeOne]);
		TimeDigitOne->SetBrushSize(FVector2D(225.0f, 225.0f));
	}
	else if (time == 0)//0�ʰ� �Ǿ� ���� �ð��� �����ٸ�...
	{
		//Ÿ�Ӹ���
		//Reset_Time();
		AOthello_Game_State* GameState = Cast<AOthello_Game_State>(GetWorld()->GetGameState());
		auto OPlayerController = Cast<AOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		OPlayerController->Server_TimeReset_Handler();
		Reset_Click_Posible();

		//�� �ٲٱ�
		if (GameState->GetColor() == false)
		{
			//UE_LOG(LogTemp, Warning, TEXT("%d %d"), RowColumn.Key, RowColumn.Value);
			OPlayerController->Server_Button_Handler(RowColumn.Key, RowColumn.Value, false);
			GameState->SetColor(true);
		}
		else if (GameState->GetColor() == true)
		{
			OPlayerController->Server_Button_Handler(RowColumn.Key, RowColumn.Value, true);
			GameState->SetColor(false);
		}
	}
}
void UGameUserWidget::Reset_Time()
{
	time = Cast<AOthello_Game_State>(GetWorld()->GetGameState())->GetTime();
}

void UGameUserWidget::Set_Score() 
{
	int Count_Black = 0;
	int Count_White = 0;
	
	for (int i = 0; i < CountBlock * CountBlock; i++) 
	{
		if (ButtonArray[i]->Slot_Index == 1) 
		{
			Count_Black++;
		}
		else if (ButtonArray[i]->Slot_Index == 2) 
		{
			Count_White++;
		}
	}
	Cast<AOthello_Game_State>(GetWorld()->GetGameState())->set_Black_Score(Count_Black);
	Cast<AOthello_Game_State>(GetWorld()->GetGameState())->set_White_Score(Count_White);
}
void UGameUserWidget::Set_Score_Texture(int Black, int White) 
{
	int BlackTen = Black / 10;
	int BlackOne = Black % 10;

	int WhiteTen = White / 10;
	int WhiteOne = White % 10;

	WhiteDigitTen->SetBrushFromTexture(TNumber_Texture_Array[WhiteTen]);
	WhiteDigitTen->SetBrushSize(FVector2D(225.0f, 225.0f));
	WhiteDigitOne->SetBrushFromTexture(TNumber_Texture_Array[WhiteOne]);
	WhiteDigitOne->SetBrushSize(FVector2D(225.0f, 225.0f));

	BlackDigitTen->SetBrushFromTexture(TNumber_Texture_Array[BlackTen]);
	BlackDigitTen->SetBrushSize(FVector2D(225.0f, 225.0f));
	BlackDigitOne->SetBrushFromTexture(TNumber_Texture_Array[BlackOne]);
	BlackDigitOne->SetBrushSize(FVector2D(225.0f, 225.0f));
}

void UGameUserWidget::Check_Game0ver() 
{
	auto EndScene = LoadObject<UClass>(NULL, TEXT("/Game/EndScene.EndScene_C"), NULL, LOAD_None, NULL);
	int Count_Black = 0;
	int Count_White = 0;
	int Count_Others = 0;

	for (int i = 0; i < CountBlock * CountBlock; i++) 
	{
		if (ButtonArray[i]->Slot_Index == 1) 
		{
			Count_Black++;
		}
		else if (ButtonArray[i]->Slot_Index == 2) 
		{
			Count_White++;
		}
		else 
		{
			Count_Others++;
		}
	}
	if (Count_Black == 0 || Count_White == 0) 
	{
		Cast<AOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->Client_ChangeToGameboard(EndScene);
		Cast<AOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->Server_ChangeToGameboard(EndScene);
	}
	if (Count_Others == 0) 
	{
		Cast<AOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->Client_ChangeToGameboard(EndScene);
		Cast<AOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->Server_ChangeToGameboard(EndScene);
	}
}