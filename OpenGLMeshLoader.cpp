#include "TextureBuilder.h"
#include <SFML/Audio.hpp>
#include "Model_3DS.h"
#include "GLTexture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "glut.h"
#include <iostream>
#include <chrono>
std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();

#define DEG2RAD(a) (a * 0.0174532925)
#define _USE_MATH_DEFINES // Define this before including cmath
#include <cmath>
#include <math.h>
int WIDTH = 1400;
int HEIGHT = 600;

GLuint tex;
GLuint circleTexture;
char title[] = "3D Model Loader Sample";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;

float playerPosX = -10.0f;
float playerPosY = 0.0f;
float playerPosZ = 0.0f;
float playerRotation = 90.0f;

float player2PosX = -10.0f;
float player2PosY = 0.0f;
float player2PosZ = 10.0f;
float player2Rotation = 180.0f;

float goalkeeperX = 10.5f;
float goalkeeperY = 0.0f;
float goalkeeperZ = 0.0f;
float goalkeeperRotation = -90.0f;
float goalkeeperRotSaving = 0.0f;
// Global game boundary variables based on ball position
float minX = -10.0f; // Left boundary
float maxX = -5.0f;  // Right boundary

float ballPosX = -5;
float ballPosY = 0;
float ballPosZ = 0;

int isThirdPersonView = 0;
int isFirstPersonView = 0;

boolean gameOver = false;
int isPlayerMoving = 0; // 0: Not moving, 1: Moving towards the ball

int targetRegion = 0;
int isBallMoving = 0;
int isBallDropping = 0;
int oneOut = 0;
int firstPlayerReturn = 0;
int currentlyPlaying = 1;
int isPlayer2Moving = 0;
int secondPlayerReturn = 0;
int twoOut = 0;
int currP = 1;
int goalkeeperTargetRegion = 0;
int isGoalkeeperMoving = 0;
int isGoalkeeperReturning = 0;

bool inDelay = false;

int player1Shots = 0;
int player2Shots = 0;
int player1Score = 0;
int player2Score = 0;
const int maxScore = 2; // Maximum score per player

// Global variables for light intensity
GLfloat currentLightIntensity = 1.0f; // Start with full intensity (daylight)
GLfloat intensityChangeRate = 0.02f; // Rate at which the light intensity changes
bool increasingIntensity = false; // Start with decreasing intensity (moving towards night)


bool isCelebrating = false;
float celebrationRotationAngle = 90.0f;
bool isIn1 = false;
bool isIn2 = false;
bool isIn1SecondEnvironment = false;
bool isIn2SecondEnvironment = false;

int CounterEnvironment2 = 0;
boolean SecondEnvironment = false;

float streetballPosX = -5;
float streetballPosY = 0.2;
float streetballPosZ = 0;

float streetplayerPosX = -10.0f;
float streetplayerPosY = 0.0f;
float streetplayerPosZ = 0.0f;
float streetplayerRotation = 90.0f;

float streetgoalkeeper1X = 9.2f;
float streetgoalkeeper1Y = 0.0f;
float streetgoalkeeper1Z = 0.0f;
float streetgoalkeeperRotation1 = -90.0f;
float streetgoalkeeperRotSaving1 = 0.0f;

float streetgoalkeeper2X = 9.2f;
float streetgoalkeeper2Y = 0.0f;
float streetgoalkeeper2Z = 10.0f;
float streetgoalkeeperRotation2 = 180.0f;
float streetgoalkeeperRotSaving2 = 0.0f;
boolean Currgoalkeeper = true; //represents keeper 1
int keeper1out = 0;
int keeper2out = 0;
int isStreetPlayerMoving = 0; // 0: Not moving, 1: Moving towards the ball
int isStreetBallMoving = 0;
int isStreetBallDropping = 0;
int streetPlayerReturn = 0;
int goalKeeper1Score = 0;
int goalKeeper2Score = 0;
int goalKeeper1Shots = 0;
int goalKeeper2Shots = 0;

int isStreetGoalkeeperMoving = 0;
int isStreetGoalkeeperReturning = 0;


boolean End = false;


bool player1HasNotEnteredGoal = false;
bool player2HasNotEnteredGoal = false;
bool player1BlockedGoal = false;
bool player2BlockedGoal = false;


int timeOfDay = 9;
bool firstTime = false;
float elapsedTime = 0.0f; // Time elapsed since the start of the simulation
const float cycleDuration = 24.0f;

sf::Sound sound;
sf::SoundBuffer sound_Buffer;
sf::SoundBuffer targetSoundBuffer;
sf::Sound targetSound;
sf::SoundBuffer collSoundBuffer;
sf::Sound collSound;
sf::SoundBuffer winSoundBuffer;
sf::Sound winSound;

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};
class Color {
private:
	float r;
	float g;
	float b;

public:
	// Constructor
	Color() : r(0.2f), g(0.2f), b(0.2f) {}
	Color(float red, float green, float blue) : r(red), g(green), b(blue) {}

	// Setter function to set the color values
	void set(float red, float green, float blue) {
		r = red;
		g = green;
		b = blue;
	}

	// Getter functions (optional)
	float getRed() const { return r; }
	float getGreen() const { return g; }
	float getBlue() const { return b; }
};


Vector Eye(20, 5, 20);
Vector At(0, 0, 0);
Vector Up(0, 1, 0);
Color player1ScoresColors[6];
Color player2ScoresColors[6];
Color player1ScoresColorsSecondEnvironment[6];
Color player2ScoresColorsSecondEnvironment[6];

int cameraZoom = 0;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;
Model_3DS model_soccer_player;
Model_3DS model_court;
Model_3DS model_ball;
Model_3DS model_net;
Model_3DS model_soccer2;
Model_3DS model_goalkeeper;
Model_3DS model_lamp;
Model_3DS model_lamp2;
Model_3DS model_streetball;
Model_3DS model_streetplayer;
Model_3DS model_streetgoalkeeper1;
Model_3DS model_streetgoalkeeper2;
// Textures
GLTexture tex_ground;
GLTexture tex_ground2;
GLTexture tex_wall_front;
GLTexture tex_wall_front2;

//=======================================================================
// Lighting Configuration Function
//=======================================================================
int lightX = 0;
void InitLightSource()
{

}

void setupLight() {
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.3f, 0.3f, 0.25f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.8f, 0.8f, 0.6f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 8.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);



	


}

void updateSunlight(float timeOfDay) {
	// Assuming timeOfDay is a float varying from 0.0 (midnight) to 24.0 (end of the day)
	float normalizedTime = fmod(timeOfDay, 24.0f) / 24.0f; // Normalize to 0.0 - 1.0

	// Calculate sun position and color based on time
	// For simplicity, let's assume the sun rises at 6 AM and sets at 6 PM
	float sunIntensity;
	GLfloat light_color[4];

	if (normalizedTime < 0.25) { // 6 AM - Noon
		sunIntensity = normalizedTime / 0.25; // Increase intensity
		light_color[0] = 0.8f * sunIntensity + 0.2f; // Becoming less red
		light_color[1] = 0.8f * sunIntensity + 0.2f; // Becoming less green
		light_color[2] = 0.6f * sunIntensity + 0.4f; // Becoming less blue
		light_color[3] = 1.0f;
	}
	else if (normalizedTime < 0.5) { // Noon - 6 PM
		sunIntensity = 1.0;
		light_color[0] = 0.8f;
		light_color[1] = 0.8f;
		light_color[2] = 0.6f;
		light_color[3] = 1.0f;
	}
	else if (normalizedTime < 0.75) { // 6 PM - Midnight
		sunIntensity = (0.75 - normalizedTime) / 0.25; // Decrease intensity
		light_color[0] = 0.8f * sunIntensity + 0.2f;
		light_color[1] = 0.8f * sunIntensity + 0.2f;
		light_color[2] = 0.6f * sunIntensity + 0.4f;
		light_color[3] = 1.0f;
	}
	else { // Midnight - 6 AM
		sunIntensity = 0.0;
		light_color[0] = 0.2f;
		light_color[1] = 0.2f;
		light_color[2] = 0.4f;
		light_color[3] = 1.0f;
	}

	// Update light position based on time of day
	GLfloat light_position[] = { cos(2 * M_PI * normalizedTime), sin(2 * M_PI * normalizedTime), 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	// Update light color
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_color); // You might want to have different ambient color
}

class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 1.0f, float eyeY = 1.0f, float eyeZ = 1.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		//eye = Vector3f(eyeX, eyeY, eyeZ);
		//center = Vector3f(centerX, centerY, centerZ);
		//up = Vector3f(upX, upY, upZ);


			//printf("%s", "falseeeeeeeeeeeeeeeeeeeee");

		eye = { -15.935677, 3.037808, -1.251460 };
		center = { -13.950855 ,2.888865,   -1.024079 };
		up = { 3.0, 25.0, 0.5 };

	}
	void updatePose() {
		eye = { 16.917969, 4.320749, 0.393479 };
		center = { 15.959846 ,4.034554,   0.383686 };
		up = { -0.285830, 0.957863, 0.028274 };
	}

	void drawCameraValues() {
		printf("\neyeX %f\neyeY %f\neyeZ %f\ncenterX %f\ncenterY %f\ncenterZ %f\nupX %f\nupY %f\nupZ %f", eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}

};

Camera camera;

