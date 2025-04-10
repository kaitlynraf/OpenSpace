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

#ifndef __OPENSPACE_MODULE_SPACE___RENDERABLECONSTELLATIONLINES___H__
#define __OPENSPACE_MODULE_SPACE___RENDERABLECONSTELLATIONLINES___H__

#include <modules/space/rendering/renderableconstellationsbase.h>

#include <ghoul/opengl/uniformcache.h>
#include <unordered_map>

namespace ghoul::filesystem { class File; }
namespace ghoul::fontrendering { class Font; }
namespace ghoul::opengl {
    class ProgramObject;
    class Texture;
} // namespace ghoul::opengl

namespace openspace {

namespace documentation { struct Documentation; }

class RenderableConstellationLines : public RenderableConstellationsBase {
public:
    explicit RenderableConstellationLines(const ghoul::Dictionary& dictionary);
    ~RenderableConstellationLines() override = default;

    void initialize() override;
    void initializeGL() override;
    void deinitializeGL() override;

    bool isReady() const override;

    void render(const RenderData& data, RendererTasks& tasks) override;
    void update(const UpdateData& data) override;

    static documentation::Documentation Documentation();

private:
    struct ConstellationLine {
        bool isEnabled = true;
        std::string name;
        int lineIndex;
        int colorIndex;
        int numV;
        GLuint vaoArray;
        GLuint vboArray;
        std::vector<GLfloat> vertices;
    };

    void createConstellations();
    void renderConstellations(const RenderData& data, const glm::dmat4& modelViewMatrix,
        const glm::dmat4& projectionMatrix);

    void loadData();

    /**
     * Callback method that gets triggered when `_constellationSelection` changes.
     */
    void selectionPropertyHasChanged() override;

    properties::StringProperty _speckFile;
    properties::BoolProperty _drawElements;

    std::unique_ptr<ghoul::opengl::ProgramObject> _program = nullptr;
    UniformCache(modelViewTransform, projectionTransform, opacity,
        color) _uniformCache;

    DistanceUnit _constellationUnit = DistanceUnit::Parsec;

    std::vector<float> _fullData;

    std::unordered_map<int, glm::vec3> _constellationColorMap;
    std::unordered_map<int, ConstellationLine> _renderingConstellationsMap;
};
} // namespace openspace

#endif // __OPENSPACE_MODULE_SPACE___RENDERABLECONSTELLATIONLINES___H__
