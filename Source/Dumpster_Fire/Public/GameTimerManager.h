// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StartButton.h"
#include "GameTimerManager.generated.h"

UCLASS()
class DUMPSTER_FIRE_API AGameTimerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameTimerManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_Minutes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_Seconds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_PointsToWin = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_RedTeamWon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_BlueTeamWon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_IsGameEnded;

	UPROPERTY(EditAnywhere, Category = "Button")
		AStartButton* m_pStartButton;
	


private:
	float m_ElaspedTime;
	bool m_Run;
	bool m_Started;

	void GetWinnerWhenTimeIsOver();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetIsGameStarted() const;
	void EndGame();

};
