#include "WaterRenderer.h"
#include "Graphic.h"
#include "vs2017/FrameBufferTexturing.h"
#include "Texture.h"
WaterRenderer::WaterRenderer()
{
	waterReflectTexture = new FrameBuffer();
	waterRefractTexture = new FrameBuffer();
	waterDuDv = new Texture("dudvMap.png");
	normalMap = new Texture("normal.png");
}

WaterRenderer::~WaterRenderer()
{
	delete waterReflectTexture;
	delete waterRefractTexture;
	delete waterDuDv;
	delete normalMap;
}

void WaterRenderer::Render(float dt, Matrix ndcMat, Matrix camMat)
{
	Object* waterObj = Graphic::water;
	
	if (waterObj != nullptr)
	{
		moveFactor += moveSpeed * dt;

		if (moveFactor > 1.f)
		{
			moveFactor = 0.f;
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		Shader* shader = waterObj->Get_Mesh()->GetShader();
		unsigned vaoId = waterObj->Get_Mesh()->Get_VAO_Id();
		Matrix model = waterObj->Get_Model_To_World();
		Point cameraPos = CameraManager::instance->GetCamera()->Eye();
		Vector3 cameraPosInVec3 = Vector3{ cameraPos.x ,cameraPos.y , cameraPos.z };
		Point lightPosition = Graphic::light->Get_Obj_Pos();
		Vector3 lightPosInVec3{ lightPosition.x, lightPosition.y, lightPosition.z };
		Vector3 lightColor{ 1.f, 1.f, 1.f };
		shader->Use();
		glBindVertexArray(vaoId);
		shader->SendUniformMat("model", &model);
		shader->SendUniformMat("to_ndc", &ndcMat);
		shader->SendUniformMat("cam", &camMat);
		shader->SendUniformFloat("moveFactor", &moveFactor);
		shader->SendUniformVec3("cameraPosition", &cameraPosInVec3);
		shader->SendUniformVec3("lightPosition", &lightPosInVec3);
		shader->SendUniformVec3("lightColor", &lightColor);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, waterReflectTexture->TextureId());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, waterRefractTexture->TextureId());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, waterDuDv->GetTextureId());
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, normalMap->GetTextureId());
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, waterRefractTexture->DepthTextureId());

		glDrawArrays(GL_TRIANGLES, 0, 3 * waterObj->Get_Mesh()->FaceCount());

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glDisable(GL_BLEND);
}

FrameBuffer* WaterRenderer::GetReflectTexture()
{
	return waterReflectTexture;
}

FrameBuffer* WaterRenderer::GetRefractTexture()
{
	return waterRefractTexture;
}
