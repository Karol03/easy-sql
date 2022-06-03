# easy-sql
Is a simple library to handle SQL databases such a PostgreSQL or SQLite. </br>
The goal is to treat database tables as easy as collection of elements. </br>

# Examples
The main.cpp file was created to present some examples of "easy-sql" usage such a table manipulations or searching for an object. Some mechanisms are presented below.

**Create table class**
```cpp
CREATE_TABLE(Things,
             Id,       NOT_NULL(Int),
             UserId,   FOREIGN_KEY(Users::Id),
             Name,     NOT_NULL(Text),
             Value,    NULLABLE(Smallint));
```
This is an example of creating structure 'Things' with four fields. This piece of code does not directly create table in database but the structure which is table type and can be bind to a datatable in the database.
Notice, by proper indentation and readable macros we keep code clean and consistent. </br>

**Connect to database**
```cpp
sql.connect("dbname = testdb user = postgres password = cohondob " \
            "hostaddr = 127.0.0.1 port = 5432");
```
Connetion to database is as simple as that. We need only proper connectionString with database name, user, pasword, address and port. </br> 

**Disconnect from database**
```cpp
sql.disconnect();
```
There is no need to manually disconnect database. The SQL object applies RAII pattern for database and it will automatically release resources when program will go out of SQL variable scope. However, if you need to do it, just call 'disconnect'. </br>

**Create table**
```cpp
sql.create<Things>();
```
This example creates table in database based on table type structure such as 'Things' from *Create table class* example. This function will emit proper SQL query to the database and returns boolean value if request ends with success or not. </br>

**Remove table**
```cpp
sql.remove<Things>();
```
Simillary to creation. Just use remove function and everything happens. </br>

**Create new record**
```cpp
auto record = sql.insert<Things>();
record->Id = 1;
record->Name = "Name";

record.commit();

sql.push();
```

Use 'insert' method to create a new record in the table. It creates a new mutable object which can be easly set up. When the object is ready don't forget to call 'commit' to save your changes. To avoid dozens of SQL queries object is not automatically synchronized with database. To make changes to the database, call the 'push' function on the database object. </br>

**Update/Remove a record**
Update/Remove of existing object is simillar to creation. Just find/create your record and set member fields. Don't forget about 'commit' and 'push'. </br>

**Find record**
```cpp
  auto thing = sql.find<Things>(where({
      auto name = Field(Thing::Name);
      return name.startsWith("Thing");
  }));
```
The most interesting part is lambda mechanism for object finding. </br>
It can be achieved using 'find' method and a new macro 'where({ ... })' which takes body of query. In query we can define which fields we are interested in, and create a request using C++ common comparators. </br>

```cpp
  auto thing = sql.findAll<Things>(where({
      auto userName = Field(User::Name);
      return userName.startsWith("user");
  }));
```
Check the example above, there is another special feature of easy-sql. If there is any relation defined between two tables (by 'relation' means one table use foreign key constraint on the other one) the lambda may use this implicit relation to find proper objects. </br>
In example above, we will find all 'Things' which match (by ForeignKey(Users::Id) see example *Create table class*) to any 'Users' whose name starts with "user". </br>
Moreover, not only directly related tables are subject to deduction. When we have multiple arrays such that
```
A <- B(foreign key to A) <- C (foreign key to B)
```
We could search object A based on properties of object C! (more in *main.cpp*) </br>

**Failures**
```cpp
    auto str = std::make_shared<std::stringstream>();
    esql::PostgreSQL sql(str);
```
In case of fail it is most likely that nothing will happen. In case of an error, 'easy-sql' will throw an exception, but it will be caught by the main SQL class. To record the reason for the error, provide the SQL class with an appropriate object of type 'std::ostream', which will be used as an output channel for error messages.


# Todos
- Default value of variable
- Object fetching
