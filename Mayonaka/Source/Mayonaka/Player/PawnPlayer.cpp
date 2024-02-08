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

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Player"));
	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	Flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	SpringArmCamera = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	
	CapsuleCollider->SetupAttachment(RootComponent);
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
}

void APawnPlayer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	Walk(DeltaTime);
}

void APawnPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APawnPlayer::HandleInputMove);
	}
}

void APawnPlayer::HandleInputMove(const FInputActionValue& Value) {

	const FVector2D Input = Value.Get<FVector2D>();
	InputMove = (new FVector(Input.X, Input.Y, 0.f))->GetClampedToMaxSize(1.f);
	UE_LOG(LogTemp, Warning, TEXT("Input %s"), *InputMove.ToString());
}

void APawnPlayer::Walk(float DeltaTime) {
	FVector DesiredVelocity = InputMove * MaxWalkSpeed;
	float MaxSpeedChange = DeltaTime * MaxAcceleration;

	Velocity.X = FMath::FInterpTo(Velocity.X, DesiredVelocity.X, DeltaTime, MaxSpeedChange);
	Velocity.Y = FMath::FInterpTo(Velocity.Y, DesiredVelocity.Y, DeltaTime, MaxSpeedChange);

	FVector Displacement = Velocity * DeltaTime;
	AddActorWorldOffset(Displacement, true);
}