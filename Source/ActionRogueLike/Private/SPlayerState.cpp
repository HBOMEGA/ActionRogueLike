// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

ASPlayerState::ASPlayerState()
{
	PlayerCredit = 0;
}

int32 ASPlayerState::GetPlayerCredit()
{
	return PlayerCredit;
}

void ASPlayerState::ApplyCreditChange(AActor* InstigatedActor, int32 CreditValue)
{
	if ( CreditValue == 0 )
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor: %s, Gave %i Credits"), *GetNameSafe(InstigatedActor), CreditValue);
		return;
	}	

	int32 NewCredit =  PlayerCredit + CreditValue;
	if ( NewCredit < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyCreditChange: %s Does Not Have Enough Credit: %i!"), *GetNameSafe(InstigatedActor), PlayerCredit );
		return;
	}

	PlayerCredit += CreditValue;
	

	OnCreditChanged.Broadcast( InstigatedActor, PlayerCredit );
	
	UE_LOG(LogTemp, Warning, TEXT(" %s has %i Credits"), *GetNameSafe(InstigatedActor), PlayerCredit );
}


