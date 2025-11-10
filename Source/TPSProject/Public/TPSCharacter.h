// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "TPSCharacter.generated.h"

UCLASS()
class TPSPROJECT_API ATPSCharacter : public ACharacter
{	
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* tpsCamComp;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputMappingContext* imcDefault;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputMappingContext* imcMouseLook;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* iaMove;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* iaMouseLook;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* iaJump;

	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float walkSpeed = 600;

	FVector direction;

	void Move(const struct FInputActionValue& inputValue);
	void Look(const struct FInputActionValue& inputValue);
	void PlayerMove();
};
