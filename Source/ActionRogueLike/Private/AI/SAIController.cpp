// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"

// #include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();
	if (ensureMsgf(BehaviorTree, TEXT("BehaviorTree is null pointer! Please assign BehaviorTree in your AI Controller. ")))
	{
		RunBehaviorTree(BehaviorTree);
	}
	

	// APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	// if (MyPawn)
	// {
	// 	GetBlackboardComponent()->SetValueAsVector(TEXT("MoveToLocation"), MyPawn->GetActorLocation() );
	//
	// 	GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), MyPawn );
	// }
}
