// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskPlayerController.h"

#include "EngineUtils.h"
#include "TaskPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "task6/task6.h"
#include "task6/Game/TaskGameModeBase.h"
#include "task6/UI/TaskChatInput.h"

class ATaskPlayerState;

void ATaskPlayerController::test()
{
	return;
}

void ATaskPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController() == false)
	{
		return;
	}
	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
	
	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);
	
	if (IsValid(ChatInputWidgetClass) == true ){
		ChatInputWidgetInstance = CreateWidget <UTaskChatInput>(this,ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true ){
			ChatInputWidgetInstance->AddToViewport();
		}
	}
}

void ATaskPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	if (IsLocalController() == true)
	{
		ServerRPCPrintChatMessageString(InChatMessageString);
	}
}

void ATaskPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	
	// FString NetModeString = Task6FunctionLibrary::GetNetModeString(this);
	// FString CombinedMessageString = FString::Printf(TEXT("%s: %s"), *NetModeString, *InChatMessageString);
		Task6FunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);

}

void ATaskPlayerController::GetLifetimeReplicatedProps(
		TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, NotificationText);
}


void ATaskPlayerController::ServerRPCPrintChatMessageString_Implementation(
		const FString& InChatMessageString)
{
	// for (TActorIterator<ACXPlayerController> It(GetWorld()); It; ++It)
	// {
	// 	ACXPlayerController* CXPlayerController = *It;
	// 	if (IsValid(CXPlayerController) == true)
	// 	{
	// 		CXPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
	// 	}
	// }

	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		ATaskGameModeBase* TaskGM = Cast<ATaskGameModeBase>(GM);
		if (IsValid(TaskGM) == true)
		{
			TaskGM->PrintChatMessageString(this, InChatMessageString);
		}
	}


}

void ATaskPlayerController::ClientRPCPrintChatMessageString_Implementation(
		const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}
