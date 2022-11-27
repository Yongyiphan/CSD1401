#include "CProcessing.h"

static CP_Image spriteSheetImage;
static float timeElapsed;
static const float DISPLAY_DURATION = 2.0f;
static int imageIndex;
static float middleX, middleY;
static const float FRAME_DIMENSION = 8.0f;

#define SPRITESHEET_DEMO_1 0

void SpriteSheetDemo_Init(void)
{
	/**Notice the file paths here are with respect to the Assets folder. 
	Put the given spritesheet samples into the assets folder*/
#if SPRITESHEET_DEMO_1 
	spriteSheetImage = CP_Image_Load("./Assets/SpriteSheetSample.png");
#else
	spriteSheetImage = CP_Image_Load("./Assets/SpriteSheetSample2.png");
#endif
	imageIndex = 0;
	timeElapsed = 0.0f;

	middleX = (CP_System_GetWindowWidth()) / 2.0f;
	middleY = (CP_System_GetWindowHeight()) / 2.0f;
}

void SpriteSheetDemo_Update(void)
{
	timeElapsed += CP_System_GetDt();
	if (timeElapsed >= DISPLAY_DURATION)
	{
#if SPRITESHEET_DEMO_1 
		imageIndex = (imageIndex + 1) % 4;
#else
		imageIndex = (imageIndex + 1) % 8;
#endif
		timeElapsed = 0.0f;
	}
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 0));

#if SPRITESHEET_DEMO_1 
	CP_Image_DrawSubImage(spriteSheetImage,
		middleX, middleY,
		200.0f, 200.0f,
		imageIndex * FRAME_DIMENSION, 0,
		(imageIndex + 1) * FRAME_DIMENSION,  FRAME_DIMENSION,
		255);
#else
	CP_Image_DrawSubImage(spriteSheetImage,
		middleX, middleY,
		200.0f, 200.0f,
		(imageIndex % 4) * FRAME_DIMENSION, (imageIndex < 4) ? 0 : FRAME_DIMENSION ,
		((imageIndex % 4) + 1) * FRAME_DIMENSION, (imageIndex < 4) ? FRAME_DIMENSION : FRAME_DIMENSION * 2,
		255);
#endif
}

void SpriteSheetDemo_Exit()
{
	CP_Image_Free(&spriteSheetImage);
}