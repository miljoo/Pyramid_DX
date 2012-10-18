#include "GameObject.h"

GameObject::GameObject( )
{

}

GameObject::~GameObject( )
{

}

void GameObject::SetObjectId( int id )
{
	id_ = id;
}

void GameObject::SetSprite( Sprite sprite )
{
	sprite_ = sprite;
}