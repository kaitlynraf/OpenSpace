/*****************************************************************************************
 *                                                                                       *
 * OpenSpace                                                                             *
 *                                                                                       *
 * Copyright (c) 2014-2025                                                               *
 *                                                                                       *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this  *
 * software and associated documentation files (the "Software"), to deal in the Software *
 * without restriction, including without limitation the rights to use, copy, modify,    *
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to    *
 * permit persons to whom the Software is furnished to do so, subject to the following   *
 * conditions:                                                                           *
 *                                                                                       *
 * The above copyright notice and this permission notice shall be included in all copies *
 * or substantial portions of the Software.                                              *
 *                                                                                       *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,   *
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A         *
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT    *
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF  *
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  *
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                         *
 ****************************************************************************************/

#ifndef __OPENSPACE_MODULE_BASE___RENDERABLEPLANEIMAGEONLINE___H__
#define __OPENSPACE_MODULE_BASE___RENDERABLEPLANEIMAGEONLINE___H__

#include <modules/base/rendering/renderableplane.h>

#include <openspace/engine/downloadmanager.h>

namespace ghoul::filesystem { class File; }
namespace ghoul::opengl { class Texture; }

namespace openspace {

struct RenderData;
struct UpdateData;

namespace documentation { struct Documentation; }

class RenderablePlaneImageOnline : public RenderablePlane {
public:
    explicit RenderablePlaneImageOnline(const ghoul::Dictionary& dictionary);

    void deinitializeGL() override;

    void update(const UpdateData& data) override;

    static documentation::Documentation Documentation();

protected:
    virtual void bindTexture() override;

private:
    std::future<DownloadManager::MemoryFile> downloadImageToMemory(
        const std::string& url);

    properties::StringProperty _texturePath;

    std::future<DownloadManager::MemoryFile> _imageFuture;
    std::unique_ptr<ghoul::opengl::Texture> _texture;
    glm::vec2 _textureDimensions = glm::vec2(0.f);
    bool _textureIsDirty = false;
};

} // namespace openspace

#endif // __OPENSPACE_MODULE_BASE___RENDERABLEPLANEIMAGEONLINE___H__
