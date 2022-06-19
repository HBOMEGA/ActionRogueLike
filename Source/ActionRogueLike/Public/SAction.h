// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category= "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintNativeEvent, Category= "Action")
	bool IsRunning() const;
	
	UFUNCTION(BlueprintNativeEvent, Category= "Action")
	bool CanStart(AActor* InstigatorActor);

	UFUNCTION(BlueprintNativeEvent, Category= "Action")
	void StartAction(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category= "Action")
	void StopAction(AActor* InstigatorActor);

	UPROPERTY(EditDefaultsOnly, Category= "Action")
	FName ActionName;

	virtual UWorld* GetWorld() const override;

protected:

	bool bIsRunning { false };

	UFUNCTION(BlueprintCallable, Category= "Action")
	class USActionComponent* GetOwningComponent() const;

	UPROPERTY(EditDefaultsOnly, Category= "Tags")
	FGameplayTagContainer GrantTags;

	UPROPERTY(EditDefaultsOnly, Category= "Tags")
	FGameplayTagContainer BlockedTags;
	
};
