#include "sqlite_orm.h"


using namespace sqlite_orm;


struct User{
    int id;
    std::string firstName;
    std::string lastName;
    int birthDate;
    std::unique_ptr<std::string> imageUrl;
    int typeId;
};

struct UserType {
    int id;
    std::string name;
};


int main() {
  auto storage = make_storage("db.sqlite",
                              make_table("users",
                                         make_column("id", &User::id, primary_key().autoincrement()),
                                         make_column("first_name", &User::firstName),
                                         make_column("last_name", &User::lastName),
                                         make_column("birth_date", &User::birthDate),
                                         make_column("image_url", &User::imageUrl),
                                         make_column("type_id", &User::typeId)),
                              make_table("user_types",
                                         make_column("id", &UserType::id, primary_key().autoincrement()),
                                         make_column("name", &UserType::name, default_value("name_placeholder"))));
  storage.sync_schema(); 
  return 0;
}
