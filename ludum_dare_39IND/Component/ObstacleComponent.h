#pragma once
#include "Component.h"

class ComponentArranger;
class ObstacleComponent :	public Component{
public:
	friend class ComponentArranger;
public:
	ObstacleComponent(Entity* entity);
	virtual ~ObstacleComponent();

	bool isDestroyer() const;
	std::size_t getObstacleCategory() const;
private:
	std::size_t m_obstacle_category;
	bool mIsDestroyer;
};

