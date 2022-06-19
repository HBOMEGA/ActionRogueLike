// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"

#include "SAttributeComponent.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{ 	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh Comp"));
	RootComponent = MeshComp;

	AttribComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("Attribute Comp"));
}

void ASTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	AttribComp->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChanged);
	
}

void ASTargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
                                    float MaximumHealth, float Delta)
{
	if (Delta < 0.0f )
	{
		MeshComp->SetScalarParameterValueOnMaterials( TEXT("TimeToHit"), GetWorld()->TimeSeconds );
	}	
}






