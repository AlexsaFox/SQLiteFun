#include <iostream>
#include "sqlite_orm.h"

  
  

using namespace sqlite_orm;

  
  

// struct User{
//     int id;
//     std::string firstName;
//     std::string lastName;
//     int birthDate;
//     std::unique_ptr<std::string> imageUrl;
//     int typeId;
// };

  

// struct UserType {
//     int id;
//     std::string name;
// };

  

struct Car {
    int car_id;
    std::string name;
};

  

struct Pilot {
    int pilot_id;
    std::string name;
};

  

struct Track {
    int track_id;
    std::string name;
    float length;
    std::string address;
    std::string start_position;
    std::string finish_position;
};

  

struct Turn {
    int id;
    int track_id; // ref to Track id
    std::string position;
    float inner_radius;
    float outer_radius;
    float angle;
    float steering;
};

  
  

struct CarParameter {
    int id;
    int car_id; // ref to Car id

    int type;
    float min_value;
    float max_value;
    float step;
};

  

struct DrivingEvent {
    int id;
    int track_id; // ref to Track id
    int pilot_id; // ref to Pilot id
    int car_id; // ref to Car id
   
    int event_type;
    float value;
    float duration;
};

  

struct Simulation {
    int id;
    int track_id; // ref to Track id
    int pilot_id; // ref to Pilot id
    int car_id; // ref to Car id
    float result_angle;
    float result_style;
    float result_trajectory;
};

  

struct SimulationCarParam {
    int id;
    int simulation_id; // ref to Simulation id
    int parameter_id; // ref to CarParameter id
    float value;
};

  
  
  

int main() {

auto storage = make_storage("db.sqlite",
            // make_table("users",
            //         make_column("id", &User::id, primary_key().autoincrement()),
            //         make_column("first_name", &User::firstName),
            //         make_column("last_name", &User::lastName),
            //         make_column("birth_date", &User::birthDate),
            //         make_column("image_url", &User::imageUrl),
            //         make_column("type_id", &User::typeId)),
            // make_table("user_types",
            //         make_column("id", &UserType::id, primary_key().autoincrement()),
            //         make_column("name", &UserType::name, default_value("name_placeholder"))),
            make_table("Tracks",
                    make_column("id", &Track::track_id, primary_key().autoincrement()),
                    make_column("name", &Track::name),
                    make_column("length", &Track::length),
                    make_column("address", &Track::address),
                    make_column("start_position", &Track::start_position),
                    make_column("finish_position", &Track::finish_position)),
            make_table("Turns",
                    make_column("id", &Turn::id, primary_key().autoincrement()),
                    make_column("track", &Turn::track_id),
                    make_column("position", &Turn::position),
                    make_column("inner_radius", &Turn::inner_radius),
                    make_column("outer_radius", &Turn::outer_radius),
                    make_column("angle", &Turn::angle),
                    make_column("steering", &Turn::steering),
                    foreign_key(&Turn::track_id).references(&Track::track_id)),
            make_table("Pilots",
                    make_column("id", &Pilot::pilot_id, primary_key().autoincrement()),
                    make_column("name", &Pilot::name)),
            make_table("Cars",
                    make_column("id", &Car::car_id, primary_key().autoincrement()),
                    make_column("name", &Car::name)),
            make_table("CarParameters",
                    make_column("id", &CarParameter::id, primary_key().autoincrement()),
                    make_column("car", &CarParameter::car_id),
                    make_column("type", &CarParameter::type),
                    make_column("min_value", &CarParameter::min_value),
                    make_column("max_value", &CarParameter::max_value),
                    make_column("step", &CarParameter::step),
                    foreign_key(&CarParameter::car_id).references(&Car::car_id)),
            make_table("DrivingEvents",
                    make_column("id", &DrivingEvent::id, primary_key().autoincrement()),
                    make_column("track", &DrivingEvent::track_id),
                    make_column("pilot", &DrivingEvent::pilot_id),
                    make_column("car", &DrivingEvent::car_id),
                    make_column("event_type", &DrivingEvent::event_type),
                    make_column("value", &DrivingEvent::value),
                    make_column("duration", &DrivingEvent::duration),
                    foreign_key(&DrivingEvent::track_id).references(&Track::track_id),
                    foreign_key(&DrivingEvent::pilot_id).references(&Pilot::pilot_id),
                    foreign_key(&DrivingEvent::car_id).references(&Car::car_id)),
            make_table("Simulation",
                    make_column("id", &Simulation::id, primary_key().autoincrement()),
                    make_column("track", &Simulation::track_id),
                    make_column("pilot", &Simulation::pilot_id),
                    make_column("car", &Simulation::car_id),
                    make_column("result_angle", &Simulation::result_angle),
                    make_column("result_style", &Simulation::result_style),
                    make_column("result_trajectory", &Simulation::result_trajectory),
                    foreign_key(&Simulation::track_id).references(&Track::track_id),
                    foreign_key(&Simulation::pilot_id).references(&Pilot::pilot_id),
                    foreign_key(&Simulation::car_id).references(&Car::car_id)),
            make_table("SimulationCarParam",
                    make_column("id", &SimulationCarParam::id, primary_key().autoincrement()),
                    make_column("simulation", &SimulationCarParam::simulation_id),
                    make_column("parameter", &SimulationCarParam::parameter_id),
                    make_column("value", &SimulationCarParam::value),
                    foreign_key(&SimulationCarParam::parameter_id).references(&CarParameter::id),
                    foreign_key(&SimulationCarParam::simulation_id).references(&Simulation::id)));

storage.sync_schema();

Car rage {
    0,
    "Rage",
};
rage.car_id = storage.insert(rage);

CarParameter tires_pressure {
    0,
    rage.car_id,
    3,
    0.2,
    1.6,
    0.1,
};

tires_pressure.id = storage.insert(tires_pressure);

auto rows = storage.get_all<CarParameter>();

std::cout << rows[0].type << std::endl;

auto objects = storage.get_all<Car>(where(is_equal(&Car::car_id, rows[0].car_id)));

std::cout << objects.front().name << std::endl;

return 0;

}
