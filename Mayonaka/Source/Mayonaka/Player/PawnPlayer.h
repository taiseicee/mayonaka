// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnPlayer.generated.h"

UCLASS()
class MAYONAKA_API APawnPlayer : public APawn {
	GENERATED_BODY()

public:
	APawnPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY()
	class UCapsuleComponent* CapsuleCollider;
	UPROPERTY()
	class UPaperFlipbookComponent* Flipbook;

};
