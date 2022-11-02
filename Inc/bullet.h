#define BULLET_CAP 50
#define BULLET_MOB 0
#define BULLET_PLAYER 1
#define TRUE 1
#define FLASE 0
#define PBULLET_NORMAL 1
#define BULLET_TEST 2

typedef struct Bullet
{
	float x;
	float y;
	float degree;
	float speed;
	float size;
	float traveldistance;
	float maxdistance;
	float timer;
	float damage;
	int type;
	int friendly;
	int exist;
}Bullet;

Bullet bullet[BULLET_CAP];

// Reset all data in bullet[i]
void BulletReset(void);

// Call BulletReset for all bullets
void Bulletinit(void);

// Sets the bullet Coords and angle (x,y,degree)
void BulletCoor(float, float, float);

// Sets the data for bullet type presets
void BulletType(int);

// Calculate the direction of the bullet
void BulletDirection(float, int);

// Call to shoot bullet
void BulletShoot(float, float, float, int, int);

// Draw the location of all bullets
void BulletDraw(void);

// Check existing bullets collision against a target
int BulletCollision(float, float, float);

