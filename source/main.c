#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include <citro2d.h>
#include <tex3ds.h>

typedef struct {
	float x, y;
	float w, h;
	u32 color;
} Rectangle;
	
typedef struct {
	int id;
	float x, y;
	float w, h;
	u32 color;
	bool exists;
	int zOrd;
	bool clickable;
	char name[64];
} Window;

int menuFunction() {
	return 0;
}

bool MenuOpen = false;

int CheckExist(Window windows[], int max, int clickX, int clickY) {
	for (int i = 0; i < max; i++) {
		if (!windows[i].exists || !windows[i].clickable) {
			continue;
		}
		
		if (clickX >= windows[i].x && clickX <= windows[i].x + windows[i].w && clickY >= windows[i].y && clickY <= windows[i].y + windows[i].h) {
			printf("Window %d exists and was clicked!\n", windows[i].id);
			if (strcmp(windows[i].name, "Menu-Button") == 0) {
				MenuOpen = !MenuOpen;
				if (MenuOpen) {
					windows[1].exists = true;
					windows[1].clickable = true;
					windows[1].x = 0.0f;
					windows[1].y = 100.0f;
					windows[1].w = 100.0f;
					windows[1].h = 120.0f;
					windows[1].color = C2D_Color32(255, 255, 255, 255);
					windows[1].id = 2;
				} else {
					windows[1].exists = false;
					windows[1].clickable = false;
				}
				return 0;
			} else if (windows[i].id == 0) {
				return 0;
			} else if (windows[i].id == 2) {
				windows[1].exists = true;
				windows[1].clickable = true;
				return 0;
			} else {
				windows[1].exists = false;
				windows[1].clickable = false;
				printf("Unknown window ID: %d\n", windows[i].id);
			}
		}
	}
	return 0;
}

int main() {
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	
	consoleInit(GFX_BOTTOM, NULL);
	gfxSetDoubleBuffering(GFX_BOTTOM, false);
	
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	
	#define MAX_WINDOWS 20
	
	Window windows[MAX_WINDOWS] = {0};
	for (int i = 0; i < MAX_WINDOWS; i++) {
		windows[i].color = C2D_Color32(255, 255, 255, 255);
	}
	
	Window taskbar = {1, 0.0f, 220.0f, 400.0f, 20.0f, C2D_Color32(250, 250, 250, 255), true, 18, true};
	
	// Menu Window
	strcpy(windows[0].name, "Menu-Button");
	windows[0].color = C2D_Color32(100, 255, 100, 255);
	windows[0].h = 15;
	windows[0].w = 20;
	windows[0].y = (taskbar.y + 2);
	windows[0].x = (taskbar.x + 2);
	windows[0].id = 1;
	windows[0].exists = true;
	windows[0].clickable = true;
	
	Rectangle mouse = {0.0f, 0.0f, 5.0f, 5.0f, C2D_Color32(0, 0, 0, 255)};
	int frame = 0;
	
	printf("This is a program made for 2ds\n");
	
	while (aptMainLoop()) {
		hidScanInput();
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		
		if (kDown & KEY_SELECT) {
			break;
		}
		if (kDown & KEY_A) {
			CheckExist(windows, MAX_WINDOWS, mouse.x, mouse.y);
		}
		if (kDown & KEY_B) {
			return 0;
		}
		if (kHeld & KEY_CPAD_RIGHT) {
			mouse.x += 2.0f;
		}
		if (kHeld & KEY_CPAD_LEFT) {
			mouse.x -= 2.0f;
		}
		if (kHeld & KEY_CPAD_UP) {
			mouse.y -= 2.0f;
		}
		if (kHeld & KEY_CPAD_DOWN) {
			mouse.y += 2.0f;
		}
		
		frame++;
		if (frame % 2 != 0) {
			continue;
		}
		
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_SceneBegin(top);
		C2D_TargetClear(top, C2D_Color32(155, 155, 255, 255));
		
		for (int i = 0; MAX_WINDOWS > i; i++) {
			if (windows[i].exists) {
				C2D_DrawRectSolid(windows[i].x, windows[i].y, 0.0f, windows[i].w, windows[i].h, windows[i].color);
			}
		}
		
		C2D_DrawRectSolid(taskbar.x, taskbar.y, 0.0f, taskbar.w, taskbar.h, taskbar.color);
		C2D_DrawRectSolid(windows[0].x, windows[0].y, 0.0f, windows[0].w, windows[0].h, windows[0].color);
		C2D_DrawRectSolid(mouse.x, mouse.y, 0.0f, mouse.w, mouse.h, mouse.color);
		C3D_FrameEnd(0);
	}   
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}