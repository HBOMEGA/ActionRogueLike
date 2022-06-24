// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"


#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"


ASGameModeBase::ASGameModeBase()
{
	PlayerStateClass = ASPlayerState::StaticClass();
	SpawnTimerInterval = 2.0f;
	AIKillCredit = 1;
	DesiredPickUpCount = 10;
	RequiredPickUpDistance = 2000.0f;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	if( ensure(PickUpClasses.Num() > 0) )
	{
		UEnvQueryInstanceBlueprintWrapper* PUSpawnQueryInst = UEnvQueryManager::RunEQSQuery(this, SpawnPUQuery, this, EEnvQueryRunMode::AllMatching, nullptr );
        if (ensure(PUSpawnQueryInst))
        {
        	PUSpawnQueryInst->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnPuSpawnQueryCompleted );
        }
	}

	
}

void ASGameModeBase::KillEmAll()
{
	for ( TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttribComp = USAttributeComponent::GetAttributes(Bot);
		if (AttribComp && AttribComp->bIsAlive())
		{
			AttribComp->KillEm(this); // Credit for the kill goes to GameMode for now
		}
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{

	if ( !CVarSpawnBots.GetValueOnGameThread() )
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot Spawining Disabled via 'CVarSpawnBots'. "));
		return;
	}
	int32 NumberOfAliveBots = 0;
    for ( TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
    {
    	ASAICharacter* Bot = *It;

    	USAttributeComponent* AttribComp = USAttributeComponent::GetAttributes(Bot);
    	if (AttribComp && AttribComp->bIsAlive())
    	{
    		NumberOfAliveBots++;
    	}
    }

	UE_LOG(LogTemp, Warning, TEXT("Found %i Alive Bots!"), NumberOfAliveBots);

	float MaxBotsCount = 10.0f;
	if (DifficultyCurve)
	{
		MaxBotsCount = DifficultyCurve->GetFloatValue( GetWorld()->TimeSeconds );
	}
	
	if (NumberOfAliveBots >= MaxBotsCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("At Maximum Bot Capacity. Skipping Bot Spawn!"));
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr );
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}
	
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot EQS Failed!"));
		return;
	}	
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0) )
	{
		GetWorld()->SpawnActor<AActor>( MinionClass, Locations[0], FRotator::ZeroRotator );

		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}

void ASGameModeBase::OnPuSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* PUQueryInstance,
	EEnvQueryStatus::Type PUQueryStatus)
{
	if ( PUQueryStatus != EEnvQueryStatus::Success )
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn PickUp EQS Failed!"));
		return;
	}
	TArray<FVector> PuLocations = PUQueryInstance->GetResultsAsLocations();

	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;
	while (SpawnCounter < DesiredPickUpCount && PuLocations.Num() > 0)
	{
		// Pick a Random Location
		int32 RandLocationIndex = FMath::RandRange(0, PuLocations.Num() -1 );

		FVector PickedLocation = PuLocations[RandLocationIndex];

		// Remove to avoid picking again
		PuLocations.RemoveAt(RandLocationIndex);

		//Check Minimum distance Requirements
		bool bValidLocation = true;
		for ( FVector OtherLocation : UsedLocations )
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();
			if (DistanceTo < RequiredPickUpDistance )
			{
				 // Too Close Try Next Location, Lets still draw a Red Debug Sphere
				DrawDebugSphere(GetWorld(), PickedLocation, 50.0f, 20, FColor::Red, false, 3.0f);

				// Lets Skip to the next attempt
				bValidLocation = false;
				break;
			}
		}

		// failed the distance test
		if (!bValidLocation )
		{
			continue;
		}

		int32 RandomClassIndex = FMath::RandRange(0, PickUpClasses.Num() -1);
		TSubclassOf<AActor> RandomPickupClass = PickUpClasses[RandomClassIndex];
		
		GetWorld()->SpawnActor<AActor>( RandomPickupClass, PickedLocation, FRotator::ZeroRotator );

		// Keep for Distance Check
		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction( this, TEXT("RespawnPlayerElapsed"), Player->GetController() );
		float RespawnDelay = 2.0f;
		

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false );
	}
	UE_LOG(LogTemp, Warning, TEXT("OnActorKilled: Vicitim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer) );

	// Apply Credit Change

	Player = Cast<ASCharacter>(Killer);
	if (Player)
	{		
		if (ASPlayerState* PlayerState = Player->GetInstigatorController()->GetPlayerState<ASPlayerState>())
		{
			PlayerState->ApplyCreditChange(Killer, AIKillCredit );
		}
	}	
	
}


void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if ( ensure(Controller) )
	{
		Controller->UnPossess();
		
		RestartPlayer( Controller );
	}
	
}

