// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// #include "SAction.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ASCharacter();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category= "Damage");
	TSubclassOf<class USActionEffect> ThornActionClass;

	// UPROPERTY()
	// class USEffect_Thorn* Thorn;

	UPROPERTY(EditAnywhere, Category= "Rage")
	float SPAttackRageCost;
	
	UPROPERTY(VisibleAnywhere, Category= "Effects")
	FName TimeToHitParamName;
	
	UPROPERTY(EditAnywhere, Category= "Attack")
	UAnimMontage* SpAttackAnim;

	UPROPERTY(EditAnywhere, Category= "Attack")
	UAnimMontage* TeleportAnim;

	UPROPERTY(EditAnywhere, Category= "Attack")
	UAnimMontage* AttackAnim;
		
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArmComp;
	
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	class USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	class USAttributeComponent* AttribComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	class USActionComponent* ActionComp;
	
	
	void MoveForward(float Value);
	
	void MoveRight(float Value);
	

	void SprintStart();

	void SprintStop();

	// void ThornBuff();

	void PrimaryAttack();

	void SpecialAttack();

	void TeleportAttack();
	
	void PrimaryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float MaximumHealth, float Delta );

	 UFUNCTION()
	 void OnRageChanged(AActor* InstigatedActor, USAttributeComponent* OwningComp, float NewRage, float MaximumRage, float Delta);

	virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override; 
	
	
public:	
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf( float Amount = 100.0f );

};
