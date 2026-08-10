// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TaskPlayerController.generated.h"

class UTaskChatInput;
/**
 * 
 */
UCLASS()
class TASK6_API ATaskPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void test();
	virtual void BeginPlay() override;
	void SetChatMessageString(const FString& InChatMessageString);
	void PrintChatMessageString(const FString& InChatMessageString);
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	
	UFUNCTION(Client, Reliable)
		void ClientRPCPrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Server, Reliable)
		void ServerRPCPrintChatMessageString(const FString& InChatMessageString);
public:
	UPROPERTY(Replicated, BlueprintReadOnly)
		FText NotificationText;
	
protected:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UTaskChatInput> ChatInputWidgetClass;
	UPROPERTY()
		TObjectPtr<UTaskChatInput> ChatInputWidgetInstance;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUserWidget> NotificationTextWidgetClass;
	UPROPERTY()
		TObjectPtr<UUserWidget> NotificationTextWidgetInstance;
	
protected:
	FString ChatMessageString;

	

	
};
