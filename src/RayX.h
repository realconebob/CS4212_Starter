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
    /**
     * @brief Return the origin of the ray
     * 
     * @return const VecX<T, X>& A constant reference to the origin of the ray in Xth dimensional space
     */
    const VecX<T, X>& origin() const {return point_;}
    /**
     * @brief Return the direction of the ray
     * 
     * @return const VecX<T, X>& A constant reference to the direction fo the ray in Xth dimensional space
     */
    const VecX<T, X>& dir() const {return dir_;}

    #pragma endregion

    #pragma region Misc
    /**
     * @brief Get a point along the ray
     * 
     * @param t The magnitude to walk along the ray
     * @return VecX<T, X> An Xth dimensional point in space
     */
    VecX<T, X> at(T t) const {
        return origin() + t * dir();
    }

    #pragma endregion
};

#endif