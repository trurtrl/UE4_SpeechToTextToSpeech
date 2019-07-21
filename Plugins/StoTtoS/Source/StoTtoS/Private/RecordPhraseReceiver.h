#pragma once

#include "CoreMinimal.h"
#include "StoTtoS/Public/AndroidGatewayDelegate.h"


class STOTTOS_API FRecordPhraseReceiver
{
public:

	static FRecordPhraseReceiver* GetInstance()
	{
		if (!m_Receiver)
		{
			m_Receiver = new FRecordPhraseReceiver();
		}
		return m_Receiver;
	}

	static void DeleteInstance()
	{
		if (m_Receiver)
		{
			delete m_Receiver;
			m_Receiver = nullptr;
		}
	}

	FOnRecordPhraseRecive OnRecordPhraseRecive;

	FString m_RecordedPhrase;

private:

	FRecordPhraseReceiver()
	{
		m_RecordedPhrase = "";
	}

	~FRecordPhraseReceiver()
	{

	}

	static FRecordPhraseReceiver* m_Receiver;

};

FRecordPhraseReceiver* FRecordPhraseReceiver::m_Receiver = nullptr;