void Special(int key, int x, int y) {
	float a = 1.0;

	switch (key) {
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}
void renderBitmapString(float x, float y, float z, void* font, const char* string) {
	const char* c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}
void displayWinner() {
	std::string winnerText;
	if (player1Score > player2Score) {
		winnerText = "Player 1 wins with a score of " + std::to_string(player1Score);
	}
	else if (player2Score > player1Score) {
		winnerText = "Player 2 wins with a score of " + std::to_string(player2Score);
	}
	else {
		winnerText = "It's a draw!";
	}

	// Display the text at a specific position
	renderBitmapString(200, 300, 0, GLUT_BITMAP_TIMES_ROMAN_24, winnerText.c_str());
}
void renderNewScene() {
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set up a new projection and modelview matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WIDTH, 0, HEIGHT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Here, you can add your logic to draw the new scene

	glutSwapBuffers();
}
void UpdateStreetCameraPosition() {
	if (!isThirdPersonView) {
		// Existing code for updating the camera position
	}
	else {
		if (Currgoalkeeper) {
			float distanceBehindPlayer = 5.0f; // Distance behind the player
			float heightAbovePlayer = 5.0f; // Height above the player

			// Calculate the camera position based on player's position and angle
			float radianAngle = DEG2RAD(streetgoalkeeperRotation1);
			float camX = streetgoalkeeper1X - distanceBehindPlayer * sin(radianAngle);
			float camY = streetgoalkeeper1Y + heightAbovePlayer;
			float camZ = streetgoalkeeper1Z - distanceBehindPlayer * cos(radianAngle);

			// Update camera position
			camera.eye.x = camX;
			camera.eye.y = camY;
			camera.eye.z = camZ;

			// Update camera's center to look at the player
			camera.center.x = streetgoalkeeper1X;
			camera.center.y = streetgoalkeeper1Y + 2.4;
			camera.center.z = streetgoalkeeper1Z;
		}
		else {
			float distanceBehindPlayer = 5.0f; // Distance behind the player
			float heightAbovePlayer = 5.0f; // Height above the player

			// Calculate the camera position based on player's position and angle
			float radianAngle = DEG2RAD(streetgoalkeeperRotation2);
			float camX = streetgoalkeeper2X - distanceBehindPlayer * sin(radianAngle);
			float camY = streetgoalkeeper2Y + heightAbovePlayer;
			float camZ = streetgoalkeeper2Z - distanceBehindPlayer * cos(radianAngle);

			// Update camera position
			camera.eye.x = camX;
			camera.eye.y = camY;
			camera.eye.z = camZ;

			// Update camera's center to look at the player
			camera.center.x = streetgoalkeeper2X;
			camera.center.y = streetgoalkeeper2Y + 2.4;
			camera.center.z = streetgoalkeeper2Z;
		}
	}

}
void UpdateCameraPosition() {
	if (!isThirdPersonView) {
		// Existing code for updating the camera position
	}
	else {
		if (currP == 1) {
			float distanceBehindPlayer = 5.0f; // Distance behind the player
			float heightAbovePlayer = 5.0f; // Height above the player

			// Calculate the camera position based on player's position and angle
			float radianAngle = DEG2RAD(playerRotation);
			float camX = playerPosX - distanceBehindPlayer * sin(radianAngle);
			float camY = playerPosY + heightAbovePlayer;
			float camZ = playerPosZ - distanceBehindPlayer * cos(radianAngle);

			// Update camera position
			camera.eye.x = camX;
			camera.eye.y = camY;
			camera.eye.z = camZ;

			// Update camera's center to look at the player
			camera.center.x = playerPosX;
			camera.center.y = playerPosY + 2.4;
			camera.center.z = playerPosZ;
		}
		else {
			float distanceBehindPlayer = 5.0f; // Distance behind the player
			float heightAbovePlayer = 5.0f; // Height above the player

			// Calculate the camera position based on player's position and angle
			float radianAngle = DEG2RAD(player2Rotation);
			float camX = player2PosX - distanceBehindPlayer * sin(radianAngle);
			float camY = player2PosY + heightAbovePlayer;
			float camZ = player2PosZ - distanceBehindPlayer * cos(radianAngle);

			// Update camera position
			camera.eye.x = camX;
			camera.eye.y = camY;
			camera.eye.z = camZ;

			// Update camera's center to look at the player
			camera.center.x = player2PosX;
			camera.center.y = player2PosY + 2.4;
			camera.center.z = player2PosZ;
		}
	}

}


float getDeltaTime() {
	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<float> elapsed = now - lastTime;
	lastTime = now;
	return elapsed.count();
}
void UpdateCameraPositionStreetFirst() {
	if (isFirstPersonView) {
		if (Currgoalkeeper) {
			// Assuming the player's full height is 3.5 units, set the eye level slightly lower
			float eyeHeight = 3.5f * 0.9f; // For example, 90% of the full height

			// In first-person view, the camera is at the player's eye position, so there is no distance behind the player
			float distanceBehindPlayer = -0.3f;
			float lookDownAngle = 0.2f;

			// Calculate the camera position based on player's position and angle for the first-person view
			float radianAngle = DEG2RAD(streetgoalkeeperRotation1);
			float camX = streetgoalkeeper1X - distanceBehindPlayer * sin(radianAngle); // This will be equal to playerPosX
			float camY = streetgoalkeeper1Y + eyeHeight;
			float camZ = streetgoalkeeper1Z - distanceBehindPlayer * cos(radianAngle); // This will be equal to playerPosZ

			// Update camera position
			camera.eye.x = camX;
			camera.eye.y = camY;
			camera.eye.z = camZ;

			// The camera's center point should be in the direction the player is looking
			float lookDistance = 1.0f; // This can be any value, it just determines how far ahead the player is looking
			camera.center.x = streetgoalkeeper1X + lookDistance * sin(radianAngle);
			camera.center.y = streetgoalkeeper1Y + eyeHeight - lookDownAngle;
			camera.center.z = streetgoalkeeper1Z + lookDistance * cos(radianAngle);
		}
		else {
			// Assuming the player's full height is 3.5 units, set the eye level slightly lower
			float eyeHeight = 3.5f * 0.9f; // For example, 90% of the full height

			// In first-person view, the camera is at the player's eye position, so there is no distance behind the player
			float distanceBehindPlayer = -0.3f;
			float lookDownAngle = 0.2f;

			// Calculate the camera position based on player's position and angle for the first-person view
			float radianAngle = DEG2RAD(streetgoalkeeperRotation2);
			float camX = streetgoalkeeper2X - distanceBehindPlayer * sin(radianAngle); // This will be equal to playerPosX
			float camY = streetgoalkeeper2Y + eyeHeight;
			float camZ = streetgoalkeeper2Z - distanceBehindPlayer * cos(radianAngle); // This will be equal to playerPosZ

			// Update camera position
			camera.eye.x = camX;
			camera.eye.y = camY;
			camera.eye.z = camZ;

			// The camera's center point should be in the direction the player is looking
			float lookDistance = 1.0f; // This can be any value, it just determines how far ahead the player is looking
			camera.center.x = streetgoalkeeper2X + lookDistance * sin(radianAngle);
			camera.center.y = streetgoalkeeper2Y + eyeHeight - lookDownAngle;
			camera.center.z = streetgoalkeeper2Z + lookDistance * cos(radianAngle);
		}

	}
	else {
		// Existing third-person camera code...
	}
}
void UpdateCameraPositionFirst() {
	if (isFirstPersonView) {
		if (currP == 1) {
			// Assuming the player's full height is 3.5 units, set the eye level slightly lower
			float eyeHeight = 3.5f * 0.9f; // For example, 90% of the full height

			// In first-person view, the camera is at the player's eye position, so there is no distance behind the player
			float distanceBehindPlayer = -0.3f;
			float lookDownAngle = 0.2f;

			// Calculate the camera position based on player's position and angle for the first-person view
			float radianAngle = DEG2RAD(playerRotation);
			float camX = playerPosX - distanceBehindPlayer * sin(radianAngle); // This will be equal to playerPosX
			float camY = playerPosY + eyeHeight;
			float camZ = playerPosZ - distanceBehindPlayer * cos(radianAngle); // This will be equal to playerPosZ

			// Update camera position
			camera.eye.x = camX;
			camera.eye.y = camY;
			camera.eye.z = camZ;

			// The camera's center point should be in the direction the player is looking
			float lookDistance = 1.0f; // This can be any value, it just determines how far ahead the player is looking
			camera.center.x = playerPosX + lookDistance * sin(radianAngle);
			camera.center.y = playerPosY + eyeHeight - lookDownAngle;
			camera.center.z = playerPosZ + lookDistance * cos(radianAngle);
		}
		else {
			// Assuming the player's full height is 3.5 units, set the eye level slightly lower
			float eyeHeight = 3.5f * 0.9f; // For example, 90% of the full height

			// In first-person view, the camera is at the player's eye position, so there is no distance behind the player
			float distanceBehindPlayer = -0.3f;
			float lookDownAngle = 0.2f;

			// Calculate the camera position based on player's position and angle for the first-person view
			float radianAngle = DEG2RAD(player2Rotation);
			float camX = player2PosX - distanceBehindPlayer * sin(radianAngle); // This will be equal to playerPosX
			float camY = player2PosY + eyeHeight;
			float camZ = player2PosZ - distanceBehindPlayer * cos(radianAngle); // This will be equal to playerPosZ

			// Update camera position
			camera.eye.x = camX;
			camera.eye.y = camY;
			camera.eye.z = camZ;

			// The camera's center point should be in the direction the player is looking
			float lookDistance = 1.0f; // This can be any value, it just determines how far ahead the player is looking
			camera.center.x = player2PosX + lookDistance * sin(radianAngle);
			camera.center.y = player2PosY + eyeHeight - lookDownAngle;
			camera.center.z = player2PosZ + lookDistance * cos(radianAngle);
		}

	}
	else {
		// Existing third-person camera code...
	}
}




void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{

}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************//

	InitLightSource();

	//InitMaterial();

}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}
//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround2()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground2.texture[0]);	// Bind the ground texture

	glPushMatrix();

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}
void RenderWalls()
{
	// Wall textures for fans should be loaded similar to how ground texture is loaded
	// Assuming wall textures are loaded and have IDs: tex_wall_front, tex_wall_back, tex_wall_left, tex_wall_right

	float wallHeight = 10.0f; // Change as needed
	float wallWidth = 40.0f; // Assuming the ground is 40 units wide
	float groundLevel = 0.0f; // Change if the ground is not at y = 0

	// Render Front Wall
	glBindTexture(GL_TEXTURE_2D, tex_wall_front.texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-wallWidth / 2, groundLevel, -wallWidth / 2);
	glTexCoord2f(1, 0); glVertex3f(wallWidth / 2, groundLevel, -wallWidth / 2);
	glTexCoord2f(1, 1); glVertex3f(wallWidth / 2, wallHeight, -wallWidth / 2);
	glTexCoord2f(0, 1); glVertex3f(-wallWidth / 2, wallHeight, -wallWidth / 2);
	glEnd();

	// Render Back Wall
	glBindTexture(GL_TEXTURE_2D, tex_wall_front.texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-wallWidth / 2, groundLevel, wallWidth / 2);
	glTexCoord2f(1, 0); glVertex3f(wallWidth / 2, groundLevel, wallWidth / 2);
	glTexCoord2f(1, 1); glVertex3f(wallWidth / 2, wallHeight, wallWidth / 2);
	glTexCoord2f(0, 1); glVertex3f(-wallWidth / 2, wallHeight, wallWidth / 2);
	glEnd();

	// Render Left Wall
	glBindTexture(GL_TEXTURE_2D, tex_wall_front.texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-wallWidth / 2, groundLevel, -wallWidth / 2);
	glTexCoord2f(1, 0); glVertex3f(-wallWidth / 2, groundLevel, wallWidth / 2);
	glTexCoord2f(1, 1); glVertex3f(-wallWidth / 2, wallHeight, wallWidth / 2);
	glTexCoord2f(0, 1); glVertex3f(-wallWidth / 2, wallHeight, -wallWidth / 2);
	glEnd();

	// Render Right Wall
	glBindTexture(GL_TEXTURE_2D, tex_wall_front.texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(wallWidth / 2, groundLevel, -wallWidth / 2);
	glTexCoord2f(1, 0); glVertex3f(wallWidth / 2, groundLevel, wallWidth / 2);
	glTexCoord2f(1, 1); glVertex3f(wallWidth / 2, wallHeight, wallWidth / 2);
	glTexCoord2f(0, 1); glVertex3f(wallWidth / 2, wallHeight, -wallWidth / 2);
	glEnd();
}
void RenderWalls2()
{
	// Wall textures for fans should be loaded similar to how ground texture is loaded
	// Assuming wall textures are loaded and have IDs: tex_wall_front, tex_wall_back, tex_wall_left, tex_wall_right

	float wallHeight = 10.0f; // Change as needed
	float wallWidth = 40.0f; // Assuming the ground is 40 units wide
	float groundLevel = 0.0f; // Change if the ground is not at y = 0

	// Render Front Wall
	glBindTexture(GL_TEXTURE_2D, tex_wall_front2.texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-wallWidth / 2, groundLevel, -wallWidth / 2);
	glTexCoord2f(1, 0); glVertex3f(wallWidth / 2, groundLevel, -wallWidth / 2);
	glTexCoord2f(1, 1); glVertex3f(wallWidth / 2, wallHeight, -wallWidth / 2);
	glTexCoord2f(0, 1); glVertex3f(-wallWidth / 2, wallHeight, -wallWidth / 2);
	glEnd();

	// Render Back Wall
	glBindTexture(GL_TEXTURE_2D, tex_wall_front2.texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-wallWidth / 2, groundLevel, wallWidth / 2);
	glTexCoord2f(1, 0); glVertex3f(wallWidth / 2, groundLevel, wallWidth / 2);
	glTexCoord2f(1, 1); glVertex3f(wallWidth / 2, wallHeight, wallWidth / 2);
	glTexCoord2f(0, 1); glVertex3f(-wallWidth / 2, wallHeight, wallWidth / 2);
	glEnd();

	// Render Left Wall
	glBindTexture(GL_TEXTURE_2D, tex_wall_front2.texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-wallWidth / 2, groundLevel, -wallWidth / 2);
	glTexCoord2f(1, 0); glVertex3f(-wallWidth / 2, groundLevel, wallWidth / 2);
	glTexCoord2f(1, 1); glVertex3f(-wallWidth / 2, wallHeight, wallWidth / 2);
	glTexCoord2f(0, 1); glVertex3f(-wallWidth / 2, wallHeight, -wallWidth / 2);
	glEnd();

	// Render Right Wall
	glBindTexture(GL_TEXTURE_2D, tex_wall_front2.texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(wallWidth / 2, groundLevel, -wallWidth / 2);
	glTexCoord2f(1, 0); glVertex3f(wallWidth / 2, groundLevel, wallWidth / 2);
	glTexCoord2f(1, 1); glVertex3f(wallWidth / 2, wallHeight, wallWidth / 2);
	glTexCoord2f(0, 1); glVertex3f(wallWidth / 2, wallHeight, -wallWidth / 2);
	glEnd();
}



int ChooseRandomRegion() {
	return rand() % 6 + 1;  // Returns a random number between 1 and 6
}




void returnPlayer1() {
	float stepSize = -0.03f;
	if (playerPosX > -10) {
		playerPosX += stepSize;
	}
	else {
		firstPlayerReturn = 0;
		playerRotation = 180;
		oneOut = 1;

	}
	glutPostRedisplay();

}
void returnStreetPlayer() {
	float stepSize = -0.03f;
	if (streetplayerPosX > -10) {
		streetplayerPosX += stepSize;
	}
	else {
		streetPlayerReturn = 0;
		streetplayerRotation = 90;


	}
	glutPostRedisplay();

}


void returnPlayer2() {
	float stepSize = -0.03f;
	if (player2PosX > -10) {
		player2PosX += stepSize;
	}
	else {
		secondPlayerReturn = 0;
		player2Rotation = 0;
		currP = 1;
		twoOut = 1;
	}

	glutPostRedisplay();

}

void dropBall() {
	if (isIn1)
	{
		player1Score++;
		player1ScoresColors[player1Shots - 1].set(0.0, 0.5, 0.0);
		isIn1 = false;
	}
	if (player1HasNotEnteredGoal)
	{
		player1ScoresColors[player1Shots - 1].set(0.5, 0.0, 0.0);
		player1HasNotEnteredGoal = false;
	}


	if (isIn2)
	{
		player2Score++;
		player2ScoresColors[player2Shots - 1].set(0.0, 0.5, 0.0);
		isIn2 = false;
	}

	if (player2HasNotEnteredGoal)
	{
		player2ScoresColors[player2Shots - 1].set(0.5, 0.0, 0.0);
		player2HasNotEnteredGoal = false;
	}


	if (ballPosY > 0) {
		ballPosY -= 0.03;
	}
	if (ballPosY <= 0) {
		isBallDropping = 0;
		if (currentlyPlaying == 1) {
			firstPlayerReturn = 1;
			playerRotation = 270;
			returnPlayer1();
		}
		else {
			secondPlayerReturn = 1;
			player2Rotation = 270;
			returnPlayer2();
		}
	}
	glutPostRedisplay();
}
void dropStreetBall() {
	if (isIn1SecondEnvironment)
	{
		player1ScoresColorsSecondEnvironment[goalKeeper1Shots - 1].set(0.5, 0.0, 0.0);
		isIn1SecondEnvironment = false;
	}
	if (player1BlockedGoal)
	{
		goalKeeper1Score++;

		player1ScoresColorsSecondEnvironment[goalKeeper1Shots - 1].set(0.0, 0.5, 0.0);
		player1BlockedGoal = false;
	}


	if (player2BlockedGoal)
	{
		goalKeeper2Score++;
		player2ScoresColorsSecondEnvironment[goalKeeper2Shots - 1].set(0.0, 0.5, 0.0);
		player2BlockedGoal = false;
	}

	if (isIn2SecondEnvironment)
	{
		player2ScoresColorsSecondEnvironment[goalKeeper2Shots - 1].set(0.5, 0.0, 0.0);
		isIn2SecondEnvironment = false;
	}

	if (streetballPosY > 0.2) {
		streetballPosY -= 0.03;
	}
	if (streetballPosY <= 0.2) {
		isStreetBallDropping = 0;
		streetPlayerReturn = 1;

		if (currentlyPlaying == 1) {
			streetplayerRotation = 270;
			returnStreetPlayer();
		}
	}
	if (Currgoalkeeper) {
		streetgoalkeeperRotation1 = 180;
	}
	else {
		streetgoalkeeperRotation2 = 0;
		//printf("keeper 2 drop ball");
	}
	glutPostRedisplay();
}
void ReturnGoalkeeper() {
	float originalX = 10.5f; // Original X position
	float originalY = 0.0f;  // Original Y position
	float originalZ = 0.0f;  // Original Z position
	float stepSize = 0.005f; // Adjust as needed
	float angleStep = 1.1f;  // Adjust the rotation speed as needed

	// Move the goalkeeper towards the original position
	goalkeeperX = originalX;
	goalkeeperY = originalY;
	goalkeeperZ = originalZ;
	goalkeeperRotation = -90;
	goalkeeperRotSaving = 0;
	isGoalkeeperReturning = 0;
}
void ReturnStreetGoalkeeper() {
	float originalX = 9.2f; // Original X position
	float originalY = 0.0f;  // Original Y position
	float originalZ = 0.0f;  // Original Z position
	float stepSize = 0.005f; // Adjust as needed
	float angleStep = 1.1f;  // Adjust the rotation speed as needed
	if (Currgoalkeeper) {
		// Move the goalkeeper towards the original position
		streetgoalkeeper1X = originalX;
		streetgoalkeeper1Y = originalY;
		streetgoalkeeper1Z = originalZ;
		streetgoalkeeperRotation1 = -90;
		streetgoalkeeperRotSaving1 = 0;
		isStreetGoalkeeperReturning = 0;

		lightX = 0;
		keeper1out = 1;

	}
	else {
		streetgoalkeeper2X = originalX;
		streetgoalkeeper2Y = originalY;
		streetgoalkeeper2Z = originalZ;
		streetgoalkeeperRotation2 = -90;
		streetgoalkeeperRotSaving2 = 0;
		isStreetGoalkeeperReturning = 0;
		keeper2out = 1;
	}
}
void MoveStreetGoalkeeper(int region) {
	// Define target positions for the goalkeeper based on the region
	float netWidth = 7.0f;
	float netHeight = 3.0f;

	//printf("%i", goalkeeperTargetRegion);

	float targetY = 0; // Y position will be determined by the region
	float targetZ = 0; // Z position will be determined by the region

	float stepSize = 0.1f;
	//printf("%d", Currgoalkeeper);

	if (Currgoalkeeper) {
		//printf("%i", isStreetGoalkeeperMoving);
		float targetX = streetgoalkeeper1X;
		// Set the target positions based on the region.
		switch (region) {
		case 1: // Top right

			// x : -5 >> 15  20
			// y : 0 >> 4    4
			// z : 0 >> 5    5
			//goalkeeperX += 20.0 / 160;
			streetgoalkeeper1Y += 3.0 / 250;
			streetgoalkeeper1Z += 4.0 / 250;
			streetgoalkeeperRotSaving1 -= 60.0 / 180;
			targetY = netHeight - 1;
			targetZ = netWidth / 2;

			break;
		case 2: // Lower right
			// x : -5>>15  20
			// y : 0>>0    0
			// z : 0>>5    5
			//goalkeeperX += 20.0 / 160;
			streetgoalkeeper1Y = 0;
			streetgoalkeeper1Z += 5.0 / 240;
			streetgoalkeeperRotSaving1 -= 90.0 / 180;
			targetY = 0;
			targetZ = netWidth / 2;

			break;
		case 3: // Middle top
			// x : -5>>15  20
			// y : 0>>4    4
			// z : 0>>0    0
			//goalkeeperX += 20.0 / 160;
			streetgoalkeeper1Y += 3.0 / 250;
			streetgoalkeeper1Z = 0;
			targetY = netHeight - 1;
			targetZ = 0;
			break;
		case 4: // Middle low
			// x : -5>>15  20
			// y : 0>>0   0
			// z : 0>>0    0
			//goalkeeperX += 20.0 / 160;
			streetgoalkeeper1Y += 0;
			streetgoalkeeper1Z += 0;
			targetY = 0;
			targetZ = 0;
			break;
		case 5: // Top left
			// x : -5>>15  20
			// y : 0>>4   4
			// z : 0>>-5    -5
			//goalkeeperX += 20.0 / 160;
			streetgoalkeeper1Y += 3.0 / 250;
			streetgoalkeeper1Z -= 4.0 / 250;
			streetgoalkeeperRotSaving1 += 60.0 / 180;
			targetY = netHeight - 1;
			targetZ = -netWidth / 2;
			break;
		case 6: // Lower left
			// x : -5>>15  20
			// y : 0>>0   0
			// z : 0>>-5    -5
		   //goalkeeperX += 20.0 / 160;
			streetgoalkeeper1Y = 0;
			streetgoalkeeper1Z -= 5.0 / 250;
			streetgoalkeeperRotSaving1 += 90.0 / 180;
			targetY = 0;
			targetZ = -(netWidth / 2);
			break;
		}


		// Check if goalkeeper has reached the target position
		if (region <= 5) {
			if (streetgoalkeeper1X >= targetX && streetgoalkeeper1Y >= targetY && streetgoalkeeper1Z >= targetZ) {
				streetgoalkeeper1X = targetX;
				streetgoalkeeper1Y = targetY;
				streetgoalkeeper1Z = targetZ;
				isStreetGoalkeeperMoving = 0; // Stop moving the goalkeeper ////hnaaaaaaaaaa
				isStreetGoalkeeperReturning = 1;
				if (targetRegion == goalkeeperTargetRegion)
				collSound.play();
				else {
					winSound.play();
				}

			}
		}
		else {
			if (streetgoalkeeper1X >= targetX && streetgoalkeeper1Y >= targetY && streetgoalkeeper1Z < targetZ) {
				streetgoalkeeper1X = targetX;
				streetgoalkeeper1Y = targetY;
				streetgoalkeeper1Z = targetZ;
				isStreetGoalkeeperMoving = 0; // Stop moving the goalkeeper
				isStreetGoalkeeperReturning = 1;
				if (targetRegion == goalkeeperTargetRegion)
				collSound.play();
				else {
					winSound.play();
				}

			}

		}
	}
	else {
		//printf("dkhl hnaaaa");
		float targetX = streetgoalkeeper2X;
		switch (region) {
		case 1: // Top right

			// x : -5 >> 15  20
			// y : 0 >> 4    4
			// z : 0 >> 5    5
			//goalkeeperX += 20.0 / 160;
			streetgoalkeeper2Y += 3.0 / 250;
			streetgoalkeeper2Z += 4.0 / 250;
			streetgoalkeeperRotSaving2 -= 60.0 / 180;
			targetY = netHeight - 1;
			targetZ = netWidth / 2;

			break;
		case 2: // Lower right
			// x : -5>>15  20
			// y : 0>>0    0
			// z : 0>>5    5
			//goalkeeperX += 20.0 / 160;
			streetgoalkeeper2Y = 0;
			streetgoalkeeper2Z += 5.0 / 240;
			streetgoalkeeperRotSaving2 -= 90.0 / 180;
			targetY = 0;
			targetZ = netWidth / 2;

			break;
		case 3: // Middle top
			// x : -5>>15  20
			// y : 0>>4    4
			// z : 0>>0    0
			//goalkeeperX += 20.0 / 160;
			streetgoalkeeper2Y += 3.0 / 250;
			streetgoalkeeper2Z = 0;
			targetY = netHeight - 1;
			targetZ = 0;
			break;
		case 4: // Middle low
			// x : -5>>15  20
			// y : 0>>0   0
			// z : 0>>0    0
			//goalkeeperX += 20.0 / 160;
			streetgoalkeeper2Y += 0;
			streetgoalkeeper2Z += 0;
			targetY = 0;
			targetZ = 0;
			break;
		case 5: // Top left
			// x : -5>>15  20
			// y : 0>>4   4
			// z : 0>>-5    -5
			//goalkeeperX += 20.0 / 160;
			streetgoalkeeper2Y += 3.0 / 250;
			streetgoalkeeper2Z -= 4.0 / 250;
			streetgoalkeeperRotSaving2 += 60.0 / 180;
			targetY = netHeight - 1;
			targetZ = -netWidth / 2;
			break;
		case 6: // Lower left
			// x : -5>>15  20
			// y : 0>>0   0
			// z : 0>>-5    -5
		   //goalkeeperX += 20.0 / 160;
			streetgoalkeeper2Y = 0;
			streetgoalkeeper2Z -= 5.0 / 250;
			streetgoalkeeperRotSaving2 += 90.0 / 180;
			targetY = 0;
			targetZ = -(netWidth / 2);
			break;
		}


		// Check if goalkeeper has reached the target position
		if (region <= 5) {
			if (streetgoalkeeper2X >= targetX && streetgoalkeeper2Y >= targetY && streetgoalkeeper2Z >= targetZ) {
				streetgoalkeeper2X = targetX;
				streetgoalkeeper2Y = targetY;
				streetgoalkeeper2Z = targetZ;
				isStreetGoalkeeperMoving = 0; // Stop moving the goalkeeper ////hnaaaaaaaaaa
				isStreetGoalkeeperReturning = 1;
				if(targetRegion == goalkeeperTargetRegion)
				collSound.play();
				else {
					winSound.play();
				}

			}
		}
		else {
			if (streetgoalkeeper2X >= targetX && streetgoalkeeper2Y >= targetY && streetgoalkeeper2Z < targetZ) {
				streetgoalkeeper2X = targetX;
				streetgoalkeeper2Y = targetY;
				streetgoalkeeper2Z = targetZ;
				isStreetGoalkeeperMoving = 0; // Stop moving the goalkeeper
				isStreetGoalkeeperReturning = 1;
				if (targetRegion == goalkeeperTargetRegion)
				collSound.play();
				else {
					winSound.play();
				}

			}

		}
	}

	glutPostRedisplay();
}

void MoveGoalkeeper(int region) {
	// Define target positions for the goalkeeper based on the region
	float netWidth = 7.0f;
	float netHeight = 3.0f;

	//printf("%i", goalkeeperTargetRegion);
	float targetX = goalkeeperX;
	float targetY = 0; // Y position will be determined by the region
	float targetZ = 0; // Z position will be determined by the region

	float stepSize = 0.1f;
	// Set the target positions based on the region.
	switch (region) {
	case 1: // Top right

		// x : -5 >> 15  20
		// y : 0 >> 4    4
		// z : 0 >> 5    5
		//goalkeeperX += 20.0 / 160;
		goalkeeperY += 3.0 / 250;
		goalkeeperZ += 4.0 / 250;
		goalkeeperRotSaving -= 60.0 / 180;
		targetY = netHeight - 1;
		targetZ = netWidth / 2;

		break;
	case 2: // Lower right
		// x : -5>>15  20
		// y : 0>>0    0
		// z : 0>>5    5
		//goalkeeperX += 20.0 / 160;
		goalkeeperY = 0;
		goalkeeperZ += 5.0 / 240;
		goalkeeperRotSaving -= 90.0 / 180;
		targetY = 0;
		targetZ = netWidth / 2;

		break;
	case 3: // Middle top
		// x : -5>>15  20
		// y : 0>>4    4
		// z : 0>>0    0
		//goalkeeperX += 20.0 / 160;
		goalkeeperY += 3.0 / 250;
		goalkeeperZ = 0;
		targetY = netHeight - 1;
		targetZ = 0;
		break;
	case 4: // Middle low
		// x : -5>>15  20
		// y : 0>>0   0
		// z : 0>>0    0
		//goalkeeperX += 20.0 / 160;
		goalkeeperY += 0;
		goalkeeperZ += 0;
		targetY = 0;
		targetZ = 0;
		break;
	case 5: // Top left
		// x : -5>>15  20
		// y : 0>>4   4
		// z : 0>>-5    -5
		//goalkeeperX += 20.0 / 160;
		goalkeeperY += 3.0 / 250;
		goalkeeperZ -= 4.0 / 250;
		goalkeeperRotSaving += 60.0 / 180;
		targetY = netHeight - 1;
		targetZ = -netWidth / 2;
		break;
	case 6: // Lower left
		// x : -5>>15  20
		// y : 0>>0   0
		// z : 0>>-5    -5
	   //goalkeeperX += 20.0 / 160;
		goalkeeperY = 0;
		goalkeeperZ -= 5.0 / 250;
		goalkeeperRotSaving += 90.0 / 180;
		targetY = 0;
		targetZ = -(netWidth / 2);
		break;
	}


	// Check if goalkeeper has reached the target position
	if (region <= 5) {
		if (goalkeeperX >= targetX && goalkeeperY >= targetY && goalkeeperZ >= targetZ) {
			goalkeeperX = targetX;
			goalkeeperY = targetY;
			goalkeeperZ = targetZ;
			isGoalkeeperMoving = 0; // Stop moving the goalkeeper
			isGoalkeeperReturning = 1;
			if (targetRegion == goalkeeperTargetRegion)
			collSound.play();
			else {
				winSound.play();
			}
		}
	}
	else
		if (goalkeeperX >= targetX && goalkeeperY >= targetY && goalkeeperZ < targetZ) {
			goalkeeperX = targetX;
			goalkeeperY = targetY;
			goalkeeperZ = targetZ;
			isGoalkeeperMoving = 0; // Stop moving the goalkeeper
			isGoalkeeperReturning = 1;

			//glEnable(GL_LIGHT3);
			if (targetRegion == goalkeeperTargetRegion)
			collSound.play();
			else {
				winSound.play();
			}
		}


	glutPostRedisplay();
}
void TimerCallback(int value) {
	// This function will be called 2 seconds after MoveBall is called
	if (value == 0) {
		inDelay = false;
		// Any other code you need to execute after the delay
	}
}
void playerCelebration() {
	// Increase the rotation angle
	celebrationRotationAngle += 5.0f;
	if (celebrationRotationAngle > 360.0f) {
		celebrationRotationAngle -= 270.0f; // Reset the angle after a full rotation
		isCelebrating = false; // End celebration after one full rotation
	}

	// Apply the rotation to the player
	if (currentlyPlaying == 1) {
		playerRotation = celebrationRotationAngle;
	}
	else {
		player2Rotation = celebrationRotationAngle;
	}

	glutPostRedisplay();
}
void StreetplayerCelebration() {
	// Increase the rotation angle
	celebrationRotationAngle += 5.0f;
	if (celebrationRotationAngle > 360.0f) {
		celebrationRotationAngle -= 270.0f; // Reset the angle after a full rotation
		isCelebrating = false; // End celebration after one full rotation
	}

	// Apply the rotation to the player

	streetplayerRotation = celebrationRotationAngle;


	glutPostRedisplay();
}
void MoveBall(int region) {
	// Determine the ball's target position based on the chosen region.
	// Assume netWidth = 10, netHeight = 5, netX = 15.
	float netWidth = 10.0f;
	float netHeight = 5.0f;
	float netX = 15.0f;
	float netXsaved = 10.5f;
	float targetX = netX;
	if (goalkeeperTargetRegion == targetRegion)
		targetX = netXsaved;
	//printf("%f", targetX);
	// The net is at x=15
	float targetY = 0;    // Y position will be determined by the region
	float targetZ = 0;    // Z position will be determined by the region

	// Set the target positions based on the region.

	float stepSize = 0.05f;
	//printf("%i", isBallMoving);

	/*
		// initial position of ball x huwa -5 <<<<
		// end = 15
		// moveX 20

		// fel y 0 >> 0 aw 4 3la hasab

		// fel z 0 >> 5 aw 0 aw -5 3la hasab
		// fel z
		if (playerPosX < -5) {
			playerPosX += stepSize;
		}
		else {
			isPlayerMoving = 0; // Stop moving the player
			MoveBall(targetRegion); // Move the ball after reaching the ball's position
		}
		glutPostRedisplay();
	*/
	switch (region) {
	case 1: // Top right

		// x : -5 >> 15  20
		// y : 0 >> 4    4
		// z : 0 >> 5    5

		ballPosX += (targetX + 5) / 160;
		ballPosY += 4.0 / 160;
		ballPosZ += 5.0 / 160;
		targetY = netHeight - 1;
		targetZ = netWidth / 2;


		break;
	case 2: // Lower right
		// x : -5>>15  20
		// y : 0>>0    0
		// z : 0>>5    5
		ballPosX += (targetX + 5) / 160;
		ballPosY = 0;
		ballPosZ += 5.0 / 160;
		targetY = 0;
		targetZ = netWidth / 2;

		break;
	case 3: // Middle top
		// x : -5>>15  20
		// y : 0>>4    4
		// z : 0>>0    0
		ballPosX += (targetX + 5) / 160;
		ballPosY += 4.0 / 160;
		ballPosZ += 0;
		targetY = netHeight - 1;
		targetZ = 0;
		break;
	case 4: // Middle low
		// x : -5>>15  20
		// y : 0>>0   0
		// z : 0>>0    0
		ballPosX += (targetX + 5) / 160;
		ballPosY += 0;
		ballPosZ += 0;
		targetY = 0;
		targetZ = 0;
		break;
	case 5: // Top left
		// x : -5>>15  20
		// y : 0>>4   4
		// z : 0>>-5    -5
		ballPosX += (targetX + 5) / 160;
		ballPosY += 4.0 / 160;
		ballPosZ -= 5.0 / 160;
		targetY = netHeight - 1;
		targetZ = -netWidth / 2;
		break;
	case 6: // Lower left
		// x : -5>>15  20
		// y : 0>>0   0
		// z : 0>>-5    -5
		ballPosX += (targetX + 5) / 160;
		ballPosY = 0;
		ballPosZ -= 5.0 / 160;
		targetY = 0;
		targetZ = -netWidth / 2;
		break;
	}

	// Assuming model_ball has position attributes
	if (ballPosX >= targetX && ballPosY >= targetY && ballPosZ >= targetZ) {
		isBallMoving = 0;
		//if (targetY != 0) {   // sheltha 3shan player changes f method dropBall()
		isBallDropping = 1;
		dropBall();
		if (targetRegion != goalkeeperTargetRegion) {
			isCelebrating = true;
			playerCelebration();
		}
		///put sound lma y colloide m3 goalkeeper

	//}
	}


	glutPostRedisplay();

}
void MoveStreetBall(int region) {
	// Determine the ball's target position based on the chosen region.
	// Assume netWidth = 10, netHeight = 5, netX = 15.
	float netWidth = 10.0f;
	float netHeight = 5.0f;
	float netX = 15.0f;
	float netXsaved = 10.5f;
	float targetX = netX;
	if (goalkeeperTargetRegion == targetRegion)
		targetX = netXsaved;
	//printf("%f", targetX);
	// The net is at x=15
	float targetY = 0.2;    // Y position will be determined by the region
	float targetZ = 0;    // Z position will be determined by the region

	// Set the target positions based on the region.

	float stepSize = 0.05f;


	switch (region) {
	case 1: // Top right


		streetballPosX += (targetX + 5) / 160;
		streetballPosY += 4.0 / 160;
		streetballPosZ += 5.0 / 160;
		targetY = netHeight - 1;
		targetZ = netWidth / 2;


		break;
	case 2: // Lower right

		streetballPosX += (targetX + 5) / 160;
		streetballPosY = 0.2;
		streetballPosZ += 5.0 / 160;
		targetY = 0;
		targetZ = netWidth / 2;

		break;
	case 3: // Middle top

		streetballPosX += (targetX + 5) / 160;
		streetballPosY += 4.0 / 160;
		streetballPosZ += 0;
		targetY = netHeight - 1;
		targetZ = 0;
		break;
	case 4: // Middle low
		streetballPosX += (targetX + 5) / 160;
		streetballPosY = 0.2;
		streetballPosZ = 0;
		targetY = 0;
		targetZ = 0;
		break;
	case 5: // Top left

		streetballPosX += (targetX + 5) / 160;
		streetballPosY += 4.0 / 160;
		streetballPosZ -= 5.0 / 160;
		targetY = netHeight - 1;
		targetZ = -netWidth / 2;
		break;
	case 6: // Lower left
		streetballPosX += (targetX + 5) / 160;
		streetballPosY = 0.2;
		streetballPosZ -= 5.0 / 160;
		targetY = 0;
		targetZ = -netWidth / 2;
		break;
	}

	// Assuming model_ball has position attributes
	if (streetballPosX >= targetX && streetballPosY >= targetY && streetballPosZ >= targetZ) {
		isStreetBallMoving = 0;
		//if (targetY != 0) {   // sheltha 3shan player changes f method dropBall()
		isStreetBallDropping = 1;
		dropStreetBall();
		if (targetRegion != goalkeeperTargetRegion) {
			isCelebrating = true;
			StreetplayerCelebration();
		}
		///put sound lma y colloide m3 goalkeeper

	//}
	}


	glutPostRedisplay();

}
void MovePlayerTowardBall() {

	float stepSize = 0.04f;
	if (playerPosX < -5) {
		playerPosX += stepSize;
	}
	else {
		isPlayerMoving = 0; // Stop moving the player
		isBallMoving = 1;
		targetSound.play();
		MoveBall(targetRegion); // Move the ball after reaching the ball's position
	}
	glutPostRedisplay();

}

void MoveStreetPlayerTowardBall() {

	float stepSize = 0.04f;
	if (streetplayerPosX < -5) {
		streetplayerPosX += stepSize;
	}
	else {
		isStreetPlayerMoving = 0; // Stop moving the player
		isStreetBallMoving = 1;
		targetSound.play();
		MoveStreetBall(targetRegion); // Move the ball after reaching the ball's position
	}
	glutPostRedisplay();

}
//=======================================================================
// Display Function
//=======================================================================

void keeperoneOutTwoIn() {   ///nkhly oneout b 1 3nd dropball
	float stepSize = 0.03;
	streetgoalkeeper1Z -= stepSize;
	streetgoalkeeper2Z -= stepSize;
	if (streetgoalkeeper2Z <= 0) {
		keeper1out = 0;
		streetgoalkeeperRotation1 = 0;
		streetgoalkeeperRotation2 = -90;
		streetballPosX = -5;
		streetballPosY = 0.2;
		streetballPosZ = 0;
		Currgoalkeeper = false;
		//UpdateCameraPosition();
		UpdateStreetCameraPosition();
		UpdateCameraPositionStreetFirst();
		//UpdateCameraPositionFirst();
	}
	//currP = 2;
	glutPostRedisplay();
}
void oneOutTwoIn() {
	float stepSize = 0.03;
	playerPosZ -= stepSize;
	player2PosZ -= stepSize;
	if (player2PosZ <= 0) {
		oneOut = 0;
		playerRotation = 0;
		player2Rotation = 90;
		ballPosX = -5;
		ballPosY = 0;
		ballPosZ = 0;
		currentlyPlaying = 2;
		UpdateCameraPosition();
		UpdateCameraPositionFirst();
	}
	currP = 2;
	glutPostRedisplay();
}
void keepertwoOutOneIn() {
	float stepSize = 0.03;
	streetgoalkeeper1Z += stepSize;
	streetgoalkeeper2Z += stepSize;
	if (streetgoalkeeper1Z >= 0) {
		keeper2out = 0;
		streetgoalkeeperRotation2 = 180;
		streetgoalkeeperRotation1 = -90;
		streetballPosX = -5;
		streetballPosY = 0.2;
		streetballPosZ = 0;
		Currgoalkeeper = true;
		//printf("mafrod rg3 mkano");
		//UpdateCameraPosition();
		//UpdateCameraPositionFirst();
		UpdateStreetCameraPosition();
		UpdateCameraPositionStreetFirst();
	}
	//currP = 1;
	//if (player2Shots == maxScore) {
		//gameOver = true;
	//}
	if (goalKeeper2Shots == maxScore) {
		End = true;
	}
	glutPostRedisplay();
}

void twoOutOneIn() {
	float stepSize = 0.03;
	playerPosZ += stepSize;
	player2PosZ += stepSize;
	if (playerPosZ >= 0) {
		twoOut = 0;
		player2Rotation = 180;
		playerRotation = 90;
		ballPosX = -5;
		ballPosY = 0;
		ballPosZ = 0;
		currentlyPlaying = 1;
		UpdateCameraPosition();
		UpdateCameraPositionFirst();
	}
	currP = 1;
	if (player2Shots == maxScore) {
		gameOver = true;
	}
	glutPostRedisplay();
}

void MovePlayer2TowardBall() {
	float stepSize = 0.04f;
	if (player2PosX < -5) {
		player2PosX += stepSize;
	}
	else {
		isPlayer2Moving = 0; // Stop moving the player
		isBallMoving = 1;
		targetSound.play();
		MoveBall(targetRegion); // Move the ball after reaching the ball's position
	}
	glutPostRedisplay();
}
void determineWinner() {
	int winner = 0; // means draw
	if (player1Score > player2Score) {
		winner = 1;
	}
	else if (player2Score > player1Score) {
		winner = 2;
	}

}

void generateCircleTexture(float radius, int numSegments) {
	unsigned char* data = new unsigned char[numSegments * numSegments * 4]; // 4 components (RGBA) per pixel
	float centerX = radius;
	float centerY = radius;

	for (int i = 0; i < numSegments; ++i) {
		for (int j = 0; j < numSegments; ++j) {
			float distance = sqrt((i - centerX) * (i - centerX) + (j - centerY) * (j - centerY));
			if (distance < radius) {
				int index = (i + j * numSegments) * 4;
				data[index] = 255;  // R
				data[index + 1] = 255;  // G
				data[index + 2] = 255;  // B
				data[index + 3] = 255;  // A
			}
		}
	}

	glGenTextures(1, &circleTexture);
	glBindTexture(GL_TEXTURE_2D, circleTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, numSegments, numSegments, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	delete[] data;
}

void drawCircle(float x, float y, float radius, int numSegments, int numStripes) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, circleTexture);

	glBegin(GL_TRIANGLE_FAN);

	for (int i = 0; i <= numSegments; ++i) {
		float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
		float cx = x + radius * cos(theta);
		float cy = y + radius * sin(theta);

		glVertex2f(cx, cy);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void displayScoresAndShots() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Left part of the screen (top-left corner)
	glColor3f(1.0f, 0.0f, 0.0f); // Red color for player 1
	glRasterPos2f(10.0f, glutGet(GLUT_WINDOW_HEIGHT) - 20.0f);  // Top-left corner
	std::string scoreText1 = "Player 1 Score: ";
	for (const char& c : scoreText1) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
	float circleY = glutGet(GLUT_WINDOW_HEIGHT) - 15.0f; // Adjusted position

	for (int i = 0; i < 5; i++)
	{
		glColor3f(player1ScoresColors[i].getRed(), player1ScoresColors[i].getGreen(), player1ScoresColors[i].getBlue());
		float circleX1 = 10.0f + glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, scoreText1[0]) + 150.0f + 20 * i; // Adjusted position
		drawCircle(circleX1, circleY, 8.0f, 100, 3);
	}

	// Right part of the screen (top-right corner)
	glColor3f(0.0f, 0.0f, 1.0f); // Blue color for player 2
	glRasterPos2f(glutGet(GLUT_WINDOW_WIDTH) / 2 + 375, glutGet(GLUT_WINDOW_HEIGHT) - 20.0f);  // Top-right corner
	std::string scoreText2 = "Player 2 Score: ";
	for (const char& c : scoreText2) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}

	for (int i = 0; i < 5; i++)
	{
		glColor3f(player2ScoresColors[i].getRed(), player2ScoresColors[i].getGreen(), player2ScoresColors[i].getBlue());
		float circleX2 = glutGet(GLUT_WINDOW_WIDTH) / 2 + 375 + 20 * i + glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, scoreText2[0]) + 150.0f; // Adjusted position
		drawCircle(circleX2, circleY, 8.0f, 100, 3);
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Request a redraw of the window
	glutPostRedisplay();
}

