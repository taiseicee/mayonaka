// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnPlayer.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"

APawnPlayer::APawnPlayer() {
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	Flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));

	RootComponent = CapsuleCollider;
	Flipbook->SetupAttachment(RootComponent);
}

void APawnPlayer::BeginPlay() {
	Super::BeginPlay();
	
}

void APawnPlayer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void APawnPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

