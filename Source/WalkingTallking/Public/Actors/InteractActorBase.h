// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractInterface.h"
#include "InteractActorBase.generated.h"

class APlayerCharacterBase;
class UAnimMontage;

UCLASS()
class WALKINGTALLKING_API AInteractActorBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractActorBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RootSceneComponent, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerPlace, meta = (AllowPrivateAccess = "true"))
	USceneComponent* PlayerSceneLocation;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact();
	virtual void Interact_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateInteract();
	virtual void UpdateInteract_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void BeginOverlaping(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void EndOverlaping(AActor* OverlappedActor, AActor* OtherActor);

	bool bIsInteractActive;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = PlayerPlace)
	bool bNeedTeleport;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AnimationToPlay;

	APlayerCharacterBase* PlayerCharacterRef;
};
