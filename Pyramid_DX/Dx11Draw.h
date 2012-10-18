#ifndef _DX11DRAW
#define _DX11DRAW

#include"Dx11Base.h"
#include"Sprite.h"


class Dx11Draw : public Dx11Base
{
    public:
        Dx11Draw( );
        virtual ~Dx11Draw( );

        bool LoadContent( );
        void UnloadContent( );

        void Update( float dt );
        void Render( );

    private:
        bool DrawSprite( );

    private:
        ID3D11VertexShader* solidColorVS_;
        ID3D11PixelShader* solidColorPS_;

        ID3D11InputLayout* inputLayout_;
        ID3D11Buffer* vertexBuffer_;

        ID3D11ShaderResourceView* colorMap_;
        ID3D11SamplerState* colorMapSampler_;
        ID3D11BlendState* alphaBlendState_;

		Sprite sprite_;

};

#endif