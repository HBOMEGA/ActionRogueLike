// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWorldUserWidget.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	
	UPROPERTY(EditAnywhere, Category= "UI")
	float LifeTime;
	
	UPROPERTY(VisibleAnywhere, Category= "UI")
	USWorldUserWidget* ActiveHealthBar;
	
	UPROPERTY(EditDefaultsOnly, Category= "UI")
	TSubclassOf< USWorldUserWidget> HealthBarWidgetClass;

	UPROPERTY(VisibleAnywhere, Category= "UI")
	USWorldUserWidget* PlayerSpottedPopup;

	UPROPERTY(EditDefaultsOnly, Category= "UI")
	TSubclassOf< USWorldUserWidget> PlayerSpottedWidgetClass;
	
	
	UPROPERTY(VisibleAnywhere, Category= "Effects")
	FName TimeToHitParamName;
	
	UPROPERTY(EditAnywhere, Category= "Components")
	class UPawnSensingComponent* PawnSensingComp;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	class USAttributeComponent* AttribComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	class USActionComponent* ActionComp;

	
	virtual void PostInitializeComponents() override;

	void SetTargetActor(AActor* NewTargetActor);

	UFUNCTION()
	void RemovePlayerWidget( USWorldUserWidget* WidgetToRemove);
	

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor,class USAttributeComponent* OwningComp, float NewHealth, float MaximumHealth, float Delta );	

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
	
};
