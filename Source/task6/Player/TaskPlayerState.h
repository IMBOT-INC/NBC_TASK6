// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TaskPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TASK6_API ATaskPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ATaskPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UPROPERTY(Replicated)
	FString PlayerNameString;

	UPROPERTY(Replicated)
		int32 CurrentGuessCount;
	UPROPERTY(Replicated)
		int32 MaxGuessCount;
	
	FString GetPlayerInfoString();
	
};
