
/*
	Title
		: 0 => Mob
		: 1 => MiniBoss
		: 2 => Boss
*/

typedef struct Coordinates {
	double x;
	double y;
}Coor;

typedef struct MobBase {
	int Title;

	double HP;
	double DEF;
	double Speed;

	int Range;
	double Dmg;
	int size;
	Coor coor;
	
}Mob;



Mob CreateMob(int Title, double hp, double def, double speed, int Range, double Dmg, double x, double y, int size);

void MobFactory(Mob *arr[], int MobQuantity, int mobCount);

void DrawMob(Mob *mob);

double drand(double low, double high);
