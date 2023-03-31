// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/InteractInterface.h"
#include "PlayerCharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class UInteractWidget;

UCLASS()
class WALKINGTALLKING_API APlayerCharacterBase : public ACharacter, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacterBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UInteractWidget* InteractWidgetRef;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float WalkSpeed;
	float RunSpeed;

	bool bFreeCameraMode;

	bool bIsPlayAnimation;
	FTimerHandle TimerMontage;

	UPROPERTY()
	UAnimMontage* PlayingMontage;

	void StopMontage();

	void StartSprint();
	void EndSprint();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void MoveForward(float Value);
	UFUNCTION()
	void MoveRight(float Value);

	void ChangeInteractWidgetHiden(bool NewHiden);

	void InteractInput();

	void StartInteract(UAnimMontage* PlayMontage);

	void CameraFreeInput();

	UPROPERTY()
	AActor* InteractActorRef;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact();
	virtual void Interact_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateInteract();
	virtual void UpdateInteract_Implementation() override;
};