void displayScoresAndShotsSecondEnvironment() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Left part of the screen (top-left corner)
	glColor3f(1.0f, 0.0f, 0.0f); // Red color for player 1
	glRasterPos2f(10.0f, glutGet(GLUT_WINDOW_HEIGHT) - 20.0f);  // Top-left corner
	std::string scoreText1 = "Player 1 Score: ";
	for (const char& c : scoreText1) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
	float circleY = glutGet(GLUT_WINDOW_HEIGHT) - 15.0f; // Adjusted position

	for (int i = 0; i < 5; i++)
	{
		glColor3f(player1ScoresColorsSecondEnvironment[i].getRed(), player1ScoresColorsSecondEnvironment[i].getGreen(), player1ScoresColorsSecondEnvironment[i].getBlue());
		float circleX1 = 10.0f + glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, scoreText1[0]) + 150.0f + 20 * i; // Adjusted position
		drawCircle(circleX1, circleY, 8.0f, 100, 3);
	}

	// Right part of the screen (top-right corner)
	glColor3f(0.0f, 0.0f, 1.0f); // Blue color for player 2
	glRasterPos2f(glutGet(GLUT_WINDOW_WIDTH) / 2 + 375, glutGet(GLUT_WINDOW_HEIGHT) - 20.0f);  // Top-right corner
	std::string scoreText2 = "Player 2 Score: ";
	for (const char& c : scoreText2) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}

	for (int i = 0; i < 5; i++)
	{
		glColor3f(player2ScoresColorsSecondEnvironment[i].getRed(), player2ScoresColorsSecondEnvironment[i].getGreen(), player2ScoresColorsSecondEnvironment[i].getBlue());
		float circleX2 = glutGet(GLUT_WINDOW_WIDTH) / 2 + 375 + 20 * i + glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, scoreText2[0]) + 150.0f; // Adjusted position
		drawCircle(circleX2, circleY, 8.0f, 100, 3);
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Request a redraw of the window
	glutPostRedisplay();
}


