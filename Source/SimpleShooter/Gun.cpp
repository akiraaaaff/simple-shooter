// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = Root;
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void AGun::PullTrigger()
{
	FVector ShotDirection;
	FHitResult HitResult;
	
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	bool bTraceSuccess = GunTrace(HitResult, ShotDirection);

	if (bTraceSuccess)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.Location, ShotDirection.Rotation(), true);
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, HitResult.Location);
		if (HitResult.GetActor())
		{
			AController* OwnerController = GetOwnerController();
			if (OwnerController == nullptr) return;
			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			HitResult.GetActor()->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}		
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult& HitResult, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) return false;

	FVector ViewPointLocation;
	FRotator ViewPointRotation;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	OwnerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);
	ShotDirection = -ViewPointRotation.Vector();
	FVector End = ViewPointLocation + ViewPointRotation.Vector() * MaxRange;

	return GetWorld()->LineTraceSingleByChannel(HitResult, ViewPointLocation, End, ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;

	return OwnerPawn->GetController();	
}

