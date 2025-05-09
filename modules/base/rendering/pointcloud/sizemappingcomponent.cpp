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

#include <modules/base/rendering/pointcloud/sizemappingcomponent.h>

#include <openspace/documentation/documentation.h>
#include <openspace/util/distanceconversion.h>
#include <ghoul/logging/logmanager.h>

namespace {
    constexpr std::string_view _loggerCat = "SizeMapping";

    constexpr openspace::properties::Property::PropertyInfo EnabledInfo = {
        "Enabled",
        "Size Mapping Enabled",
        "If this value is set to 'true' and at least one column was loaded as an option "
        "for size mapping, the chosen data column will be used to scale the size of the "
        "points. The first option in the list is selected per default.",
        openspace::properties::Property::Visibility::NoviceUser
    };

    constexpr openspace::properties::Property::PropertyInfo OptionInfo = {
        "Parameter",
        "Parameter Option",
        "This value determines which parameter is used for scaling of the point. The "
        "parameter value will be used as a multiplicative factor to scale the size of "
        "the points. Note that they may however still be scaled by max size adjustment "
        "effects.",
        openspace::properties::Property::Visibility::AdvancedUser
    };

    constexpr openspace::properties::Property::PropertyInfo ScaleFactorInfo = {
        "ScaleFactor",
        "Scale Factor",
        "This value is a multiplicative factor that is applied to the data values that "
        "are used to scale the points, when size mapping is applied.",
        openspace::properties::Property::Visibility::AdvancedUser
    };

    constexpr openspace::properties::Property::PropertyInfo IsRadiusInfo = {
        "IsRadius",
        "Size is Radius",
        "If true, the size value in the data is interpreted as the radius of the points. "
        "Otherwise, it is interpreted as the diameter.",
        openspace::properties::Property::Visibility::AdvancedUser
    };

    struct [[codegen::Dictionary(SizeMappingComponent)]] Parameters {
        // [[codegen::verbatim(EnabledInfo.description)]]
        std::optional<bool> enabled;

        // A list specifying all parameters that may be used for size mapping, i.e.
        // scaling the points based on the provided data columns.
        std::optional<std::vector<std::string>> parameterOptions;

        // [[codegen::verbatim(OptionInfo.description)]]
        std::optional<std::string> parameter;

        // [[codegen::verbatim(ScaleFactorInfo.description)]]
        enum class [[codegen::map(openspace::DistanceUnit)]] ScaleUnit {
            Nanometer,
            Micrometer,
            Millimeter,
            Centimeter,
            Decimeter,
            Meter,
            Kilometer,
            AU,
            Lighthour,
            Lightday,
            Lightmonth,
            Lightyear,
            Parsec,
            Kiloparsec,
            Megaparsec,
            Gigaparsec,
            Gigalightyear
        };

        // The scale used for the size values in the dataset, given as either a string
        // representing a specific unit or a value to multiply all the datapoints with
        // to convert the value to meter. The resulting value will be applied as a
        // multiplicative factor. For example, if the size data is given in is in
        // kilometers then specify either <code>ScaleFactor = 'Kilometer'</code> or
        // <code>ScaleFactor = 1000.0</code>.
        std::optional<std::variant<ScaleUnit, double>> scaleFactor;

        // [[codegen::verbatim(IsRadiusInfo.description)]]
        std::optional<bool> isRadius;
    };
#include "sizemappingcomponent_codegen.cpp"
}  // namespace

namespace openspace {

documentation::Documentation SizeMappingComponent::Documentation() {
    return codegen::doc<Parameters>("base_sizemappingcomponent");
}

SizeMappingComponent::SizeMappingComponent()
    : properties::PropertyOwner({ "SizeMapping", "Size Mapping", "" })
    , enabled(EnabledInfo, true)
    , parameterOption(
        OptionInfo,
        properties::OptionProperty::DisplayType::Dropdown
    )
    , scaleFactor(ScaleFactorInfo, 1.f, 0.f, 1000.f)
    , isRadius(IsRadiusInfo, false)
{
    addProperty(enabled);
    addProperty(parameterOption);
    addProperty(scaleFactor);
    addProperty(isRadius);
}

SizeMappingComponent::SizeMappingComponent(const ghoul::Dictionary& dictionary)
    : SizeMappingComponent()
{
    const Parameters p = codegen::bake<Parameters>(dictionary);

    enabled = p.enabled.value_or(enabled);

    int indexOfProvidedOption = -1;

    if (p.parameterOptions.has_value()) {
        std::vector<std::string> opts = *p.parameterOptions;
        for (size_t i = 0; i < opts.size(); ++i) {
            // Note that options are added in order
            parameterOption.addOption(static_cast<int>(i), opts[i]);

            if (p.parameter.has_value() && *p.parameter == opts[i]) {
                indexOfProvidedOption = static_cast<int>(i);
            }
        }
    }

    if (indexOfProvidedOption >= 0) {
        parameterOption = indexOfProvidedOption;
    }
    else if (p.parameter.has_value()) {
        LERROR(std::format(
            "Error when reading Parameter. Could not find provided parameter '{}' in "
            "list of parameter options. Using default.", *p.parameter
        ));
    }

    if (p.scaleFactor.has_value()) {
        if (std::holds_alternative<Parameters::ScaleUnit>(*p.scaleFactor)) {
            const Parameters::ScaleUnit scaleUnit =
                std::get<Parameters::ScaleUnit>(*p.scaleFactor);
            const DistanceUnit distanceUnit = codegen::map<DistanceUnit>(scaleUnit);
            scaleFactor = static_cast<float>(toMeter(distanceUnit));
        }
        else if (std::holds_alternative<double>(*p.scaleFactor)) {
            scaleFactor = static_cast<float>(std::get<double>(*p.scaleFactor));
        }
    }

    isRadius = p.isRadius.value_or(isRadius);
}

} // namespace openspace
