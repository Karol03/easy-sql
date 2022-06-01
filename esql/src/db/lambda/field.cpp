/**
 * Created by Karol Dudzic @ 2022
 */

#include "esql/db/lambda/field.hpp"


namespace esql::db::lambda
{

Field::Field(FieldManager& manager,
             const char* tableAndFieldName,
             uint64_t fieldTypeId,
             const char* nullableName)
    : m_isNullable{strcmp(nullableName, "NULL") == 0}
    , m_manager{manager}
    , m_fieldTypeId{fieldTypeId}
    , m_tableFieldName{tableAndFieldName}
{
    m_tableFieldName.replace(m_tableFieldName.find(':'), 2, ".");
}

}  // namespace esql::db::lambda
