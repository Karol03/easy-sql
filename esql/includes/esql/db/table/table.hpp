/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <cstring>
#include <type_traits>
#include <vector>

#include "esql/db/value/default.hpp"
#include "esql/db/value/foreignkey.hpp"
#include "esql/db/value/value.hpp"
#include "esql/utils/hash.hpp"
#include "esql/utils/typeof.hpp"
#include "reflectiongroup.hpp"


#define PRINT(_v1) #_v1
#define PRINT0(...) static_assert(false && "Invalid number of PRINTS(...) arguments! The number should be bigger than zero!")
#define PRINT1(_v1, ...) PRINT(_v1)
#define PRINT2(_v1, ...) PRINT(_v1), PRINT1(__VA_ARGS__)
#define PRINT3(_v1, ...) PRINT(_v1), PRINT2(__VA_ARGS__)
#define PRINT4(_v1, ...) PRINT(_v1), PRINT3(__VA_ARGS__)
#define PRINT5(_v1, ...) PRINT(_v1), PRINT4(__VA_ARGS__)
#define PRINT6(_v1, ...) PRINT(_v1), PRINT5(__VA_ARGS__)
#define PRINT7(_v1, ...) PRINT(_v1), PRINT6(__VA_ARGS__)
#define PRINT8(_v1, ...) PRINT(_v1), PRINT7(__VA_ARGS__)
#define PRINT9(_v1, ...) PRINT(_v1), PRINT8(__VA_ARGS__)
#define PRINT10(_v1, ...) PRINT(_v1), PRINT9(__VA_ARGS__)
#define PRINT11(_v1, ...) PRINT(_v1), PRINT10(__VA_ARGS__)
#define PRINT12(_v1, ...) PRINT(_v1), PRINT11(__VA_ARGS__)
#define PRINT13(_v1, ...) PRINT(_v1), PRINT12(__VA_ARGS__)
#define PRINT14(_v1, ...) PRINT(_v1), PRINT13(__VA_ARGS__)
#define PRINT15(_v1, ...) PRINT(_v1), PRINT14(__VA_ARGS__)
#define PRINT16(_v1, ...) PRINT(_v1), PRINT15(__VA_ARGS__)
#define PRINT17(_v1, ...) PRINT(_v1), PRINT16(__VA_ARGS__)
#define PRINT18(_v1, ...) PRINT(_v1), PRINT17(__VA_ARGS__)
#define PRINT19(_v1, ...) PRINT(_v1), PRINT18(__VA_ARGS__)
#define PRINT20(_v1, ...) PRINT(_v1), PRINT19(__VA_ARGS__)

#define PRINTS_(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20, n, ...) \
    PRINT##n(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20)

#define PRINTS(...) \
    PRINTS_(__VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)


#define NAME(_v1) _v1
#define NAME0(...) static_assert(false && "Invalid number of NAMES(...) arguments! The number should be even!")
#define NAME1(_v1, _v2, ...) NAME(_v1)
#define NAME2(_v1, _v2, ...) NAME(_v1), NAME1(__VA_ARGS__)
#define NAME3(_v1, _v2, ...) NAME(_v1), NAME2(__VA_ARGS__)
#define NAME4(_v1, _v2, ...) NAME(_v1), NAME3(__VA_ARGS__)
#define NAME5(_v1, _v2, ...) NAME(_v1), NAME4(__VA_ARGS__)
#define NAME6(_v1, _v2, ...) NAME(_v1), NAME5(__VA_ARGS__)
#define NAME7(_v1, _v2, ...) NAME(_v1), NAME6(__VA_ARGS__)
#define NAME8(_v1, _v2, ...) NAME(_v1), NAME7(__VA_ARGS__)
#define NAME9(_v1, _v2, ...) NAME(_v1), NAME8(__VA_ARGS__)
#define NAME10(_v1, _v2, ...) NAME(_v1), NAME9(__VA_ARGS__)
#define NAME11(_v1, _v2, ...) NAME(_v1), NAME10(__VA_ARGS__)
#define NAME12(_v1, _v2, ...) NAME(_v1), NAME11(__VA_ARGS__)
#define NAME13(_v1, _v2, ...) NAME(_v1), NAME12(__VA_ARGS__)
#define NAME14(_v1, _v2, ...) NAME(_v1), NAME13(__VA_ARGS__)
#define NAME15(_v1, _v2, ...) NAME(_v1), NAME14(__VA_ARGS__)
#define NAME16(_v1, _v2, ...) NAME(_v1), NAME15(__VA_ARGS__)
#define NAME17(_v1, _v2, ...) NAME(_v1), NAME16(__VA_ARGS__)
#define NAME18(_v1, _v2, ...) NAME(_v1), NAME17(__VA_ARGS__)
#define NAME19(_v1, _v2, ...) NAME(_v1), NAME18(__VA_ARGS__)
#define NAME20(_v1, _v2, ...) NAME(_v1), NAME19(__VA_ARGS__)

#define NAMES_(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21,_v22,_v23,_v24,_v25,_v26,_v27,_v28,_v29,_v30,_v31,_v32,_v33,_v34,_v35,_v36,_v37,_v38,_v39,_v40, n, ...) \
    NAME##n(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21,_v22,_v23,_v24,_v25,_v26,_v27,_v28,_v29,_v30,_v31,_v32,_v33,_v34,_v35,_v36,_v37,_v38,_v39,_v40)

#define NAMES(...) \
    NAMES_(__VA_ARGS__, 20, 0, 19, 0, 18, 0, 17, 0, 16, 0, 15, 0, 14, 0, 13, 0, 12, 0, 11, 0, 10, 0, 9, 0, 8, 0, 7, 0, 6, 0, 5, 0, 4, 0, 3, 0, 2, 0, 1, 0, 0, 0)