void displayEndMessage(const std::string& message) {
	// Calculate the center position for the text
	int textWidth = message.length() * 9; // Approximate width of each character
	float centerX = (WIDTH - textWidth) / 2.0f;
	float centerY = HEIGHT / 2.0f;

	// Switch to orthographic projection for 2D text
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, WIDTH, 0, HEIGHT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Clear the depth buffer to make sure the text is not hidden by any 3D object
	glClear(GL_DEPTH_BUFFER_BIT);

	// Set the text color and position
	glColor3f(1.0, 1.0, 1.0); // White color for the text
	glRasterPos2f(centerX, centerY);

	// Render the text
	for (const char& c : message) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}

	// Restore the matrices
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void Timer(int value) {
	// Update light intensity
	currentLightIntensity += (increasingIntensity ? intensityChangeRate : -intensityChangeRate);

	// Clamp the intensity between 0 (night) and 1 (day)
	if (currentLightIntensity <= 0.0f) {
		currentLightIntensity = 0.0f;
		increasingIntensity = true; // Start increasing intensity (moving towards day)
	}
	else if (currentLightIntensity >= 1.0f) {
		currentLightIntensity = 1.0f;
		increasingIntensity = false; // Start decreasing intensity (moving towards night)
	}

	lightX+= 10;
	// Reinitialize light source with updated intensity
	InitLightSource();
	if (gameOver) {
		CounterEnvironment2++;
		if (CounterEnvironment2 > 120) {
			gameOver = false;
			CounterEnvironment2 = 0;
			SecondEnvironment = true;
			currentlyPlaying = 1;
			isGoalkeeperMoving = 0;
			isThirdPersonView = 0;
			isFirstPersonView = 0;
			camera.updatePose();

		}

	}
	glutTimerFunc(16, Timer, 0);
	glutPostRedisplay();
}

