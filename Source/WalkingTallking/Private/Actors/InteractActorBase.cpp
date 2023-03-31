// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/InteractActorBase.h"
#include "Characters/PlayerCharacterBase.h"
#include "Components/ArrowComponent.h"
#include "Animation/AnimMontage.h"

// Sets default values
AInteractActorBase::AInteractActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	OnActorBeginOverlap.AddDynamic(this, &AInteractActorBase::BeginOverlaping);
	OnActorEndOverlap.AddDynamic(this, &AInteractActorBase::EndOverlaping);

	bIsInteractActive = false;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));

	PlayerSceneLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Player location in scene"));
	PlayerSceneLocation->SetupAttachment(RootScene);
}

void AInteractActorBase::Interact_Implementation()
{
	if (!bIsInteractActive)
	{
		if (bNeedTeleport)
		{
			PlayerCharacterRef->TeleportTo(PlayerSceneLocation->GetComponentLocation(), PlayerSceneLocation->GetComponentRotation(), false, true);
		}
		if (AnimationToPlay)
		{
			PlayerCharacterRef->StartInteract(AnimationToPlay);
		}
		bIsInteractActive = true;
	}
	else
	{
		bIsInteractActive = false;
		IInteractInterface::Execute_Interact(PlayerCharacterRef);
	}

}

void AInteractActorBase::UpdateInteract_Implementation()
{
	bIsInteractActive = false;
}

// Called when the game starts or when spawned
void AInteractActorBase::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractActorBase::BeginOverlaping(AActor* OverlappedActor, AActor* OtherActor)
{
	APlayerCharacterBase* PlayerRef = Cast<APlayerCharacterBase>(OtherActor);
	if (PlayerRef)
	{
		PlayerCharacterRef = PlayerRef;
		PlayerCharacterRef->InteractActorRef = this;
		PlayerCharacterRef->ChangeInteractWidgetHiden(false);
	}
}

void AInteractActorBase::EndOverlaping(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!PlayerCharacterRef) return;
	APlayerCharacterBase* PlayerRef = Cast<APlayerCharacterBase>(OtherActor);
	if (PlayerRef == PlayerCharacterRef)
	{
		PlayerCharacterRef->InteractActorRef = nullptr;
		PlayerCharacterRef->ChangeInteractWidgetHiden(true);
		PlayerCharacterRef = nullptr;
		bIsInteractActive = false;
	}
}

// Called every frame
void AInteractActorBase::Tick(float DeltaTime){Super::Tick(DeltaTime);}