#define TYPE(_v2) ::esql::utils::TypeOf<_v2::ValueType>().type()
#define TYPE0(...) static_assert(false && "Invalid number of TYPES(...) arguments! The number should be even!")
#define TYPE1(_v1, _v2, ...) TYPE(_v2)
#define TYPE2(_v1, _v2, ...) TYPE(_v2), TYPE1(__VA_ARGS__)
#define TYPE3(_v1, _v2, ...) TYPE(_v2), TYPE2(__VA_ARGS__)
#define TYPE4(_v1, _v2, ...) TYPE(_v2), TYPE3(__VA_ARGS__)
#define TYPE5(_v1, _v2, ...) TYPE(_v2), TYPE4(__VA_ARGS__)
#define TYPE6(_v1, _v2, ...) TYPE(_v2), TYPE5(__VA_ARGS__)
#define TYPE7(_v1, _v2, ...) TYPE(_v2), TYPE6(__VA_ARGS__)
#define TYPE8(_v1, _v2, ...) TYPE(_v2), TYPE7(__VA_ARGS__)
#define TYPE9(_v1, _v2, ...) TYPE(_v2), TYPE8(__VA_ARGS__)
#define TYPE10(_v1, _v2, ...) TYPE(_v2), TYPE9(__VA_ARGS__)
#define TYPE11(_v1, _v2, ...) TYPE(_v2), TYPE10(__VA_ARGS__)
#define TYPE12(_v1, _v2, ...) TYPE(_v2), TYPE11(__VA_ARGS__)
#define TYPE13(_v1, _v2, ...) TYPE(_v2), TYPE12(__VA_ARGS__)
#define TYPE14(_v1, _v2, ...) TYPE(_v2), TYPE13(__VA_ARGS__)
#define TYPE15(_v1, _v2, ...) TYPE(_v2), TYPE14(__VA_ARGS__)
#define TYPE16(_v1, _v2, ...) TYPE(_v2), TYPE15(__VA_ARGS__)
#define TYPE17(_v1, _v2, ...) TYPE(_v2), TYPE16(__VA_ARGS__)
#define TYPE18(_v1, _v2, ...) TYPE(_v2), TYPE17(__VA_ARGS__)
#define TYPE19(_v1, _v2, ...) TYPE(_v2), TYPE18(__VA_ARGS__)
#define TYPE20(_v1, _v2, ...) TYPE(_v2), TYPE19(__VA_ARGS__)

#define TYPES_(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21,_v22,_v23,_v24,_v25,_v26,_v27,_v28,_v29,_v30,_v31,_v32,_v33,_v34,_v35,_v36,_v37,_v38,_v39,_v40, n, ...) \
    TYPE##n(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21,_v22,_v23,_v24,_v25,_v26,_v27,_v28,_v29,_v30,_v31,_v32,_v33,_v34,_v35,_v36,_v37,_v38,_v39,_v40)

#define TYPES(...) \
    TYPES_(__VA_ARGS__, 20, 0, 19, 0, 18, 0, 17, 0, 16, 0, 15, 0, 14, 0, 13, 0, 12, 0, 11, 0, 10, 0, 9, 0, 8, 0, 7, 0, 6, 0, 5, 0, 4, 0, 3, 0, 2, 0, 1, 0, 0)


#define FIELD(_v1, _v2) _v2 _v1
#define FIELD0(...) static_assert(false && "Invalid number of FIELDS(...) arguments! The number should be even!")
#define FIELD1(_v1, _v2, ...) FIELD(_v1, _v2);
#define FIELD2(_v1, _v2, ...) FIELD(_v1, _v2); FIELD1(__VA_ARGS__)
#define FIELD3(_v1, _v2, ...) FIELD(_v1, _v2); FIELD2(__VA_ARGS__)
#define FIELD4(_v1, _v2, ...) FIELD(_v1, _v2); FIELD3(__VA_ARGS__)
#define FIELD5(_v1, _v2, ...) FIELD(_v1, _v2); FIELD4(__VA_ARGS__)
#define FIELD6(_v1, _v2, ...) FIELD(_v1, _v2); FIELD5(__VA_ARGS__)
#define FIELD7(_v1, _v2, ...) FIELD(_v1, _v2); FIELD6(__VA_ARGS__)
#define FIELD8(_v1, _v2, ...) FIELD(_v1, _v2); FIELD7(__VA_ARGS__)
#define FIELD9(_v1, _v2, ...) FIELD(_v1, _v2); FIELD8(__VA_ARGS__)
#define FIELD10(_v1, _v2, ...) FIELD(_v1, _v2); FIELD9(__VA_ARGS__)
#define FIELD11(_v1, _v2, ...) FIELD(_v1, _v2); FIELD10(__VA_ARGS__)
#define FIELD12(_v1, _v2, ...) FIELD(_v1, _v2); FIELD11(__VA_ARGS__)
#define FIELD13(_v1, _v2, ...) FIELD(_v1, _v2); FIELD12(__VA_ARGS__)
#define FIELD14(_v1, _v2, ...) FIELD(_v1, _v2); FIELD13(__VA_ARGS__)
#define FIELD15(_v1, _v2, ...) FIELD(_v1, _v2); FIELD14(__VA_ARGS__)
#define FIELD16(_v1, _v2, ...) FIELD(_v1, _v2); FIELD15(__VA_ARGS__)
#define FIELD17(_v1, _v2, ...) FIELD(_v1, _v2); FIELD16(__VA_ARGS__)
#define FIELD18(_v1, _v2, ...) FIELD(_v1, _v2); FIELD17(__VA_ARGS__)
#define FIELD19(_v1, _v2, ...) FIELD(_v1, _v2); FIELD18(__VA_ARGS__)
#define FIELD20(_v1, _v2, ...) FIELD(_v1, _v2); FIELD19(__VA_ARGS__)

