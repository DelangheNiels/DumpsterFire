// Copyright Epic Games, Inc. All Rights Reserved.

#include "Dumpster_FireCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Dumpster_Fire/BaseTrash.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Misc/MapErrors.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Public/MetalPipe.h"
#include "Public/Wheel.h"
#include "Public/MetalScrap.h"
#include "Public/TrashBag.h"
#include "Public/JerryCan.h"



//////////////////////////////////////////////////////////////////////////
// ADumpster_FireCharacter

ADumpster_FireCharacter::ADumpster_FireCharacter()
{
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	m_IsHoldingItem = false;
	m_ThrowStrength = 950.0f;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	m_pPlayerStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh2"));


	FAttachmentTransformRules rules = FAttachmentTransformRules{ EAttachmentRule{ } , false };
	PlayerMesh->AttachToComponent(RootComponent, rules);
	m_pPlayerStaticMesh->AttachToComponent(PlayerMesh, rules);

	m_pPlayerPickupBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Area Pickup"));
	m_pPlayerPickupBox->SetBoxExtent({ 60,60,40 });
	m_pPlayerPickupBox->SetWorldLocation({ 140,0,-60 });
	m_pPlayerPickupBox->SetCollisionProfileName("Trigger");
	m_pPlayerPickupBox->AttachToComponent(PlayerMesh, rules);

	m_pPlayerHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Player Hit Box"));
	m_pPlayerHitBox->SetBoxExtent({ 90,125,180 });
	m_pPlayerHitBox->SetWorldLocation({ -5,0,20 });
	m_pPlayerHitBox->SetCollisionProfileName("Trigger");
	m_pPlayerHitBox->AttachToComponent(PlayerMesh, rules);
	/*m_pPlayerHitBox->SetVisibility(true);
	m_pPlayerHitBox->SetHiddenInGame(false);*/

	//Particles
	m_StunSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Particles/Stun/NS_Stun.NS_Stun'"), nullptr, LOAD_None, nullptr);

	//Text Player Number
	m_pText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Player Number"));
	m_pText->AttachToComponent(PlayerMesh, rules);
	auto location = m_pPlayerStaticMesh->GetRelativeLocation();

	const auto offset = 120.f;
	const auto offsetZ = 100.f;
	location.Z += offset;
	location.Y += offset;

	m_pText->SetWorldLocation(location);
	m_pText->SetVisibility(true);
	m_pText->SetWorldSize(100);
	m_pText->SetTextRenderColor(FColor::White);
	m_pText->SetUsingAbsoluteRotation(true);
	m_pText->BodyInstance.bLockTranslation = true;


	auto platform = UGameplayStatics::GetActorOfClass(GetWorld(), ADeathPlatform::StaticClass());
	m_pDeathPlatform = Cast<ADeathPlatform>(platform);

	m_pAttackArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Area"));
	m_pAttackArea->SetBoxExtent({ 110,110,110 });

	m_pAttackArea->SetCollisionProfileName("Trigger");
	//m_pAttackArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_pAttackArea->AttachToComponent(PlayerMesh, rules);


	//Attack montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> animmont(TEXT("AnimMontage'/Game/Models/Characters/Animations/Sphere/ANIMM_Atack2.ANIMM_Atack2'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ThrowMontage(TEXT("AnimMontage'/Game/Models/Characters/Animations/Sphere/ANIM_Throw_Montage.ANIM_Throw_Montage'"));

	if (animmont.Succeeded())
	{
		m_pPlayerAttackMontage = animmont.Object;
		m_AttackDuration = m_pPlayerAttackMontage->GetPlayLength();
		m_pThrowDuration = ThrowMontage.Object->GetPlayLength();
	}

	m_pRunningParticle = CreateDefaultSubobject<UNiagaraComponent>("Running Particle");
	m_pRunningParticle->AttachToComponent(PlayerMesh, rules);
	

	m_pHoldingParticle = CreateDefaultSubobject<UNiagaraComponent>("Holding Particle");
	m_pHoldingParticle->AttachToComponent(PlayerMesh, rules);


}

//////////////////////////////////////////////////////////////////////////
// Input

void ADumpster_FireCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PickUp", IE_Released, this, &ADumpster_FireCharacter::Pickup);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Punch", IE_Released, this, &ADumpster_FireCharacter::Punch);
	PlayerInputComponent->BindAction("BackToMain", IE_Pressed, this, &ADumpster_FireCharacter::BackToMainMenu);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADumpster_FireCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADumpster_FireCharacter::MoveRight);


	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ADumpster_FireCharacter::TurnAtRate);
}

