#include "Skeleton.h"

#include "Globals.h"

Skeleton::Skeleton() :
	pRoot(NULL)
	{
		pRoot = new Node();
}


Skeleton::~Skeleton(){
	SAFEDELETE(pRoot);
}

Node* Skeleton::GetRoot(){
	return pRoot;
}