#define FIELDS_(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21,_v22,_v23,_v24,_v25,_v26,_v27,_v28,_v29,_v30,_v31,_v32,_v33,_v34,_v35,_v36,_v37,_v38,_v39,_v40, n, ...) \
    FIELD##n(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21,_v22,_v23,_v24,_v25,_v26,_v27,_v28,_v29,_v30,_v31,_v32,_v33,_v34,_v35,_v36,_v37,_v38,_v39,_v40)

#define FIELDS(...) \
    FIELDS_(__VA_ARGS__, 20, 0, 19, 0, 18, 0, 17, 0, 16, 0, 15, 0, 14, 0, 13, 0, 12, 0, 11, 0, 10, 0, 9, 0, 8, 0, 7, 0, 6, 0, 5, 0, 4, 0, 3, 0, 2, 0, 1, 0, 0)


#define COPY_FIELD(_v1) this->_v1 = table._v1
#define COPY_FIELD0(...) static_assert(false && "Invalid number of COPY_FIELDS(...) arguments! The number should be bigger than zero!")
#define COPY_FIELD1(_v1, ...) COPY_FIELD(_v1);
#define COPY_FIELD2(_v1, ...) COPY_FIELD(_v1); COPY_FIELD1(__VA_ARGS__)
#define COPY_FIELD3(_v1, ...) COPY_FIELD(_v1); COPY_FIELD2(__VA_ARGS__)
#define COPY_FIELD4(_v1, ...) COPY_FIELD(_v1); COPY_FIELD3(__VA_ARGS__)
#define COPY_FIELD5(_v1, ...) COPY_FIELD(_v1); COPY_FIELD4(__VA_ARGS__)
#define COPY_FIELD6(_v1, ...) COPY_FIELD(_v1); COPY_FIELD5(__VA_ARGS__)
#define COPY_FIELD7(_v1, ...) COPY_FIELD(_v1); COPY_FIELD6(__VA_ARGS__)
#define COPY_FIELD8(_v1, ...) COPY_FIELD(_v1); COPY_FIELD7(__VA_ARGS__)
#define COPY_FIELD9(_v1, ...) COPY_FIELD(_v1); COPY_FIELD8(__VA_ARGS__)
#define COPY_FIELD10(_v1, ...) COPY_FIELD(_v1); COPY_FIELD9(__VA_ARGS__)
#define COPY_FIELD11(_v1, ...) COPY_FIELD(_v1); COPY_FIELD10(__VA_ARGS__)
#define COPY_FIELD12(_v1, ...) COPY_FIELD(_v1); COPY_FIELD11(__VA_ARGS__)
#define COPY_FIELD13(_v1, ...) COPY_FIELD(_v1); COPY_FIELD12(__VA_ARGS__)
#define COPY_FIELD14(_v1, ...) COPY_FIELD(_v1); COPY_FIELD13(__VA_ARGS__)
#define COPY_FIELD15(_v1, ...) COPY_FIELD(_v1); COPY_FIELD14(__VA_ARGS__)
#define COPY_FIELD16(_v1, ...) COPY_FIELD(_v1); COPY_FIELD15(__VA_ARGS__)
#define COPY_FIELD17(_v1, ...) COPY_FIELD(_v1); COPY_FIELD16(__VA_ARGS__)
#define COPY_FIELD18(_v1, ...) COPY_FIELD(_v1); COPY_FIELD17(__VA_ARGS__)
#define COPY_FIELD19(_v1, ...) COPY_FIELD(_v1); COPY_FIELD18(__VA_ARGS__)
#define COPY_FIELD20(_v1, ...) COPY_FIELD(_v1); COPY_FIELD19(__VA_ARGS__)

#define COPY_FIELDS_(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20, n, ...) \
    COPY_FIELD##n(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20)

#define COPY_FIELDS(...) \
    COPY_FIELDS_(__VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)


