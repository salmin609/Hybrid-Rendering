/*************************************************************
 *	Author				: Kim sang min.
 *	Assignment number	: Project assignment
 *	Course				: CS250.
 *  Term				: Spring 2020.
 *************************************************************/

#pragma once
class Texture;
class Shader;

class Ground
{
public:
    Ground();
    ~Ground();
    static Texture* groundHeightMapTexture;
	static Shader* groundShader;
private:
    //Texture* groundHeightMapTexture;
    unsigned terrianColor;
};