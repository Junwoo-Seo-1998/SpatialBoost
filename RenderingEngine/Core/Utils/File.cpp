/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs350_1
Author: Junwoo Seo, junwoo.seo, 0055213
End Header --------------------------------------------------------*/
#include "File.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Core/Data/TextureData.h"
#include "stb_image.h"
std::string File::ReadFileToString(const std::string& file_name)
{
    std::ifstream file(file_name);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

std::shared_ptr<TextureData> File::ReadImageToTexture(const std::string& file_name)
{
    if (!std::filesystem::exists(file_name))
    {
        //Log::Error("There is no file : {}", file_name);
        return {};
    }
    std::shared_ptr<TextureData> texture = std::make_shared<TextureData>();
    stbi_set_flip_vertically_on_load(true);
    int number_of_channels = 0;
    unsigned char* img = stbi_load(file_name.c_str(), &(texture->width), &(texture->height), &number_of_channels, 0);
    if (img == nullptr)
    {
        std::cout << "error - on loading texture" << std::endl;
    }
    switch (number_of_channels)
    {
    case 3:
        texture->channel = TextureChannel::RGB;
        break;
    case 4:
        texture->channel = TextureChannel::RGBA;
        break;
    default:
        //ENGINE_ASSERT(false, "Not Supported Channel");
        break;
    }
    std::shared_ptr<unsigned char[]> data{ img, [](unsigned char* to_delete) { stbi_image_free(to_delete); } };
    texture->data = data;
    return texture;
}
