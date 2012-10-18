#ifndef _SPRITE
#define _SPRITE

#include<xnamath.h>


class Sprite
{
    public:
        Sprite( );
        virtual ~Sprite( );

        XMMATRIX GetWorldMatrix( );

        void SetPosition( XMFLOAT2& position );
        void SetRotation( float rotation );
        void SetScale( XMFLOAT2& scale );
		void SetZLayer( float layer );
		void SetCurrentFrame( int currentFrame );
		void SetTotalFrames( int totalFrames );

		XMFLOAT2 GetPosition( );
		int GetCurrentFrame( );
		int GetTotalFrames( );


    private:
        XMFLOAT2 position_;
        float rotation_;
        XMFLOAT2 scale_;
		float layer_;
		int currentFrame_;
		int totalFrames_;
};

#endif