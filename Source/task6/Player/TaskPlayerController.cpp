// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "task6/task6.h"
#include "task6/UI/TaskChatInput.h"

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
	PrintChatMessageString(ChatMessageString);

}

void ATaskPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	
	// FString NetModeString = Task6FunctionLibrary::GetNetModeString(this);
	// FString CombinedMessageString = FString::Printf(TEXT("%s: %s"), *NetModeString, *InChatMessageString);
		Task6FunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);

}
