#pragma once
#include "Component.h"
#include "../Map/MovingNode.h"
//#include "PathFinder.h"
#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>

class AutomaticPathComponent :	public Component{
public:
	//System has to iterate this type from the front
	typedef std::vector<MovingNode> AutomaticPathList;
public:
	AutomaticPathComponent(Entity* entity);
	virtual ~AutomaticPathComponent();

	void drawAutomaticPaths(sf::RenderTarget& target);

	void setDestinationToCalculate(GridNode* destNode);
	void setDestinationToCalculate(const sf::Vector2f& pos);
	void setDestinationToCalculate(float posX, float posY);

	/*exclusive for script usage*/
	void setDestToCalculate(float posX, float posY);
	/**/

	bool isAutomaticPathsEmpty() const;

	void clearAutomaticPaths();
	void setAutomaticPaths(const std::vector<MovingNode>& automaticPaths, 
		GridNode* finalDestNode);

	GridNode* getCurrentDestNode() const;

	sf::Vector2f getUncalculatedDest() const;

	AutomaticPathList& getAutomaticPaths();


	bool isUncalculatedPosValid() const;

	void setPathSmooth(bool flag);
	bool isPathSmooth() const;
private:
	//value that indicates the returned getUncalculatedDest() equals to this return
	//means that it does not need to be calculated
	sf::Vector2f getNoNeedCalculationVal() const;

private:
	AutomaticPathList mAutomaticPaths;
	sf::Vector2f mUncalculatedDest;
	GridNode* mCurrentDestNode;

	bool m_is_path_smooth;
};

