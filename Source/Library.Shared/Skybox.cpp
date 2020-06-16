#include "pch.h"
#include "Skybox.h"
#include "Utility.h"
#include "Model.h"
#include "Camera.h"

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
		glDeleteSamplers(1, &mSkyboxTextureSampler);
		glDeleteTextures(1, &mSkyboxTexture);
		glDeleteBuffers(1, &mIndexBuffer);
		glDeleteBuffers(1, &mVertexBuffer);
		glDeleteVertexArrays(1, &mVertexArrayObject);
	}

	void Skybox::Initialize()
	{
		// Build the shader program
		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Skybox.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Skybox.frag"));
		mShaderProgram.BuildProgram(shaders);

		Model model("Content/Models/Sphere.obj");

		// Create the vertex and index buffers
		shared_ptr<Mesh> mesh = model.Meshes().at(0);
		VertexPositionColor::CreateVertexBuffer(*mesh, mVertexBuffer);
		mesh->CreateIndexBuffer(mIndexBuffer);
		mIndexCount = mesh->Indices().size();

		mSkyboxTexture = SOIL_load_OGL_cubemap(mPosXFilename.c_str(), mNegXFilename.c_str(), mPosYFilename.c_str(), mNegYFilename.c_str(), mPosZFilename.c_str(), mNegZFilename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		if (mSkyboxTexture == 0)
		{
			throw runtime_error("SOIL_load_OGL_cubemap() failed.");
		}

		glGenSamplers(1, &mSkyboxTextureSampler);
		glSamplerParameteri(mSkyboxTextureSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glSamplerParameteri(mSkyboxTextureSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(mSkyboxTextureSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(mSkyboxTextureSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(mSkyboxTextureSampler, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

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
		glBindSampler(0, mSkyboxTextureSampler);

		mShaderProgram.Use();

		mat4 wvp = mCamera->ViewProjectionMatrix() * mWorldMatrix;
		mShaderProgram.WorldViewProjection() << wvp;

		glBindTexture(GL_TEXTURE_CUBE_MAP, mSkyboxTexture);

		glDisable(GL_CULL_FACE);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);
	}
}