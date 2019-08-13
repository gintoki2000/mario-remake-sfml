#ifndef FONTMANAGER_H
#define FONTMANAGER_H
#include <SFML/Graphics/Font.hpp>
#include "base.h"
#include <map>
enum TFontID
{
    EUbuntuRegularFont
};
class FontManager
{
public:
    static TBoolean load(const TString& fileName, TInt ID);

    static sf::Font* get(TInt ID);

    static void unLoad(TInt ID);

    static void unLoadAll();

private:
    static std::map<TInt, sf::Font*> sFonts;
};

#endif // FONTMANAGER_H
