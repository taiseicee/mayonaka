// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileFire.h"
#include "NiagaraComponent.h"

// Sets default values
AProjectileFire::AProjectileFire() {
	PrimaryActorTick.bCanEverTick = true;
	Fire = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Fire"));
	RootComponent = Fire;
}

void AProjectileFire::BeginPlay() {
	Super::BeginPlay();
	
}

void AProjectileFire::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

