// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"



// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("Interaction Component"));

	AttribComp = CreateDefaultSubobject<USAttributeComponent>(TEXT(" Attribute Component"));
	

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttribComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged );
}


// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("PrimaryAttack"), IE_Pressed, this, &ASCharacter::PrimaryAttack );
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump );
	PlayerInputComponent->BindAction(TEXT("PrimaryInteract"), IE_Pressed, this, &ASCharacter::PrimaryInteract );
	PlayerInputComponent->BindAction(TEXT("SpecialAttack"), IE_Pressed, this, &ASCharacter::SpecialAttack );
	PlayerInputComponent->BindAction(TEXT("Teleport"),IE_Pressed, this, &ASCharacter::Teleport );

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASCharacter::MoveForward );
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASCharacter::MoveRight );
	
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput );
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput );

}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	AddMovementInput( ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput( RightVector, Value);
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, PrimaryAttackRate );	
	
}

void ASCharacter::SpecialAttack()
{
	PlayAnimMontage(SpAttackAnim);
	
	GetWorldTimerManager().SetTimer(TimerHandle_SpAttack, this, &ASCharacter::SpAttack_TimeElapsed, PrimaryAttackRate );
}

void ASCharacter::Teleport()
{
	PlayAnimMontage(TeleportAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ASCharacter::Teleport_TimeElapsed, PrimaryAttackRate );
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass);
}

void ASCharacter::SpAttack_TimeElapsed()
{
	SpawnProjectile(SpProjectileClass);
}

void ASCharacter::Teleport_TimeElapsed()
{
	SpawnProjectile(TeleportProjectileClass);
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		float SweepRadius = 20.0f;
		int32 SweepDistanceFallback = 5000;
		
		FCollisionShape Shape;
		Shape.SetSphere(SweepRadius);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
		
		// FVector Location;
		// FRotator Rotation;
		// GetController()->GetPlayerViewPoint(Location, Rotation) ;    
		
		FVector TraceDirection = GetControlRotation().Vector();
		FVector TraceStart = CameraComp->GetComponentLocation() + ( TraceDirection * SweepRadius); // GetPawnViewLocation();
		FVector TraceEnd =  TraceStart + ( TraceDirection * SweepDistanceFallback);

		FHitResult Hit;

		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape, QueryParams))
		{
			TraceEnd = Hit.ImpactPoint;
		}
		// find new direction/location from hand pointing to impact point in world
		FRotator ProjRotation = (TraceEnd - HandLocation).Rotation();
		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		
		GetWorld()->SpawnActor<AActor>( ClassToSpawn,  SpawnTM, SpawnParams);
	}	
}

void ASCharacter::PrimaryInteract()
{
	if ( InteractionComp )
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if ( NewHealth <= 0.0f && Delta <= 0.0f )
	{
		APlayerController* Pc = Cast<APlayerController>(GetController() );
		DisableInput(Pc);
		
	}
}
