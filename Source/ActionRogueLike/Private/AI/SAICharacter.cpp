// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
 	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));
	AttribComp = CreateDefaultSubobject<USAttributeComponent>(TEXT(" Attribute Component"));
	ActionComp = CreateDefaultSubobject<USActionComponent>(TEXT("Action Component"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	TimeToHitParamName = TEXT("TimeToHit");
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttribComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged );
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
}

void ASAICharacter::SetTargetActor(AActor* NewTargetActor)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if( AIC )
	{
		AIC->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), NewTargetActor );				
	}
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
                                    float MaximumHealth, float Delta)
{
	
	if (Delta < 0.0f )
	{
				
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);			
		}
		
		if (ActiveHealthBar == nullptr )
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass );
			if( ActiveHealthBar )
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}
		
		GetMesh()->SetScalarParameterValueOnMaterials( TimeToHitParamName, GetWorld()->TimeSeconds );

		// Died
		if (NewHealth <= 0.0f)
		{
			// Stop BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if( AIC )
			{
				AIC->GetBrainComponent()->StopLogic(TEXT("Killed"));
			}

			// Ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			// Set Lifespan
			SetLifeSpan(10.0f);			
		}		
	}
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	DrawDebugString(GetWorld(), GetActorLocation(), TEXT("Player Spotted"), nullptr, FColor::White, 4.0f, true );	
}



