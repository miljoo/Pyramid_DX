#include"Dx11Draw.h"
#include<xnamath.h>


struct VertexPos
{
    XMFLOAT3 pos;
    XMFLOAT2 tex0;
};


Dx11Draw::Dx11Draw( ) : solidColorVS_( 0 ), solidColorPS_( 0 ),
                              inputLayout_( 0 ), vertexBuffer_( 0 ),
                              colorMap_( 0 ), colorMapSampler_( 0 ),
                              alphaBlendState_( 0 )
{

}


Dx11Draw::~Dx11Draw( )
{

}


bool Dx11Draw::LoadContent( )
{
    ID3DBlob* vsBuffer = 0;

    bool compileResult = CompileD3DShader( "TextureMap.fx", "VS_Main", "vs_4_0", &vsBuffer );

    if( compileResult == false )
    {
        DXTRACE_MSG( "Error compiling the vertex shader!" );
        return false;
    }

    HRESULT d3dResult;

    d3dResult = d3dDevice_->CreateVertexShader( vsBuffer->GetBufferPointer( ),
        vsBuffer->GetBufferSize( ), 0, &solidColorVS_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating the vertex shader!" );

        if( vsBuffer )
            vsBuffer->Release( );

        return false;
    }

    D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    unsigned int totalLayoutElements = ARRAYSIZE( solidColorLayout );

    d3dResult = d3dDevice_->CreateInputLayout( solidColorLayout, totalLayoutElements,
        vsBuffer->GetBufferPointer( ), vsBuffer->GetBufferSize( ), &inputLayout_ );

    vsBuffer->Release( );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating the input layout!" );
        return false;
    }

    ID3DBlob* psBuffer = 0;

    compileResult = CompileD3DShader( "TextureMap.fx", "PS_Main", "ps_4_0", &psBuffer );

    if( compileResult == false )
    {
        DXTRACE_MSG( "Error compiling pixel shader!" );
        return false;
    }

    d3dResult = d3dDevice_->CreatePixelShader( psBuffer->GetBufferPointer( ),
        psBuffer->GetBufferSize( ), 0, &solidColorPS_ );

    psBuffer->Release( );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating pixel shader!" );
        return false;
    }

    d3dResult = D3DX11CreateShaderResourceViewFromFile( d3dDevice_,
        "SpriteSheet.bmp", 0, 0, &colorMap_, 0 );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to load the texture image!" );
        return false;
    }

	XMFLOAT2 spritePosition( 100.0f, 300.0f );
    sprite_.SetPosition( spritePosition );
	sprite_.SetCurrentFrame( 1 );
	sprite_.SetTotalFrames( 2 );

    D3D11_SAMPLER_DESC colorMapDesc;
    ZeroMemory( &colorMapDesc, sizeof( colorMapDesc ) );
    colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

    d3dResult = d3dDevice_->CreateSamplerState( &colorMapDesc, &colorMapSampler_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to create color map sampler state!" );
        return false;
    }

    D3D11_BUFFER_DESC vertexDesc;
    ZeroMemory( &vertexDesc, sizeof( vertexDesc ) );
    vertexDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.ByteWidth = sizeof( VertexPos ) * 6;

    d3dResult = d3dDevice_->CreateBuffer( &vertexDesc, 0, &vertexBuffer_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to create vertex buffer!" );
        return false;
    }
	
	// Spriten mustan läpinäkyvyys
	D3D11_BLEND_DESC blendDesc;
    ZeroMemory( &blendDesc, sizeof( blendDesc ) );
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

    d3dDevice_->CreateBlendState( &blendDesc, &alphaBlendState_ );
    d3dContext_->OMSetBlendState( alphaBlendState_, blendFactor, 0xFFFFFFFF );
	
    return true;
}


