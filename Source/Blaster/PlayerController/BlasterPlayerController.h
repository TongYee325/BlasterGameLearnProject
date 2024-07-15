// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlasterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDScore(float Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDCarriedAmmo(int32 Ammo);
	void SetHUDMatchCountdown(float CounddownTime);
	virtual void OnPossess(APawn* InPawn)override;
	virtual void Tick(float Deltatime)override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual float GetServerTime();//Synced with server world clock
	virtual void ReceivedPlayer()override;//Sync with server clock as soon as possible

	void OnMatchStateSet(FName State);

protected:
	virtual void BeginPlay();
	void SetHUDTime();
	void PollInit();

	/**
	* Sync time between clinet and server
	*/

	//Requests the current server time , passing in the client's time when the request was sent
	UFUNCTION(Server,Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest);

	//Reports the current server time to the client in response to ServerRequestServerTime
	UFUNCTION(Client,Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	float ClientServerDelta = 0.f;//Difference between server and client

	UPROPERTY(EditAnywhere,Category = Time)
	float TimeSyncFrequency = 5.f;

	float TimeSyncRunningTime = 0.f;

	void CheckTimeSync(float Deltatime);

private:
	UPROPERTY()
	class ABlasterHUD* BlasterHUD;
	
	float MatchTime = 120.f;
	uint32 CountdownInt=0;

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	UFUNCTION()
	void OnRep_MatchState();

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;
	bool bInitializeCharacterOverlay = false;

	float HUDHealth;
	float HUDMaxHealth;
	float HUDScore;
	int32 HUDDefeats;

};
