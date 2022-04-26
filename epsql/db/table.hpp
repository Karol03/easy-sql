/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <algorithm>
#include <type_traits>
#include <vector>

#include "utils/typeof.hpp"
#include "value.hpp"


#define PRINT1(__v1, ...) #__v1
#define PRINT2(__v1, ...) #__v1, PRINT1(__VA_ARGS__)
#define PRINT3(__v1, ...) #__v1, PRINT2(__VA_ARGS__)
#define PRINT4(__v1, ...) #__v1, PRINT3(__VA_ARGS__)
#define PRINT5(__v1, ...) #__v1, PRINT4(__VA_ARGS__)
#define PRINT6(__v1, ...) #__v1, PRINT5(__VA_ARGS__)
#define PRINT7(__v1, ...) #__v1, PRINT6(__VA_ARGS__)
#define PRINT8(__v1, ...) #__v1, PRINT7(__VA_ARGS__)
#define PRINT9(__v1, ...) #__v1, PRINT8(__VA_ARGS__)
#define PRINT10(__v1, ...) #__v1, PRINT9(__VA_ARGS__)
#define PRINT11(__v1, ...) #__v1, PRINT10(__VA_ARGS__)
#define PRINT12(__v1, ...) #__v1, PRINT11(__VA_ARGS__)
#define PRINT13(__v1, ...) #__v1, PRINT12(__VA_ARGS__)
#define PRINT14(__v1, ...) #__v1, PRINT13(__VA_ARGS__)
#define PRINT15(__v1, ...) #__v1, PRINT14(__VA_ARGS__)
#define PRINT16(__v1, ...) #__v1, PRINT15(__VA_ARGS__)
#define PRINT17(__v1, ...) #__v1, PRINT16(__VA_ARGS__)
#define PRINT18(__v1, ...) #__v1, PRINT17(__VA_ARGS__)
#define PRINT19(__v1, ...) #__v1, PRINT18(__VA_ARGS__)
#define PRINT20(__v1, ...) #__v1, PRINT19(__VA_ARGS__)

#define PRINT_(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20, n, ...) \
    PRINT##n(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20)

#define PRINT(_v1, ...) \
    PRINT_(_v1, __VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)


#define NAME1(_v1, _v2, _v3, ...) _v1
#define NAME2(_v1, _v2, _v3, ...) _v1, NAME1(__VA_ARGS__)
#define NAME3(_v1, _v2, _v3, ...) _v1, NAME2(__VA_ARGS__)
#define NAME4(_v1, _v2, _v3, ...) _v1, NAME3(__VA_ARGS__)
#define NAME5(_v1, _v2, _v3, ...) _v1, NAME4(__VA_ARGS__)
#define NAME6(_v1, _v2, _v3, ...) _v1, NAME5(__VA_ARGS__)
#define NAME7(_v1, _v2, _v3, ...) _v1, NAME6(__VA_ARGS__)
#define NAME8(_v1, _v2, _v3, ...) _v1, NAME7(__VA_ARGS__)
#define NAME9(_v1, _v2, _v3, ...) _v1, NAME8(__VA_ARGS__)
#define NAME10(_v1, _v2, _v3, ...) _v1, NAME9(__VA_ARGS__)
#define NAME11(_v1, _v2, _v3, ...) _v1, NAME10(__VA_ARGS__)
#define NAME12(_v1, _v2, _v3, ...) _v1, NAME11(__VA_ARGS__)
#define NAME13(_v1, _v2, _v3, ...) _v1, NAME12(__VA_ARGS__)
#define NAME14(_v1, _v2, _v3, ...) _v1, NAME13(__VA_ARGS__)
#define NAME15(_v1, _v2, _v3, ...) _v1, NAME14(__VA_ARGS__)
#define NAME16(_v1, _v2, _v3, ...) _v1, NAME15(__VA_ARGS__)
#define NAME17(_v1, _v2, _v3, ...) _v1, NAME16(__VA_ARGS__)
#define NAME18(_v1, _v2, _v3, ...) _v1, NAME17(__VA_ARGS__)
#define NAME19(_v1, _v2, _v3, ...) _v1, NAME18(__VA_ARGS__)
#define NAME20(_v1, _v2, _v3, ...) _v1, NAME19(__VA_ARGS__)

