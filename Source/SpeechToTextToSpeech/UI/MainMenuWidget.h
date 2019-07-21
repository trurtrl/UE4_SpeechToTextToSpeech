// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/MultiLineEditableText.h"
#include "Components/TextBlock.h"
#include "StoTtoS/Public/AndroidGateway.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPEECHTOTEXTTOSPEECH_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:


private:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* m_ButtonPronounce;

	UPROPERTY(meta = (BindWidget))
	UButton* m_ButtonRecord;

	UPROPERTY(meta = (BindWidget))
	UImage* m_ImageRecord;

	UPROPERTY(meta = (BindWidget))
	UMultiLineEditableText* m_TextInput;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* m_TextOutput;

	UPROPERTY(EditDefaultsOnly, meta = (DysplayName = "Record Button Unpressed"), Category = "Color Setup")
	FLinearColor m_ColorUnpressed;

	UPROPERTY(EditDefaultsOnly, meta = (DysplayName = "Record Button Pressed"), Category = "Color Setup")
	FLinearColor m_ColorPressed;


	UPROPERTY()
	UAndroidGateway* m_AndroidGateway;

	UFUNCTION()
	void ButtonPronounceReleasedHandle();

	UFUNCTION()
	void ButtonRecordPressedHandle();

	UFUNCTION()
	void ButtonRecordReleasedHandle();

	UFUNCTION()
	void OnRecordPhraseDelegateHandle(const FString& Phrase);

	bool b_InRecord;

};
