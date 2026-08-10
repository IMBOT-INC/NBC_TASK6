// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGameState.h"

#include "Kismet/GameplayStatics.h"
#include "task6/Player/TaskPlayerController.h"


void ATaskGameState::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			ATaskPlayerController* CXPC = Cast<ATaskPlayerController>(PC);
			if (IsValid(CXPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				CXPC->PrintChatMessageString(NotificationString);
			}
		}
		
	}
}
