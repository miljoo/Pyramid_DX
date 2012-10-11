#ifndef _KEYBOARD_DEMO_H_
#define _KEYBOARD_DEMO_H_

#include"Dx11BaseClass.h"

class KeyboardDemo : public Dx11BaseClass
{
    public:
        KeyboardDemo( );
        virtual ~KeyboardDemo( );

        bool LoadContent( );
        void UnloadContent( );

        void Update( float dt );
        void Render( );

    private:
        ID3D11VertexShader* customColorVS_;
        ID3D11PixelShader* customColorPS_;

        ID3D11InputLayout* inputLayout_;
        ID3D11Buffer* vertexBuffer_;

		ID3D11Buffer* colorCB_;
		int selectedColor_;
};

#endif