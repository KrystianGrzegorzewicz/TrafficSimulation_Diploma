#include "behavior/BehaviorAV.h"

#include <algorithm>

BehaviorAV::BehaviorAV(std::unique_ptr<ILongitudinalModel> model)
	: longitudinalModel(std::move(model)) {
}

MotionCommand BehaviorAV::compute(
	Travel& travel,
	int segment,
	float t,
	const CarState& self,
	float maxSpeed,
	float maxAccel,
	float maxDecel,
	float,
	float,
	const PerceptionState& perception)
{
	MotionCommand cmd;

	const float speed =
		self.velocity.length();

	PathPlan plan =
		planner.compute(
			travel,
			segment,
			t,
			speed,
			7.5f
		);

	cmd.targetPoint =
		plan.targetPoint;

	/*
	 * Prędkość wynikająca z ograniczenia krzywizny.
	 */
	float desiredSpeed =
		std::min(
			maxSpeed,
			plan.maxCurveSpeed
		);

	/*
	 * --------------------------------------------------------
	 * Przeszkody fizyczne
	 * --------------------------------------------------------
	 */

	if (perception.hasBlockHazard &&
		perception.hazardIsActive)
	{
		const float hazardFactor =
			std::clamp(
				perception.hazardDistance /
				45.0f,
				0.0f,
				1.0f
			);

		desiredSpeed *=
			hazardFactor;
	}

	/*
	 * --------------------------------------------------------
	 * Punkty konfliktowe
	 * --------------------------------------------------------
	 *
	 * Cała logika:
	 *
	 * AV-AV     -> mniejszy margines
	 * AV-Human  -> większy margines
	 */
	evaluateConflictPoints(
		self,
		maxDecel,
		desiredSpeed,
		perception,
		cmd
	);

	/*
	 * Nie pozwalamy na ujemną prędkość docelową.
	 */
	desiredSpeed =
		std::max(
			0.0f,
			desiredSpeed
		);

	/*
	 * --------------------------------------------------------
	 * Model wzdłużny
	 * --------------------------------------------------------
	 */

	cmd.longitudinalAcceleration =
		longitudinalModel->computeAcceleration(
			self,
			perception,
			desiredSpeed,
			maxAccel,
			maxDecel
		);

	/*
	 * --------------------------------------------------------
	 * Awaryjne hamowanie
	 * --------------------------------------------------------
	 */

	if (perception.hasBlockHazard &&
		perception.hazardDistance < 6.0f)
	{
		cmd.emergencyBrake = true;
	}

	if (perception.hasConflict &&
		!perception.alreadyEnteringConflict &&
		perception.conflictDistance < 3.0f)
	{
		cmd.emergencyBrake = true;
	}

	if (perception.hasCarAhead &&
		perception.distanceToCarAhead < 6.0f)
	{
		cmd.emergencyBrake = true;
	}

	cmd.desiredSpeed =
		desiredSpeed;

	return cmd;
}

void BehaviorAV::evaluateConflictPoints(
	const CarState& self,
	float maxDecel,
	float& desiredSpeed,
	const PerceptionState& perception,
	MotionCommand& cmd)
{
	if (!perception.hasConflict)
		return;

	if (perception.alreadyEnteringConflict)
		return;

	if (perception.priorityCarsTTA.empty())
		return;

	/*
	 * Bazowy margines czasowy dla AV-AV.
	 *
	 * 0.8 s pozwala zachować bardziej płynny przejazd
	 * niż w przypadku człowieka.
	 */
	constexpr float kAVMargin = 0.8f;

	/*
	 * Dodatkowy margines wynikający z nieprzewidywalności
	 * kierowcy ludzkiego.
	 */
	constexpr float kHumanExtraMargin = 1.0f;

	/*
	 * Bufor przestrzenny przed strefą konfliktu.
	 */
	constexpr float kAVStopBuffer = 2.0f;
	constexpr float kHumanStopBuffer = 4.0f;

	/*
	 * Analizujemy wszystkie pojazdy mające pierwszeństwo,
	 * nie tylko pierwszy znaleziony.
	 */
	for (const auto& candidate :
		perception.priorityCarsTTA)
	{
		if (candidate.ttaEntry >= 999998.f)
			continue;

		/*
		 * Ustalenie typu pojazdu.
		 */
		const float safetyMargin =
			candidate.isAV
			? kAVMargin
			: kAVMargin + kHumanExtraMargin;

		const float stopBuffer =
			candidate.isAV
			? kAVStopBuffer
			: kHumanStopBuffer;

		/*
		 * Czy istnieje konflikt czasowy?
		 *
		 * Różnica pomiędzy naszym wejściem
		 * a wejściem pojazdu z pierwszeństwem.
		 */
		const float arrivalDifference =
			std::fabs(
				perception.selfTtaEntry -
				candidate.ttaEntry
			);

		/*
		 * Jeżeli różnica czasów jest większa
		 * od wymaganego marginesu, możemy przejechać.
		 */
		if (arrivalDifference >= safetyMargin)
			continue;

		/*
		 * Dodatkowo sprawdzamy faktyczne nakładanie
		 * się przedziałów czasowych.
		 */
		const bool temporalConflict =
			perception.selfTtaEntry <
			candidate.ttaExit &&
			candidate.ttaEntry <
			perception.selfTtaExit;

		if (!temporalConflict)
			continue;

		/*
		 * ----------------------------------------------------
		 * Konflikt rzeczywisty.
		 * Musimy ograniczyć prędkość.
		 * ----------------------------------------------------
		 */

		const float availableDistance =
			std::max(
				0.0f,
				perception.conflictDistance -
				stopBuffer
			);

		/*
		 * Maksymalna prędkość, z której możemy
		 * zatrzymać pojazd przed strefą.
		 */
		const float safeSpeed =
			std::sqrt(
				2.0f *
				maxDecel *
				availableDistance
			);

		desiredSpeed =
			std::min(
				desiredSpeed,
				safeSpeed
			);

		/*
		 * Dla człowieka zachowujemy dodatkowy
		 * margines przestrzenny.
		 */
		if (!candidate.isAV)
		{
			const float conservativeSpeed =
				std::sqrt(
					2.0f *
					maxDecel *
					std::max(
						0.0f,
						availableDistance - 1.0f
					)
				);

			desiredSpeed =
				std::min(
					desiredSpeed,
					conservativeSpeed
				);
		}

		/*
		 * Jeżeli jesteśmy już bardzo blisko strefy
		 * i nadal istnieje konflikt, zatrzymujemy pojazd.
		 */
		if (availableDistance < 1.0f)
		{
			desiredSpeed = 0.0f;
		}

		/*
		 * Awaryjne hamowanie wyłącznie przy małym
		 * dystansie do strefy konfliktowej.
		 */
		if (perception.conflictDistance <
			stopBuffer + 1.0f)
		{
			cmd.emergencyBrake = true;
		}

		/*
		 * Najbardziej krytyczny konflikt wystarczy,
		 * nie trzeba przetwarzać pozostałych.
		 */
		break;
	}
}