void myDisplay(void)
{
	if (firstTime)
	{
		setupLight();
		firstTime = false;
	}
	else
	{
		float deltaTime = getDeltaTime(); // Calculate the time difference since the last frame
		elapsedTime += deltaTime;

		// Reset elapsedTime after each cycle
		if (elapsedTime > cycleDuration) {
			elapsedTime -= cycleDuration;
		}

		// Calculate 'timeOfDay' based on 'elapsedTime'
		float timeOfDay = (elapsedTime / cycleDuration) * 24.0f; // Scale to 24-hour cycle

		updateSunlight(timeOfDay);
	}

	if (gameOver) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
		//GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
		//glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		//glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
		if (player1Score > player2Score)
			displayEndMessage("player 1 wins \n now let's move to the challenging level");
		else if (player1Score < player2Score)
			displayEndMessage("player 2 wins \n now let's move to the challenging level");
		else if (player1Score == player2Score)
			displayEndMessage("Draw \n now let's move to the challenging level");
	}
	else if (End) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
		//GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
		//glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		//glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
		

		if (goalKeeper1Score > goalKeeper2Score)
			displayEndMessage("player 1 wins \n ");
		else if (goalKeeper1Score < goalKeeper2Score)
			displayEndMessage("player 2 wins \n ");
		else if (goalKeeper1Score == goalKeeper2Score)
			displayEndMessage("Draw \n ");
	}

	else if (SecondEnvironment) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
			//GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
			//glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
			//glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
		//camera.drawCameraValues();
		setupCamera();
		// Draw Ground
		RenderGround2();
		//sky box
		glPushMatrix();

		GLUquadricObj* qobj;
		qobj = gluNewQuadric();
		glScalef(1, 1, 1);
		glTranslated(50, 0, 0);
		glRotated(90, 1, 0, 1);
		glBindTexture(GL_TEXTURE_2D, tex);
		gluQuadricTexture(qobj, true);
		gluQuadricNormals(qobj, GL_SMOOTH);
		gluSphere(qobj, 100, 100, 100);
		gluDeleteQuadric(qobj);
		UpdateStreetCameraPosition();
		UpdateCameraPositionStreetFirst();
		glPopMatrix();
		// Draw Walls
		RenderWalls2();

		// Draw lamp Model
		glPushMatrix();
		glTranslatef(15, 0, 15);
		glScalef(0.1, 0.1, 0.1);
		model_lamp.Draw();
		glPopMatrix();

		// Draw lamp Model2
		glPushMatrix();
		glTranslatef(15, 0, -15);
		glScalef(0.1, 0.1, 0.1);
		model_lamp.Draw();
		glPopMatrix();

		//draw net
		glPushMatrix();
		glTranslatef(15, 3, 0);
		glScalef(0.3, 0.3, 0.3);
		model_net.Draw();
		glPopMatrix();

		//draw ball
		glPushMatrix();
		glTranslatef(streetballPosX, streetballPosY, streetballPosZ);
		glScalef(0.0002, 0.0002, 0.0002);
		model_streetball.Draw();
		glPopMatrix();

		// Draw soccer_player computer that will shoot the ball
		glPushMatrix();
		glTranslatef(streetplayerPosX, streetplayerPosY, streetplayerPosZ);
		glRotatef(streetplayerRotation, 0, 1, 0);
		glScalef(0.05, 0.05, 0.05);
		model_streetplayer.Draw();
		glPopMatrix();

		// Draw streetgoalkeeper1
		glPushMatrix();
		glTranslatef(streetgoalkeeper1X, streetgoalkeeper1Y, streetgoalkeeper1Z);
		glRotatef(streetgoalkeeperRotation1, 0, 1, 0);
		glRotatef(streetgoalkeeperRotSaving1, 0, 0, 1);
		glScalef(0.05, 0.05, 0.05);
		model_streetgoalkeeper1.Draw();
		glPopMatrix();

		// Draw streetgoalkeeper2
		glPushMatrix();
		glTranslatef(streetgoalkeeper2X, streetgoalkeeper2Y, streetgoalkeeper2Z);
		glRotatef(streetgoalkeeperRotation2, 0, 1, 0);
		glRotatef(streetgoalkeeperRotSaving2, 0, 0, 1);
		glScalef(0.05, 0.05, 0.05);
		model_streetgoalkeeper2.Draw();
		glPopMatrix();
		displayScoresAndShotsSecondEnvironment();
		//	printf("%i", isStreetBallMoving);
		if (isStreetPlayerMoving == 1) {
			MoveStreetPlayerTowardBall();
		}
		if (isStreetBallMoving == 1) {
			MoveStreetBall(targetRegion);
			//if (targetRegion == goalkeeperTargetRegion)
				isStreetGoalkeeperMoving = 1;

		}
		if (isStreetBallDropping == 1) {
			dropStreetBall();
		}
		if (streetPlayerReturn == 1) {

			returnStreetPlayer();
			//inDelay = true;
			//glutTimerFunc(1000, TimerCallback, 0);
		}
		if (isStreetGoalkeeperMoving) {
			MoveStreetGoalkeeper(goalkeeperTargetRegion);
			//isStreetGoalkeeperMoving = 0;
		}
		if (isStreetGoalkeeperReturning) {
			ReturnStreetGoalkeeper();
			isStreetGoalkeeperReturning = 0;

		}
		if (keeper1out == 1) {

			keeperoneOutTwoIn();
		}
		if (keeper2out == 1) {

			keepertwoOutOneIn();
		}
		if (isCelebrating) {
			StreetplayerCelebration(); // Call celebration animation

		}

	}
	else {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
		//GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
		//glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		//glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);


		//camera.drawCameraValues();
		setupCamera();
		// Draw Ground
		RenderGround();
		// Draw Walls
		RenderWalls();

		// Draw Tree Model
		glPushMatrix();
		glTranslatef(10, 0, 0);
		glScalef(0.7, 0.7, 0.7);
		model_tree.Draw();
		glPopMatrix();

		// Draw house Model
		glPushMatrix();
		glRotatef(90.f, 1, 0, 0);
		model_house.Draw();
		glPopMatrix();

		// Draw ball Model
		glPushMatrix();
		glTranslatef(ballPosX, ballPosY, ballPosZ);
		glScalef(0.02, 0.02, 0.02);
		model_ball.Draw();
		glPopMatrix();

		// Draw lamp Model
		glPushMatrix();
		glTranslatef(15, 0, 15);
		glScalef(0.1, 0.1, 0.1);
		model_lamp.Draw();
		glPopMatrix();

		// Draw lamp Model2
		glPushMatrix();
		glTranslatef(15, 0, -15);
		glScalef(0.1, 0.1, 0.1);
		model_lamp.Draw();
		glPopMatrix();

		// draw the court
		glPushMatrix();
		//glRotatef(90.f, 1, 0, 0);
		glScalef(100.5, 100.5, 100.5);
		model_court.Draw();
		glPopMatrix();

		// Draw soccer_player
		glPushMatrix();
		glTranslatef(playerPosX, playerPosY, playerPosZ);
		glRotatef(playerRotation, 0, 1, 0);
		glScalef(0.05, 0.05, 0.05);
		model_soccer_player.Draw();
		glPopMatrix();

		// Draw goalkeeper
		glPushMatrix();
		glTranslatef(goalkeeperX, goalkeeperY, goalkeeperZ);
		glRotatef(goalkeeperRotation, 0, 1, 0);
		glRotatef(goalkeeperRotSaving, 0, 0, 1);
		glScalef(0.05, 0.05, 0.05);
		model_goalkeeper.Draw();
		glPopMatrix();


		// Draw soccer2
		glPushMatrix();
		glTranslatef(player2PosX, player2PosY, player2PosZ);
		glRotatef(player2Rotation, 0, 1, 0);
		//glRotatef(90.f, 0, 1, 0);
		glScalef(0.05, 0.05, 0.05);
		model_soccer2.Draw();
		glPopMatrix();


		//draw net
		glPushMatrix();
		glTranslatef(15, 3, 0);
		glScalef(0.3, 0.3, 0.3);
		model_net.Draw();
		glPopMatrix();

		//sky box
		glPushMatrix();

		GLUquadricObj* qobj;
		qobj = gluNewQuadric();
		glScalef(1, 1, 1);
		glTranslated(-50, 0, 0);
		glRotated(90, 1, 0, 1);
		glBindTexture(GL_TEXTURE_2D, tex);
		gluQuadricTexture(qobj, true);
		gluQuadricNormals(qobj, GL_SMOOTH);
		gluSphere(qobj, 100, 100, 100);
		gluDeleteQuadric(qobj);
		UpdateCameraPosition();
		UpdateCameraPositionFirst();

		glPopMatrix();

		if (isPlayerMoving == 1) {
			MovePlayerTowardBall();
		}
		if (isBallMoving == 1) {
			MoveBall(targetRegion);

			isGoalkeeperMoving = 1;

		}
		if (isBallDropping == 1) {
			dropBall();
		}
		if (firstPlayerReturn == 1) {

			returnPlayer1();
			//inDelay = true;
			//glutTimerFunc(1000, TimerCallback, 0);
		}
		if (oneOut == 1) {

			oneOutTwoIn();
		}

		if (isPlayer2Moving == 1) {
			MovePlayer2TowardBall();
		}
		if (secondPlayerReturn == 1) {
			returnPlayer2();
			//inDelay = true;
			//glutTimerFunc(2000, TimerCallback, 0);
		}
		if (twoOut == 1) {
			twoOutOneIn();
		}
		if (isGoalkeeperMoving) {
			MoveGoalkeeper(goalkeeperTargetRegion);
		}
		if (isGoalkeeperReturning) {
			ReturnGoalkeeper();
			isGoalkeeperReturning = 0;

		}
		if (isCelebrating) {
			playerCelebration(); // Call celebration animation

		}
		if (inDelay) {
			return; // Skip rendering when in delay
		}
		displayScoresAndShots();
		//printf(" player1score %i , player2score %i \n", player1Score, player2Score);
	}
	glutSwapBuffers();
}

