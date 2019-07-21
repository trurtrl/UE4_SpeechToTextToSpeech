// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_AndroidGateway = NewObject<UAndroidGateway>();
	m_AndroidGateway->OnRecordPhraseDelegate.BindUObject(this, &UMainMenuWidget::OnRecordPhraseDelegateHandle);

	if (m_ButtonPronounce && !m_ButtonPronounce->OnReleased.IsBound())
	{
		m_ButtonPronounce->OnReleased.AddDynamic(this, &UMainMenuWidget::ButtonPronounceReleasedHandle);
	}

	if (m_ButtonRecord)
	{
		//if (!m_ButtonRecord->OnPressed.IsBound())
		//{
		//	m_ButtonRecord->OnPressed.AddDynamic(this, &UMainMenuWidget::ButtonRecordPressedHandle);
		//}
		if (!m_ButtonRecord->OnReleased.IsBound())
		{
			m_ButtonRecord->OnReleased.AddDynamic(this, &UMainMenuWidget::ButtonRecordReleasedHandle);
		}
	}

	b_InRecord = false;
}

void UMainMenuWidget::ButtonPronounceReleasedHandle()
{
	if (m_AndroidGateway && m_TextInput)
	{
		FString phrase = m_TextInput->GetText().ToString();
		if (phrase != "")
		{
			m_AndroidGateway->PronouncePhrase(phrase);
		}
	}
}

void UMainMenuWidget::ButtonRecordPressedHandle()
{
	if (m_TextOutput)
	{
		m_TextOutput->SetText(FText::FromString(""));
	}

	if (m_ImageRecord)
	{
		m_ImageRecord->SetColorAndOpacity(m_ColorPressed);
	}

	if (m_AndroidGateway)
	{
		m_AndroidGateway->StartListening();
	}
}

void UMainMenuWidget::ButtonRecordReleasedHandle()
{
	if (!b_InRecord)
	{
		b_InRecord = true;
		ButtonRecordPressedHandle();
	}
	else
	{
		b_InRecord = false;

		if (m_ImageRecord)
		{
			m_ImageRecord->SetColorAndOpacity(m_ColorUnpressed);
		}

		if (m_AndroidGateway)
		{
			m_AndroidGateway->StopListening();
		}
	}
}

void UMainMenuWidget::OnRecordPhraseDelegateHandle(const FString& Phrase)
{
	if (m_TextOutput)
	{
		m_TextOutput->SetText(FText::FromString(Phrase));
	}
}