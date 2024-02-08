// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "PaperFlipbookComponent.h"

APawnPlayer::APawnPlayer() {
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	Flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	SpringArmCamera = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	
	RootComponent = CapsuleCollider;
	Flipbook->SetupAttachment(RootComponent);
	SpringArmCamera->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArmCamera);
}

void APawnPlayer::BeginPlay() {
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(MappingContextPlayer, 0);
		}
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
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APawnPlayer::HandleInputMove);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &APawnPlayer::HandleRunState);
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