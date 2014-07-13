/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
#include "FxerPch.h"
#include "TextureBrowseCell.h"

namespace tools
{
	void TextureBrowseCell::getCellDimension(MyGUI::Widget* _sender, MyGUI::IntCoord& _coord, bool _drop)
	{
		_coord.set(0, 0, 130, 130);
	}

	TextureBrowseCell::TextureBrowseCell(MyGUI::Widget* _parent) :
		wraps::BaseCellView<std::pair<std::string,std::string>>("FxTextureBrowseCell.layout", _parent)
	{
		assignWidget(mTextureName, "TextureName");
		assignWidget(mSelector, "Selector");
		assignWidget(mBack, "Back");
		assignWidget(mParentBack, "ParentBack");
		assignWidget(mImage, "Image");
        
        mTextureName->setFontName("DejaVuSans.13");
	}

	void TextureBrowseCell::update(const MyGUI::IBDrawItemInfo& _info, std::pair<std::string,std::string> _dataPair)
	{
        std::string _data = _dataPair.first;
        std::string _forceTexture = _dataPair.second;
		if (_info.update)
		{
            DiString filename = _data.c_str();
			mTextureName->setCaption(filename.ExtractFileName().c_str());
			const MyGUI::IntSize textureSize = MyGUI::texture_utility::getTextureSize(_forceTexture.empty() ? _data : _forceTexture);

			if (textureSize.width != 0 && textureSize.height != 0)
			{
				mBack->setVisible(true);
                mImage->setImageTexture(_forceTexture.empty() ? _data : _forceTexture);

				const MyGUI::IntSize& targetSize = mParentBack->getSize();

				float k1 = (float)targetSize.width / textureSize.width;
				float k2 = (float)targetSize.height / textureSize.height;
				float k = std::min( k1, k2);
				MyGUI::IntSize size = MyGUI::IntSize((int)(k *  (float)textureSize.width), (int)(k *  (float)textureSize.height));

				MyGUI::IntSize parentSize = mBack->getParent()->getSize();
				mBack->setCoord((parentSize.width - size.width) / 2, (parentSize.height - size.height) / 2, size.width, size.height);
			}
			else
			{
				mBack->setVisible(false);
			}
		}

		if (_info.select)
			mSelector->setAlpha(1);
		else
			mSelector->setAlpha(0);
	}

} // namespace tools
