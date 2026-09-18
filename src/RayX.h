#ifndef CS4212_GRAPHICS_RAYX__147651356622894__
#define CS4212_GRAPHICS_RAYX__147651356622894__

#include "helpers.h"
#include "vecx.h"

template<Floating T, std::size_t X>
class RayX {
    protected:
    VecX<T, X> 
        point_,
        dir_; 

    public:
    #pragma region Constructor
    RayX(){}
    RayX(const VecX<T, X>& point, const VecX<T, X>& dir): point_{point}, dir_{dir} {}
    
    #pragma endregion

    #pragma region Getters
    const VecX<T, X>& origin() const {return point_;}
    const VecX<T, X>& direction() const {return dir_;}

    #pragma endregion

    #pragma region Misc
    VecX<T, X> at(T t) const {
        return origin() + t * direction();
    }

    #pragma endregion
};

#endif