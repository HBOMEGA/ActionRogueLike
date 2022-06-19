// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"

#include "SAction.h"


USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;	
}

void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for ( TSubclassOf<USAction> ActionClass : DefaultActions)
	{
		AddAction( GetOwner(), ActionClass);
	}
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage( -1, 0.0f, FColor::White, DebugMsg );
}

void USActionComponent::AddAction(AActor* InstigatorActor, TSubclassOf<USAction> ActionClass)
{
	if ( !ensure(ActionClass ) )
	{
		//UE_LOG(LogTemp, Warning, TEXT("AddAction: No Action Class Found!"))
		return;
	}
	USAction* NewAction = NewObject<USAction>(this, ActionClass );
	if ( ensure(NewAction) )
	{
		Actions.Add(NewAction);

		if ( NewAction->bAutoStart && ensure(NewAction->CanStart(InstigatorActor)) )
		{
			NewAction->StartAction(InstigatorActor);
		}
	}
}

bool USActionComponent::StartActionByName(AActor* InstigatorActor, FName ActionName)
{
	for (USAction* Action : Actions )
	{
		if ( Action && Action->ActionName == ActionName )
		{
			if ( !Action->CanStart(InstigatorActor))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed To Run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg );
				continue;				
			}
			Action->StartAction(InstigatorActor);
			return true;
		}
	}
	return false;
}

bool USActionComponent::StopActionByName(AActor* InstigatorActor, FName ActionName)
{
	for (USAction* Action : Actions )
	{
		if ( Action && Action->ActionName == ActionName )
		{
			if( Action->IsRunning())
			{
				Action->StopAction(InstigatorActor);
				return true;
			}
			
		}
	}
	return false;
}

void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if ( !ensure(ActionToRemove && !ActionToRemove->IsRunning()) )
	{
		return;
	}
		
	Actions.Remove(ActionToRemove);
}
