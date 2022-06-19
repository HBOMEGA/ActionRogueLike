// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect.h"

#include "SActionComponent.h"

USActionEffect::USActionEffect()
{
	bAutoStart = true;
}

void USActionEffect::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);
	if (Duration > 0.0f )
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", InstigatorActor );

		GetWorld()->GetTimerManager().SetTimer(Duration_Handle, Delegate, Duration, false );
	}

	if (Period > 0.0f )
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodEffect", InstigatorActor );

		GetWorld()->GetTimerManager().SetTimer(Period_Handle, Delegate, Period, true );
		
	}
}

void USActionEffect::StopAction_Implementation(AActor* InstigatorActor)
{
	if ( GetWorld()->GetTimerManager().GetTimerRemaining(Period_Handle) < KINDA_SMALL_NUMBER )
	{
		ExecutePeriodEffect( InstigatorActor );
	}
	
	Super::StopAction_Implementation(InstigatorActor);

	GetWorld()->GetTimerManager().ClearTimer(Period_Handle );
	GetWorld()->GetTimerManager().ClearTimer(Duration_Handle );

	USActionComponent* Comp = GetOwningComponent();
	if ( Comp )
	{
		Comp->RemoveAction(this);
	}
}

void USActionEffect::ExecutePeriodEffect_Implementation(AActor* Instigator)
{
}
