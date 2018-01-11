/**
 * \struct ContainerCategory
 * @author Sebastian Hahn (t.hahn < at > pmacc) 
 * @brief This trait is used to specify a categorie of a container. Currently are
 * three categories implemented. They are in the folder Iterator/Categorie. If 
 * a container has no categorie, you need to specify the following traits to use
 * the DeepIterator:
 * The first four are needed for the accesor:
 * 1. pmacc::traits::accessor::Ahead
 * 2. pmacc::traits::accessor::Behind
 * 3. pmacc::traits::accessor::Equal
 * 4. pmacc::traits::accessor::Get
 * The next six are needed for the navigator
 * 5. pmacc::traits::navigator::AfterLastElement
 * 6. pmacc::traits::navigator::BeforeFirstElement
 * 7. pmacc::traits::navigator::FirstElement
 * 8. pmacc::traits::navigator::LastElement 
 * 9. pmacc::traits::navigator::NextElement
 * 10. pmacc::traits::navigator::PreviousElement 
 * If a container isnt bidirectional you doesnt need 1, 2, 6, 8, 10.
 * If a container isnt randon accessable you doesnt need 1,2
 */ 

#pragma once

namespace pmacc 
{
namespace traits
{

    
template<typename T>
struct ContainerCategory
{
    typedef T type;
};

}// namespace traits
    
}// namespace pmacc
