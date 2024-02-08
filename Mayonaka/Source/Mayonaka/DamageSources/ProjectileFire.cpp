// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileFire.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectileFire::AProjectileFire() {
	PrimaryActorTick.bCanEverTick = true;
	Fire = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Fire"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->MaxSpeed = 900.f;
	ProjectileMovement->InitialSpeed = 900.f;

	RootComponent = Fire;
}

void AProjectileFire::BeginPlay() {
	Super::BeginPlay();
	
}

void AProjectileFire::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