#define NAME_(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21,_v22,_v23,_v24,_v25,_v26,_v27,_v28,_v29,_v30,_v31,_v32,_v33,_v34,_v35,_v36,_v37,_v38,_v39,_v40,_v41,_v42,_v43,_v44,_v45,_v46,_v47,_v48,_v49,_v50,_v51,_v52,_v53,_v54,_v55,_v56,_v57,_v58,_v59,_v60, n, ...) \
    NAME##n(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21,_v22,_v23,_v24,_v25,_v26,_v27,_v28,_v29,_v30,_v31,_v32,_v33,_v34,_v35,_v36,_v37,_v38,_v39,_v40,_v41,_v42,_v43,_v44,_v45,_v46,_v47,_v48,_v49,_v50,_v51,_v52,_v53,_v54,_v55,_v56,_v57,_v58,_v59,_v60)

#define NAME(_v1, _v2, _v3, ...) \
    NAME_(_v1, _v2, _v3, __VA_ARGS__, 20, 20, 20, 19, 19, 19, 18, 18, 18, 17, 17, 17, 16, 16, 16, 15, 15, 15, 14, 14, 14, 13, 13, 13, 12, 12, 12, 11, 11, 11, 10, 10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1)


#define TYPE1(_v1, _v2, _v3, ...) _v2
#define TYPE2(_v1, _v2, _v3, ...) _v2, TYPE1(__VA_ARGS__)
#define TYPE3(_v1, _v2, _v3, ...) _v2, TYPE2(__VA_ARGS__)
#define TYPE4(_v1, _v2, _v3, ...) _v2, TYPE3(__VA_ARGS__)
#define TYPE5(_v1, _v2, _v3, ...) _v2, TYPE4(__VA_ARGS__)
#define TYPE6(_v1, _v2, _v3, ...) _v2, TYPE5(__VA_ARGS__)
#define TYPE7(_v1, _v2, _v3, ...) _v2, TYPE6(__VA_ARGS__)
#define TYPE8(_v1, _v2, _v3, ...) _v2, TYPE7(__VA_ARGS__)
#define TYPE9(_v1, _v2, _v3, ...) _v2, TYPE8(__VA_ARGS__)
#define TYPE10(_v1, _v2, _v3, ...) _v2, TYPE9(__VA_ARGS__)
#define TYPE11(_v1, _v2, _v3, ...) _v2, TYPE10(__VA_ARGS__)
#define TYPE12(_v1, _v2, _v3, ...) _v2, TYPE11(__VA_ARGS__)
#define TYPE13(_v1, _v2, _v3, ...) _v2, TYPE12(__VA_ARGS__)
#define TYPE14(_v1, _v2, _v3, ...) _v2, TYPE13(__VA_ARGS__)
#define TYPE15(_v1, _v2, _v3, ...) _v2, TYPE14(__VA_ARGS__)
#define TYPE16(_v1, _v2, _v3, ...) _v2, TYPE15(__VA_ARGS__)
#define TYPE17(_v1, _v2, _v3, ...) _v2, TYPE16(__VA_ARGS__)
#define TYPE18(_v1, _v2, _v3, ...) _v2, TYPE17(__VA_ARGS__)
#define TYPE19(_v1, _v2, _v3, ...) _v2, TYPE18(__VA_ARGS__)
#define TYPE20(_v1, _v2, _v3, ...) _v2, TYPE19(__VA_ARGS__)

#define TYPE_(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21,_v22,_v23,_v24,_v25,_v26,_v27,_v28,_v29,_v30,_v31,_v32,_v33,_v34,_v35,_v36,_v37,_v38,_v39,_v40,_v41,_v42,_v43,_v44,_v45,_v46,_v47,_v48,_v49,_v50,_v51,_v52,_v53,_v54,_v55,_v56,_v57,_v58,_v59,_v60, n, ...) \
    TYPE##n(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21,_v22,_v23,_v24,_v25,_v26,_v27,_v28,_v29,_v30,_v31,_v32,_v33,_v34,_v35,_v36,_v37,_v38,_v39,_v40,_v41,_v42,_v43,_v44,_v45,_v46,_v47,_v48,_v49,_v50,_v51,_v52,_v53,_v54,_v55,_v56,_v57,_v58,_v59,_v60)

