#include "Level1.h"
#include "Texture.h"
#include "Graphic.h"
#include "Ground.h"


const Vector    WHITE(1, 1, 1),
                PURPLE(1, 0, 1),
                BLACK(0, 0, 0),
                RED(1, 0, 0),
                GREEN(0, 1, 0),
                SKY(0.313725490f, 0.73725490196f, 0.8745098039f),
                HOT_PINK(1.f, 0.00784313725f, 0.55294117647f),
                YELLOW(1.f, 0.83137254901f, 0.f);

Level1::Level1()
{
	deerObj = new LoadedObj("Deer");
	quadObj = new LoadedObj("LowpolyTree");
	Initialize_Material();
	Init_Objects();

	ground = new Ground();
	Level1::Load();
}

void Level1::Load()
{
	Graphic::objects.clear();
	Graphic::objects.push_back(center_circle);
	Graphic::objects.push_back(light);
	Graphic::objects.push_back(right_circle);
	Graphic::objects.push_back(deer);
	Graphic::objects.push_back(tree);
	Graphic::groundId = Ground::groundShader;
	Graphic::light = light;
	Graphic::water = nullptr;
}

void Level1::Update(float dt)
{
	(dt);
}

void Level1::UnLoad()
{
}

Level1::~Level1()
{
	delete deerObj;
	delete quadObj;
	delete center_circle;
	delete right_circle;
	delete light;
	delete deer;
	delete tree;
}

void Level1::Init_Objects()
{
	center_circle = new Object(&snub_mesh, Point(0.f, 1.f, 0.f), &emerald);
	right_circle = new Object(&snub_mesh, Point(2.f, 1.f, 0.f), &obsidian);

	//Need to erase this
	//light = new Object(&light_mesh, Point{ 1.f, 3.f, 0.f }, &pearl, WHITE);
	deer = new Object(deerObj, Point(0.f, 1.f, 0.f), nullptr);
	tree = new Object(quadObj, Point(2.f, 0.f, 0.f), &emerald);
	const Vector scale{ 0.35f, 0.35f, 0.35f };
	center_circle->Set_Scale(scale);
	right_circle->Set_Scale(scale);
	light->Set_Scale(scale);
}

void Level1::Initialize_Material()
{
	emerald.ambient = Vector(0.0215f, 0.1745f, 0.0215f);
	emerald.diffuse = Vector(0.07568f, 0.61424f, 0.07568f);
	emerald.specular = Vector(0.633f, 0.727811f, 0.633f);
	emerald.shiness = 0.6f;

	obsidian.ambient = Vector(0.05375f, 0.05f, 0.06625f);
	obsidian.diffuse = Vector(0.18275f, 0.17f, 0.22525f);
	obsidian.specular = Vector(0.332741f, 0.328634f, 0.346435f);
	obsidian.shiness = 0.3f;

	jade.ambient = Vector(0.135f, 0.2225f, 0.1575f);
	jade.diffuse = Vector(0.54f, 0.89f, 0.63f);
	jade.specular = Vector(0.316228f, 0.316228f, 0.316228f);
	jade.shiness = 0.1f;

	pearl.ambient = Vector(0.25f, 0.20725f, 0.20725f);
	pearl.diffuse = Vector(1.f, 0.829f, 0.829f);
	pearl.specular = Vector(0.296648f, 0.296648f, 0.296648f);
	pearl.shiness = 0.088f;

	ruby.ambient = Vector(0.1745f, 0.01175f, 0.01175f);
	ruby.diffuse = Vector(0.61424f, 0.04136f, 0.04136f);
	ruby.specular = Vector(0.727811f, 0.626959f, 0.626959f);
	ruby.shiness = 0.6f;

	chrome.ambient = Vector(0.25f, 0.25f, 0.25f);
	chrome.diffuse = Vector(0.4f, 0.4f, 0.4f);
	chrome.specular = Vector(0.774597f, 0.774597f, 0.774597f);
	chrome.shiness = 0.6f;

	red_plastic.ambient = Vector(0.0f, 0.0f, 0.0f);
	red_plastic.diffuse = Vector(0.5f, 0.f, 0.0f);
	red_plastic.specular = Vector(0.70f, 0.60f, 0.60f);
	red_plastic.shiness = 0.25f;
}