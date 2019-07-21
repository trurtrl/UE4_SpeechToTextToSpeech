#include "StoTtoS/Public/AndroidGateway.h"
#include "AndroidPermission/Classes/AndroidPermissionFunctionLibrary.h"
#include "AndroidPermission/Classes/AndroidPermissionCallbackProxy.h"

#if PLATFORM_ANDROID
#include "AndroidPlatform.h"
#include "Android/AndroidJava.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include <android_native_app_glue.h>
#endif

#include <string>
#include "RecordPhraseReceiver.h"


#if PLATFORM_ANDROID
jmethodID UAndroidGateway::PronouncePhradeMethod;
jmethodID UAndroidGateway::StartListeningMethod;
jmethodID UAndroidGateway::StopListeningMethod;
#endif


UAndroidGateway::UAndroidGateway()
	: m_RecordPermission("android.permission.RECORD_AUDIO")
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		PronouncePhradeMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "Pronounce", "(Ljava/lang/String;)V", false);
		StartListeningMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "StartListening", "()V", false);
		StopListeningMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "StopListening", "()V", false);
	}
#endif
}

UAndroidGateway::~UAndroidGateway()
{
	FRecordPhraseReceiver::DeleteInstance();
}


/*
**	Work with Text and Audio
*/

void UAndroidGateway::PronouncePhrase(FString& Phrase)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring jStringParam = Env->NewStringUTF((std::string(TCHAR_TO_UTF8(*Phrase))).c_str());

		if (!jStringParam)
		{
			UE_LOG(LogTemp, Fatal, TEXT("Could Not generate jstring from Phrase"));
		}

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UAndroidGateway::PronouncePhradeMethod, jStringParam);

		Env->DeleteLocalRef(jStringParam);
	}
#endif
}

void UAndroidGateway::StartListening()
{
	if (!IsPermissionRecordAudioGranted())
	{
		AskPermissionRecordAudio();
	}
	else
	{
		StartListeningOnAndroid();
	}
	
}

void UAndroidGateway::StopListening()
{
	if (IsPermissionRecordAudioGranted())
	{
#if PLATFORM_ANDROID
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UAndroidGateway::StopListeningMethod);
		}
#endif
	}
}

/*
**	Permissions
*/

bool UAndroidGateway::IsPermissionRecordAudioGranted()
{
	return UAndroidPermissionFunctionLibrary::CheckPermission(m_RecordPermission);
}

void UAndroidGateway::AskPermissionRecordAudio()
{
	TArray<FString> permissions;
	permissions.Add(m_RecordPermission);
	static UAndroidPermissionCallbackProxy* permissionProxy = UAndroidPermissionFunctionLibrary::AcquirePermissions(permissions);
	if (permissionProxy)
	{
		if (!permissionProxy->OnPermissionsGrantedDynamicDelegate.IsBound())
		{
			permissionProxy->OnPermissionsGrantedDynamicDelegate.AddDynamic(this, &UAndroidGateway::OnRecordPermissionGrantedHandle);
		}
	}
}


void UAndroidGateway::OnRecordPermissionGrantedHandle(const TArray<FString>& Permissions, const TArray<bool>& GrantResults)
{
	for (int32 i = 0; i < Permissions.Num(); ++i)
	{
		if (Permissions[i] == m_RecordPermission && GrantResults[i] == true)
		{
			StartListeningOnAndroid();
		}
	}
}

void UAndroidGateway::StartListeningOnAndroid()
{
	FRecordPhraseReceiver::GetInstance()->OnRecordPhraseRecive = OnRecordPhraseDelegate;

#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UAndroidGateway::StartListeningMethod);
	}
#endif
}


extern "C"
{
#if PLATFORM_ANDROID
	//	Get received data within Message
	JNIEXPORT void Java_com_epicgames_ue4_GameActivity_handleRecordedSpeech(JNIEnv * Env, jobject Obj, jstring RecordedPhrase)
	{
		const char *resultCStr = Env->GetStringUTFChars(RecordedPhrase, 0);
		FRecordPhraseReceiver::GetInstance()->m_RecordedPhrase = FString(UTF8_TO_TCHAR(resultCStr));
		FRecordPhraseReceiver::GetInstance()->OnRecordPhraseRecive.ExecuteIfBound(FRecordPhraseReceiver::GetInstance()->m_RecordedPhrase);

		Env->ReleaseStringUTFChars(RecordedPhrase, resultCStr);
	}
#endif
}