void ADumpster_FireCharacter::BeginPlay()
{
	Super::BeginPlay();

	USkeletalMeshComponent* characterMesh = GetMesh();
	characterMesh->SetSkeletalMesh(m_pSkeletalMesh);
	characterMesh->SetMaterial(0, m_pPlayerMaterial);
	characterMesh->SetRelativeRotation(FRotator(0, -90, 0));
	characterMesh->SetRelativeLocation(FVector(0, 0, -95));
	characterMesh->SetWorldScale3D(FVector(9.6f, 9.6f, 10.f));
	characterMesh->SetVisibility(true);

	m_pPlayerPickupBox->OnComponentBeginOverlap.AddDynamic(this, &ADumpster_FireCharacter::OnOverlapBegin);
	m_pPlayerPickupBox->OnComponentEndOverlap.AddDynamic(this, &ADumpster_FireCharacter::OnOverlapEnd);

	m_pAttackArea->OnComponentBeginOverlap.AddDynamic(this, &ADumpster_FireCharacter::AttackPlayer);
	m_pAttackArea->OnComponentEndOverlap.AddDynamic(this, &ADumpster_FireCharacter::RemovePlayer);

	m_IsDead = false;
	m_IsStunned = false;
	SetStunned(true);

}

void ADumpster_FireCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ABaseTrash::StaticClass()))
	{
		m_pTrashInView.push_back(Cast<ABaseTrash>(OtherActor));
		m_pTrashInView[0]->m_pTrashMesh->SetRenderCustomDepth(true);
		m_isItemSpotted = true;
	}
}

void ADumpster_FireCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(ABaseTrash::StaticClass()))
	{

		m_pTrashInView[0]->m_pTrashMesh->SetRenderCustomDepth(false);
		m_isItemSpotted = false;
		m_pTrashInView.erase(std::remove(m_pTrashInView.begin(), m_pTrashInView.end(), Cast<ABaseTrash>(OtherActor)), m_pTrashInView.end());
	}
}

void ADumpster_FireCharacter::AttackPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ADumpster_FireCharacter::StaticClass()))
	{
		if (m_Index != Cast<ADumpster_FireCharacter>(OtherActor)->m_Index)
			m_playersToStun.push_back(Cast<ADumpster_FireCharacter>(OtherActor));

	}
}

void ADumpster_FireCharacter::RemovePlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(ADumpster_FireCharacter::StaticClass()))
	{

		m_playersToStun.erase(std::remove(m_playersToStun.begin(), m_playersToStun.end(), Cast<ADumpster_FireCharacter>(OtherActor)), m_playersToStun.end());
	}
}

void ADumpster_FireCharacter::SetMesh()
{

	FString player = "Player" + FString::FromInt(m_Index + 1);
	m_pText->SetText(player);
	m_pText->SetRelativeRotation(FRotator(45, 180, 0));
	m_IsMeshSet = true;
}



bool ADumpster_FireCharacter::GetStunned()
{
	return m_IsStunned;
}

bool ADumpster_FireCharacter::IsAttacking()
{
	return m_IsAttacking;
}

bool ADumpster_FireCharacter::IsHoldingItem()
{
	return m_IsHoldingItem;
}

bool ADumpster_FireCharacter::IsThrowing()
{
	return m_IsThrowing;
}

float ADumpster_FireCharacter::GetRespawnTimer()
{
	return m_RespawnTimer;
}

float ADumpster_FireCharacter::GetTimeToRespawn()
{
	return m_TimeToRespawn;
}

bool ADumpster_FireCharacter::GetHasWon()
{
	return m_HasWon;
}

bool ADumpster_FireCharacter::GetIsDead()
{
	return m_IsDead;
}

bool ADumpster_FireCharacter::GetIsLost()
{
	return m_HasLost;
}

int ADumpster_FireCharacter::GetIndex()
{
	return m_Index;
}

void ADumpster_FireCharacter::PlayerDied()
{
	m_IsDead = true;

	GetCharacterMovement()->ClearAccumulatedForces();

	FDetachmentTransformRules detachRules = FDetachmentTransformRules{ EDetachmentRule{}, true };
	detachRules.LocationRule = EDetachmentRule{ EDetachmentRule::KeepWorld };
	detachRules.RotationRule = EDetachmentRule{ EDetachmentRule::KeepWorld };
	detachRules.ScaleRule = EDetachmentRule{ EDetachmentRule::KeepWorld };

	if (m_IsHoldingItem)
	{
		const auto mesh = m_pHeldItem->FindComponentByClass<UStaticMeshComponent>();

		if (mesh)
		{
			mesh->SetSimulatePhysics(true);
			m_pHeldItem->DetachFromActor(detachRules);
			m_pHeldItem->SetIsThrown(true);
			m_pHeldItem->SetPlayer(this);
			m_IsHoldingItem = false;
			m_pHeldItem = nullptr;

		}
	}

	auto spawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(m_pDeathPlatform->GetActorLocation(), m_pDeathPlatform->GetMesh()->GetStaticMesh()->GetBounds().GetBox().GetExtent());
	spawnPoint.Z += 50;
	SetActorLocation(spawnPoint);



}

