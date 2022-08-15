// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Dumpster_Fire/BaseTrash.h"
#include "Dumpster_FireGameMode.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Public/DeathPlatform.h"
#include "Dumpster_FireCharacter.generated.h"

UCLASS(config = Game)
class ADumpster_FireCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADumpster_FireCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Push)
		float m_pushStrenth = 500.0f;

	UPROPERTY(VisibleAnywhere, Category = SkeletalMesh)
		class USkeletalMeshComponent* PlayerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
		USkeletalMesh* m_pSkeletalMesh;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* m_pPlayerStaticMesh;

	UPROPERTY(EditAnywhere)
		UMaterialInstance* m_pPlayerMaterial;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* m_pPlayerPickupBox;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* m_pPlayerHitBox;

	UPROPERTY(VisibleAnywhere)
		class UParticleSystem* m_MyParticleSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UTextRenderComponent* m_pText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
		UBoxComponent* m_pAttackArea;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Animation)
		UAnimMontage* m_pPlayerAttackMontage {};

	


protected:

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Pickup();
	void TurnAtRate(float Rate);

	UFUNCTION(BlueprintCallable)
		virtual void Tick(float DeltaSeconds) override;

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;
public:
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void AttackPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void RemovePlayer(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		bool GetStunned();

	UFUNCTION(BlueprintCallable)
		bool IsAttacking();

	UFUNCTION(BlueprintCallable)
		bool IsHoldingItem();

	UFUNCTION(BlueprintCallable)
		bool IsThrowing();

	UFUNCTION(BlueprintCallable)
		float GetRespawnTimer();

	UFUNCTION(BlueprintCallable)
		float GetTimeToRespawn();

	UFUNCTION(BlueprintCallable)
		bool GetHasWon();

	UFUNCTION(BlueprintCallable)
		bool GetIsDead();

	UFUNCTION(BlueprintCallable)
		bool GetIsLost();

	UFUNCTION(BlueprintCallable)
		int GetIndex();

	void PlayerDied();

	void SetStunned(bool isStunned);
	void StartStunParticle();
	void SetWon(bool hasWon);
	void SetLost(bool hasLost);

private:
	void SetMesh();

	UPROPERTY(VisibleAnywhere)
		ABaseTrash* m_pTrashToPickUp;

	UPROPERTY(VisibleAnywhere)
		ABaseTrash* m_pHeldItem;

	bool m_IsHoldingItem;

	ADeathPlatform* m_pDeathPlatform;

	UPROPERTY(EditAnywhere)
		float m_ThrowStrength;

	void Throw();
	void Punch();

	void Respawn();

	void BackToMainMenu();

	bool m_IsStunned{};
	float m_TotalTimeStunned{};
	float m_timeStunned{ 1.0f };

	bool m_IsMeshSet{ false };
	bool m_isItemSpotted{ false };
	bool m_IsAttacking{ false };
	bool m_IsThrowing{ false };
	bool m_HasWon{ false };
	bool m_HasLost{ false };
	bool m_CanAttack{ true };
	bool m_CanGoBack{ false };

	float m_AttackDuration;
	float m_pThrowDuration;
	float m_attackTime{ 0 };
	float m_ThrowLength;
	float m_TimemeshSet{ 0 };
	float m_MaxCooldown{ 1.5f };
	float m_CooldownTimer{ 0.0f };

	std::vector<ABaseTrash*> m_pTrashInView{};

	UNiagaraSystem* m_StunSystem;
	UNiagaraComponent* m_StunComponent = nullptr;

	float m_textTime = 0;
	float m_TimeToShowText{ 5.0f };

	bool m_IsDead{ false };
	float m_RespawnTimer{ 0.0f };
	float m_TimeToRespawn{ 3.f };

	std::vector<ADumpster_FireCharacter*> m_playersToStun{};

	UAnimBlueprint* m_pAnimBPSquareP{};

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* m_pRunningParticle;



	UPROPERTY(EditAnywhere)
		UNiagaraComponent* m_pHoldingParticle;



};

