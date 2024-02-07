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
}

void APawnPlayer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void APawnPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APawnPlayer::HandleInputMove);
	}
}

void APawnPlayer::HandleInputMove(const FInputActionValue& Value) {
	const bool CurrentValue = Value.Get<bool>();
	if (CurrentValue) {
		UE_LOG(LogTemp, Warning, TEXT("IA_Move Triggered"));
	}
}

