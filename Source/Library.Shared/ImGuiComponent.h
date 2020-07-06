#pragma once
#include "DrawableGameComponent.h"
namespace Library
{
	class ImGuiComponent : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(ImGuiComponent, DrawableGameComponent);
	public:
		enum class Styles
		{
			Classic,
			Light,
			Dark
		};

		using RenderBlock = std::function<void(void)>;

		ImGuiComponent(Game& game, Styles style = Styles::Dark, bool useCustomDraw = false);
		~ImGuiComponent();

		inline Styles Style() const { return mStyle; }
		void SetStyle(Styles style);

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

		inline bool UseCustomDraw() const { return mUseCustomDraw; }
		inline void SetUseCustomDraw(bool useCustomDraw) { mUseCustomDraw = useCustomDraw; }
		void CustomDraw();

		inline const std::vector<std::shared_ptr<RenderBlock>>& RenderBlocks() const { return mRenderBlocks; }
		void AddRenderBlock(std::shared_ptr<RenderBlock> block);
		void RemoveRenderBlock(std::shared_ptr<RenderBlock> block);
	private:
		std::vector<std::shared_ptr<RenderBlock>> mRenderBlocks;
		Styles mStyle;
		bool mUseCustomDraw;
	};

	template <typename T>
	void AddImGuiTextField(const std::string& description, const T& value, std::streamsize precision = 0)
	{
		std::stringstream label;
		if (precision > 0)
		{
			label << std::setprecision(precision);
		}
		label << description << value;
		ImGui::Text(label.str().c_str());
	}
}

