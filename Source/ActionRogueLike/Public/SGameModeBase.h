// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"


static TAutoConsoleVariable<bool>CVarSpawnBots( TEXT("su.SpawnBots"), true, TEXT("Enable Spawning of Bots VIA Timer!"), ECVF_Cheat );


UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category= "Credits")
	int32 AIKillCredit;
	
	UPROPERTY(EditDefaultsOnly, Category= "AI")
	TSubclassOf<AActor> MinionClass;
		
	UPROPERTY(EditDefaultsOnly, Category= "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category= "AI")
	UEnvQuery* SpawnPUQuery;

	UPROPERTY(EditDefaultsOnly, Category= "Pickup")
	TArray<TSubclassOf<AActor>> PickUpClasses; 

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category= "AI")
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, Category= "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditAnywhere, Category= "Pickup")
	int32 DesiredPickUpCount;

	UPROPERTY(EditAnywhere, Category= "Pickup")
	float RequiredPickUpDistance;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnQueryCompleted( class UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus );

	UFUNCTION()
	void OnPuSpawnQueryCompleted( UEnvQueryInstanceBlueprintWrapper* PUQueryInstance, EEnvQueryStatus::Type PUQueryStatus );

	UFUNCTION()
	void RespawnPlayerElapsed( AController* Controller);

public:
	ASGameModeBase();

	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void KillEmAll();

	virtual void OnActorKilled( AActor* VictimActor, AActor* Killer );

	// virtual void OnAIKilled( AActor* VictimActor, AActor* Killer );
};