//=======================================================================
// Keyboard Function
//=======================================================================
void myKeyboard(unsigned char button, int x, int y)
{
	float movementSpeed = 0.1f;
	float proposedX = playerPosX;
	int region = 0;
	float d = 0.1;
	switch (button)
	{
	case '7':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case '8':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case '9': //top


		camera.eye = { 3.0f, 50.0f, 0.2f };
		camera.center = { 3.0f, 0.0f, 0.2f };
		camera.up = { 0.0f, 0.0f, -1.0f };

		break;
	case '0':
		if (SecondEnvironment) {
			camera.eye = { 16.917969, 4.320749, 0.393479 };
			camera.center = { 15.959846 ,4.034554,   0.383686 };
			camera.up = { -0.285830, 0.957863, 0.028274 };
		}
		else {
			camera.eye = { -15.935677, 3.037808, -1.251460 };
			camera.center = { -13.950855 ,2.888865,   -1.024079 };
			camera.up = { 3.0, 25.0, 0.5 };
		}
	case 's': // Move down
		//playerPosX -= movementSpeed;
		//playerRotation = 270.0f;
		//UpdateCameraPosition();

		break;
	case 'w': // Move up
		proposedX += movementSpeed;
		playerRotation = 90.0f;
		UpdateCameraPosition();

		break;
	case 'd': // Move right
		//playerPosZ -= movementSpeed;
		//playerRotation = 180.0f;
		//UpdateCameraPosition();

		break;
	case 'a': // Move left
		//playerPosZ += movementSpeed;
		//playerRotation = 0.0f;
		//UpdateCameraPosition();

		break;
	case 'i':   //w
		camera.moveY(d);
		break;
	case 'k':   //s
		camera.moveY(-d);
		break;
	case 'j':  //a
		camera.moveX(d);
		break;
	case 'l':  //d
		camera.moveX(-d);
		break;
	case 'u':
		camera.moveZ(d);
		break;
	case 'o':
		camera.moveZ(-d);
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
	if (proposedX >= minX && proposedX <= maxX) {
		playerPosX = proposedX;
	}

	if (SecondEnvironment) {
		printf("hhh");
		if (button >= '1' && button <= '6' && button != '4' && isStreetPlayerMoving == 0 && Currgoalkeeper) {

			goalkeeperTargetRegion = button - '0';
			targetRegion = ChooseRandomRegion();
			if (targetRegion == goalkeeperTargetRegion)
			{
				player1BlockedGoal = true;
			}
			else
			{
				isIn1SecondEnvironment = true;
			}
			goalKeeper1Shots++;
			isStreetPlayerMoving = 1;
			printf("%i target \n", targetRegion);
			printf("%i keeper\n", goalkeeperTargetRegion);
		}
		if (button >= '1' && button <= '6' && button != '4' && isStreetPlayerMoving == 0 && !Currgoalkeeper) {
			printf("second goalkeeper in");

			goalkeeperTargetRegion = button - '0';
			targetRegion = ChooseRandomRegion();
			if (targetRegion == goalkeeperTargetRegion)
			{
				player2BlockedGoal = true;
			}
			else
			{
				isIn2SecondEnvironment = true;
			}
			goalKeeper2Shots++;
			isStreetPlayerMoving = 1;
			printf("%i target \n", targetRegion);
			printf("%i keeper\n", goalkeeperTargetRegion);
		}

	}
	else {
		if (button >= '1' && button <= '6' && isPlayerMoving == 0 && currentlyPlaying == 1) {
			targetRegion = button - '0'; // Convert ASCII value to integer (region 1-6)
			goalkeeperTargetRegion = ChooseRandomRegion();
			if (goalkeeperTargetRegion != targetRegion) {
				isIn1 = true;
			}
			else
			{
				player1HasNotEnteredGoal = true;
			}
			player1Shots++;
			isPlayerMoving = 1;
		}
		if (button >= '1' && button <= '6' && isPlayer2Moving == 0 && currentlyPlaying == 2) {
			targetRegion = button - '0'; // Convert ASCII value to integer (region 1-6)
			goalkeeperTargetRegion = ChooseRandomRegion();
			if (goalkeeperTargetRegion != targetRegion) {
				isIn2 = true;
			}
			else
			{
				player2HasNotEnteredGoal = true;
			}
			player2Shots++;
			isPlayer2Moving = 1;
		}
	}
	//printf("%i", player2Shots);
	if (player1Shots == maxScore && player2Shots == maxScore) {
		//n render scond environment 
		determineWinner();
	}
	if (button == 't') {
		isThirdPersonView = !isThirdPersonView; // Toggle third-person view
		UpdateCameraPosition(); // Update camera position based on the current view mode
	}
	if (button == 'f') { // Press 'f' to toggle first-person view
		isFirstPersonView = !isFirstPersonView;
		UpdateCameraPositionFirst();

	}
	//printf("%i \n", goalkeeperTargetRegion);

	glutPostRedisplay();
}

//=======================================================================
// Motion Function
//=======================================================================
void myMotion(int x, int y)
{
	y = HEIGHT - y;

	if (cameraZoom - y > 0)
	{
		Eye.x += -0.1;
		Eye.z += -0.1;
	}
	else
	{
		Eye.x += 0.1;
		Eye.z += 0.1;
	}

	cameraZoom = y;

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}

//=======================================================================
// Mouse Function
//=======================================================================
void myMouse(int button, int state, int x, int y) {
	// Check if the left button is pressed
	if (!SecondEnvironment) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			// Toggle third-person view
			isThirdPersonView = !isThirdPersonView;
			UpdateCameraPosition(); // Update camera position based on the current view mode
		}
		if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
			// Toggle first-person view
			isFirstPersonView = !isFirstPersonView;  // Assuming you have a variable isFirstPersonView
			UpdateCameraPositionFirst(); // Update camera position for first-person view
		}
		// Existing code for handling zoom
		y = HEIGHT - y;
		if (state == GLUT_DOWN) {
			cameraZoom = y;
		}
	}
	else {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			// Toggle third-person view
			isThirdPersonView = !isThirdPersonView;
			UpdateStreetCameraPosition(); // Update camera position based on the current view mode
		}
		if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
			// Toggle first-person view
			isFirstPersonView = !isFirstPersonView;  // Assuming you have a variable isFirstPersonView
			UpdateCameraPositionStreetFirst(); // Update camera position for first-person view
		}

	}

}

