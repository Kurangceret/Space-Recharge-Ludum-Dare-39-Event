#include "Math.h"


sf::Vector2f Math::adjustVectorWithCombination(const sf::Vector2f& vecToAdjust, float divideVal)
{
	sf::Vector2f vecDir = Math::unitVector(vecToAdjust);
	float vecDirCombination = std::abs(vecDir.x) + std::abs(vecDir.y);

	sf::Vector2f finalV = vecToAdjust;
	if (vecDirCombination >= divideVal){
		//float combinationVelo = std::abs(vecToAdjust.x) + std::abs(vecToAdjust.y);
		finalV /= std::sqrt(vecDirCombination);
	}
	return finalV;
}
