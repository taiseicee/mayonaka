// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	class UCapsuleComponent* CapsuleCollider;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	class UPaperFlipbookComponent* Flipbook;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	class USpringArmComponent* SpringArmCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	class UInputMappingContext* MappingContextPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	class UInputAction* RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	float MaxWalkSpeed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	float MaxRunSpeed = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	float MaxAcceleration = 240.f;

	FVector Velocity = FVector::ZeroVector;
	FVector InputMove = FVector::ZeroVector;
	float MaxSpeed = 0.f;

	void HandleInputMove(const FInputActionValue& Value);
	void HandleRunState(const FInputActionValue& Value);

	void Move(float DeltaTime);
};
