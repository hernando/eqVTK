#ifndef EQVTK_FRAMEDATA_H
#define EQVTK_FRAMEDATA_H

#include <co/serializable.h>
#include <eq/client/types.h>

/**
   Frame-specific data.
*/
class FrameData : public co::Serializable
{
public:
    /* Public constructors and destructor */

    FrameData();

    virtual ~FrameData() {}

    /* Public member functions */
    void reset();

    void setCameraPosition(const eq::Vector3f &position);
    void setRotation(const eq::Vector3f &rotation);
    void setModelRotation(const eq::Vector3f &rotation);
    void spinCamera(const float x, const float y );
    void spinModel(const float x, const float y, const float z);
    void moveCamera(const float x, const float y, const float z);

    const eq::Matrix4f &getCameraRotation() const
    {
        return _rotation;
    }
    const eq::Matrix4f &getModelRotation() const
    {
        return _modelRotation;
    }
    const eq::Vector3f &getCameraPosition() const
    {
        return _position;
    }

protected:
    /* Protected member functions */

    /** @sa Object::serialize() */
    virtual void serialize(co::DataOStream &os,
                           const uint64_t dirtyBits);
    /** @sa Object::deserialize() */
    virtual void deserialize(co::DataIStream &is,
                             const uint64_t dirtyBits);

    virtual ChangeType getChangeType() const
    {
        return DELTA;
    }

    /** The changed parts of the data since the last pack(). */
    enum DirtyBits
    {
        DIRTY_CAMERA  = co::Serializable::DIRTY_CUSTOM << 0
    };

private:
    /* Private member attributes */

    eq::Matrix4f _rotation;
    eq::Matrix4f _modelRotation;
    eq::Vector3f _position;
};

#endif
