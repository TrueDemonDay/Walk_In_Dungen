// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Widgets/InteractWidget.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
APlayerCharacterBase::APlayerCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bIsPlayAnimation = false;
	PlayingMontage = nullptr;
	InteractWidgetRef = nullptr;
	bFreeCameraMode = false;

	GetMesh()->SetRelativeLocation(FVector(0.f,0.f,-1*GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0.f,-90.f,0.f));

	CameraSpringArm = CreateDefaultSubobject < USpringArmComponent>(TEXT("CameraSpringArmComponent"));
	CameraSpringArm->SetupAttachment(GetCapsuleComponent());
	CameraSpringArm->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	CameraSpringArm->bUsePawnControlRotation = true;
	CameraSpringArm->TargetArmLength = 300.f;
	CameraSpringArm->bEnableCameraLag = true;
	CameraSpringArm->bEnableCameraRotationLag = true;
	CameraSpringArm->CameraLagSpeed = 2.f;
	CameraSpringArm->CameraRotationLagSpeed = 10.f;
	CameraSpringArm->CameraLagMaxDistance = 75.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(CameraSpringArm);

	WalkSpeed = 150.f;
	RunSpeed = 500.f;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MaxAcceleration = 500;
	GetCharacterMovement()->GroundFriction = 0.5f;
	GetCharacterMovement()->BrakingDecelerationWalking = 400;
}

// Called when the game starts or when spawned
void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacterBase::Tick(float DeltaTime){Super::Tick(DeltaTime);}

// Called to bind functionality to input
void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacterBase::MoveRight);

	PlayerInputComponent->BindAction("Interact", IE_Released, this, &APlayerCharacterBase::InteractInput);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacterBase::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacterBase::EndSprint);

	PlayerInputComponent->BindAction("FreeCamera", IE_Pressed, this, &APlayerCharacterBase::CameraFreeInput);
}

void APlayerCharacterBase::MoveForward(float Value)
{
	if (Value!=0 && !bIsPlayAnimation)
	{
		FVector ForwardVector = !bFreeCameraMode ? GetActorForwardVector() :
			UKismetMathLibrary::GetForwardVector(FRotator(0.f, GetControlRotation().Yaw, 0.f));
		AddMovementInput(ForwardVector , Value);
	}
}

void APlayerCharacterBase::MoveRight(float Value)
{
	if (Value != 0 && !bIsPlayAnimation)
	{
		FVector RightVector = !bFreeCameraMode ? GetActorRightVector() :
			UKismetMathLibrary::GetRightVector(FRotator(0.f, GetControlRotation().Yaw, 0.f));
		AddMovementInput(RightVector, Value);
	}
}

void APlayerCharacterBase::ChangeInteractWidgetHiden(bool NewHiden)
{
	if (InteractWidgetRef)
	{
		InteractWidgetRef->SetVisibility(NewHiden ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}
}

void APlayerCharacterBase::InteractInput()
{
	if (IsValid(InteractActorRef))
	{
		if (InteractActorRef->Implements<UInteractInterface>())
		{
			IInteractInterface::Execute_Interact(InteractActorRef);
		}
	}
}

void APlayerCharacterBase::StartInteract(UAnimMontage* PlayMontage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(PlayMontage, 1.f);
		bIsPlayAnimation = true;
		PlayingMontage = PlayMontage;
		GetWorldTimerManager().SetTimer(TimerMontage, this, &APlayerCharacterBase::StopMontage, PlayingMontage->GetPlayLength(), false);
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void APlayerCharacterBase::CameraFreeInput()
{
	bFreeCameraMode = !bFreeCameraMode;
	if (!bIsPlayAnimation)
	{
		bUseControllerRotationYaw = !bFreeCameraMode;
		GetCharacterMovement()->bOrientRotationToMovement = bFreeCameraMode;
	}
}

void APlayerCharacterBase::StopMontage()
{
	if (bIsPlayAnimation && PlayingMontage)
	{
		GetWorldTimerManager().ClearTimer(TimerMontage);
		bIsPlayAnimation = false;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			if (AnimInstance->Montage_IsPlaying(PlayingMontage))
			{
				AnimInstance->StopAllMontages(0.25f);
			}
		}
		IInteractInterface::Execute_UpdateInteract(InteractActorRef);
		PlayingMontage = nullptr;
		bUseControllerRotationYaw = true && !bFreeCameraMode;
		GetCharacterMovement()->bOrientRotationToMovement = false || bFreeCameraMode;
	}
}

void APlayerCharacterBase::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void APlayerCharacterBase::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacterBase::Interact_Implementation()
{
	StopMontage();
}

void APlayerCharacterBase::UpdateInteract_Implementation()
{
	//ToDo:
}

