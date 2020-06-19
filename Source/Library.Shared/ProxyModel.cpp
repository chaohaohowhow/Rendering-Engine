#include "pch.h"
#include "ProxyModel.h"
#include "Utility.h"
#include "Model.h"
#include "MatrixHelper.h"
#include "Camera.h"

using namespace glm;
using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(ProxyModel);

	ProxyModel::ProxyModel(Game& game, shared_ptr<Camera> camera, const string& modelFileName, float scale) :
		DrawableGameComponent(game, move(camera)),
		mModelFileName(modelFileName)
	{
		mScaleMatrix = glm::scale(mat4(1), vec3(scale));
	}

	ProxyModel::~ProxyModel()
	{
		glDeleteBuffers(1, &mIndexBuffer);
		glDeleteBuffers(1, &mVertexBuffer);
		glDeleteVertexArrays(1, &mVertexArrayObject);
	}

	const vec3& ProxyModel::Position() const
	{
		return mPosition;
	}

	const vec3& ProxyModel::Direction() const
	{
		return mDirection;
	}

	const vec3& ProxyModel::Up() const
	{
		return mUp;
	}

	const vec3& ProxyModel::Right() const
	{
		return mRight;
	}

	bool& ProxyModel::DisplayWireframe()
	{
		return mDisplayWireframe;
	}

	void ProxyModel::SetPosition(float x, float y, float z)
	{
		mPosition = vec3(x, y, z);
	}

	void ProxyModel::SetPosition(const vec3& position)
	{
		mPosition = position;
	}

	void ProxyModel::ApplyRotation(const mat4& transform)
	{
		vec4 direction = transform * vec4(mDirection, 0.0f);
		mDirection = static_cast<vec3>(normalize(direction));

		vec4 up = transform * vec4(mUp, 0.0f);
		mUp = static_cast<vec3>(normalize(up));

		mRight = cross(mDirection, mUp);
		mUp = cross(mRight, mDirection);
	}

	void ProxyModel::Initialize()
	{
		// Build the shader program
		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content\\Shaders\\BasicEffect.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content\\Shaders\\BasicEffect.frag"));
		mShaderProgram.BuildProgram(shaders);

		Model model(mModelFileName, true);

		// Create the vertex and index buffers
		shared_ptr<Mesh> mesh = model.Meshes().at(0);
		VertexPositionColor::CreateVertexBuffer(*mesh, mVertexBuffer);
		mesh->CreateIndexBuffer(mIndexBuffer);
		mIndexCount = mesh->Indices().size();

		// Create the vertex array object
		glGenVertexArrays(1, &mVertexArrayObject);
		mShaderProgram.Initialize(mVertexArrayObject);
		glBindVertexArray(0);
	}

	void ProxyModel::Update(const GameTime& /*gameTime*/)
	{
		mat4 worldMatrix(1);
		MatrixHelper::SetForward(worldMatrix, mDirection);
		MatrixHelper::SetUp(worldMatrix, mUp);
		MatrixHelper::SetRight(worldMatrix, mRight);
		MatrixHelper::SetTranslation(worldMatrix, mPosition);

		mWorldMatrix = worldMatrix * mScaleMatrix;
	}

	void ProxyModel::Draw(const GameTime& /*gameTime*/)
	{
		glBindVertexArray(mVertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

		mShaderProgram.Use();

		mat4 wvp = mCamera->ViewProjectionMatrix() * mWorldMatrix;
		mShaderProgram.WorldViewProjection() << wvp;

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		if (mDisplayWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);
		}

		glBindVertexArray(0);
	}
}