// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FOnCreditChanged,  AActor*, InstigatedActor, int32, PCredit);


UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ASPlayerState();

	UFUNCTION(BlueprintCallable, Category= "Credit")
	int32 GetPlayerCredit();
	
	UFUNCTION(BlueprintCallable, Category= "Credit")
	void ApplyCreditChange( AActor* InstigatedActor, int32 CreditValue );

	UPROPERTY(BlueprintAssignable)
	FOnCreditChanged OnCreditChanged;

protected:
	UPROPERTY(VisibleAnywhere, Category= "Credit")
	int32 PlayerCredit;
};
