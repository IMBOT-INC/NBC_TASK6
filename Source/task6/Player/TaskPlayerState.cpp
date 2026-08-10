// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskPlayerState.h"

#include "Net/UnrealNetwork.h"

ATaskPlayerState::ATaskPlayerState():
	PlayerNameString(TEXT(""))
, CurrentGuessCount(0)
, MaxGuessCount(3)
{
	bReplicates = true;
}

void ATaskPlayerState::GetLifetimeReplicatedProps(
		TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ATaskPlayerState, PlayerNameString);
	DOREPLIFETIME(ATaskPlayerState, CurrentGuessCount);
	DOREPLIFETIME(ATaskPlayerState, MaxGuessCount);
}

FString ATaskPlayerState::GetPlayerInfoString()
{
	FString PlayerInfoString = PlayerNameString + TEXT("(") + FString::FromInt(CurrentGuessCount) + TEXT("/") + FString::FromInt(MaxGuessCount) + TEXT(")");
	return PlayerInfoString;

}
