// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(EditAnywhere, Category= "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category= "Attack")
	TSubclassOf<AActor> SpProjectileClass;

	UPROPERTY(EditAnywhere, Category= "Attack")
	TSubclassOf<AActor> TeleportProjectileClass;

	UPROPERTY(EditAnywhere, Category= "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category= "Attack")
	UAnimMontage* SpAttackAnim;

	UPROPERTY(EditAnywhere, Category= "Attack")
	UAnimMontage* TeleportAnim;
	
	UPROPERTY(EditAnywhere)
	float PrimaryAttackRate = 0.2f;
	
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArmComp;
	
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	class USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	class USAttributeComponent* AttribComp;

	FTimerHandle TimerHandle_PrimaryAttack;

	FTimerHandle TimerHandle_SpAttack;

	FTimerHandle TimerHandle_Teleport;


	

	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void MoveForward(float Value);
	
	void MoveRight(float Value);

	void PrimaryAttack();

	void SpecialAttack();

	void Teleport();

	void PrimaryAttack_TimeElapsed();

	void SpAttack_TimeElapsed();

	void Teleport_TimeElapsed();

	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

	void PrimaryInteract();

	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
