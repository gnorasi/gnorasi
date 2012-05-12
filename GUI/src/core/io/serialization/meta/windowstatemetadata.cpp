/**********************************************************************
 *                                                                    *
 * Voreen - The Volume Rendering Engine                               *
 *                                                                    *
 * Created between 2005 and 2012 by The Voreen Team                   *
 * as listed in CREDITS.TXT <http://www.voreen.org>                   *
 *                                                                    *
 * This file is part of the Voreen software package. Voreen is free   *
 * software: you can redistribute it and/or modify it under the terms *
 * of the GNU General Public License version 2 as published by the    *
 * Free Software Foundation.                                          *
 *                                                                    *
 * Voreen is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       *
 * GNU General Public License for more details.                       *
 *                                                                    *
 * You should have received a copy of the GNU General Public License  *
 * in the file "LICENSE.txt" along with this program.                 *
 * If not, see <http://www.gnu.org/licenses/>.                        *
 *                                                                    *
 * The authors reserve all rights not expressly granted herein. For   *
 * non-commercial academic use see the license exception specified in *
 * the file "LICENSE-academic.txt". To get information about          *
 * commercial licensing please contact the authors.                   *
 *                                                                    *
 **********************************************************************/

#include "voreen/core/io/serialization/meta/windowstatemetadata.h"
#include "voreen/core/utils/stringconversion.h"

namespace voreen {

WindowStateMetaData::WindowStateMetaData(const bool& visible, const int& x, const int& y, const int& width, const int& height)
    : visible_(visible)
    , x_(x)
    , y_(y)
    , width_(width)
    , height_(height)
{
}

std::string WindowStateMetaData::toString() const {
    std::stringstream s;
    if (visible_)
        s << "visible = true; ";
    else
        s << "visible = false; ";

    s << "x = " << itos(x_) << ", ";
    s << "y = " << itos(y_) << "; ";

    s << "width = " << itos(width_) << ", ";
    s << "height = " << itos(height_) << ";";

    return s.str();
}

void WindowStateMetaData::serialize(XmlSerializer& s) const {
    s.serialize("visible", visible_);
    if (x_ != -1)
        s.serialize("x", x_);
    if (y_ != -1)
        s.serialize("y", y_);
    if (width_ != -1)
        s.serialize("width", width_);
    if (height_ != -1)
        s.serialize("height", height_);
}

void WindowStateMetaData::deserialize(XmlDeserializer& s) {
    s.deserialize("visible", visible_);
    try {
        s.deserialize("x", x_);
    } catch (XmlSerializationNoSuchDataException&) {
        // x position was not serialized so just ignore...
        s.removeLastError();
    }
    try {
        s.deserialize("y", y_);
    } catch (XmlSerializationNoSuchDataException&) {
        // y position was not serialized so just ignore...
        s.removeLastError();
    }
    try {
        s.deserialize("width", width_);
    } catch (XmlSerializationNoSuchDataException&) {
        // width was not serialized so just ignore...
        s.removeLastError();
    }
    try {
        s.deserialize("height", height_);
    } catch (XmlSerializationNoSuchDataException&) {
        // height was not serialized so just ignore...
        s.removeLastError();
    }
}

void WindowStateMetaData::setVisible(const bool& value) {
    visible_ = value;
}

bool WindowStateMetaData::getVisible() const {
    return visible_;
}

void WindowStateMetaData::setX(const int& value) {
    x_ = value;
}

int WindowStateMetaData::getX() const {
    return x_;
}

void WindowStateMetaData::setY(const int& value) {
    y_ = value;
}

int WindowStateMetaData::getY() const {
    return y_;
}

void WindowStateMetaData::setWidth(const int& value) {
    width_ = value;
}

int WindowStateMetaData::getWidth() const {
    return width_;
}

void WindowStateMetaData::setHeight(const int& value) {
    height_ = value;
}

int WindowStateMetaData::getHeight() const {
    return height_;
}

MetaDataBase* WindowStateMetaData::clone() const {
    return new WindowStateMetaData(visible_, x_, y_, width_, height_);
}

Serializable* WindowStateMetaData::create() const {
    return new WindowStateMetaData();
}

} // namespace
