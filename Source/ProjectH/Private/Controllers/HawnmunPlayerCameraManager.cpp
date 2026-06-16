#include "Controllers/HawnmunPlayerCameraManager.h"
#include "Characters/HawnmunPlayer.h" 
#include "GameFramework/PlayerController.h"

AHawnmunPlayerCameraManager::AHawnmunPlayerCameraManager()
{
	DefaultFOV = 95.f; 
	SprintFOV = 120.f;
	CurrentFOV = DefaultFOV; 

	FOVInterpSpeed = 3.f; 

	ViewPitchMin = -20.f;
	ViewPitchMax = 25.f;
}

void AHawnmunPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if (AHawnmunPlayer* Player = Cast<AHawnmunPlayer>(GetOwningPlayerController()->GetPawn()))
	{
		const float TargetFOV = Player->GetIsSprinting() ? SprintFOV : DefaultFOV;
		CurrentFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, FOVInterpSpeed);
		OutVT.POV.FOV = CurrentFOV;
	}
}