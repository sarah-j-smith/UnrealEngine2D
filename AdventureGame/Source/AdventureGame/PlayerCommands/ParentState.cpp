#include "ParentState.h"

// Note to self - any template implementation have to go in the ParentState.h file, or
// possibly in a ParentState.inl file so that the compiler can see them to generate the
// template code. They cannot go into the ParentState.cpp file translation unit as they
// won't get compiled into the templates for the concrete template instantiations.
//
//    template <ECommandCodes E>
//    void TFParentState<E>::SomeMemberFunction()
//
// This (above) won't work in this .cpp file. 
