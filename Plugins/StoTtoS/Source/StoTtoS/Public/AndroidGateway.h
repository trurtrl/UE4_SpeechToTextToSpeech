#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StoTtoS/Public/AndroidGatewayDelegate.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#include <android_native_app_glue.h>
#endif

#include "AndroidGateway.generated.h"


UCLASS(BlueprintType/*, Blueprintable*/)
class STOTTOS_API UAndroidGateway : public UObject
{
	GENERATED_BODY()

public:

	UAndroidGateway();
	~UAndroidGateway();

	UFUNCTION(BlueprintCallable)
	void PronouncePhrase(FString& Phrase);

	UFUNCTION(BlueprintCallable)
	void StartListening();

	UFUNCTION(BlueprintCallable)
	void StopListening();

//	UPROPERTY(BlueprintAssignable)
	FOnRecordPhraseRecive OnRecordPhraseDelegate;


#if PLATFORM_ANDROID
	// JNI Methods
	static jmethodID PronouncePhradeMethod;
	static jmethodID StartListeningMethod;
	static jmethodID StopListeningMethod;
#endif

private:

	FString m_RecordPermission;

	bool IsPermissionRecordAudioGranted();
	void AskPermissionRecordAudio();

	UFUNCTION()
	void OnRecordPermissionGrantedHandle(const TArray<FString>& Permissions, const TArray<bool>& GrantResults);

	void StartListeningOnAndroid();

};
