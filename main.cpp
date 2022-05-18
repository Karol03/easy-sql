/**
 * Created by Karol Dudzic @ 2022
 */
#include <iostream>

#include "epsql/postgresql.hpp"


using namespace std;

/**
 *   DECLARE TABLES
 *
 *  - Use CREATE_TABLE macro to create new table
 *  - First argument is always table name
 *  - Next arguments (group by 2) are: Field name, field database type
 *  - First parameter (in this example 'Id') is always primary key
 *  - Single table can store max. 20 fields
 *
 *  Allowed database types:
 *          NULLABLE(type)
 *          NOT_NULL(type)
 *          FOREIGN_KEY(referenced field 'Class::Field', <optional> NULL/NOTNULL - if field nullable/not nullable)
 *                                                       default value is NULL
 *          DEFAULT(type, value, <optional> NULL/NOTNULL)
 *                               default value is NULL
 *  Allowed types:
 *      text:       Text
 *      bool:       Boolean
 *      arithmetic: Smallint, Int, Bigint,
 *                  Real, Float8, Double
 */
CREATE_TABLE(Users,
             Id,       NOT_NULL(Int),
             Email,    NOT_NULL(Text),
             Name,     NULLABLE(Text),
             Password, NOT_NULL(Text),
             Age,      NULLABLE(Smallint));

CREATE_TABLE(Things,
             Id,       NOT_NULL(Int),
             UserId,   FOREIGN_KEY(Users::Id),
             Name,     NOT_NULL(Text));



void Step1_HowToConnectDatabase(epsql::PostgreSQL& sql)
{
    std::cout << "\t\t\t=== Step1_HowToConnectDatabase ===\n";
    if (!sql.connect("dbname = testdb user = postgres password = cohondob " \
                     "hostaddr = 127.0.0.1 port = 5432"))
    {
        throw std::runtime_error{"Failed to connect to the database\n"};
    }
}


void Step2_HowToCreateDefinedTables(epsql::PostgreSQL& sql)
{
    std::cout << "\n\t\t\t=== Step2_HowToCreateDefinedTables ===\n";
    if (!sql.create<Users>())
        throw std::runtime_error{"Failed to create table 'Users'\n"};
    if (!sql.create<Things>())
        throw std::runtime_error{"Failed to create table 'Things'\n"};
    /* If table already existed, nothing happens - no recreation, no exception */
}


void Step3_HowToCreateNewRecordInTable(epsql::PostgreSQL& sql)
{
    std::cout << "\n\n\t\t\t=== Step3_HowToCreateNewRecordInTable ===\n";
    auto user = sql.insert<Users>();
    user->Id = 1;
    user->Email = "user@user.com";
    user->Name = "username";
    user->Password = "userpassword";

    auto thing = sql.insert<Things>();
    thing->Id = 1;
    thing->UserId = user->Id;
    thing->Name = "Thing!";

    user.commit();  // save user record changes
    thing.commit(); // save thing record changes

    sql.push();     // push changes
}


void Step4_HowToUpdateRecordInTable(epsql::PostgreSQL& sql)
{
    std::cout << "\n\n\t\t\t=== Step4_HowToUpdateRecordInTable ===\n";
    auto user = sql.insert<Users>();
    user->Id = 2;
    user->Email = "user@user.com";
    user->Name = "User2";
    user->Password = "User2Password";

    user.commit();  // save user record changes
    sql.push();     // push changes

    user->Email = "user2mail@user.com";
    user.commit();  // update user email

    user->Password = "newStronGP4ssword87!";
    user.commit();  // update user password

    user->Password = "13NewEvenStronGeRP4ssword!52";
    user.commit();  // update user password once again

    sql.push();     // push changes
}


void Step5_HowToRemoveRecordInTable(epsql::PostgreSQL& sql)
{
    std::cout << "\n\n\t\t\t=== Step5_HowToRemoveRecordInTable ===\n";
    auto user = sql.insert<Users>();
    user->Id = 3;
    user->Email = "user3@user.com";
    user->Name = "User3";
    user->Password = "User3Password";

    user.commit();
    sql.push();

    user.remove();
    user.commit();
    sql.push();
}


void Step6_HowToFindRecordInTable(epsql::PostgreSQL& sql)
{
    std::cout << "\n\n\t\t\t=== Step6_HowToFindRecordInTable ===\n";
    auto user = sql.find<Users>(where({     // find single record in 'Users' table
        auto userId = Field(Things::UserId);  // declare fields using Field(...) macro
        auto thingName = Field(Things::Name);
        auto id = Field(Users::Id);
        auto name = Field(Users::Name);
        return (userId == id) && thingName == "thing" && name.startsWith("user");     // find record which field 'Name' starts with 'user'
    }));

    if (user)
        std::cout << "User found!"
                  << "\n  Id = " << *user->Id
                  << "\n  Name = " << (user->Name ? *user->Name : Text{"UNKNOWN NAME"})
                  << "\n  Email = " << *user->Email
                  << "\n  Password = " << *user->Password
                  << "\n  Age = " << (user->Age ? *user->Age : Smallint(0)) << "\n";
}


void Step7_HowToFindAndRemoveRecordInTable(epsql::PostgreSQL& sql)
{
    std::cout << "\n\n\t\t\t=== Step7_HowToFindAndRemoveRecordInTable ===\n";
    auto user = sql.find<Users>(where({     // find single record in 'Users' table
        auto name = Field(Users::Name);     // declare fields using Field(...) macro
        return name.startsWith("user");     // find record which field 'Name' starts with 'user'
    }));

    if (user)
    {
        user.remove();
        user.commit();
        sql.push();
    }
}


void Step8_HowToFindRecordUsingImplicitForeignKeyProperty(epsql::PostgreSQL& sql)
{
    std::cout << "\n\n\t\t\t=== Step8_HowToFindRecordUsingImplicitForeignKeyProperty ===\n";
    auto user = sql.find<Users>(where({         // find single record in 'Users' table
        auto thingName = Field(Things::Name);   // declare fields using Field(...) macro
        return thingName == "Thing!";           // cause Things table has defined foreign key related
                                                // to Users table, there is no need to define relation
                                                // between tables, the default one (foreign key)
                                                // will be choosen
    }));

    if (user)
    {
        user.remove();
        user.commit();
        sql.push();
    }
}


void Step9_HowToRemoveTable(epsql::PostgreSQL& sql)
{
    std::cout << "\n\n\t\t\t=== Step8_HowToFindRecordUsingImplicitForeignKeyProperty ===\n";
    sql.remove<Things>();
}


int main()
{
    auto str = std::make_shared<std::stringstream>();
    epsql::PostgreSQL sql(str);

    Step1_HowToConnectDatabase(sql);
    Step2_HowToCreateDefinedTables(sql);
    Step3_HowToCreateNewRecordInTable(sql);
    Step4_HowToUpdateRecordInTable(sql);
    Step5_HowToRemoveRecordInTable(sql);
    Step6_HowToFindRecordInTable(sql);
    Step7_HowToFindAndRemoveRecordInTable(sql);
    Step8_HowToFindRecordUsingImplicitForeignKeyProperty(sql);
    Step9_HowToRemoveTable(sql);

    std::cerr << "Error stream : {\n" << str->str() << "\n}\n";

    return 0;
}

