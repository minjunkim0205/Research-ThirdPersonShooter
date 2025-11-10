// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Bullet.h"

// Sets default values
ATPSCharacter::ATPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));	
	springArmComp->TargetArmLength = 400;
	springArmComp->bUsePawnControlRotation = true;

	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);	
	tpsCamComp->bUsePawnControlRotation = true;

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	JumpMaxCount = 2;

	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* pc = Cast<APlayerController>(Controller);
	if (pc != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsys != nullptr)
		{
			subsys->AddMappingContext(imcDefault, 0);
			subsys->AddMappingContext(imcMouseLook, 0);
		}
	}
}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerMove();
}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(iaMove, ETriggerEvent::Triggered, this, &ATPSCharacter::Move);
		enhancedInputComponent->BindAction(iaMouseLook, ETriggerEvent::Triggered, this, &ATPSCharacter::Look);
		enhancedInputComponent->BindAction(iaJump, ETriggerEvent::Started, this, &ACharacter::Jump);
		enhancedInputComponent->BindAction(iaJump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		enhancedInputComponent->BindAction(iaFire, ETriggerEvent::Started, this, &ATPSCharacter::InputFire);
	}
}

void ATPSCharacter::Move(const struct FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();
	direction.X = value.X;
	direction.Y = value.Y;
}

void ATPSCharacter::Look(const struct FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();
	AddControllerYawInput(value.X);
	AddControllerPitchInput(value.Y);
}

void ATPSCharacter::PlayerMove()
{
	direction = FTransform(GetControlRotation()).TransformVector(direction);
	/*
	FVector P0 = GetActorLocation();
	FVector vt = direction * walkSpeed * DeltaTime;
	FVector P = P0 + vt;
	SetActorLocation(P);
	*/
	AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void ATPSCharacter::InputFire(const struct FInputActionValue& inputValue)
{
	FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
}