// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplodingBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
ASExplodingBarrel::ASExplodingBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh Component"));
	RootComponent = MeshComp;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce Component"));
	ForceComp->SetupAttachment(MeshComp);
	ForceComp->SetAutoActivate(false);

	ForceComp->Radius = 750.0f;
	ForceComp->ImpulseStrength = 2000.0f;
	// Optional Ignores 'Mass' of other objects, (if false will take objects mass into consideration
	ForceComp->bImpulseVelChange = true;

	// Optional, default constructor of component already adds 4 object types to effects, excluding WorldDynamic
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);

}

// Called when the game starts or when spawned
void ASExplodingBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASExplodingBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MeshComp->OnComponentHit.AddDynamic( this, &ASExplodingBarrel::OnActorHit);
}

void ASExplodingBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();

	UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explosive Barrel!"));
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, At Game Time: %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);
	UE_LOG(LogTemp, Warning, TEXT("OtherComp: %s, At Game Time: %f"), *GetNameSafe(OtherComp), GetWorld()->TimeSeconds);

	FString CombineString = FString::Printf(TEXT("Hit at Location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, TEXT("What the fuck"), nullptr, FColor::Emerald, 5.0f, true, 3.0f);
}