bool Dx11Draw::DrawSprite( )
{
    // Size in bytes for a single Dx11Draw.
    const int sizeOfDx11Draw = sizeof( VertexPos ) * 6;


    // Sprites's width on screen.
    float frameWidth = 64 * (2.0f / 800.0f); // ei 1:1 koska piirrettävä alue ei ole tasan WIN ikkunan kokoinen

    // Sprites's height on screen.
    float frameHeight = 64 * (2.0f / 640.0f); // ei 1:1 koska piirrettävä alue ei ole tasan WIN ikkunan kokoinen
    
    // Sprites's texel width (sprite width / sheet width).
    float texelWidth = 64.0f / 192.0f;

    D3D11_MAPPED_SUBRESOURCE mapResource;
    HRESULT d3dResult = d3dContext_->Map( vertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to map resource!" );
        return false;
    }

    // Point to our vertex buffer's internal data.
    VertexPos *Dx11DrawPtr = ( VertexPos* )mapResource.pData;

    float halfWidth = ( float )frameWidth / 2.0f;
    float halfHeight = ( float )frameHeight / 2.0f;

    Dx11DrawPtr[0].pos = XMFLOAT3(  halfWidth,  halfHeight, 1.0f );
    Dx11DrawPtr[1].pos = XMFLOAT3(  halfWidth, -halfHeight, 1.0f );
    Dx11DrawPtr[2].pos = XMFLOAT3( -halfWidth, -halfHeight, 1.0f );

    Dx11DrawPtr[3].pos = XMFLOAT3( -halfWidth, -halfHeight, 1.0f );
    Dx11DrawPtr[4].pos = XMFLOAT3( -halfWidth,  halfHeight, 1.0f );
    Dx11DrawPtr[5].pos = XMFLOAT3(  halfWidth,  halfHeight, 1.0f );

	float tuStart = texelWidth * sprite_.GetCurrentFrame( );
    float tuEnd = tuStart + texelWidth;

    Dx11DrawPtr[0].tex0 = XMFLOAT2( tuEnd, 0.0f );
    Dx11DrawPtr[1].tex0 = XMFLOAT2( tuEnd, 1.0f );
    Dx11DrawPtr[2].tex0 = XMFLOAT2( tuStart, 1.0f );
    Dx11DrawPtr[3].tex0 = XMFLOAT2( tuStart, 1.0f );
    Dx11DrawPtr[4].tex0 = XMFLOAT2( tuStart, 0.0f );
    Dx11DrawPtr[5].tex0 = XMFLOAT2( tuEnd, 0.0f );

    d3dContext_->Unmap( vertexBuffer_, 0 );
    d3dContext_->Draw( 6, 0 );

    return true;
}


void Dx11Draw::UnloadContent( )
{
    if( colorMapSampler_ ) colorMapSampler_->Release( );
    if( colorMap_ ) colorMap_->Release( );
    if( solidColorVS_ ) solidColorVS_->Release( );
    if( solidColorPS_ ) solidColorPS_->Release( );
    if( inputLayout_ ) inputLayout_->Release( );
    if( vertexBuffer_ ) vertexBuffer_->Release( );

    colorMapSampler_ = 0;
    colorMap_ = 0;
    solidColorVS_ = 0;
    solidColorPS_ = 0;
    inputLayout_ = 0;
    vertexBuffer_ = 0;
}


void Dx11Draw::Update( float dt )
{
    // Nothing to update
}


void Dx11Draw::Render( )
{
    if( d3dContext_ == 0 )
        return;

    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    d3dContext_->ClearRenderTargetView( backBufferTarget_, clearColor );

    unsigned int stride = sizeof( VertexPos );
    unsigned int offset = 0;

    d3dContext_->IASetInputLayout( inputLayout_ );
    d3dContext_->IASetVertexBuffers( 0, 1, &vertexBuffer_, &stride, &offset );
    d3dContext_->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    d3dContext_->VSSetShader( solidColorVS_, 0, 0 );
    d3dContext_->PSSetShader( solidColorPS_, 0, 0 );
    d3dContext_->PSSetShaderResources( 0, 1, &colorMap_ );
    d3dContext_->PSSetSamplers( 0, 1, &colorMapSampler_ );
    
    DrawSprite( );

    swapChain_->Present( 0, 0 );
}