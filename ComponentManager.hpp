#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include <memory>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "EntityManager.hpp"

class ParentComponentArray {
public:
    virtual ~ParentComponentArray() = default; 
};




using pComponentArrayPointer = std::shared_ptr< ParentComponentArray >;

/**
 * @brief This class stores all the entities that share the specific Component type.
 * The components are stored contigious.
 * This class should not be touched outside engine.
 * @tparam T The Component Type. Must be a subclass of class Component
 */
template <typename T>
class ComponentArray : public ParentComponentArray {
private:

    EntityManager *entity_manager;

    std::size_t size = 0;
    static constexpr std::size_t max_size = 1000;

    std::unordered_map< Entity, size_t > entity_to_index;

    std::array<T, max_size> component_array;
    

public:
    ComponentArray( EntityManager *entity_manager ) {
        this->entity_manager = entity_manager;
        std::cout << std::endl << std::endl << "Created Component Array" << std::endl << std::endl;
    };

    ~ComponentArray() {
        
        std::cout << std::endl << "Size -> " << size << std::endl << "Destruct" << std::endl;
    }

    /**
     * @brief This function ensures that the given Entity for this specific Component Type is registered.
     * 
     * @param entity The entity to register
     * @param component The component to register
     */
    void insert( Entity entity, T component ) {
        std::cout << std::endl << "Called insert with" << std::endl << "  Entity: " << entity << std::endl << "  Component: " << get_name<T>() << std::endl;
        component_array[ size ] = component;
        entity_to_index[ entity ] = size;
        
        entity_manager->register_componente<T>( entity );
        
        ++size;

        std::cout << "Entity: " << entity << " " << component_array.data() << " | Size -> " << size << std::endl;
    }

    /**
     * @brief This function returns the ComponentData for the given entity.
     * 
     * @param entity The entity to return the component data for.
     * @return std::shared_ptr<T> A pointer to the data
     */
    T *get( Entity entity ) {
        std::cout << std::endl << "Called get with" << std::endl << "  Entity: " << entity << std::endl << "  Component: " << get_name<T>() << std::endl;

        return &component_array[ entity_to_index[ entity ] ];
    }

    /**
     * @brief Get the all objectThis function returns the entire list of components.
     * 
     * @return std::shared_ptr< std::array<T, max_size> > The pointer to the list of components.
     */
    std::shared_ptr< std::array<T, max_size> > get_all() {
        std::cout << std::endl << "Called get_all with" << std::endl << "  Component: " << get_name<T>() << std::endl;
        
        std::shared_ptr< std::array<T, max_size> > ptr = std::make_shared< std::array<T, max_size> >( component_array );
        return ptr;
    }

    size_t get_size() {
        return size;
    }

};

/**
 * @brief This class manages the Components. It stores all the registered components with a pointer pointing to the containing ComponentArray.
 * 
 */
class ComponentManager {
private:
    EntityManager *entity_manager;
    std::unordered_map< std::string, pComponentArrayPointer > components{};

public:

    ComponentManager( EntityManager *entity_manager ) {
        this->entity_manager = entity_manager;
    };

    ~ComponentManager() = default;

    /**
     * @brief This function registers a given component
     * 
     * @tparam T The type of the Component to register.
     */
    template <typename T>
    void register_component() {
        std::cout << "Begin Registrering component" << get_name<T>() << std::endl;
        
        components[ get_name<T>() ] = std::make_shared<ComponentArray<T>>( entity_manager );
        
        std::cout << "End Registrering component" << std::endl;
    }

    /**
     * @brief This function returns a pointer to the given Component array.
     * 
     * @tparam T The type of the Component
     * @return std::shared_ptr<ComponentArray<T>> A pointer pointing to the containing ComponentArray.
     */
    template <typename T>
    std::shared_ptr<ComponentArray<T>> get_component_array() {
        std::cout << "String name: " << get_name<T>() << std::endl;
        return std::static_pointer_cast<ComponentArray<T>>( components[ get_name<T>() ] ); 
    }

    template <typename T>
    void add_component( Entity entity, T component ) {
        // get ComponentArray
        std::shared_ptr< ComponentArray<T> > comp_arr = get_component_array<T>();

        // Insert entity into it.
        std::cout << "Adding component: " << get_name<T>() << std::endl;
        
        comp_arr->insert( entity, component );
    }

private:

};

#endif