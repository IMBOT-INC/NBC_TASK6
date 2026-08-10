// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGameModeBase.h"

#include "EngineUtils.h"
#include "TaskGameState.h"
#include "task6/Player/TaskPlayerController.h"
#include "task6/Player/TaskPlayerState.h"


void ATaskGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SecretNumberString = GenerateSecretNumber();
	UE_LOG(LogTemp, Warning, TEXT("SecretNumberString: %s"), *SecretNumberString);
}

void ATaskGameModeBase::PrintChatMessageString(ATaskPlayerController* InChattingPlayerController,
		const FString& InChatMessageString)
{
	const FString GuessNumberString = InChatMessageString.Right(3);
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		const FString JudgeResultString = JudgeResult(GuessNumberString, SecretNumberString);
		UE_LOG(LogTemp, Warning, TEXT("Secret: [%s], Guess: [%s], Result: [%s]"),
			*SecretNumberString, *GuessNumberString, *JudgeResultString);
		IncreaseGuessCount(InChattingPlayerController);
		const int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));

		for (TActorIterator<ATaskPlayerController> It(GetWorld()); It; ++It)
		{
			ATaskPlayerController* TaskPlayerController = *It;
			if (IsValid(TaskPlayerController) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				TaskPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);
			}
		}

		JudgeGame(InChattingPlayerController, StrikeCount);
	}
	else
	{
		for (TActorIterator<ATaskPlayerController> It(GetWorld()); It; ++It)
		{
			ATaskPlayerController* TaskPlayerController = *It;
			if (IsValid(TaskPlayerController) == true)
			{
				TaskPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}

void ATaskGameModeBase::IncreaseGuessCount(ATaskPlayerController* InChattingPlayerController)
{
	ATaskPlayerState* TaskPS = InChattingPlayerController->GetPlayerState<ATaskPlayerState>();
	if (IsValid(TaskPS) == true)
	{
		TaskPS->CurrentGuessCount++;
	}
}

void ATaskGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();
	UE_LOG(LogTemp, Warning, TEXT("SecretNumberString: %s"), *SecretNumberString);
	for (const auto& TaskPlayerController : AllPlayerControllers)
	{
		ATaskPlayerState* TaskPS = TaskPlayerController->GetPlayerState<ATaskPlayerState>();
		if (IsValid(TaskPS) == true)
		{
			TaskPS->CurrentGuessCount = 0;
		}
	}
}

void ATaskGameModeBase::JudgeGame(ATaskPlayerController* InChattingPlayerController,
		int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		ATaskPlayerState* TaskPS = InChattingPlayerController->GetPlayerState<ATaskPlayerState>();
		if (IsValid(TaskPS) == true)
		{
			const FString CombinedMessageString = TaskPS->PlayerNameString + TEXT(" has won the game.");
			for (const auto& TaskPlayerController : AllPlayerControllers)
			{
				if (IsValid(TaskPlayerController) == true)
				{
					TaskPlayerController->NotificationText = FText::FromString(CombinedMessageString);
				}
			}

			ResetGame();
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& TaskPlayerController : AllPlayerControllers)
		{
			ATaskPlayerState* TaskPS = TaskPlayerController->GetPlayerState<ATaskPlayerState>();
			if (IsValid(TaskPS) == true)
			{
				if (TaskPS->CurrentGuessCount < TaskPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const auto& CXPlayerController : AllPlayerControllers)
			{
				if (IsValid(CXPlayerController) == true)
				{
					CXPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));
				}
			}

			ResetGame();
		}
	}
}
	
void ATaskGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	{
		// 	Super::OnPostLogin(NewPlayer);
		//
		// 	ATaskGameState* CXGameStateBase =  GetGameState<ATaskGameState>();
		// 	if (IsValid(CXGameStateBase) == true)
		// 	{
		// 		CXGameStateBase->MulticastRPCBroadcastLoginMessage(TEXT("XXXXXXX"));
		// 	}
		// }
		// ATaskPlayerController* CXPlayerController = Cast<ATaskPlayerController>(NewPlayer);
		// if (IsValid(CXPlayerController) == true)
		// {
		// 	AllPlayerControllers.Add(CXPlayerController);
		// }
		ATaskPlayerController* TaskPlayerController = Cast<ATaskPlayerController>(NewPlayer);
		if (IsValid(TaskPlayerController) == true)
		{
			AllPlayerControllers.Add(TaskPlayerController);

			ATaskPlayerState* CXPS = TaskPlayerController->GetPlayerState<ATaskPlayerState>();
			if (IsValid(CXPS) == true)
			{
				TaskPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
				CXPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
			}

			ATaskGameState* CXGameStateBase =  GetGameState<ATaskGameState>();
			if (IsValid(CXGameStateBase) == true)
			{
				CXGameStateBase->MulticastRPCBroadcastLoginMessage(CXPS->PlayerNameString);
			}
		}
	}
}

	
FString ATaskGameModeBase::GenerateSecretNumber()
{
	TArray<int32> NumberArray;
	for (int32 i = 1; i <= 9; ++i)
	{
		NumberArray.Add(i);
	}

	FString NewSecretNumberString;

	for (int32 i = 0; i < 3; ++i)
	{
		const int32 Index = FMath::RandRange(0, NumberArray.Num() - 1);
		NewSecretNumberString.Append(FString::FromInt(NumberArray[Index]));
		NumberArray.RemoveAt(Index);
	}

	return NewSecretNumberString;
}

bool ATaskGameModeBase::IsGuessNumberString(const FString& InGuessNumberString)
{
	bool bCanPlay = false;

	do {

		if (InGuessNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InGuessNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			if (UniqueDigits.Contains(C) == true)
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;
		
	} while (false);	

	return bCanPlay;
}

FString ATaskGameModeBase::JudgeResult(const FString& InGuessNumberString,
		const FString& InSecretNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else 
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;				
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}
