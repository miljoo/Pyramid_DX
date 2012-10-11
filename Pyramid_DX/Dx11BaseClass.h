#ifndef _BASE_CLASS_H_
#define _BASE_CLASS_H_

#include<d3d11.h>
#include<d3dx11.h>
#include<DxErr.h>
#include <D3Dcompiler.h>

#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>

class Dx11BaseClass
{
    public:
        Dx11BaseClass( );
        virtual ~Dx11BaseClass( );

        bool Initialize( HINSTANCE hInstance, HWND hwnd );
        void Shutdown( );

        bool CompileD3DShader( char* filePath, char* entry,
                               char* shaderModel, ID3DBlob** buffer );

        virtual bool LoadContent( );
        virtual void UnloadContent( );

        virtual void Update( float dt ) = 0;
        virtual void Render( ) = 0;

    protected:
        HINSTANCE hInstance_;
        HWND hwnd_;

        D3D_DRIVER_TYPE driverType_;
        D3D_FEATURE_LEVEL featureLevel_;

        ID3D11Device* d3dDevice_;
        ID3D11DeviceContext* d3dContext_;
        IDXGISwapChain* swapChain_;
        ID3D11RenderTargetView* backBufferTarget_;

        LPDIRECTINPUT8 directInput_;
        LPDIRECTINPUTDEVICE8 keyboardDevice_;
		char keyboardKeys_[256];
        char prevKeyboardKeys_[256];

		LPDIRECTINPUTDEVICE8 mouseDevice_;
        DIMOUSESTATE mouseState_;
        DIMOUSESTATE prevMouseState_;
        long mousePosX_;
        long mousePosY_;
        long mouseWheel_;
};

#endif