#define TYPE(_v1, _v2, _v3, ...) \
    TYPE_(_v1, _v2, _v3, __VA_ARGS__, 20, 20, 20, 19, 19, 19, 18, 18, 18, 17, 17, 17, 16, 16, 16, 15, 15, 15, 14, 14, 14, 13, 13, 13, 12, 12, 12, 11, 11, 11, 10, 10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1)


#define FIELD1(_v1, _v2, _v3, ...) ::epsql::db::_v3<_v2> _v1;
#define FIELD2(_v1, _v2, _v3, ...) ::epsql::db::_v3<_v2> _v1; FIELD1(__VA_ARGS__)
#define FIELD3(_v1, _v2, _v3, ...) ::epsql::db::_v3<_v2> _v1; FIELD2(__VA_ARGS__)
#define FIELD4(_v1, _v2, _v3, ...) ::epsql::db::_v3<_v2> _v1; FIELD3(__VA_ARGS__)
#define FIELD5(_v1, _v2, _v3, ...) ::epsql::db::_v3<_v2> _v1; FIELD4(__VA_ARGS__)
#define FIELD6(_v1, _v2, _v3, ...) ::epsql::db::_v3<_v2> _v1; FIELD5(__VA_ARGS__)
#define FIELD7(_v1, _v2, _v3, ...) ::epsql::db::_v3<_v2> _v1; FIELD6(__VA_ARGS__)
#define FIELD8(_v1, _v2, _v3, ...) ::epsql::db::_v3<_v2> _v1; FIELD7(__VA_ARGS__)
#define FIELD9(_v1, _v2, _v3, ...) ::epsql::db::_v3<_v2> _v1; FIELD8(__VA_ARGS__)
#define FIELD10(_v1, _v2, _v3, ...) ::epsql::db::_v3<_v2> _v1; FIELD9(__VA_ARGS__)
#define FIELD11(_v1, _v2, _v3, ...) ::epsql::db::_v3<_v2> _v1; FIELD10(__VA_ARGS__)
#define FIELD12(_v1, _v2, _v3, ...) ::epsql::db::_v3<_v2> _v1; FIELD11(__VA_ARGS__)
#define FIELD13(_v1, _v2, _v3, ...) ::epsql::db::_v3<_v2> _v1; FIELD12(__VA_ARGS__)
#define FIELD14(_v1, _v2, _v3, ...) ::epsql::db::_v3<_v2> _v1; FIELD13(__VA_ARGS__)
#define FIELD15(_v1, _v2, _v3, ...) ::epsql::db::_v3<_v2> _v1; FIELD14(__VA_ARGS__)
#define FIELD16(_v1, _v2, _v3, ...) ::epsql::db::_v3<_v2> _v1; FIELD15(__VA_ARGS__)
#define FIELD17(_v1, _v2, _v3, ...) ::epsql::db::_v3<_v2> _v1; FIELD16(__VA_ARGS__)
#define FIELD18(_v1, _v2, _v3, ...) ::epsql::db::_v3<_v2> _v1; FIELD17(__VA_ARGS__)
#define FIELD19(_v1, _v2, _v3, ...) ::epsql::db::_v3<_v2> _v1; FIELD18(__VA_ARGS__)
#define FIELD20(_v1, _v2, _v3, ...) ::epsql::db::_v3<_v2> _v1; FIELD19(__VA_ARGS__)

#define FIELD_(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21,_v22,_v23,_v24,_v25,_v26,_v27,_v28,_v29,_v30,_v31,_v32,_v33,_v34,_v35,_v36,_v37,_v38,_v39,_v40,_v41,_v42,_v43,_v44,_v45,_v46,_v47,_v48,_v49,_v50,_v51,_v52,_v53,_v54,_v55,_v56,_v57,_v58,_v59,_v60, n, ...) \
    FIELD##n(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21,_v22,_v23,_v24,_v25,_v26,_v27,_v28,_v29,_v30,_v31,_v32,_v33,_v34,_v35,_v36,_v37,_v38,_v39,_v40,_v41,_v42,_v43,_v44,_v45,_v46,_v47,_v48,_v49,_v50,_v51,_v52,_v53,_v54,_v55,_v56,_v57,_v58,_v59,_v60)

