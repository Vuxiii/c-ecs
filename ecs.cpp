#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <memory>
#include <typeinfo>

#include "EntityManager.hpp"
#include "ComponentManager.hpp"

#include "utils.hpp"

using Entity = unsigned long;

template <typename T>
using pComponentArray = std::shared_ptr<ComponentArray<T>>;

struct Component {
    Entity id;

    virtual ~Component() = default;
};

struct HealthComponent : public Component {
    int health;
};

struct PositionComponent : public Component {
    int x;
    int y;    
};

struct VelocityComponent : public Component {
    int dx;
    int dy;
};

int main() {
    EntityManager entity_manager;
    ComponentManager component_manager( &entity_manager );

    Entity e1 = entity_manager.new_entity();
    Entity e2 = entity_manager.new_entity();

    HealthComponent hc1 = {};
    hc1.health = 100;
    hc1.id = e1;

    HealthComponent hc2 = {};
    hc2.health = 50;
    hc2.id = e2;

    PositionComponent pc1 = {};
    pc1.id = e1;
    pc1.x = 10;
    pc1.y = 20;
    
    PositionComponent pc2 = {};
    pc2.id = e2;
    pc2.x = 40;
    pc2.y = 1000;

    VelocityComponent v1 = {};
    v1.id = e1;
    v1.dx = 1;
    v1.dy = -1;
    
    VelocityComponent v2 = {};
    v2.id = e2;
    v2.dx = -2;
    v2.dy = 3;


    component_manager.register_component<HealthComponent>();
    component_manager.register_component<PositionComponent>();
    component_manager.register_component<VelocityComponent>();


    pComponentArray<HealthComponent> arr = component_manager.get_component_array<HealthComponent>();

    component_manager.add_component<HealthComponent>( e1, hc1 );
    component_manager.add_component<HealthComponent>( e2, hc2 );

    component_manager.add_component<PositionComponent>( e1, pc1 );
    component_manager.add_component<PositionComponent>( e2, pc2 );

    component_manager.add_component<VelocityComponent>( e1, v1 );
    component_manager.add_component<VelocityComponent>( e2, v2 );

    pComponentArray<HealthComponent> ca = component_manager.get_component_array<HealthComponent>();
    auto *hc = ca->get_all().get();
    
    std::cout << std::endl << std::endl;

    for ( int i = 0; i < ca.get()->get_size(); ++i ) {
        HealthComponent h = hc->at( i );
        std::cout << "HealthComponent" << std::endl << " Health: " << h.health << std::endl << " ID: " << h.id << std::endl << std::endl;
    }

    std::shared_ptr< std::vector<Entity> > entities = entity_manager.query_components( std::vector<std::string>( 
        {
            get_name<VelocityComponent>(), 
            get_name<PositionComponent>() 
        } ) );

    
    for ( auto it = entities->cbegin(); it != entities->cend(); ++it ) {
        std::cout << "Entity ID " << *it << std::endl;
    }


    return 0;
}