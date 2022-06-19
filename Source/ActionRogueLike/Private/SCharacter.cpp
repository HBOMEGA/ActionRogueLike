// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "EngineUtils.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
ASCharacter::ASCharacter()
{
 	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	
	InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("Interaction Component"));

	AttribComp = CreateDefaultSubobject<USAttributeComponent>(TEXT(" Attribute Component"));

	ActionComp = CreateDefaultSubobject<USActionComponent>(TEXT("Action Component"));
	

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
	TimeToHitParamName = TEXT("TimeToHit");

	AttribComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged );
	
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("PrimaryAttack"), IE_Pressed, this, &ASCharacter::PrimaryAttack );
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump );
	PlayerInputComponent->BindAction(TEXT("PrimaryInteract"), IE_Pressed, this, &ASCharacter::PrimaryInteract );
	PlayerInputComponent->BindAction(TEXT("SpecialAttack"), IE_Pressed, this, &ASCharacter::SpecialAttack );
	PlayerInputComponent->BindAction(TEXT("Teleport"),IE_Pressed, this, &ASCharacter::TeleportAttack );
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ASCharacter::SprintStart );
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ASCharacter::SprintStop );
	// PlayerInputComponent->BindAction(TEXT("Parry"), IE_Pressed, this, &ASCharacter::Parry);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASCharacter::MoveForward );
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASCharacter::MoveRight );
	
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput );
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput );

}

void ASCharacter::HealSelf(float Amount/* =100.0f */ )
{
	AttribComp->ApplyHealthChanges(this, Amount);
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

void ASCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, TEXT("Sprint"));
}

void ASCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, TEXT("Sprint"));
}

void ASCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, TEXT("PrimaryAttack"));
}

void ASCharacter::SpecialAttack()
{
	ActionComp->StartActionByName(this, TEXT("SpecialAttack"));
}

void ASCharacter::TeleportAttack()
{
	ActionComp->StartActionByName(this, TEXT("TeleportAttack"));
}

void ASCharacter::PrimaryInteract()
{
	if ( InteractionComp )
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float MaximumHealth, float Delta)
{
	if (Delta < 0.0f )
	{
		GetMesh()->SetScalarParameterValueOnMaterials( TimeToHitParamName, GetWorld()->TimeSeconds );
	}
	if ( NewHealth <= 0.0f && Delta <= 0.0f )
	{
		APlayerController* Pc = Cast<APlayerController>(GetController() );
		DisableInput(Pc);
		
	}
}
