/*************************************************************
 *	Author				: Kim sang min.
 *	Assignment number	: Project assignment
 *	Course				: CS250.
 *  Term				: Spring 2020.
 *************************************************************/

#include "Object_ExceptShadow.h"

Object_ExceptShadow::Object_ExceptShadow(Mesh* mesh_val, Vector color) : color_val(color)
{
	mesh = mesh_val;
	obj_pos = Point();
	shader_id = mesh->Get_Shader_Id();
	vao_id = mesh->Get_VAO_Id();
	color_uniform = glGetUniformLocation(shader_id, "color_val");
	ndc_uniform = glGetUniformLocation(shader_id, "to_ndc");
	world_uniform = glGetUniformLocation(shader_id, "model");
	cam_uniform = glGetUniformLocation(shader_id, "cam");
	shadow_uniform = glGetUniformLocation(shader_id, "shadow_mat");
	light_uniform = glGetUniformLocation(shader_id, "light_pos");
}

Object_ExceptShadow::Object_ExceptShadow(Mesh* mesh_val, Point obj_pos, Vector obj_scale, Vector color) : mesh(mesh_val), obj_pos(obj_pos), obj_scale(obj_scale), color_val(color)
{
	shader_id = mesh->Get_Shader_Id();
	vao_id = mesh->Get_VAO_Id();
	color_uniform = glGetUniformLocation(shader_id, "color_val");
	ndc_uniform = glGetUniformLocation(shader_id, "to_ndc");
	world_uniform = glGetUniformLocation(shader_id, "model");
	cam_uniform = glGetUniformLocation(shader_id, "cam");
	shadow_uniform = glGetUniformLocation(shader_id, "shadow_mat");
	light_uniform = glGetUniformLocation(shader_id, "light_pos");
}

void Object_ExceptShadow::Draw(Matrix& ndc, Matrix& cam_mat, Matrix& world_mat, Matrix& shadow_mat)
{
	Select_Mesh();
	Send_Uniform(ndc, cam_mat, world_mat, shadow_mat);
	glDrawArrays(GL_TRIANGLES, 0, mesh->FaceCount() * 3);
	End_Draw();
}

void Object_ExceptShadow::Draw(Matrix& ndc, Matrix& cam_mat, Matrix shadow_mat)
{
	Select_Mesh();
	Matrix world_mat = Get_Model_To_World();
	Send_Uniform(ndc, cam_mat, world_mat, shadow_mat);
	glDrawArrays(GL_TRIANGLES, 0, mesh->FaceCount() * 3);
	End_Draw();
}

Matrix Object_ExceptShadow::Get_Model_To_World()
{
	return translate(obj_pos - Point(0, 0, 0)) * scale(obj_scale.x, obj_scale.y, obj_scale.z);
}



void Object_ExceptShadow::Set_Color(Vector color)
{
	color_val = color;
}

void Object_ExceptShadow::End_Draw()
{
	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Object_ExceptShadow::Select_Mesh()
{
	glUseProgram(mesh->Get_Shader_Id());
	glBindVertexArray(mesh->Get_VAO_Id());
}

void Object_ExceptShadow::Send_Uniform(Matrix& ndc_mat, Matrix& cam_mat, Matrix& world_mat, Matrix& shadow_mat)
{
	glUniformMatrix4fv(ndc_uniform, 1, GL_TRUE, &ndc_mat[0][0]);
	glUniformMatrix4fv(world_uniform, 1, GL_TRUE, &world_mat[0][0]);
	glUniformMatrix4fv(cam_uniform, 1, GL_TRUE, &cam_mat[0][0]);
	glUniformMatrix4fv(shadow_uniform, 1, GL_TRUE, &shadow_mat[0][0]);
	glUniform3f(color_uniform, color_val.x, color_val.y, color_val.z);
	glUniform3f(light_uniform, light_pos.x, light_pos.y, light_pos.z);
}


void Object_ExceptShadow::Set_Light_Pos(Point light)
{
	light_pos = light;
}