#define FIELD(_v1, _v2, _v3, ...) \
    FIELD_(_v1, _v2, _v3, __VA_ARGS__, 20, 20, 20, 19, 19, 19, 18, 18, 18, 17, 17, 17, 16, 16, 16, 15, 15, 15, 14, 14, 14, 13, 13, 13, 12, 12, 12, 11, 11, 11, 10, 10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1)


#define FIELD_GETTER(__v1, __v2) inline static const char* __v2##NameGetter() { return #__v1 "." #__v2; }
#define GET_FIELD1(__v1, __v2, ...) FIELD_GETTER(__v1, __v2);
#define GET_FIELD2(__v1, __v2, ...) FIELD_GETTER(__v1, __v2); GET_FIELD1(__v1, __VA_ARGS__)
#define GET_FIELD3(__v1, __v2, ...) FIELD_GETTER(__v1, __v2); GET_FIELD2(__v1, __VA_ARGS__)
#define GET_FIELD4(__v1, __v2, ...) FIELD_GETTER(__v1, __v2); GET_FIELD3(__v1, __VA_ARGS__)
#define GET_FIELD5(__v1, __v2, ...) FIELD_GETTER(__v1, __v2); GET_FIELD4(__v1, __VA_ARGS__)
#define GET_FIELD6(__v1, __v2, ...) FIELD_GETTER(__v1, __v2); GET_FIELD5(__v1, __VA_ARGS__)
#define GET_FIELD7(__v1, __v2, ...) FIELD_GETTER(__v1, __v2); GET_FIELD6(__v1, __VA_ARGS__)
#define GET_FIELD8(__v1, __v2, ...) FIELD_GETTER(__v1, __v2); GET_FIELD7(__v1, __VA_ARGS__)
#define GET_FIELD9(__v1, __v2, ...) FIELD_GETTER(__v1, __v2); GET_FIELD8(__v1, __VA_ARGS__)
#define GET_FIELD10(__v1, __v2, ...) FIELD_GETTER(__v1, __v2); GET_FIELD9(__v1, __VA_ARGS__)
#define GET_FIELD11(__v1, __v2, ...) FIELD_GETTER(__v1, __v2); GET_FIELD10(__v1, __VA_ARGS__)
#define GET_FIELD12(__v1, __v2, ...) FIELD_GETTER(__v1, __v2); GET_FIELD11(__v1, __VA_ARGS__)
#define GET_FIELD13(__v1, __v2, ...) FIELD_GETTER(__v1, __v2); GET_FIELD12(__v1, __VA_ARGS__)
#define GET_FIELD14(__v1, __v2, ...) FIELD_GETTER(__v1, __v2); GET_FIELD13(__v1, __VA_ARGS__)
#define GET_FIELD15(__v1, __v2, ...) FIELD_GETTER(__v1, __v2); GET_FIELD14(__v1, __VA_ARGS__)
#define GET_FIELD16(__v1, __v2, ...) FIELD_GETTER(__v1, __v2); GET_FIELD15(__v1, __VA_ARGS__)
#define GET_FIELD17(__v1, __v2, ...) FIELD_GETTER(__v1, __v2); GET_FIELD16(__v1, __VA_ARGS__)
#define GET_FIELD18(__v1, __v2, ...) FIELD_GETTER(__v1, __v2); GET_FIELD17(__v1, __VA_ARGS__)
#define GET_FIELD19(__v1, __v2, ...) FIELD_GETTER(__v1, __v2); GET_FIELD18(__v1, __VA_ARGS__)
#define GET_FIELD20(__v1, __v2, ...) FIELD_GETTER(__v1, __v2); GET_FIELD19(__v1, __VA_ARGS__)
#define GET_FIELD21(__v1, __v2, ...) FIELD_GETTER(__v1, __v2); GET_FIELD20(__v1, __VA_ARGS__)

#define GET_FIELD_(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21, n, ...) \
    GET_FIELD##n(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21)

#define GET_FIELD(_v1, ...) \
    GET_FIELD_(_v1, __VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)


