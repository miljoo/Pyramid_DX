#ifndef _GAMEOBJECT
#define _GAMEOBJECT

#include"Dx11Base.h"
#include "Sprite.h"

class GameObject
{
    public:
        GameObject( );
        virtual ~GameObject( );

		void SetObjectId( int id );
		void SetSprite( Sprite sprite );


    private:
        int id_;
		Sprite sprite_;
};

#endif