#define COMPARE_FIELD(_v1, _v2) (compare(this->_v2, _v1._v2))
#define COMPARE_FIELD0(...) static_assert(false && "Invalid number of COMPARE_FIELDS(...) arguments! The number should be even!")
#define COMPARE_FIELD1(_v1, _v2, ...) COMPARE_FIELD(_v1, _v2)
#define COMPARE_FIELD2(_v1, _v2, ...) COMPARE_FIELD(_v1, _v2) && COMPARE_FIELD1(_v1, __VA_ARGS__)
#define COMPARE_FIELD3(_v1, _v2, ...) COMPARE_FIELD(_v1, _v2) && COMPARE_FIELD2(_v1, __VA_ARGS__)
#define COMPARE_FIELD4(_v1, _v2, ...) COMPARE_FIELD(_v1, _v2) && COMPARE_FIELD3(_v1, __VA_ARGS__)
#define COMPARE_FIELD5(_v1, _v2, ...) COMPARE_FIELD(_v1, _v2) && COMPARE_FIELD4(_v1, __VA_ARGS__)
#define COMPARE_FIELD6(_v1, _v2, ...) COMPARE_FIELD(_v1, _v2) && COMPARE_FIELD5(_v1, __VA_ARGS__)
#define COMPARE_FIELD7(_v1, _v2, ...) COMPARE_FIELD(_v1, _v2) && COMPARE_FIELD6(_v1, __VA_ARGS__)
#define COMPARE_FIELD8(_v1, _v2, ...) COMPARE_FIELD(_v1, _v2) && COMPARE_FIELD7(_v1, __VA_ARGS__)
#define COMPARE_FIELD9(_v1, _v2, ...) COMPARE_FIELD(_v1, _v2) && COMPARE_FIELD8(_v1, __VA_ARGS__)
#define COMPARE_FIELD10(_v1, _v2, ...) COMPARE_FIELD(_v1, _v2) && COMPARE_FIELD9(_v1, __VA_ARGS__)
#define COMPARE_FIELD11(_v1, _v2, ...) COMPARE_FIELD(_v1, _v2) && COMPARE_FIELD10(_v1, __VA_ARGS__)
#define COMPARE_FIELD12(_v1, _v2, ...) COMPARE_FIELD(_v1, _v2) && COMPARE_FIELD11(_v1, __VA_ARGS__)
#define COMPARE_FIELD13(_v1, _v2, ...) COMPARE_FIELD(_v1, _v2) && COMPARE_FIELD12(_v1, __VA_ARGS__)
#define COMPARE_FIELD14(_v1, _v2, ...) COMPARE_FIELD(_v1, _v2) && COMPARE_FIELD13(_v1, __VA_ARGS__)
#define COMPARE_FIELD15(_v1, _v2, ...) COMPARE_FIELD(_v1, _v2) && COMPARE_FIELD14(_v1, __VA_ARGS__)
#define COMPARE_FIELD16(_v1, _v2, ...) COMPARE_FIELD(_v1, _v2) && COMPARE_FIELD15(_v1, __VA_ARGS__)
#define COMPARE_FIELD17(_v1, _v2, ...) COMPARE_FIELD(_v1, _v2) && COMPARE_FIELD16(_v1, __VA_ARGS__)
#define COMPARE_FIELD18(_v1, _v2, ...) COMPARE_FIELD(_v1, _v2) && COMPARE_FIELD17(_v1, __VA_ARGS__)
#define COMPARE_FIELD19(_v1, _v2, ...) COMPARE_FIELD(_v1, _v2) && COMPARE_FIELD18(_v1, __VA_ARGS__)
#define COMPARE_FIELD20(_v1, _v2, ...) COMPARE_FIELD(_v1, _v2) && COMPARE_FIELD19(_v1, __VA_ARGS__)

#define COMPARE_FIELDS_(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21, n, ...) \
    COMPARE_FIELD##n(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21)

#define COMPARE_FIELDS(_v1, ...) \
    COMPARE_FIELDS_(_v1, __VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)


#define GET_FIELD_NAME(_v1, _v2) #_v1 "::" #_v2
#define GET_FIELD_NAME0(...) static_assert(false && "Invalid number of GET_FIELD_NAMES(...) arguments! The number should be even!")
#define GET_FIELD_NAME1(_v1, _v2, ...) GET_FIELD_NAME(_v1, _v2)
#define GET_FIELD_NAME2(_v1, _v2, ...) GET_FIELD_NAME(_v1, _v2), GET_FIELD_NAME1(_v1, __VA_ARGS__)
#define GET_FIELD_NAME3(_v1, _v2, ...) GET_FIELD_NAME(_v1, _v2), GET_FIELD_NAME2(_v1, __VA_ARGS__)
#define GET_FIELD_NAME4(_v1, _v2, ...) GET_FIELD_NAME(_v1, _v2), GET_FIELD_NAME3(_v1, __VA_ARGS__)
#define GET_FIELD_NAME5(_v1, _v2, ...) GET_FIELD_NAME(_v1, _v2), GET_FIELD_NAME4(_v1, __VA_ARGS__)
#define GET_FIELD_NAME6(_v1, _v2, ...) GET_FIELD_NAME(_v1, _v2), GET_FIELD_NAME5(_v1, __VA_ARGS__)
#define GET_FIELD_NAME7(_v1, _v2, ...) GET_FIELD_NAME(_v1, _v2), GET_FIELD_NAME6(_v1, __VA_ARGS__)
#define GET_FIELD_NAME8(_v1, _v2, ...) GET_FIELD_NAME(_v1, _v2), GET_FIELD_NAME7(_v1, __VA_ARGS__)
#define GET_FIELD_NAME9(_v1, _v2, ...) GET_FIELD_NAME(_v1, _v2), GET_FIELD_NAME8(_v1, __VA_ARGS__)
#define GET_FIELD_NAME10(_v1, _v2, ...) GET_FIELD_NAME(_v1, _v2), GET_FIELD_NAME9(_v1, __VA_ARGS__)
#define GET_FIELD_NAME11(_v1, _v2, ...) GET_FIELD_NAME(_v1, _v2), GET_FIELD_NAME10(_v1, __VA_ARGS__)
#define GET_FIELD_NAME12(_v1, _v2, ...) GET_FIELD_NAME(_v1, _v2), GET_FIELD_NAME11(_v1, __VA_ARGS__)
#define GET_FIELD_NAME13(_v1, _v2, ...) GET_FIELD_NAME(_v1, _v2), GET_FIELD_NAME12(_v1, __VA_ARGS__)
#define GET_FIELD_NAME14(_v1, _v2, ...) GET_FIELD_NAME(_v1, _v2), GET_FIELD_NAME13(_v1, __VA_ARGS__)
#define GET_FIELD_NAME15(_v1, _v2, ...) GET_FIELD_NAME(_v1, _v2), GET_FIELD_NAME14(_v1, __VA_ARGS__)
#define GET_FIELD_NAME16(_v1, _v2, ...) GET_FIELD_NAME(_v1, _v2), GET_FIELD_NAME15(_v1, __VA_ARGS__)
#define GET_FIELD_NAME17(_v1, _v2, ...) GET_FIELD_NAME(_v1, _v2), GET_FIELD_NAME16(_v1, __VA_ARGS__)
#define GET_FIELD_NAME18(_v1, _v2, ...) GET_FIELD_NAME(_v1, _v2), GET_FIELD_NAME17(_v1, __VA_ARGS__)
#define GET_FIELD_NAME19(_v1, _v2, ...) GET_FIELD_NAME(_v1, _v2), GET_FIELD_NAME18(_v1, __VA_ARGS__)
#define GET_FIELD_NAME20(_v1, _v2, ...) GET_FIELD_NAME(_v1, _v2), GET_FIELD_NAME19(_v1, __VA_ARGS__)

