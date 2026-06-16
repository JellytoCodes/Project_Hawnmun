#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "HawnmunPlayerCameraManager.generated.h"

UCLASS()
class PROJECTH_API AHawnmunPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AHawnmunPlayerCameraManager();

protected:
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float SprintFOV;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float FOVInterpSpeed;

private:
	float CurrentFOV; 
};