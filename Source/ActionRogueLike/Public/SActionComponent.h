// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SActionEffect.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USActionComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Tags")
	FGameplayTagContainer ActiveGameplayTags; 

	UFUNCTION(BlueprintCallable, Category= "Action")
	void AddAction( AActor* InstigatorActor, TSubclassOf<class USAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category= "Action")
	bool StartActionByName( AActor* InstigatorActor, FName ActionName );

	UFUNCTION(BlueprintCallable, Category= "Action")
	bool StopActionByName( AActor* InstigatorActor, FName ActionName );

	UFUNCTION(BlueprintCallable, Category= "Action")
	void RemoveAction(USAction* ActionToRemove);

protected:
	UPROPERTY(EditAnywhere, Category= "Action")
	TArray<TSubclassOf<USAction>> DefaultActions;

	UPROPERTY()
	TArray< USAction*> Actions;
	
	
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