#define GET_FIELD_NAMES_(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21, n, ...) \
    GET_FIELD_NAME##n(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21)

#define GET_FIELD_NAMES(_v1, ...) \
    GET_FIELD_NAMES_(_v1, __VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)


#define GET_FIELD(_v1, _v2) inline constexpr static const char* _v2##NameGetter() { return #_v1 "::" #_v2; }
#define GET_FIELD0(...) static_assert(false && "Invalid number of GET_FIELDS(...) arguments! The number should be even!")
#define GET_FIELD1(_v1, _v2, ...) GET_FIELD(_v1, _v2);
#define GET_FIELD2(_v1, _v2, ...) GET_FIELD(_v1, _v2); GET_FIELD1(_v1, __VA_ARGS__)
#define GET_FIELD3(_v1, _v2, ...) GET_FIELD(_v1, _v2); GET_FIELD2(_v1, __VA_ARGS__)
#define GET_FIELD4(_v1, _v2, ...) GET_FIELD(_v1, _v2); GET_FIELD3(_v1, __VA_ARGS__)
#define GET_FIELD5(_v1, _v2, ...) GET_FIELD(_v1, _v2); GET_FIELD4(_v1, __VA_ARGS__)
#define GET_FIELD6(_v1, _v2, ...) GET_FIELD(_v1, _v2); GET_FIELD5(_v1, __VA_ARGS__)
#define GET_FIELD7(_v1, _v2, ...) GET_FIELD(_v1, _v2); GET_FIELD6(_v1, __VA_ARGS__)
#define GET_FIELD8(_v1, _v2, ...) GET_FIELD(_v1, _v2); GET_FIELD7(_v1, __VA_ARGS__)
#define GET_FIELD9(_v1, _v2, ...) GET_FIELD(_v1, _v2); GET_FIELD8(_v1, __VA_ARGS__)
#define GET_FIELD10(_v1, _v2, ...) GET_FIELD(_v1, _v2); GET_FIELD9(_v1, __VA_ARGS__)
#define GET_FIELD11(_v1, _v2, ...) GET_FIELD(_v1, _v2); GET_FIELD10(_v1, __VA_ARGS__)
#define GET_FIELD12(_v1, _v2, ...) GET_FIELD(_v1, _v2); GET_FIELD11(_v1, __VA_ARGS__)
#define GET_FIELD13(_v1, _v2, ...) GET_FIELD(_v1, _v2); GET_FIELD12(_v1, __VA_ARGS__)
#define GET_FIELD14(_v1, _v2, ...) GET_FIELD(_v1, _v2); GET_FIELD13(_v1, __VA_ARGS__)
#define GET_FIELD15(_v1, _v2, ...) GET_FIELD(_v1, _v2); GET_FIELD14(_v1, __VA_ARGS__)
#define GET_FIELD16(_v1, _v2, ...) GET_FIELD(_v1, _v2); GET_FIELD15(_v1, __VA_ARGS__)
#define GET_FIELD17(_v1, _v2, ...) GET_FIELD(_v1, _v2); GET_FIELD16(_v1, __VA_ARGS__)
#define GET_FIELD18(_v1, _v2, ...) GET_FIELD(_v1, _v2); GET_FIELD17(_v1, __VA_ARGS__)
#define GET_FIELD19(_v1, _v2, ...) GET_FIELD(_v1, _v2); GET_FIELD18(_v1, __VA_ARGS__)
#define GET_FIELD20(_v1, _v2, ...) GET_FIELD(_v1, _v2); GET_FIELD19(_v1, __VA_ARGS__)

#define GET_FIELDS_(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21, n, ...) \
    GET_FIELD##n(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21)

#define GET_FIELDS(_v1, ...) \
    GET_FIELDS_(_v1, __VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)


