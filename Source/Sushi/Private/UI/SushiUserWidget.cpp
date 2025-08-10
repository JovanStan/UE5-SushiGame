

#include "UI/SushiUserWidget.h"

#include "Components/TextBlock.h"

void USushiUserWidget::SetInfoText(const FText& NewText)
{
	if (InfoText)
	{
		InfoText->SetText(NewText);
		GetWorld()->GetTimerManager().ClearTimer(ClearTextTimerHandle);
		
		GetWorld()->GetTimerManager().SetTimer(ClearTextTimerHandle,this, &USushiUserWidget::ClearInfoText,2.0f,false);
	}
}

void USushiUserWidget::SetScoreText(int32 NewScore)
{
	if (ScoreText)
	{
		ScoreText->SetText(FText::Format(FText::FromString("Score: {0}"), FText::AsNumber(NewScore)));
	}
}

void USushiUserWidget::ClearInfoText()
{
	if (InfoText)
	{
		InfoText->SetText(FText::GetEmpty());
	}
}
