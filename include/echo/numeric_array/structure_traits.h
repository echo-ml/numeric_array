#pragma once

#include <echo/numeric_array/structure.h>

namespace echo {
namespace numeric_array {
namespace structure_traits {

template <class... Structures>
struct fuse {};

template <class Structure1, class Structure2, class... StructuresRest>
struct fuse<Structure1, Structure2, StructuresRest...>
    : fuse<typename fuse<Structure1, Structure2>::type, StructuresRest...> {};

template <class Structure1>
struct fuse<Structure1, structure::scalar> {
  using type = Structure1;
};

template <class Structure1>
struct fuse<structure::scalar, Structure1>
    : fuse<Structure1, structure::scalar> {
};

template<>
struct fuse<structure::scalar, structure::scalar> {
  using type = structure::scalar;
};

template<class Structure>
struct fuse<Structure, Structure> {
  using type = Structure;
};

template<class... Structures>
using fuse_t = typename fuse<Structures...>::type;

}
}
}
