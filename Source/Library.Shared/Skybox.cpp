#include "pch.h"
#include "Skybox.h"
#include "Utility.h"
#include "Model.h"
#include "Camera.h"
#include "TextureHelper.h"

using namespace glm;
using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(Skybox);

	Skybox::Skybox(Game& game, std::shared_ptr<Camera> camera, const string& posXFilename, const string& negXFilename, const string& posYFilename, const string& negYFilename, const string& posZFilename, const string& negZFilename, float scale) :
		DrawableGameComponent(game, move(camera)),
		mPosXFilename(posXFilename), mNegXFilename(negXFilename), mPosYFilename(posYFilename), mNegYFilename(negYFilename), mPosZFilename(posZFilename), mNegZFilename(negZFilename),
		mScaleMatrix (glm::scale(mat4(1), vec3(scale)))
	{
	}

	Skybox::~Skybox()
	{
		glDeleteTextures(1, &mSkyboxTexture);
		glDeleteBuffers(1, &mIndexBuffer);
		glDeleteBuffers(1, &mVertexBuffer);
		glDeleteVertexArrays(1, &mVertexArrayObject);
	}

	void Skybox::Initialize()
	{
		// Build the shader program
		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content\\Shaders\\Skybox.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content\\Shaders\\Skybox.frag"));
		mShaderProgram.BuildProgram(shaders);

		Model model("Content/Models/Sphere.obj.bin");

		// Create the vertex and index buffers
		shared_ptr<Mesh> mesh = model.Meshes().at(0);
		VertexPositionColor::CreateVertexBuffer(*mesh, mVertexBuffer);
		mesh->CreateIndexBuffer(mIndexBuffer);
		mIndexCount = mesh->Indices().size();

		// Load cube map texture
		mSkyboxTexture = TextureHelper::LoadCubeMap(mPosXFilename.c_str(), mNegXFilename.c_str(), mPosYFilename.c_str(), mNegYFilename.c_str(), mPosZFilename.c_str(), mNegZFilename.c_str(), GL_CLAMP_TO_EDGE, GL_LINEAR);

		// Create the vertex array object
		glGenVertexArrays(1, &mVertexArrayObject);
		mShaderProgram.Initialize(mVertexArrayObject);
	}

	void Skybox::Update(const GameTime& /*gameTime*/)
	{
		mWorldMatrix = translate(mat4(1), mCamera->Position()) * mScaleMatrix;
	}

	void Skybox::Draw(const GameTime& /*gameTime*/)
	{
		glBindVertexArray(mVertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

		mShaderProgram.Use();

		mat4 wvp = mCamera->ViewProjectionMatrix() * mWorldMatrix;
		mShaderProgram.WorldViewProjection() << wvp;

		glBindTexture(GL_TEXTURE_CUBE_MAP, mSkyboxTexture);

		glDisable(GL_CULL_FACE);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);
	}
}