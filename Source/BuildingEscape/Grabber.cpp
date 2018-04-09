#include "Grabber.h"

#define OUT


UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	GetAttachedPhysicsHandle();
	SetupInputComponent();

}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FVector PlayerViewPointLocation;
	FRotator Rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT Rotation); // OUT does nothing, but it allows us to mark out params!
	// Draw a debug line for now
	FVector RotAsVector = Rotation.Vector();
	float Reach = 100.0f;
	FVector EndLocation = PlayerViewPointLocation + Reach * RotAsVector;
	LineTraceEnd = EndLocation;

	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

/////////////////////////////////////////////////////////
// EVENTS
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key pressed!"));
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit) {
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Release key pressed!"));
	PhysicsHandle->ReleaseComponent();
}

/////////////////////////////////////////////////////////
// AUXILIARY
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Grabber does not contain a UInputComponent!"));
	}
	else {
		InputComponent->BindAction("GRAB", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("GRAB", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::GetAttachedPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Warning, TEXT("Grabber does not contain a PhysicsHandleComponent!"));
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	GetLineTraceEndpoints();

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.0f
	);

	/// Query params
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	// Line-trace (AKA ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), // we only want to look at physics bodies
		TraceParameters
	);

	AActor* HitActor = Hit.GetActor();
	if (HitActor) {
		UE_LOG(LogTemp, Warning, TEXT("We hit %s!"), *(HitActor->GetName()));
	}

	return Hit;
}

void UGrabber::GetLineTraceEndpoints()
{
	// Get the player viewpoint
	FRotator Rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT Rotation); // OUT does nothing, but it allows us to mark out params!

																										   // Draw a debug line for now
	FVector RotAsVector = Rotation.Vector();
	float Reach = 100.f;
	LineTraceEnd = PlayerViewPointLocation + Reach * RotAsVector;
}
