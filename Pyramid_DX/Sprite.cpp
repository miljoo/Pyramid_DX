#include<d3d11.h>
#include<d3dx11.h>
#include"Sprite.h"


Sprite::Sprite( ) : rotation_( 0 )
{
	//spriten skaala on 1:1 kuvan kanssa
    scale_.x = scale_.y = 1.0f;
}


Sprite::~Sprite( )
{

}


XMMATRIX Sprite::GetWorldMatrix( )
{
    XMMATRIX translation = XMMatrixTranslation( position_.x, position_.y, 0.0f );
    XMMATRIX rotationZ = XMMatrixRotationZ( rotation_ );
    XMMATRIX scale = XMMatrixScaling( scale_.x, scale_.y, 1.0f );

    return translation * rotationZ * scale;
}


void Sprite::SetPosition( XMFLOAT2& position )
{
    position_ = position;
}


void Sprite::SetRotation( float rotation )
{
    rotation_ = rotation;
}


void Sprite::SetScale( XMFLOAT2& scale )
{
    scale_ = scale;
}

void Sprite::SetZLayer( float layer )
{
    layer_ = layer;
}

void Sprite::SetCurrentFrame( int currentFrame )
{
	currentFrame_ = currentFrame;
}

void Sprite::SetTotalFrames( int totalFrames )
{
	totalFrames_ = totalFrames;
}

XMFLOAT2 Sprite::GetPosition( )
{
    return position_;
}

int Sprite::GetCurrentFrame( )
{
	return currentFrame_;
}

int Sprite::GetTotalFrames( )
{
	return totalFrames_;
}