// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_AndroidGateway = NewObject<UAndroidGateway>();

	if (m_ButtonPronounce && !m_ButtonPronounce->OnReleased.IsBound())
	{
		m_ButtonPronounce->OnReleased.AddDynamic(this, &UMainMenuWidget::ButtonPronounceReleasedHandle);
	}
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