// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

#include "SActionComponent.h"


void USAction::StartAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Running: %s!"), *GetNameSafe(this));

	USActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGameplayTags.AppendTags(GrantTags);
	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* InstigatorActor)
{
	
	UE_LOG(LogTemp, Warning, TEXT("Running: %s!"), *GetNameSafe(this));

	ensureAlways( bIsRunning );
	
	USActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGameplayTags.RemoveTags(GrantTags);

	bIsRunning = false;
}

bool USAction::CanStart_Implementation(AActor* InstigatorActor)
{
	if (IsRunning())
	{
		return false;
	}
	
	USActionComponent* Comp = GetOwningComponent();
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

bool USAction::IsRunning_Implementation() const
{
	return bIsRunning;
}

UWorld* USAction::GetWorld() const
{
	// Outer was Set when creating action via NewObject<T>
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter() );
	if ( Comp )
	{
		return Comp->GetWorld();
	}
	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}
