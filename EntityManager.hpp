#ifndef ENTITY_MANAGER
#define ENTITY_MANAGER

#include <memory>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <iostream>

#include "utils.hpp"

using Entity = unsigned long;

class EntityManager {
private:
    Entity entity_counter = 0;
    std::vector<Entity> entites; 

    std::unordered_map< Entity, std::shared_ptr<std::unordered_set< std::string >>> entity_to_components;

public:

    /**
     * @brief This function registers a new Entity and stores a set containing all it's components.
     * 
     * @return Entity The id of the entity.
     */
    Entity new_entity();

    /**
     * @brief This function registers a component to the given entity
     * 
     * @tparam T The type of the component
     * @param entity The entity id.
     */
    template <typename T>
    void register_componente( Entity entity );

    /**
     * @brief This function returns a vector containing all the entities that shares all the given components 
     * 
     * @param list The list of components to query for.
     * @return std::shared_ptr< std::vector<Entity> > A pointer to the vector of entities.
     */
    std::shared_ptr< std::vector<Entity> > query_components( std::vector< std::string > list );

};

Entity EntityManager::new_entity() {
    Entity ent = entity_counter++;

    entity_to_components[ ent ] = std::make_shared<std::unordered_set< std::string >>();

    return ent;
};

template <typename T>
void EntityManager::register_componente( Entity entity ) {
    auto pVec = entity_to_components[ entity ].get();
    pVec->insert( get_name<T>() );
}

std::shared_ptr< std::vector<Entity> > EntityManager::query_components( std::vector< std::string > list ) {
    std::cout << "get_entities :) " << std::endl;
    std::vector<Entity> return_vec;
    for ( auto it = entity_to_components.cbegin(); it != entity_to_components.cend(); ++it ) {
        const Entity entity = it->first;
        auto set_of_components = it->second;
        
        int count = 0;
        for ( auto component = list.cbegin(); component != list.cend(); ++component )
            count += set_of_components->count( *component );

        if ( count == list.size() )
            return_vec.push_back( entity ); 
    }
    
    return std::make_shared< std::vector<Entity> >( return_vec );
}

#endif