/**
 * Created by Karol Dudzic @ 2022
 */
#include <iostream>

#include "esql/sql.hpp"


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

CREATE_TABLE(Stats,
             Id,       NOT_NULL(Int),
             ThingId,  FOREIGN_KEY(Things::Id),
             Name,     NOT_NULL(Text),
             Value,    NOT_NULL(Int));

CREATE_TABLE(SupportStats,
             Id,       NOT_NULL(Int),
             ThingId,  FOREIGN_KEY(Things::Id),
             Name,     NOT_NULL(Text),
             Value,    NOT_NULL(Int));

CREATE_TABLE(Options,
             Id,       NOT_NULL(Int),
             StatId,   FOREIGN_KEY(Stats::Id),
             Value,    NOT_NULL(Int));


void Step1_HowToConnectDatabase(esql::PostgreSQL& sql)
{
    std::cout << "\t\t\t=== Step1_HowToConnectDatabase ===\n";
    if (!sql.connect("dbname = testdb user = postgres password = cohondob " \
                     "hostaddr = 127.0.0.1 port = 5432"))
    {
        throw std::runtime_error{"Failed to connect to the database\n"};
    }
}


void Step2_HowToCreateDefinedTables(esql::PostgreSQL& sql)
{
    std::cout << "\n\t\t\t=== Step2_HowToCreateDefinedTables ===\n";
    if (!sql.create<Users>())
        throw std::runtime_error{"Failed to create table 'Users'\n"};
    if (!sql.create<Things>())
        throw std::runtime_error{"Failed to create table 'Things'\n"};
    if (!sql.create<Stats>())
        throw std::runtime_error{"Failed to create table 'Stats'\n"};
    if (!sql.create<SupportStats>())
        throw std::runtime_error{"Failed to create table 'SupportStats'\n"};
    if (!sql.create<Options>())
        throw std::runtime_error{"Failed to create table 'Options'\n"};
    /* If table already existed, nothing happens - no recreation, no exception */
}


void Step3_HowToCreateNewRecordInTable(esql::PostgreSQL& sql)
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


void Step4_HowToUpdateRecordInTable(esql::PostgreSQL& sql)
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


void Step5_HowToRemoveRecordInTable(esql::PostgreSQL& sql)
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


void Step6_HowToFindRecordInTable(esql::PostgreSQL& sql)
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


void Step7_HowToFindAndRemoveRecordInTable(esql::PostgreSQL& sql)
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


void Step8_HowToFindRecordUsingImplicitForeignKeyProperty(esql::PostgreSQL& sql)
{
    std::cout << "\n\n\t\t\t=== Step8_HowToFindRecordUsingImplicitForeignKeyProperty ===\n";
    {
        auto user = sql.find<Users>(where({          // find single record in 'Users' table
            auto thingName = Field(Things::Name);    // declare fields using Field(...) macro
            return (thingName == "Thing!");          // Because tables 'Things' and 'Users' are tied in relation chain
                                                     // Things::UserId -> Users::Id, there is no need to define relation explicitly
                                                     // the default one will be choosen
                                                     // If no relation chain between tables
                                                     // none elements will be selected
        }));
    }

    {       // even more complicated relation might occurs
        auto user = sql.find<Users>(where({             // we still looking for user
            auto thingName = Field(Things::Name);
            auto optionValue = Field(Options::Value);
            auto statName = Field(Stats::Name);
            auto statValue = Field(Stats::Value);
            auto supportStatValue = Field(SupportStats::Value);
            return (thingName == "Thing!") || (optionValue == 23) ||
                   (statName == thingName && statValue > 20) ||
                   (supportStatValue > 50 && statValue < 17);
        }));
    }
}


void Step9_HowToPassProgramVariablesInFindQuery(esql::PostgreSQL& sql)
{
    std::cout << "\n\n\t\t\t=== Step9_HowToPassProgramVariablesInFindQuery ===\n";
    const auto userNameVar = std::string{"user"};
    const auto thingNameVar = std::string{"Thing!"};

    // Approach 1. - just use variable inside where({}) - by default all variables are visible in the scopt
    {
        auto user = sql.find<Users>(where({
            auto userName = Field(Users::Name);
            auto thingName = Field(Things::Name);
            return (thingName == thingNameVar) && (userName == userNameVar);
        }));
    }

    // Approach 2. - define variables visible in scope
    {
        auto user = sql.find<Users>(where({
            auto userName = Field(Users::Name);
            auto thingName = Field(Things::Name);
            return (thingName == thingNameVar) && (userName == userNameVar);
        }, &thingNameVar, userNameVar)); // pass 'thingNameVar' by reference
                                         // and 'userNameVar' by value
    }
}


void Step10_HowToCombineProgramVariablesInFindQuery(esql::PostgreSQL& sql)
{
    std::cout << "\n\n\t\t\t=== Step10_HowToCombineProgramVariablesInFindQuery ===\n";
    const auto userNameVar = std::string{"user"};
    const auto userNameVec = std::vector<std::string>{"User", "user4"};
    const auto thingNameVar = std::string{"Thing!"};

    // Scenario 1.
    {
        auto user = sql.find<Users>(where({
            auto thingName = Field(Things::Name);
            return (thingName == thingNameVar) &&
                   (std::find(userNameVec.begin(), userNameVec.end(), userNameVar) != userNameVec.end());
            // program variable expressions are evaluated first
            // there is no sql query emitted if whole exprossion evaluates to 'false'
        }));
    }

    // Scenario 2.
    {
        auto user = sql.find<Users>(where({          // find single record in 'Users' table
            auto userName = Field(Users::Name);
            auto thingName = Field(Things::Name);
            // if (userName == "username")          // these two lines will compile but the result will be an error
            //     return thingName == "thing";     // in error stream and no query will be emitted
                                                    // creating code branches in where({}) is forbidden
            return thingName == "Thing!";
        }));
    }
}


void Step11_HowToRemoveTable(esql::PostgreSQL& sql)
{
    std::cout << "\n\n\t\t\t=== Step11_HowToRemoveTable ===\n";
    sql.remove<Things>();
}


int main()
{
    auto str = std::make_shared<std::stringstream>();
    esql::PostgreSQL sql(str);

    Step1_HowToConnectDatabase(sql);
    Step2_HowToCreateDefinedTables(sql);
    Step3_HowToCreateNewRecordInTable(sql);
    Step4_HowToUpdateRecordInTable(sql);
    Step5_HowToRemoveRecordInTable(sql);
    Step6_HowToFindRecordInTable(sql);
    Step7_HowToFindAndRemoveRecordInTable(sql);
    Step8_HowToFindRecordUsingImplicitForeignKeyProperty(sql);
    Step9_HowToPassProgramVariablesInFindQuery(sql);
    Step10_HowToCombineProgramVariablesInFindQuery(sql);
    Step11_HowToRemoveTable(sql);

    std::cerr << "Error stream : {\n" << str->str() << "\n}\n";

    return 0;
}

