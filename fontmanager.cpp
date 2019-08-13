#include "fontmanager.h"
std::map<TInt, sf::Font*> FontManager::sFonts = std::map<TInt, sf::Font*>();
TBoolean FontManager::load(const TString &fileName, TInt ID)
{
    if(sFonts.find(ID) != sFonts.end()) return true;
    sf::Font* font = new (std::nothrow) sf::Font;
    if(font && font->loadFromFile(fileName))
    {
        sFonts.insert(std::make_pair(ID, font));
        return true;
    }
    delete font;
    font = nullptr;
    return false;
}

sf::Font *FontManager::get(TInt ID)
{
    auto found = sFonts.find(ID);
    if(found == sFonts.end()) return nullptr;
    return found->second;
}

void FontManager::unLoad(TInt ID)
{
    auto found = sFonts.find(ID);
    if(found != sFonts.end())
    {
        delete found->second;
        sFonts.erase(found);
    }
}

void FontManager::unLoadAll()
{
    for(const auto& pair : sFonts)
    {
        delete pair.second;
    }
    sFonts.clear();
}
