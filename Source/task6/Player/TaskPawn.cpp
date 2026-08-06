// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskPawn.h"

#include "task6/task6.h"


// Sets default values
ATaskPawn::ATaskPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATaskPawn::BeginPlay()
{
	Super::BeginPlay();

	FString NetRoleString = Task6FunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CXPawn::BeginPlay() %s [%s]"), *Task6FunctionLibrary::GetNetModeString(this), *NetRoleString);
	Task6FunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

void ATaskPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FString NetRoleString = Task6FunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CXPawn::PossessedBy() %s [%s]"), *Task6FunctionLibrary::GetNetModeString(this), *NetRoleString);
	Task6FunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

