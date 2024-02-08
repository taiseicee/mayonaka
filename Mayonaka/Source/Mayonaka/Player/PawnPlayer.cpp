// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "PaperFlipbookComponent.h"
#include "../DamageSources/ProjectileFire.h"
#include "TimerManager.h"

APawnPlayer::APawnPlayer() {
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	Flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	SpringArmCamera = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	
	RootComponent = CapsuleCollider;
	Flipbook->SetupAttachment(RootComponent);
	ProjectileSpawnPoint->SetupAttachment(Flipbook);
	SpringArmCamera->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArmCamera);

}

void APawnPlayer::BeginPlay() {
	Super::BeginPlay();
	

	// If PlayerController is null, its likely that the pawn is not possessed
	verify((PlayerController = Cast<APlayerController>(GetController())) != nullptr);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
		Subsystem->AddMappingContext(MappingContextPlayer, 0);
	}

	MaxSpeed = MaxWalkSpeed;
}

void APawnPlayer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	Move(DeltaTime);
}

void APawnPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(ActionMove, ETriggerEvent::Triggered, this, &APawnPlayer::HandleInputMove);
		EnhancedInputComponent->BindAction(ActionRun, ETriggerEvent::Triggered, this, &APawnPlayer::HandleRunState);
		EnhancedInputComponent->BindAction(ActionAttack, ETriggerEvent::Triggered, this, &APawnPlayer::HandleAttack);
		EnhancedInputComponent->BindAction(ActionHarmonizeFire, ETriggerEvent::Triggered, this, &APawnPlayer::HandleHarmonizeFire);
		EnhancedInputComponent->BindAction(ActionHarmonizeGrass, ETriggerEvent::Triggered, this, &APawnPlayer::HandleHarmonizeGrass);
		EnhancedInputComponent->BindAction(ActionHarmonizeWater, ETriggerEvent::Triggered, this, &APawnPlayer::HandleHarmonizeWater);
	}
}

void APawnPlayer::HandleInputMove(const FInputActionValue& Value) {
	const FVector2D Input = Value.Get<FVector2D>();
	InputMove = (new FVector(Input.X, Input.Y, 0.f))->GetClampedToMaxSize(1.f);
	UE_LOG(LogTemp, Warning, TEXT("Input %s"), *InputMove.ToString());
}

void APawnPlayer::Move(float DeltaTime) {
	FVector DesiredVelocity = InputMove * MaxSpeed;
	float MaxSpeedChange = DeltaTime * MaxAcceleration;

	Velocity.X = FMath::FInterpTo(Velocity.X, DesiredVelocity.X, DeltaTime, MaxSpeedChange);
	Velocity.Y = FMath::FInterpTo(Velocity.Y, DesiredVelocity.Y, DeltaTime, MaxSpeedChange);

	FVector Displacement = Velocity * DeltaTime;
	FHitResult* OutSweepHitResult = new FHitResult();
	AddActorWorldOffset(Displacement, true, OutSweepHitResult, ETeleportType::ResetPhysics);
	if (OutSweepHitResult->bBlockingHit) Velocity = FVector::ZeroVector;
}

void APawnPlayer::HandleRunState(const FInputActionValue& Value) {
	const bool IsRunning = Value.Get<bool>();
	MaxSpeed = IsRunning ? MaxRunSpeed : MaxWalkSpeed;
}

void APawnPlayer::HandleAttack(const FInputActionValue& Value) {
	switch (Element) {
		case HarmonizedElement::Fire:
			FireAttack();
			break;
		case HarmonizedElement::Grass:
			UE_LOG(LogTemp, Warning, TEXT("Attack > Grass"));
			break;
		case HarmonizedElement::Water:
			UE_LOG(LogTemp, Warning, TEXT("Attack > Water"));
			break;
	}
}

void APawnPlayer::FireAttack() {
	if (!CanAttackFire) return;
	UE_LOG(LogTemp, Warning, TEXT("Attack > Fire"));
	FHitResult OutHitResult;
	PlayerController->GetHitResultUnderCursor(
		ECollisionChannel::ECC_Visibility,
		false,
		OutHitResult
	);

	FVector FireSpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator FireSpawnRotation = (OutHitResult.ImpactPoint - FireSpawnLocation).Rotation();
	GetWorld()->SpawnActor<AProjectileFire>(ProjectileFireClass, FireSpawnLocation, FireSpawnRotation);

	CanAttackFire = false;
	GetWorldTimerManager().SetTimer(TimerHandleFireRate, this, &APawnPlayer::EnableAttackFire, AttackRateFire, false);
}

void APawnPlayer::HandleHarmonizeFire(const FInputActionValue& Value) { Element = HarmonizedElement::Fire; }
void APawnPlayer::HandleHarmonizeGrass(const FInputActionValue& Value) { Element = HarmonizedElement::Grass; }
void APawnPlayer::HandleHarmonizeWater(const FInputActionValue& Value) { Element = HarmonizedElement::Water; }

void APawnPlayer::EnableAttackFire() { CanAttackFire = true; }