void ADumpster_FireCharacter::Respawn()
{
	m_IsDead = false;
	m_IsHoldingItem = false;

	int x = FMath::RandRange(-930, -870);
	int y = FMath::RandRange(-340, 370);
	SetActorLocation(FVector(x, y, 1040));


}

void ADumpster_FireCharacter::BackToMainMenu()
{
	if (m_CanGoBack)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Main_Menu_Level");
	}
}

void ADumpster_FireCharacter::Throw()
{
	FDetachmentTransformRules detachRules = FDetachmentTransformRules{ EDetachmentRule{}, true };
	detachRules.LocationRule = EDetachmentRule{ EDetachmentRule::KeepWorld };
	detachRules.RotationRule = EDetachmentRule{ EDetachmentRule::KeepWorld };
	detachRules.ScaleRule = EDetachmentRule{ EDetachmentRule::KeepWorld };

	m_pHeldItem->DetachFromActor(detachRules);

	const auto mesh = m_pHeldItem->FindComponentByClass<UStaticMeshComponent>();

	if (mesh)
	{
		m_IsThrowing = true;
		mesh->SetSimulatePhysics(true);
		mesh->AddImpulse(GetCapsuleComponent()->GetForwardVector() * m_ThrowStrength, "", true);

	}

	m_pHeldItem->SetIsThrown(true);
	m_pHeldItem->SetPlayer(this);
	m_IsHoldingItem = false;
	m_pHeldItem = nullptr;

}

void ADumpster_FireCharacter::Punch()
{
	if (m_CanAttack)
	{
		if (m_IsHoldingItem == false && m_IsStunned == false && m_attackTime == 0)
		{
			m_IsAttacking = true;
			m_CanAttack = false;
			for (auto player : m_playersToStun)
			{
				if (player->GetStunned() == false)
				{

					player->StartStunParticle();

					FTimer TimerHandle;
					auto location = GetCapsuleComponent()->GetRelativeLocation();
					auto enemyLocation = player->GetCapsuleComponent()->GetRelativeLocation();

					auto pushDirection = enemyLocation - location;
					player->GetCapsuleComponent()->SetSimulatePhysics(true);
					player->GetCapsuleComponent()->AddImpulse(pushDirection * m_pushStrenth);
					player->SetStunned(true);
					
				}

			}

		}
	}

	

}


void ADumpster_FireCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADumpster_FireCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(m_pRunningParticle != nullptr)
	{
		if (GetMovementComponent()->Velocity.Equals(FVector(0, 0, 0)))
		{
			m_pRunningParticle->Deactivate();
		}
		else
			m_pRunningParticle->Activate();
	}
	if (!m_IsMeshSet)
	{
		m_TimemeshSet += DeltaSeconds;

		if (m_TimemeshSet >= 0.1f)
		{
			SetMesh();


		}
	}

	if (m_IsAttacking)
	{
		m_attackTime += DeltaSeconds;

		if (m_attackTime >= 0.7)
		{
			m_IsAttacking = false;
			m_attackTime = 0;
		}
	}

	if (m_IsThrowing)
	{
		m_ThrowLength += DeltaSeconds;

		if (m_ThrowLength >= m_pThrowDuration - 1)
		{
			m_IsThrowing = false;

			m_ThrowLength = 0;
		}
	}

	m_textTime += DeltaSeconds;
	if (m_textTime >= m_TimeToShowText)
	{
		m_pText->SetVisibility(false);
	}

	if (m_IsHoldingItem && m_pHoldingParticle != nullptr)
	{
		m_pHoldingParticle->Activate();
	}
	else
		m_pHoldingParticle->Deactivate();

	if (m_IsStunned)
	{
		m_TotalTimeStunned += DeltaSeconds;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		if (m_TotalTimeStunned >= m_timeStunned)
		{
			if (m_StunComponent)
				m_StunComponent->DeactivateImmediate();

			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			m_IsStunned = false;
			GetCapsuleComponent()->SetSimulatePhysics(false);
			m_TotalTimeStunned = 0;
		}

		if (m_IsHoldingItem)
		{
			FDetachmentTransformRules detachRules = FDetachmentTransformRules{ EDetachmentRule{}, true };
			detachRules.LocationRule = EDetachmentRule{ EDetachmentRule::KeepWorld };
			detachRules.RotationRule = EDetachmentRule{ EDetachmentRule::KeepWorld };
			detachRules.ScaleRule = EDetachmentRule{ EDetachmentRule::KeepWorld };

			m_pHeldItem->DetachFromActor(detachRules);

			const auto mesh = m_pHeldItem->FindComponentByClass<UStaticMeshComponent>();

			if (mesh)
			{
				mesh->SetSimulatePhysics(true);
				mesh->AddImpulse({ FMath::FRandRange(0.f, 100.f) * 10, FMath::FRandRange(0.f, 100.f) * 10 ,FMath::FRandRange(0.f, 100.f) * 10 }, "", true);

			}


			m_IsHoldingItem = false;
			m_pHeldItem = nullptr;
		}
		auto location = GetActorLocation();
		location.Z += 80;
		if (m_StunComponent)
			m_StunComponent->SetRelativeLocation(location);

	}

	if (m_isItemSpotted)
	{
		m_pTrashInView[0]->m_pTrashMesh->SetRenderCustomDepth(true);
	}

	if (m_IsDead)
	{
		m_RespawnTimer += DeltaSeconds;
		if (m_RespawnTimer >= m_TimeToRespawn)
		{
			Respawn();
			m_RespawnTimer = 0;
		}
	}

	if (m_CanAttack == false)
	{
		m_CooldownTimer += DeltaSeconds;

		if (m_CooldownTimer >= m_MaxCooldown)
		{
			m_CooldownTimer = 0;
			m_CanAttack = true;
		}
	}
}