//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}

//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	//model_ball.Load("Models/streetball/Ball Football Nike orange N060122.3ds");
	model_streetball.Load("Models/stball/170112_ball.3DS");
	model_ball.Load("Models/kora/Ball Jabulani N230523.3ds");
	model_lamp.Load("Models/lamp/Lamp post N120808.3DS");
	model_lamp2.Load("Models/lamp/Lamp post N120808.3DS");
	//model_ball.Load("Models/bol/Ball N290808.3DS");
	//model_net.Load("Models/gate/Gate.3ds");
	model_net.Load("Models/net/Gates  N140608.3ds");

	//model_house.Load("Models/house/house.3DS");
	//model_tree.Load("Models/tree/Tree1.3ds");
	//model_soccer_player.Load("Models/Soccer_player/soccer.3ds");
	model_soccer_player.Load("Models/footballer/Man footballer Messi N241113.3DS");
	model_streetplayer.Load("Models/streetplayer/Man footballer Messi N241113.3DS");

	model_soccer2.Load("Models/footballer1/Man footballer Messi N241113.3DS");
	model_goalkeeper.Load("Models/goalkeeper/Man footballer Messi N241113.3DS");
	model_streetgoalkeeper1.Load("Models/keeper1/Man footballer Messi N241113.3DS");
	model_streetgoalkeeper2.Load("Models/2keeper/Man footballer Messi N241113.3DS");

	//model_court.Load("Models/football-court/Stadium N210517.3DS");
	// Loading texture files
	tex_ground.Load("Textures/ground.bmp");
	tex_ground2.Load("Textures/street.bmp");
	tex_wall_front.Load("Textures/fans.bmp");
	tex_wall_front2.Load("Textures/footballStreet.bmp");
	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);
	glutSpecialFunc(Special);
	glutKeyboardFunc(myKeyboard);

	glutMotionFunc(myMotion);

	glutMouseFunc(myMouse);

	glutReshapeFunc(myReshape);

	myInit();

	

	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light

	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	LoadAssets();
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);
	glutTimerFunc(16, Timer, 0);

	sound_Buffer.loadFromFile("C:/Users/HP/Downloads/background.wav");
	sound.setPitch(1.0f);
	sound.setVolume(7.0f);
	sound.setBuffer(sound_Buffer);
	sound.setMinDistance(5.0f);
	sound.setAttenuation(5.0f);
	sound.setLoop(true);
	sound.play();

	if (!targetSoundBuffer.loadFromFile("C:/Users/HP/Downloads/kick.wav")) {
		std::cerr << "Failed to load collision sound!" << std::endl;
		// Handle the error (e.g., exit the game)
	}
	targetSound.setBuffer(targetSoundBuffer);
	targetSound.setPitch(1.0f);
	targetSound.setVolume(50.0f);
	targetSound.setMinDistance(5.0f);
	targetSound.setAttenuation(5.0f);

	if (!collSoundBuffer.loadFromFile("C:/Users/HP/Downloads/save.wav")) {
		std::cerr << "Failed to load collision sound!" << std::endl;
		// Handle the error (e.g., exit the game)
	}
	collSound.setBuffer(collSoundBuffer);
	collSound.setPitch(1.0f);
	collSound.setVolume(50.0f);
	collSound.setMinDistance(5.0f);
	collSound.setAttenuation(5.0f);

	if (!winSoundBuffer.loadFromFile("C:/Users/HP/Downloads/goal.wav")) {
		std::cerr << "Failed to load collision sound!" << std::endl;
		// Handle the error (e.g., exit the game)
	}
	winSound.setBuffer(winSoundBuffer);
	winSound.setPitch(1.0f);
	winSound.setVolume(50.0f);
	winSound.setMinDistance(5.0f);
	winSound.setAttenuation(5.0f);

	glutMainLoop();
}