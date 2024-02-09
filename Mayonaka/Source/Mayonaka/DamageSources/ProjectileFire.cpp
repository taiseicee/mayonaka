// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileFire.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"

// Sets default values
AProjectileFire::AProjectileFire() {
	PrimaryActorTick.bCanEverTick = true;
	SphereHitBox = CreateDefaultSubobject<USphereComponent>(TEXT("Hit Box"));
	Fire = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Fire"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->MaxSpeed = 900.f;
	ProjectileMovement->InitialSpeed = 900.f;

	RootComponent = SphereHitBox;
	Fire->SetupAttachment(RootComponent);
}

void AProjectileFire::BeginPlay() {
	Super::BeginPlay();
	SphereHitBox->OnComponentHit.AddDynamic(this, &AProjectileFire::OnHit);
	// SphereHitBox->OnComponentBeginOverlap.AddDynamic(this, &AProjectileFire::OnOverlapBegin);
}

void AProjectileFire::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AProjectileFire::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& HitResult) {
	UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *OtherActor->GetName());
}

// void AProjectileFire::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool IsFromSweep, const FHitResult& SweepResult) {
// 	UE_LOG(LogTemp, Warning, TEXT("Fire > Overlap"));
// }
