#pragma once

#include "Node.h"

class Skeleton
{
public:
	Skeleton();
	virtual ~Skeleton();

	Node* GetRoot();
private:
	Node* pRoot;
};

