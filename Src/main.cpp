#include <Windows.h>
#include <libpois0n.h>
#include <stdio.h>
#include <stdlib.h>
#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static HINSTANCE ghInstance = NULL;

HWND hButton;
HWND hButton2;
HWND hButton3;
HWND hButton4;
HWND hButton5;
HWND hButton6;
HWND hButton7;

irecv_error_t error;

lockdownd_client_t normal_client = NULL;
char *uuid = NULL;
idevice_t idevice = NULL;

char* serial = NULL;
char* serial2 = NULL;
char* serial3 = NULL;
plist_t value = NULL;
plist_t value2 = NULL;
plist_t value3 = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX WndClass;
	HWND hWnd;
	MSG Msg;

	ghInstance = hInstance;

	WndClass.cbSize = sizeof(WNDCLASSEX);
	WndClass.style = NULL;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = ghInstance;
	WndClass.hIcon = LoadIcon(NULL, (LPCSTR)IDI_ICON1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)(0);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "iBooter";
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&WndClass))
	{
		MessageBox(0, "Error Registering Window!", "Error!", MB_ICONSTOP | MB_OK);
		return 0;
	}

	hWnd = CreateWindowEx(WS_EX_STATICEDGE, "iBooter", "iBooter", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 270, 400, NULL, NULL, ghInstance, NULL);

	if(hWnd == NULL)
	{
		MessageBox(0, "Window Creation Failed!", "Error!", MB_ICONSTOP | MB_OK);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while(GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_CREATE:
			hButton = CreateWindowEx(NULL, "Button", "Exit Recovery", WS_BORDER | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 40, 70, 170, 30, hWnd, (HMENU)1, ghInstance, NULL);
			hButton2 = CreateWindowEx(NULL, "Button", "Enter Recovery", WS_BORDER | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 40, 20, 170, 30, hWnd, (HMENU)2, ghInstance, NULL);
			hButton5 = CreateWindowEx(NULL, "Button", "Enter Pwned-DFU-Mode", WS_BORDER | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 40, 120, 170, 30, hWnd, (HMENU)5, ghInstance, NULL);
			hButton4 = CreateWindowEx(NULL, "Button", "Get Mode", WS_BORDER | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 40, 220, 170, 30, hWnd, (HMENU)4, ghInstance, NULL);
			hButton6 = CreateWindowEx(NULL, "Button", "Exit DFU-Mode", WS_BORDER | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 40, 170, 170, 30, hWnd, (HMENU)6, ghInstance, NULL);
			hButton6 = CreateWindowEx(NULL, "Button", "Get Device Info", WS_BORDER | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 40, 270, 170, 30, hWnd, (HMENU)7, ghInstance, NULL);
			hButton3 = CreateWindow("Button", "Credits", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 320, 220, 20, hWnd, (HMENU)3, ghInstance, NULL);
			break;
		case WM_CLOSE:
            DestroyWindow(hWnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_COMMAND:
			switch(wParam)
			{
				case 1:
					irecv_init();

					error = irecv_open_attempts(&client, 10);

					if(error != IRECV_E_SUCCESS)
					{
						MessageBox(hWnd, "Connecting To Device Failed!", "Error", NULL);
						return 0;
					}

					if(client->mode == kRecoveryMode1 || client->mode == kRecoveryMode2 || client->mode == kRecoveryMode3 || client->mode == kRecoveryMode4)
					{
						send_command("setenv auto-boot true");
						send_command("saveenv");
						send_command("reboot");
						irecv_close(client);
					}
					else
					{
						MessageBox(hWnd, "No Device In Recovery-Mode Found!", "Error", NULL);
					}
					break;

				case 2:
					idevice_new(&idevice, uuid);

					if(!idevice)
					{
						MessageBox(hWnd, "No Device In Normal-Mode Found!", "Error", NULL);
					}

					lockdownd_client_new(idevice, &normal_client, "test");
	
					lockdownd_enter_recovery(normal_client);	
		
					lockdownd_client_free(normal_client);
					client = NULL;

					uuid = NULL;
					free(uuid);
					idevice = NULL;
					free(idevice);
					break;

				case 3:
					MessageBox(hWnd, "Credits:\niBooter - Ra1ningsn0w\nSyringe - Chronic-Dev Team\nLibimobiledevice - Nikias Bassen\n", "Credits", NULL);
					break;

				case 4:
					irecv_init();
					error = irecv_open_attempts(&client, 10);
					idevice_new(&idevice, uuid);

					if(client->mode == kRecoveryMode1 || client->mode == kRecoveryMode2 || client->mode == kRecoveryMode3 || client->mode == kRecoveryMode4)
					{
						MessageBox(hWnd, "Found device in Recovery-Mode!", "Get Mode", NULL);
						irecv_close(client);
					}
					else if(client->mode == kDfuMode)
					{
						MessageBox(hWnd, "Found device in DFU-Mode!", "Get Mode", NULL);
						irecv_close(client);
					}
					else if(idevice)
					{
						MessageBox(hWnd, "Found device in Normal-Mode!", "Get Mode", NULL);
						idevice = NULL;
						idevice_free(idevice);
						uuid = NULL;
						free(uuid);
						idevice = NULL;
						free(idevice);
					}
					else
					{
						MessageBox(hWnd, "Found no device!", "Get Mode", NULL);
					}
					break;

				case 5:
					irecv_init();
					irecv_open_attempts(&client, 10);
					irecv_get_device(client, &device);

					if(client->mode == kDfuMode)
					{
						if(device->chip_id == 8930 || device->chip_id == 8922 || device->chip_id == 8920 || device->chip_id == 8720)
						{
							pois0n_injectonly();

							MessageBox(hWnd, "Your device is now in pwned DFU-Mode!", "Error", NULL);

							return 0;
						}
						else
						{
							MessageBox(hWnd, "Your device is not compatible with this method!", "Error", NULL);
						}
					}
					else
					{
						MessageBox(hWnd, "Found no device in DFU-Mode!", "Error", NULL);
					}

					break;

				case 6:
					irecv_init();
					irecv_open_attempts(&client, 10);
					irecv_get_device(client, &device);

					if(client->mode == kDfuMode)
					{
						if(device->chip_id == 8930 || device->chip_id == 8922 || device->chip_id == 8920 || device->chip_id == 8720)
						{
							pois0n_injectonly();

							upload_ibss();

							irecv_reconnect(client, 10);

							send_command("reboot");

							return 0;
						}
						else
						{
							MessageBox(hWnd, "Your device is not compatible with this method!", "Error", NULL);
						}
					}
					else
					{
						MessageBox(hWnd, "Found no device in DFU-Mode!", "Error", NULL);
					}

					break;

				case 7:
					idevice_new(&idevice, uuid);
					lockdownd_client_new(idevice, &normal_client, "test");
					lockdownd_get_value(normal_client, NULL, "BuildVersion", &value);
					plist_get_string_val(value, &serial);
					lockdownd_get_value(normal_client, NULL, "ProductVersion", &value2);
					plist_get_string_val(value2, &serial2);
					lockdownd_get_value(normal_client, NULL, "DeviceClass", &value3);
					plist_get_string_val(value3, &serial3);

					MessageBox(hWnd, (LPCSTR)serial3, "Device Class", NULL);
					MessageBox(hWnd, (LPCSTR)serial2, "iOS", NULL);
					MessageBox(hWnd, (LPCSTR)serial, "Build", NULL);

				}
				default:
					return DefWindowProc(hWnd, Message, wParam, lParam);
	}

	return 0;
}