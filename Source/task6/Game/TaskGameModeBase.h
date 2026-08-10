// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TaskGameModeBase.generated.h"

class ATaskPlayerController;
/**
 * 
 */
UCLASS()
class TASK6_API ATaskGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;	

	FString GenerateSecretNumber();
	FString JudgeResult(const FString& InGuessNumberString, const FString& InSecretNumberString);
	
	bool IsGuessNumberString(const FString& InGuessNumberString);
	
	void PrintChatMessageString(ATaskPlayerController * InChattingPlayerController, const FString& 
	InChatMessageString);
	void IncreaseGuessCount(ATaskPlayerController* InChattingPlayerController);

	void ResetGame();

	void JudgeGame(ATaskPlayerController* InChattingPlayerController, int InStrikeCount);

	

protected:
	FString SecretNumberString;
	TArray<TObjectPtr<ATaskPlayerController>> AllPlayerControllers;
	
	
};