void ADumpster_FireCharacter::SetStunned(bool Isstunned)
{

	m_IsStunned = Isstunned;
}

void ADumpster_FireCharacter::StartStunParticle()
{
	if (m_IsStunned == false)
	{
		m_IsStunned = true;
		auto location = GetActorLocation();
		location.Z += 80;
		m_StunComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_StunSystem, location);


	}

	else
	{
		m_TotalTimeStunned = 0;
	}

}

void ADumpster_FireCharacter::SetWon(bool hasWon)
{
	m_HasWon = hasWon;
	m_CanGoBack = true;
}

void ADumpster_FireCharacter::SetLost(bool hasLost)
{
	m_HasLost = hasLost;
	m_CanGoBack = true;
}

void ADumpster_FireCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && !m_IsDead)
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetIgnoreLookInput(true);
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, 0, 0);


		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}

void ADumpster_FireCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && !m_IsDead)
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetIgnoreLookInput(true);

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, 0, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ADumpster_FireCharacter::Pickup()
{
	//Pickup does 2 things: When not holding => Pick up Item
	//When holding item throw it
	if (!m_IsDead)
	{
		FAttachmentTransformRules rules = FAttachmentTransformRules{ EAttachmentRule{ } , true };
		rules.LocationRule = EAttachmentRule{ EAttachmentRule::SnapToTarget };
		rules.RotationRule = EAttachmentRule{ EAttachmentRule::SnapToTarget };
		rules.ScaleRule = EAttachmentRule{ EAttachmentRule::KeepWorld };


		if (m_pTrashInView.size() != 0 && m_IsHoldingItem == false)
		{
			FVector SocketLocation = GetMesh()->GetSocketLocation("BlockLocation");

			m_pHeldItem = m_pTrashInView[0];
			m_pTrashInView.erase(m_pTrashInView.begin());
			m_pHeldItem->SetActorLocation({ SocketLocation });
			m_pHeldItem->AttachToComponent(GetMesh(), rules, "BlockLocation");

			m_pHeldItem->SetActorRotation(FRotator{ 0.5,0,0 });
			const auto mesh = m_pHeldItem->FindComponentByClass<UStaticMeshComponent>();

			if (m_pHeldItem->IsA(AMetalPipe::StaticClass()) || m_pHeldItem->IsA(ATrashBag::StaticClass()))
			{
				mesh->SetWorldRotation(FRotator(90, 0, 0));
			}

			else if (m_pHeldItem->IsA(AWheel::StaticClass()) || m_pHeldItem->IsA(AMetalScrap::StaticClass()))
			{
				mesh->SetWorldRotation(FRotator(0, 90, 0));
			}

			else if (m_pHeldItem->IsA(AJerryCan::StaticClass()))
			{
				mesh->SetWorldRotation(FRotator(0, 0, 90));

			}

			if (mesh)
			{
				mesh->SetSimulatePhysics(false);
			}

			m_IsHoldingItem = true;
		}

		else if (m_pHeldItem && m_IsHoldingItem == true)
		{
			Throw();
		}
	}



}

