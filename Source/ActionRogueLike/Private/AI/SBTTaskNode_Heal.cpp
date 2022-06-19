// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTaskNode_Heal.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTaskNode_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	APawn* MyPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (!MyPawn )
	{
		return  EBTNodeResult::Failed;			
	}

	USAttributeComponent* AttribComp = Cast<USAttributeComponent>(MyPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	
	if (!AttribComp)
	{
		return EBTNodeResult::Failed;
	}

	AttribComp->ApplyHealthChanges(MyPawn, AttribComp->GetMaxHealth());
	return  EBTNodeResult::Succeeded;
	
}