/**
 *  Macro for table creation
 *      creates structure of given type with specified fields
 *
 *  __RecordName     - record name - singular
 *  __FieldName      - field name without quotes
 *  __FieldDataType  - stored data type
 *  __FieldNullType  - Nullable/NotNull - is value nullable or not
 *  ...              - the rest of structure params
 *
 *  E.g. create table Users with fields Id, Email, Name, Password
 *
 *  CREATE_TABLE(User,
 *               Id,       int,         NotNull,
 *               Email,    std::string, NotNull,
 *               Name,     std::string, Nullable,
 *               Password, std::string, NotNull);
 *
 */
#define CREATE_TABLE(__RecordName, __FieldName, __FieldDataType, __FieldNullType, ...) \
    struct __RecordName##s : public ::epsql::db::ReflectionGroup<__RecordName##s>, \
                             public ::epsql::db::ITable \
    { \
        __RecordName##s() : ReflectionGroup(NAME(__FieldName, __FieldDataType, __FieldNullType, __VA_ARGS__)) \
        { \
            reflectNames(PRINT(NAME(__FieldName, __FieldDataType, __FieldNullType, __VA_ARGS__))); \
            reflectTypes(PRINT(TYPE(__FieldName, __FieldDataType, __FieldNullType, __VA_ARGS__))); \
        } \
        GET_FIELD(__RecordName, NAME(__FieldName, __FieldDataType, __FieldNullType, __VA_ARGS__)) \
        virtual const char* name() const override { return #__RecordName "s"; } \
        virtual std::vector<const char*> fields() const override { return fieldNames(); } \
    public: \
        FIELD(__FieldName, __FieldDataType, __FieldNullType, __VA_ARGS__) \
    }


namespace epsql::db
{

class ITable
{
public:
    virtual ~ITable() = default;

    virtual const char* name() const = 0;
    virtual std::vector<const char*> fields() const = 0;
};


template <typename T>
class ReflectionGroup
{
public:
    inline const char* typeOf(const char* name)
    {
        auto it = std::find_if(m_names.begin(), m_names.end(),
                               [&name](const auto* n) { return strcmp(name, n) == 0; });
        if (it == m_names.end())
           return "";

        const auto itPos = it - m_names.begin();
        return m_types[itPos];
    }

    template <typename U>
    inline U* get(const char* name)
    {
        auto it = std::find_if(m_names.begin(), m_names.end(),
                               [&name](const auto* n) { return strcmp(name, n) == 0; });
        if (it == m_names.end())
            return nullptr;

        const auto itPos = it - m_names.begin();
        if (utils::TypeOf<NotNull<U>>().type() == m_type[itPos])
            return reinterpret_cast<NotNull<U>*>(m_position[itPos])->nonEmptyPtr();
        else if (utils::TypeOf<Nullable<U>>().type() == m_type[itPos])
            return reinterpret_cast<Nullable<U>*>(m_position[itPos])->nonEmptyPtr();
        return nullptr;
    }

    template <typename U>
    inline void set(const char* name, const U& value)
    {
        if (auto* field = get<U>(name))
            *field = value;
    }

    inline void setNull(const char* name)
    {
        auto it = std::find_if(m_names.begin(), m_names.end(),
                               [&name](const auto* n) { return strcmp(name, n) == 0; });
        if (it != m_names.end())
        {
            const auto itPos = it - m_names.begin();
            ((Value*)m_position[itPos])->setNull();
        }
    }

    inline static const auto& fieldNames() { return m_names; }

protected:
    template <typename... Field>
    ReflectionGroup(Field&&... field)
        : m_position{((void*)&field)...}
        , m_type{utils::TypeOf<std::remove_reference_t<std::remove_cv_t<Field>>>().type()...}
    {}

    template <typename... FieldName>
    inline void reflectNames(FieldName&&... name)
    {
        if (m_position.size() != m_names.size())
            m_names = std::vector{name...};
    }

   template <typename... FieldName>
   inline void reflectTypes(FieldName&&... types)
   {
       if (m_position.size() != m_types.size())
           m_types = std::vector{types...};
   }

private:
    std::vector<void*> m_position = {};
    std::vector<std::size_t> m_type = {};

    static inline std::vector<const char*> m_names = {};
    static inline std::vector<const char*> m_types = {};
};

}  // namespace epsql::db
