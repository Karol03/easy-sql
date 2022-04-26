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
 *  - First argument is always record name and created table has always additional 's' letter at the end
 *  - Next arguments (group by 3) are: Field name, type and is null
 *  - Single table can store max. 20 fields
 */
CREATE_TABLE(User,
             Id,       int,         NotNull,
             Email,    std::string, NotNull,
             Name,     std::string, Nullable,
             Password, std::string, NotNull,
             Age,      float,       Nullable);

CREATE_TABLE(Thing,
             Id,       int,         NotNull,
             UserId,   int,         Nullable,
             Name,     std::string, NotNull);

int main()
{
    epsql::PostgreSQL sql;

    /**
     *  Connect to database using connection string
     */
    if (!sql.connect("dbname = testdb user = postgres password = cohondob " \
                     "hostaddr = 127.0.0.1 port = 5432"))
    {
        std::cerr << "Cannot connect to the database\n";
        return 1;
    }

    /**
     *  Create table declared with macro CREATE_TABLE
     *  If table already existed, nothing happens
     */
    std::cout << "Create table [Users]\n";
    if (!sql.create<Users>())
    {
        std::cerr << "Failed to create table 'Users'\n";
        return 2;
    }

    /**
     *  Same as above, for different type
     */
    std::cout << "Create table [Things]\n";
    if (!sql.create<Things>())
    {
        std::cerr << "Failed to create table 'Things'\n";
        return 3;
    }

    std::cout << "\n=== BLOCK 1 ===\n";
    {
        /**
         * To insert new record into table user insert method on database
         */
        std::cout << "    Create new Users record\n";
        auto user = sql.insert<Users>();
        user->Id = 1;
        user->Email = std::string{"user@user.com"};
        user->Name = std::string{"username"};
        user->Password = std::string{"userpassword"};

        std::cout << "    Create new Things record\n";
        auto thing = sql.insert<Things>();
        thing->Id = 1;
        thing->UserId = *user->Id;
        thing->Name = std::string{"Thing!"};

        /* Use commit on object if it is ready to submit */
        std::cout << "    Insert user into [Users] locally\n";
        user.commit();
        std::cout << "    Insert thing into [Things] locally\n";
        thing.commit();

        /* Use push on sql to submit all changes */
        std::cout << "    Push changes in [Users] and [Things]\n";
        sql.push();

        /* Or clean to discard all changes */
        sql.clean();  // nothing clean as all changes submited in step above
    }

    std::cout << "\n=== BLOCK 2 ===\n";
    {
        /**
         *  To find record in database use one of available 'find' methods
         *  - method use template to specified search table
         *  - user where(...) macro to declare search conditions
         */

        std::cout << "    Find in [Users] record meet conditions\n";
        auto user = sql.find<Users>(where({     // find single record from 'Users' table
            /* Declare used table fields using Field(...) macro */
            auto name = Field(Users::Name);     // we will use 'Name' field from 'Users' table
            return name.startsWith("user");     // we want to find record which field 'Name' starts
                                                // with 'user' prefix
        }));


        /**
         *  Condition check if record exists
         *  It is equivalent of user.isExists()
         */
        if (user)
        {
            std::cout << "    User found!\n";
            /* If we want to remove some records we use remove method on them */
            std::cout << "    Remove user\n";
            user.remove();
            /* Commit removed object */
            std::cout << "    Commit changes\n";
            user.commit();

            /* And always submit changes using 'push' if everythings commited */
            std::cout << "    Push changes with removed User\n";
            sql.push();
        }
    }

    return 0;
}

