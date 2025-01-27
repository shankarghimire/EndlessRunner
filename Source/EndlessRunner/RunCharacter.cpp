// Fill out your copyright notice in the Description page of Project Settings.


#include "RunCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EndlessRunnerGameModeBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
// Sets default values
ARunCharacter::ARunCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set up Camera Arm
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->TargetArmLength = 350.f;
	CameraArm->SocketOffset = FVector(0.0, 0.0, 100.f);
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->SetupAttachment(GetRootComponent());

	//Setup Camera Component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);

}

// Called when the game starts or when spawned
void ARunCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	RunGameMode = Cast<AEndlessRunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	check(RunGameMode);

}

// Called every frame
void ARunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator ControlRotation = this->GetControlRotation();
	ControlRotation.Roll = 0.f;
	ControlRotation.Pitch = 0.f;
	AddMovementInput(UKismetMathLibrary::GetForwardVector(ControlRotation));
	//AddMovementInput(ControlRotation.Vector());

}

// Called to bind functionality to input
void ARunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Jump and Stop Jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARunCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARunCharacter::StopJumping);

	//MoveForward, Left and Right
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ARunCharacter::MoveLeft);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ARunCharacter::MoveRight);
	PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &ARunCharacter::MoveDown);
}



void ARunCharacter::ChangeLaneUpdate(const float Value)
{
	FVector Location = GetCapsuleComponent()->GetComponentLocation();
	Location.Y = FMath::Lerp(RunGameMode->LaneSwitchValues[CurrentLane], RunGameMode->LaneSwitchValues[NextLane], Value);
	SetActorLocation(Location);

}

void ARunCharacter::ChangeLaneFinished()
{
	CurrentLane = NextLane;
}

void ARunCharacter::MoveLeft()
{
	//UE_LOG(LogTemp, Warning, TEXT("Move Left input pressed!"));

	NextLane = FMath::Clamp(CurrentLane - 1, 0, 2);
	ChangeLane();

}

void ARunCharacter::MoveRight()
{
	//UE_LOG(LogTemp, Warning, TEXT("Move Right input pressed!"));
	NextLane = FMath::Clamp(CurrentLane + 1, 0, 2);
	ChangeLane();
}

void ARunCharacter::MoveDown()
{
	//UE_LOG(LogTemp, Warning, TEXT("Move Down input pressed!"));

	static FVector Impulse = FVector(0, 0, MoveDownImpulse);
	GetCharacterMovement()->AddImpulse(Impulse, true);


}

void ARunCharacter::Death()
{
	//UE_LOG(LogTemp, Warning, TEXT("Before If::Character Died!"));

	if(!bIsDead)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Inside If::Character Died!"));
		const FVector Location = GetActorLocation();
		UWorld* World = GetWorld();
		if (World)
		{

			bIsDead = true;
			DisableInput( nullptr);
			if (DeathParticleSystem)
			{
				UGameplayStatics::SpawnEmitterAtLocation(World,DeathParticleSystem, Location);
			}
			if (DeathSound)
			{
				UGameplayStatics::PlaySoundAtLocation(World, DeathSound, Location);

			}
			GetMesh()->SetVisibility(false);
			World->GetTimerManager().SetTimer(RestartTimerHandle, this,&ARunCharacter::OnDeath, 1.0f);
		}
	}
}

void ARunCharacter::OnDeath()
{
	bIsDead = false;

	if (RestartTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(RestartTimerHandle);
	}
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("RestartLevel"));
	UE_LOG(LogTemp, Warning, TEXT("New Level Started!!!"));
}

void ARunCharacter::AddCoin()
{
	RunGameMode->AddCoin();
}