#include <Windows.h>
#include <memory>
#include "KeyboardDemo.h"

LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow )
{
    UNREFERENCED_PARAMETER( prevInstance );
    UNREFERENCED_PARAMETER( cmdLine );

	// This struct holds information for the window class
    WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(WNDCLASSEX));
    wndClass.cbSize = sizeof( WNDCLASSEX ) ;
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.hInstance = hInstance;
    wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
    wndClass.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = "DX11 Class";

	// Register the window class
    if( !RegisterClassEx( &wndClass ) ) {
        return -1;
	}

	// Window (outer) size
    RECT rc = { 0, 0, 640, 480 };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	// Create the window
    HWND hwnd = CreateWindowA( "DX11 Class", "Input Direct3D 11", WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
                                NULL, NULL, hInstance, NULL );

    if( !hwnd ) {
        return -1;
	}

	// Display the window
    ShowWindow( hwnd, cmdShow );

	// Demo handle
    KeyboardDemo demo;

    // Demo Initialize
    bool result = demo.Initialize( hInstance, hwnd );

    if( result == false ) {
        return -1;
	}

    MSG msg = { 0 };

	// Game loop
    while( msg.message != WM_QUIT )
    {
        if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
        {
			// Translate keystroke message into the right format
            TranslateMessage( &msg );
			// Send the message to the WindowProc function
            DispatchMessage( &msg );
        }

        // Update and Draw
		demo.Update( 1.0f );
        demo.Render( );
    }

    // Demo Shutdown
    demo.Shutdown( );

	// Return this part of the WM_QUIT message to Windows
    return static_cast<int>( msg.wParam );
}


LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT paintStruct;
    HDC hDC;

	// Sort through and find what code to run for the message given
    switch( message )
    {
        case WM_PAINT:
            hDC = BeginPaint( hwnd, &paintStruct );
            EndPaint( hwnd, &paintStruct );
            break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

        default:
            return DefWindowProc( hwnd, message, wParam, lParam );
    }

    return 0;
}