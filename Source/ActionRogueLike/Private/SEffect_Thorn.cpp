// Fill out your copyright notice in the Description page of Project Settings.


#include "SEffect_Thorn.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"
//#include "SCharacter.h"


USEffect_Thorn::USEffect_Thorn()
{
	Duration = 0.0f;
	Period = 0.0f;

	ThornPercent = 0.3f;
	ActionName = TEXT("ThornBuff");
}


void USEffect_Thorn::OnThornBuff(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float MaximumHealth, float Delta)
{
	if ( InstigatorActor == GetOwningComponent()->GetOwner())
	{
		return;
	}

	USAttributeComponent* Comp = Cast<USAttributeComponent>( InstigatorActor->GetComponentByClass(USAttributeComponent::StaticClass()) );
	if ( Comp )
	{
        	
		float NewDelta = FMath::RoundHalfFromZero( Delta *  ThornPercent );
		Comp->ApplyHealthChanges(InstigatorActor, NewDelta );
	}
	StopAction(GetOwningComponent()->GetOwner() );
}

void USEffect_Thorn::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	USAttributeComponent* Comp = Cast<USAttributeComponent>( InstigatorActor->GetComponentByClass(USAttributeComponent::StaticClass()) );
	if ( Comp )
	{
		Comp->OnHealthChanged.AddDynamic(this, &USEffect_Thorn::OnThornBuff );
				
	}
}

void USEffect_Thorn::StopAction_Implementation(AActor* InstigatorActor)
{
	USAttributeComponent* Comp = Cast<USAttributeComponent>( InstigatorActor->GetComponentByClass(USAttributeComponent::StaticClass()) );
	if ( Comp )
	{
		Comp->OnHealthChanged.RemoveDynamic(this, &USEffect_Thorn::OnThornBuff );				
	}
	
	Super::StopAction_Implementation(InstigatorActor);
	
}

