// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_LowHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USBTService_LowHealth::USBTService_LowHealth()
{
	LowHealthAmount = 0.3f;
}

void USBTService_LowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if ( ensure(AIPawn) )
	{
		USAttributeComponent* AttribComp = Cast<USAttributeComponent>( AIPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
		if ( ensure(AttribComp) )
		{
			bLowHealth = AttribComp->GetHealth() / AttribComp->GetMaxHealth() < LowHealthAmount;

			UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
			BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bLowHealth);
		}
	}
				
}