#define FIELD_FULL_NAME(_v1, _v2) \
    (std::string{#_v1 " "} + ::esql::db::table::nameOf<_v2::ValueType>() + " " + _v2::name())
#define FIELD_FULL_NAME0(...) static_assert(false && "Invalid number of FIELD_FULL_NAME(...) arguments! The number should be even!")
#define FIELD_FULL_NAME1(_v1, _v2, ...) FIELD_FULL_NAME(_v1, _v2)
#define FIELD_FULL_NAME2(_v1, _v2, ...) FIELD_FULL_NAME(_v1, _v2), FIELD_FULL_NAME1(__VA_ARGS__)
#define FIELD_FULL_NAME3(_v1, _v2, ...) FIELD_FULL_NAME(_v1, _v2), FIELD_FULL_NAME2(__VA_ARGS__)
#define FIELD_FULL_NAME4(_v1, _v2, ...) FIELD_FULL_NAME(_v1, _v2), FIELD_FULL_NAME3(__VA_ARGS__)
#define FIELD_FULL_NAME5(_v1, _v2, ...) FIELD_FULL_NAME(_v1, _v2), FIELD_FULL_NAME4(__VA_ARGS__)
#define FIELD_FULL_NAME6(_v1, _v2, ...) FIELD_FULL_NAME(_v1, _v2), FIELD_FULL_NAME5(__VA_ARGS__)
#define FIELD_FULL_NAME7(_v1, _v2, ...) FIELD_FULL_NAME(_v1, _v2), FIELD_FULL_NAME6(__VA_ARGS__)
#define FIELD_FULL_NAME8(_v1, _v2, ...) FIELD_FULL_NAME(_v1, _v2), FIELD_FULL_NAME7(__VA_ARGS__)
#define FIELD_FULL_NAME9(_v1, _v2, ...) FIELD_FULL_NAME(_v1, _v2), FIELD_FULL_NAME8(__VA_ARGS__)
#define FIELD_FULL_NAME10(_v1, _v2, ...) FIELD_FULL_NAME(_v1, _v2), FIELD_FULL_NAME9(__VA_ARGS__)
#define FIELD_FULL_NAME11(_v1, _v2, ...) FIELD_FULL_NAME(_v1, _v2), FIELD_FULL_NAME10(__VA_ARGS__)
#define FIELD_FULL_NAME12(_v1, _v2, ...) FIELD_FULL_NAME(_v1, _v2), FIELD_FULL_NAME11(__VA_ARGS__)
#define FIELD_FULL_NAME13(_v1, _v2, ...) FIELD_FULL_NAME(_v1, _v2), FIELD_FULL_NAME12(__VA_ARGS__)
#define FIELD_FULL_NAME14(_v1, _v2, ...) FIELD_FULL_NAME(_v1, _v2), FIELD_FULL_NAME13(__VA_ARGS__)
#define FIELD_FULL_NAME15(_v1, _v2, ...) FIELD_FULL_NAME(_v1, _v2), FIELD_FULL_NAME14(__VA_ARGS__)
#define FIELD_FULL_NAME16(_v1, _v2, ...) FIELD_FULL_NAME(_v1, _v2), FIELD_FULL_NAME15(__VA_ARGS__)
#define FIELD_FULL_NAME17(_v1, _v2, ...) FIELD_FULL_NAME(_v1, _v2), FIELD_FULL_NAME16(__VA_ARGS__)
#define FIELD_FULL_NAME18(_v1, _v2, ...) FIELD_FULL_NAME(_v1, _v2), FIELD_FULL_NAME17(__VA_ARGS__)
#define FIELD_FULL_NAME19(_v1, _v2, ...) FIELD_FULL_NAME(_v1, _v2), FIELD_FULL_NAME18(__VA_ARGS__)
#define FIELD_FULL_NAME20(_v1, _v2, ...) FIELD_FULL_NAME(_v1, _v2), FIELD_FULL_NAME19(__VA_ARGS__)

#define FIELD_FULL_NAMES_(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21,_v22,_v23,_v24,_v25,_v26,_v27,_v28,_v29,_v30,_v31,_v32,_v33,_v34,_v35,_v36,_v37,_v38,_v39,_v40, n, ...) \
    FIELD_FULL_NAME##n(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21,_v22,_v23,_v24,_v25,_v26,_v27,_v28,_v29,_v30,_v31,_v32,_v33,_v34,_v35,_v36,_v37,_v38,_v39,_v40)

#define FIELD_FULL_NAMES(...) \
    FIELD_FULL_NAMES_(__VA_ARGS__, 20, 0, 19, 0, 18, 0, 17, 0, 16, 0, 15, 0, 14, 0, 13, 0, 12, 0, 11, 0, 10, 0, 9, 0, 8, 0, 7, 0, 6, 0, 5, 0, 4, 0, 3, 0, 2, 0, 1, 0, 0)


#define GET_FOREIGN_KEY(_v1, _v2) \
    InternalForeignKey{#_v1, _v2::referenceTable(), _v2::referenceField()}
#define GET_FOREIGN_KEY0(...) static_assert(false && "Invalid number of GET_FOREIGN_KEYS(...) arguments! The number should be even!")
#define GET_FOREIGN_KEY1(_v1, _v2, ...) GET_FOREIGN_KEY(_v1, _v2)
#define GET_FOREIGN_KEY2(_v1, _v2, ...) GET_FOREIGN_KEY(_v1, _v2), GET_FOREIGN_KEY1(__VA_ARGS__)
#define GET_FOREIGN_KEY3(_v1, _v2, ...) GET_FOREIGN_KEY(_v1, _v2), GET_FOREIGN_KEY2(__VA_ARGS__)
#define GET_FOREIGN_KEY4(_v1, _v2, ...) GET_FOREIGN_KEY(_v1, _v2), GET_FOREIGN_KEY3(__VA_ARGS__)
#define GET_FOREIGN_KEY5(_v1, _v2, ...) GET_FOREIGN_KEY(_v1, _v2), GET_FOREIGN_KEY4(__VA_ARGS__)
#define GET_FOREIGN_KEY6(_v1, _v2, ...) GET_FOREIGN_KEY(_v1, _v2), GET_FOREIGN_KEY5(__VA_ARGS__)
#define GET_FOREIGN_KEY7(_v1, _v2, ...) GET_FOREIGN_KEY(_v1, _v2), GET_FOREIGN_KEY6(__VA_ARGS__)
#define GET_FOREIGN_KEY8(_v1, _v2, ...) GET_FOREIGN_KEY(_v1, _v2), GET_FOREIGN_KEY7(__VA_ARGS__)
#define GET_FOREIGN_KEY9(_v1, _v2, ...) GET_FOREIGN_KEY(_v1, _v2), GET_FOREIGN_KEY8(__VA_ARGS__)
#define GET_FOREIGN_KEY10(_v1, _v2, ...) GET_FOREIGN_KEY(_v1, _v2), GET_FOREIGN_KEY9(__VA_ARGS__)
#define GET_FOREIGN_KEY11(_v1, _v2, ...) GET_FOREIGN_KEY(_v1, _v2), GET_FOREIGN_KEY10(__VA_ARGS__)
#define GET_FOREIGN_KEY12(_v1, _v2, ...) GET_FOREIGN_KEY(_v1, _v2), GET_FOREIGN_KEY11(__VA_ARGS__)
#define GET_FOREIGN_KEY13(_v1, _v2, ...) GET_FOREIGN_KEY(_v1, _v2), GET_FOREIGN_KEY12(__VA_ARGS__)
#define GET_FOREIGN_KEY14(_v1, _v2, ...) GET_FOREIGN_KEY(_v1, _v2), GET_FOREIGN_KEY13(__VA_ARGS__)
#define GET_FOREIGN_KEY15(_v1, _v2, ...) GET_FOREIGN_KEY(_v1, _v2), GET_FOREIGN_KEY14(__VA_ARGS__)
#define GET_FOREIGN_KEY16(_v1, _v2, ...) GET_FOREIGN_KEY(_v1, _v2), GET_FOREIGN_KEY15(__VA_ARGS__)
#define GET_FOREIGN_KEY17(_v1, _v2, ...) GET_FOREIGN_KEY(_v1, _v2), GET_FOREIGN_KEY16(__VA_ARGS__)
#define GET_FOREIGN_KEY18(_v1, _v2, ...) GET_FOREIGN_KEY(_v1, _v2), GET_FOREIGN_KEY17(__VA_ARGS__)
#define GET_FOREIGN_KEY19(_v1, _v2, ...) GET_FOREIGN_KEY(_v1, _v2), GET_FOREIGN_KEY18(__VA_ARGS__)
#define GET_FOREIGN_KEY20(_v1, _v2, ...) GET_FOREIGN_KEY(_v1, _v2), GET_FOREIGN_KEY19(__VA_ARGS__)

#define GET_FOREIGN_KEYS_(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21,_v22,_v23,_v24,_v25,_v26,_v27,_v28,_v29,_v30,_v31,_v32,_v33,_v34,_v35,_v36,_v37,_v38,_v39,_v40, n, ...) \
    GET_FOREIGN_KEY##n(_v1,_v2,_v3,_v4,_v5,_v6,_v7,_v8,_v9,_v10,_v11,_v12,_v13,_v14,_v15,_v16,_v17,_v18,_v19,_v20,_v21,_v22,_v23,_v24,_v25,_v26,_v27,_v28,_v29,_v30,_v31,_v32,_v33,_v34,_v35,_v36,_v37,_v38,_v39,_v40)

#define GET_FOREIGN_KEYS(...) \
    GET_FOREIGN_KEYS_(__VA_ARGS__, 20, 0, 19, 0, 18, 0, 17, 0, 16, 0, 15, 0, 14, 0, 13, 0, 12, 0, 11, 0, 10, 0, 9, 0, 8, 0, 7, 0, 6, 0, 5, 0, 4, 0, 3, 0, 2, 0, 1, 0, 0)


#define NULLABLE(__SqlType) \
    ::esql::db::value::Nullable<__SqlType>

#define NOTNULL 2
#define NOT_NULL(__SqlType) \
    ::esql::db::value::NotNull<__SqlType>


#define FOREIGN_KEY0(...) static_assert(false && "Macro FOREIGN_KEY(...) must have at least 1 argument (see table.hpp)")
#define FOREIGN_KEY1(__ReferenceField, ...) \
    ::esql::db::value::ForeignKeyDeduce<-1 * static_cast<int64_t>(::esql::utils::Hash::hash(#__ReferenceField))>::Type<decltype(__ReferenceField)::ValueType>
#define FOREIGN_KEY2(__ReferenceField, __IsNullable) \
    ::esql::db::value::ForeignKeyDeduce<(__IsNullable - 1) * static_cast<int64_t>(::esql::utils::Hash::hash(#__ReferenceField))>::Type<decltype(__ReferenceField)::ValueType>

#define FOREIGN_KEY_(__ReferenceField, __IsNullable, n, ...) \
    FOREIGN_KEY##n(__ReferenceField, __IsNullable)
/**
 *  Macro for creating foreign_key in table
 *      accepts one or two arguments
 *      __ReferenceField - first argument (required) is reference field ex. 'Users::Id' means
 *                          reference to field 'Id' in table 'Users'
 *      __FieldRelationType - optional argument, accepted values are NULL/NOTNULL
 *                            NULL - if field could be Null
 *                            NOTNULL - if field could not be Null
 */
#define FOREIGN_KEY(...) \
    FOREIGN_KEY_(__VA_ARGS__, 2, 1, 0)


#define DEFAULT0(...) static_assert(false && "Macro DEFAULT(...) must have at least 2 arguments [currently 0] (see table.hpp)")
#define DEFAULT1(...) static_assert(false && "Macro DEFAULT(...) must have at least 2 arguments [currently 1] (see table.hpp)")
#define DEFAULT2(__FieldType, __Value, ...) \
    ::esql::db::value::DefaultDeduce<1>::Type<__FieldType>
#define DEFAULT3(__FieldType, __Value, __IsNullable) \
    ::esql::db::value::DefaultDeduce<(__IsNullable - 1)>::Type<__FieldType>

#define DEFAULT_(__FieldType, __Value, __IsNullable, n, ...) \
    DEFAULT##n(__FieldType, __Value, __IsNullable)
/**
 *  Macro for creating field with default value
 *      accepts two or three arguments
 *      __FieldType - first argument (required) is member type
 *      __Value - second argument (required) is default value
 *      __IsNullable - optional argument, accepted values are NULL/NOTNULL
 *                     NULL - if field could be Null
 *                     NOTNULL - if field could not be Null
 */
#define DEFAULT(...) \
    DEFAULT_(__VA_ARGS__, 3, 2, 1, 0)

/**
 *  Macro for table creation
 *      creates structure of given type with specified fields
 *
 *  __TableName          - table name
 *  __FieldName          - field name without quotes
 *  __FieldRelationType  - NULLABLE(type)/NOT_NULL(type)/FOREIGN_KEY(referenced field, <optional> NULLABLE/NOT_NULL)
 *  ...                  - the rest of structure params
 *
 *  E.g. create table Users with fields Id, Email, Name, Password
 *
 *  CREATE_TABLE(User,
 *               Id,       NOT_NULL(Int),
 *               Email,    NOT_NULL(Text),
 *               Name,     NULLABLE(Text),
 *               Password, NOT_NULL(Text);
 *
 *  CREATE_TABLE(Thing,
 *               Id,       NOT_NULL(Int),,
 *               Name,     NOT_NULL(Text),
 *               UserId,   FOREIGN_KEY(Users::Id, NotNull));
 *
 */
#define CREATE_TABLE(__TableName, __FieldName, __FieldRelationType, ...) \
    struct __TableName : public ::esql::db::table::ReflectionGroup<__TableName>, \
                         public ::esql::db::table::ITable \
    { \
    public: \
        static auto foreignKeys() \
        { \
            return std::array{GET_FOREIGN_KEYS(__FieldName, __FieldRelationType, __VA_ARGS__)}; \
        } \
        \
        __TableName() : ReflectionGroup(NAMES(__FieldName, __FieldRelationType, __VA_ARGS__)) \
        { \
            reflectNames(PRINTS(NAMES(__FieldName, __FieldRelationType, __VA_ARGS__))); \
            reflectTypes(TYPES(__FieldName, __FieldRelationType, __VA_ARGS__)); \
            if (!isReferenced) \
            { \
                assignAll(::esql::utils::TypeOf<__TableName>().type(), \
                          #__TableName, \
                          GET_FIELD_NAMES(__TableName, NAMES(__FieldName, __FieldRelationType, __VA_ARGS__))); \
                for (const auto& foreignKey : foreignKeys()) \
                    insertForeignKey(foreignKey.field, foreignKey.referenceTable, foreignKey.referenceField); \
                isReferenced = true; \
            } \
        } \
        __TableName(const __TableName& table) : ReflectionGroup(NAMES(__FieldName, __FieldRelationType, __VA_ARGS__)) \
        { \
            reflectNames(PRINTS(NAMES(__FieldName, __FieldRelationType, __VA_ARGS__))); \
            reflectTypes(TYPES(__FieldName, __FieldRelationType, __VA_ARGS__)); \
            COPY_FIELDS(NAMES(__FieldName, __FieldRelationType, __VA_ARGS__)) \
        } \
        __TableName& operator=(const __TableName& table) \
        { \
            reflectNames(PRINTS(NAMES(__FieldName, __FieldRelationType, __VA_ARGS__))); \
            reflectTypes(TYPES(__FieldName, __FieldRelationType, __VA_ARGS__)); \
            COPY_FIELDS(NAMES(__FieldName, __FieldRelationType, __VA_ARGS__)) \
            return *this; \
        } \
        __TableName(__TableName&& table) : ReflectionGroup(NAMES(__FieldName, __FieldRelationType, __VA_ARGS__)) \
        { \
            reflectNames(PRINTS(NAMES(__FieldName, __FieldRelationType, __VA_ARGS__))); \
            reflectTypes(TYPES(__FieldName, __FieldRelationType, __VA_ARGS__)); \
            COPY_FIELDS(NAMES(__FieldName, __FieldRelationType, __VA_ARGS__)) \
        } \
        __TableName& operator=(__TableName&& table) \
        { \
            reflectNames(PRINTS(NAMES(__FieldName, __FieldRelationType, __VA_ARGS__))); \
            reflectTypes(TYPES(__FieldName, __FieldRelationType, __VA_ARGS__)); \
            COPY_FIELDS(NAMES(__FieldName, __FieldRelationType, __VA_ARGS__)) \
            return *this; \
        } \
        inline bool operator==(const __TableName& table) const { return COMPARE_FIELDS(table, NAMES(__FieldName, __FieldRelationType, __VA_ARGS__)); } \
        inline bool operator!=(const __TableName& table) const { return !operator==(table); } \
        \
        GET_FIELDS(__TableName, NAMES(__FieldName, __FieldRelationType, __VA_ARGS__)) \
        static const char* name() { return #__TableName; } \
        static const char* primaryKeyName() { return #__FieldName; } \
        static std::vector<std::string> fieldFullNames() { return std::vector{ \
            FIELD_FULL_NAMES(__FieldName, __FieldRelationType, __VA_ARGS__)}; } \
    public: \
        FIELDS(__FieldName, __FieldRelationType, __VA_ARGS__) \
    }; struct __Table##__TableName##Inializer {  __Table##__TableName##Inializer() { (void)(__TableName{}); } } __table##__TableName##Inializer


namespace esql::db::table
{

class ITable
{
public:
    virtual ~ITable() = default;

protected:
    struct InternalForeignKey
    {
        InternalForeignKey(const char* p1, const char* p2, const char* p3)
            : field{p1}
            , referenceTable{p2}
            , referenceField{p3}
        {}

        const char* field;
        const char* referenceTable;
        const char* referenceField;
    };
};

}  // namespace esql::db::table
