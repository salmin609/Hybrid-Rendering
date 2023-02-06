#include "OutLine.h"
#include "Graphic.h"
#include "Object.h"
#include "Shader.h"
#include "Shader_Table.hpp"

OutLine::OutLine()
{
	outlineShader = new Shader(shader_object_vertex.c_str(),
		shaderSingleColorFragment.c_str());
}

void OutLine::OutlinePrepare()
{
	Object* selectedObj = Graphic::instance->SelectedObject();

	if (selectedObj != nullptr)
	{
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
	}
}

void OutLine::Draw()
{
	Object* selectedObj = Graphic::instance->SelectedObject();

	if (selectedObj != nullptr)
	{
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		outlineShader->Use();
		//outlineShader->SendUniformMat("to_ndc", &ndcMat);
		//outlineShader->SendUniformMat("cam", &camMat);

		selectedObj->SetVAO();

		Matrix model2World = selectedObj->GetModelToWorldOutlineScaling(0.1f);
		Mesh* mesh = selectedObj->Get_Mesh();
		outlineShader->SendUniformMat("model", &model2World);


		if (selectedObj->IsElemented())
		{
			glDrawElements(GL_TRIANGLES, mesh->FaceCount() * 3, GL_UNSIGNED_INT, nullptr);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, mesh->FaceCount() * 3);
		}

		glBindVertexArray(0);
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);
	}
}
